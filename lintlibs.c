/* $Id: lintlibs.c,v 4.3.1.1 1996/05/30 23:37:25 tom Exp $
 *
 * C prototype/lint-library generator
 * These routines implement the semantic actions for lint libraries executed by
 * the yacc parser.
 */
#include <stdio.h>
#include <ctype.h>
#include "cproto.h"
#include "semantic.h"
#include "symbol.h"

#if OPT_LINTLIBRARY

	int	in_include;

static	SymbolTable *include_list;
static	SymbolTable *declared_list;

static	char	*strip_name      ARGS((char *s));
static	void	free_inc_stack   ARGS((int n));
static	void	make_inc_stack   ARGS((int n, char *path));
static	int	already_included ARGS((char *path));
static	void	add2implied_buf  ARGS((char *s, int append));
static	int	c_suffix         ARGS((char *path));

static	int	in_typedef;
static	int	blank_lines;	/* used to filter blank lines from typedefs */

static	int	implied_cnt;	/* state-count associated with implied_buf */
static	char	*implied_buf;

static	char	quote_l	= '"',
		quote_r = '"';

/*
 * Output a string to standard output, keeping track of the trailing newlines
 * to make it simple to format with blank lines.
 */
void
put_string(outf, s)
FILE	*outf;
char	*s;
{
	if (*s != '\0') {
		fputs(s, outf);
		if (outf == stdout) {	/* ensure we aren't doing temp-file! */
			while (*s != '\0') {
				if (*s++ == '\n')
					blank_lines++;
				else
					blank_lines = 0;
			}
		}
	}
}

/*
 * Output a single character
 */
void
put_char(outf, c)
FILE	*outf;
int	c;
{
	static	char	s[] = "?";
	s[0] = c;
	put_string(outf, s);
}

/*
 * Write a newline, taking care not to make a blank line
 */
void
put_newline(outf)
FILE	*outf;
{
	while (!blank_lines)
		put_string(outf, "\n");
}

/*
 * Make a blank line (limited to 2 successive newlines)
 */
void
put_blankline(outf)
FILE	*outf;
{
	while (blank_lines < 2)
		put_string(outf, "\n");
}

/*
 * Put a token, padded by a tab if it is short enough
 */
void
put_padded(outf, s)
FILE	*outf;
char	*s;
{
	put_string(outf, s);
	put_char(outf, (lintLibrary() && strlen(s) < 8) ? '\t' : ' ');
}

/*
 * Format lint-library so that we put a blank line before each item that may
 * take up multiple lines:
 *	0) functions
 *	1) typedefs (explicit and implied)
 * as well as when transitioning to
 *	2) variable declarations
 *
 * If the "-T" option is set, we skip a blank line around typedefs.
 */
void
fmt_library(code)
int	code;
{
	if (lintLibrary() || types_out) {
		static	int	save;

		if (!lintLibrary() && code == 0)
			code = 3;
		if (code <= 1 || code != save)
			put_blankline(stdout);
		save = code;
	}
}

/*
 * conversion for names so test-diffs are less
 * (patch: should use relpath)
 */
#ifdef	vms
static	char	*strip_name(s)
	char	*s;
	{
		static	char	stripped[BUFSIZ];
		auto	int	len = strlen(getwd(stripped));
		if (strlen(s) > len
		&& !strncmp(s, stripped, len))
			s += len;
		return (vms2name(stripped, s));
	}
#else
static	char	*strip_name(s)
	char	*s;
	{
		static	char	GccLeaf[] = "/gcc-lib/";
		static	char	IncLeaf[] = "/include/";
		char *t;
		register int	n;
		register size_t	len;
		int standard = FALSE;

		for (n = 1; n < num_inc_dir; n++) {
			len = strlen(inc_dir[n]);
			if (!strncmp(inc_dir[n], s, len)
			 && is_path_sep(s[len])) {
				standard = TRUE;
				s += len + 1;
				quote_l = '<';
				quote_r = '>';
				break;
			}
		}
		if (!standard) {
			quote_l =
			quote_r = '"';
			if (*s == '.' && is_path_sep(s[1]))
				s += 2;
			else if ((t = strstr(s, GccLeaf)) != 0
			     &&  (t = strstr(t, IncLeaf)) != 0) {
			  	s = t+sizeof(IncLeaf)-1;
				quote_l = '<';
				quote_r = '>';
			}
		}
		return s;
	}
#endif
#define	CUR_FILE	strip_name(cur_file_name())

static	int	base_level;
static	char	*inc_stack[MAX_INC_DEPTH];

#ifdef	DEBUG
static
dump_stack(tag)
char	*tag;
{
	register int	j;
	printf("/* stack%s:%s", tag, cur_file_name());
	for (j = 0; j <= in_include; j++)
		printf("\n\t%d%s:%s", j,
			j == base_level ? "*" : "",
			inc_stack[j] ? inc_stack[j] : "?");
	printf(" */\n");
}
#endif	/* DEBUG */

static
void	free_inc_stack(n)
	int	n;
{
	if (inc_stack[n] != 0) {
		free(inc_stack[n]);
		inc_stack[n] = 0;
	}
}

static
void	make_inc_stack(n, s)
	int	n;
	char	*s;
{
	free_inc_stack(n);
	inc_stack[n] = xstrdup(s);
}

/*
 * Keep track of include-files so that we only include each once.
 */
static
int	already_included (path)
	char	*path;
{
	if (!include_list)
		include_list = new_symbol_table();
	if (find_symbol(include_list, path) != NULL)
		return TRUE;
	new_symbol(include_list, path, NULL, DS_NONE);
	return FALSE;
}

/*
 * Keep track of variables that may have been implicitly declared via
 * include-files so that we declare them only once in the lint library
 * output.
 */
int	already_declared (name)
	char	*name;
{
	if (declared_list == 0)
		declared_list = new_symbol_table ();
	if (find_symbol (declared_list, name) == 0) {
		(void)new_symbol (declared_list, name, 0, 0);
		return FALSE;
	}
	return TRUE;
}

/*
 * Initialize state for 'track_in()'
 */
static	int	InitTracking;
void	begin_tracking()
{
	InitTracking = FALSE;
}

static	int	c_suffix(path)
	char	*path;
{
	char	*last = path + strlen(path);
#ifdef	vms
	char	*vers = strrchr(path, ';');
	if (vers != 0)
		last = vers;
#endif
	return ((last - path) > 2 && !strcmp(last-2, ".c"));
}

/*
 * For lint-library, we want to keep track of what file we are in so that we
 * can generate appropriate comments and include-statements.
 *
 * The main program 'cproto' is invoked with 'cpp' once for each C-file,
 * relying on it to spit out "#" comments which identify the name and line
 * number of each processed file.  After the first '#' comment, all others
 * refer to included files.
 */
void	track_in()
{
	static	char	old_file[MAX_TEXT_SIZE];	/* from last call */
	auto	boolean	show = lintLibrary();

	if (!show && !debug_trace)
		return;

#ifdef	DEBUG
	printf("/* track_in: in_include=%d line_num=%d base_file=%s */\n",
		in_include, cur_line_num(), base_file);
	dump_stack("-before");
#endif	/* DEBUG */

	if (cur_line_num() == 0) {	/* begin new (nested?) file */
		if (!InitTracking) {
			InitTracking = TRUE;
			/* yacc may omit first cpp-line! */
			in_include =
			base_level = (strcmp(cur_file_name(), base_file) != 0);
			make_inc_stack(0, base_file);
		} else if (!strcmp(cur_file_name(), base_file)) {
			flush_varargs();
			in_include = 0;	/* reset level */
		} else {
			make_inc_stack(in_include, old_file);
			if (in_include++ == 0) {
				char	*s = CUR_FILE;
				if (show && !already_included(s)) {
					fmt_library(4);
					put_string (stdout, "#include ");
					put_char   (stdout, quote_l);
					put_string (stdout, s);
					put_char   (stdout, quote_r);
					put_newline(stdout);
				}
				if (debug_trace)
					fprintf(stderr, "++ %s\n", cur_file_name());
			}
			make_inc_stack(in_include, cur_file_name());
		}
		(void)strcpy(old_file, cur_file_name());
	} else if (!strcmp(cur_file_name(), base_file)) {
		in_include = 0;	/* kludgy bison! */
		(void)strcpy(old_file, cur_file_name());
	} else if (strcmp(old_file, cur_file_name())) { /* continue/unnest ? */
		int n, found;
		char *s = cur_file_name();
#ifdef DEBUG
		char temp[80];
#endif

		flush_varargs();
		for (n = in_include, found = FALSE; n >= 0; n--) {
			if (!strcmp(inc_stack[n], s)) {
				found = TRUE;
				in_include--;
				break;
			}
		}
		if (!found) {
			/*
			 * There's two kinds of broken programs that can cause
			 * us to lose sync at this point:  (1) programs such as
			 * yacc that don't reference the grammar file, instead
			 * referencing the generated file, and (2) broken
			 * preprocessors (such as on OSF/1) that neglect to
			 * report line #1 on headers that are rejected due to
			 * prior inclusion.
			 *
			 * If the source file's extension is ".h", we'll assume
			 * the latter case (i.e., just report it).  The former
			 * case requires that we reset the stack.
			 */
#ifdef DEBUG
			sprintf(temp, "/* lost sync @%d: ", cur_line_num()+1);
			put_blankline(stdout);
			put_string(stdout, temp);
			put_string(stdout, s);
			put_string(stdout, " */\n");
#endif
			if (in_include == 1 && c_suffix(s)) {
				/* yacc did it again! */
				in_include = 0;
				make_inc_stack(in_include, strcpy(base_file, s));
#ifdef DEBUG
				put_string(stdout, "/* processed ");
				put_string(stdout, s);
				put_string(stdout, " */\n");
#endif
			}
		}
		(void)strcpy(old_file, inc_stack[in_include]);
	}
#ifdef	DEBUG
	dump_stack("-after");
#endif	/* DEBUG */
}

/*
 * Copy/append to 'implied_buf[]'
 */
static
void	add2implied_buf(s,append)
	char	*s;
	int	append;
{
	static	unsigned
			implied_len,	/* current strlen(implied_buf) */
			implied_max;	/* maximum size of implied_buf */

	if (!append)
		implied_len = 0;
	implied_len += strlen(s);

	if (implied_buf == 0)
		*(implied_buf = malloc(implied_max = BUFSIZ)) = '\0';
	if (implied_max < implied_len + 2)
		implied_buf = realloc(implied_buf, implied_max += implied_len+2);
	if (!append)
		*implied_buf = '\0';
	(void)strcat(implied_buf, s);
}

/*
 * If the "-t" option is set (or if we are generating a lint-library), we
 * intercept tokens which are part of a typedef, copying them to the output.
 *
 * The 'imply_typedef()' entrypoint is called from the grammar for the special
 * cases of struct/union/enum when we expect to be getting curly-braces which
 * define the structure.  If no curly-braces are found by the end of the
 * rule, we can discard the buffer.
 */
int	want_typedef()
{
	if (lintLibrary()) {
		if (in_include == 0)
			return (TRUE);
	} else if (types_out) {
		return (TRUE);
	}
	return (FALSE);
}

void	begin_typedef()
{
	if (want_typedef()) {
		in_typedef = TRUE;
		fmt_library(1);
		copy_typedef("typedef");
	}
}

void	copy_typedef(s)
	char	*s;
{
	if (!strcmp(s, "/*")
	 || *s == '#')
		;	/* ignore */
	else if (in_typedef) {
		if (*s == '\n')
			put_newline(stdout);
		else
			put_string(stdout, s);
	} else if (implied_cnt > 0) {	/* "KEY ID {" ? */
		add2implied_buf(s,TRUE);
		if (!isspace(*s))
			implied_cnt--;
		if ((implied_cnt == 2 || implied_cnt == 1)
		&&  !strcmp(s, "{")) {
			implied_cnt = 9999;
		}
	}
}

void	end_typedef()
{
	copy_typedef("\n");
	in_typedef = FALSE;
	(void)implied_typedef();
}

void	imply_typedef(s)
	char	*s;
{
	if (!in_typedef && want_typedef()) {
		add2implied_buf(s,FALSE);
		implied_cnt = 3;
	}
}

char *	implied_typedef()
{
	if (implied_cnt > 0) {
		implied_cnt = 0;
		return (implied_buf);
	}
	return (0);
}

/*
 * Indent lint-library stuff to make it readable
 */
void	indent(outf)
	FILE	*outf;
{
	put_string(outf, "\n\t\t");
}

/* Test for the special case of an ellipsis-parameter when trying to make a
 * lint-library
 */
int	lint_ellipsis(p)
	Parameter	*p;
{
	return (   knrLintLibrary()
		&& (!strcmp(p->declarator->name, ELLIPSIS)));
}

/*
 * Reset the data used for "VARARGS" comment.  Actually, reset almost any
 * special attribute that's attached to a function, so we don't accidentally
 * propagate it to the next function (or data) to be output.
 */
void	flush_varargs()
{
	exitlike_func = FALSE;

	varargs_num = 0;
	if (varargs_str != 0) {
		free(varargs_str);
		varargs_str = 0;
	}
}

/* If either we received a "VARARGS" comment in the lexical scanner, or if the
 * parameter list contains an ellipsis, generate a corresponding "VARARGS"
 * comment for lint-library output.
 */
void	ellipsis_varargs(d)
	Declarator	*d;
{
	int		count;
	Parameter	*p;

	fmt_library(0);
	for (p = d->params.first, count = 0; p != 0; p = p->next, count++)
		if (lint_ellipsis(p)) {
			varargs_num = count;
			break;
		}
	if (varargs_num != 0) {
		put_string(stdout, "\t/* VARARGS");
		if (varargs_num > 0) {
			printf("%d", varargs_num);
			if (varargs_str != 0) {
				put_char(stdout, ' ');
				put_string(stdout, varargs_str);
			}
		}
		flush_varargs();
		put_string(stdout, " */\n");
	}
}

/* (Attempt to) create a parameter name for lint-library applications in which
 * we are starting from a function prototype which has no explicit parameter
 * name.
 */
char *	supply_parm(count)
	int	count;
{
	static	char	temp[80];
	(void)sprintf(temp, "p%d", count);
	while (is_typedef_name(temp) && (strlen(temp) < sizeof(temp)-1))
		(void)strcat(temp, "_");
	return (temp);
}

/*
 * (Attempt to) distinguish between declarators for functions and for
 * function pointers.
 */
int	is_actual_func (d)
	Declarator *d;
{
	if (lintLibrary() && (d->func_def != FUNC_NONE)) {
		if (d->func_stack->text[0] == PAREN_L) {
			if (strstr(d->func_stack->text, "()") != 0)
		 		return TRUE;
		} else {
			return TRUE;
		}
	}
	return FALSE;
}

/*
 * Output the body (or terminating semicolon) of a procedure
 */
void	put_body(outf, decl_spec, declarator)
	FILE		*outf;
	DeclSpec	*decl_spec;	/* declaration specifier */
	Declarator	*declarator;
{
    register char	*spec_text;

    if (is_actual_func(declarator)) {
	strcut(decl_spec->text, "static");
	strcut(decl_spec->text, "extern");
	indent(outf);
	put_char(outf, CURL_L);
	if (!*(spec_text = decl_spec->text))
		spec_text = "void";
	if (exitlike_func) {
	    put_string(outf, " for(;;); /* no return */ ");
	} else if (!strcmp(spec_text, "void")
	 && declarator->text[0] != '*'
	 && declarator->func_stack->func_def == FUNC_NONE) {
	    put_string(outf, " /* void */ ");
	} else {
	    put_string(outf, " return(*(");
	    if (declarator->func_stack->func_def == FUNC_NONE) {
		put_string(outf, spec_text);
		put_char(outf, ' ');
		if (declarator->pointer) {
			char *s = declarator->text;
			while (*s++ == '*')
				put_char(outf, '*');
		}
		put_char(outf, '*');
	    } else {
		put_string(outf, spec_text);
		put_string(outf, "(*)()");
	    }
	    put_string(outf, ")0); ");
	}
	put_char(outf, CURL_R);
    } else {
	/* SVR4 lint complains if we declare const data w/o some initializer.
	 */
	if (strkey(decl_spec->text, "const") != NULL
	 || strkey(declarator->text, "const") != NULL)
	    put_string(outf, " = {0}");
	put_string(outf, ";");
    }
    put_newline(outf);
    exitlike_func = FALSE;
}

#ifdef NO_LEAKS
void
free_lintlibs()
{
    register int n;
    if (implied_buf != 0)
    	free(implied_buf);
    for (n = 0; n < MAX_INC_DEPTH; n++)
    	free_inc_stack(n);
    if (include_list != 0)
    	free_symbol_table(include_list);
    if (declared_list != 0)
    	free_symbol_table(declared_list);
}
#endif

#endif	/* OPT_LINTLIBRARY */

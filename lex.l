%{
/* $Id: lex.l,v 3.11 1993/06/09 18:08:36 tom Exp $
 *
 * Lexical analyzer for C function prototype generator
 *
 *	This is designed to parse lexically at the top level (e.g., of extern
 *	objects such as procedures).  The corresponding yacc-grammar expects
 *	that curly-braces (for function bodies) are recognized as a single
 *	token, BRACES.  Similarly, square-brackets and their contents are
 *	passed back as BRACKETS.
 *
 *	Assignments at the top level are data-initialization statements.
 *	These are returned as INITIALIZER.
 *
 *	The logic here recognizes tokens inside curly-braces, but does not
 *	pass them back to the grammar.
 *
 * Apollo extensions:
 *	"&" is ignored because we ignore expressions of all kinds.  Note that
 *	function-prototypes may use "&" to denote reference-parameters.  By
 *	ignoring that as well, we make the output compatible with lint (kludge).
 *
 *	Similarly, ignore "std_$call", since it is not compatible with lint.
 *
 *	CPP_INLINE handles a special case of the Apollo CC 6.7 compiler that
 *	uses inline attribute declarations, e.g.
 *
 *		int	foo #attribute[aligned(1)];
 *
 *	In CC 6.8 this behavior is hidden by a macro.
 *
 * VAX/VMS extensions:
 *	Treat the keywords 'globalref', etc., as 'extern'.
 *
 *	The keywords 'noshare' and 'readonly' are type-qualifiers.
 */

#define	result(nn)	count(); if (!brackets && unnested()) return(nn)

#define	is_IDENTIFIER	    save_text_offset();\
			    return type_of_name(yytext);

#ifdef	apollo
#define	apollo_keyword
#define	apollo_special	absorb_special()
#else
#define	apollo_keyword	is_IDENTIFIER
#define	apollo_special	is_IDENTIFIER
#endif

#ifdef	vms
#define	vms_extern	save_text_offset(); return(T_EXTERN);
#define	vms_keyword	save_text_offset();
#else	/* unix */
#define	vms_extern	is_IDENTIFIER
#define	vms_keyword	is_IDENTIFIER
#endif	/* vms/unix */

int	varargs_num;		/* code to save "VARARGS" */
int	debug_trace;		/* true if we trace token-level stuff */
char	base_file[BUFSIZ];	/* top-level file name */

static int save_cpp;	/* true if cpp-text within curly braces */
static int in_cpp;	/* true while we are within cpp-text */
static int curly;	/* number of curly brace nesting levels */
static int ly_count;	/* number of occurances of %% */

#ifdef FLEX_SCANNER
/* flex scanner state */
static YY_BUFFER_STATE buffer_stack[MAX_INC_DEPTH];
#endif

static int inc_depth;				/* include nesting level */
static IncludeStack inc_stack[MAX_INC_DEPTH];	/* stack of included files */
static SymbolTable *included_files;		/* files already included */

static int  type_of_name();
static void StartCpp();
static void FinishCpp();
static void update_line_num();
static void save_text();
static void save_text_offset();
static void get_comment();
static void get_cpp_directive();
static void do_include();

extern void yyerror();
#ifdef yywrap
#undef yywrap
#endif
%}

WS		[ \t]
LETTER		[A-Za-z$_]
DIGIT		[0-9]
ID		{LETTER}({LETTER}|{DIGIT})*
STRING		\"(\\.|\\\n|[^"\\])*\"
QUOTED		({STRING}|\'(\\\'|[^'\n])*\'|\\.)

%s CPP1 INIT1 INIT2 CURLY LEXYACC ASM CPP_INLINE
%%

\n			{ save_text(); cur_file->line_num++;
			    cur_declarator = NULL; }

"/*"			{ save_text(); get_comment(); }
"//".*$ 		save_text();

"&"			{ /* ignored (apollo) */ }

<LEXYACC>^"%%"		{ save_text(); if (++ly_count >= 2) BEGIN INITIAL; }
<LEXYACC>^"%{"		{ save_text(); BEGIN INITIAL; }
<LEXYACC>{QUOTED}	{ save_text(); update_line_num(); }
<LEXYACC>.		save_text();
<INITIAL>^"%}"		{ save_text(); BEGIN LEXYACC; }

<INITIAL>#{WS}* 	{ save_text(); StartCpp(0); }
<INITIAL>"??="{WS}* 	{ save_text(); StartCpp(0); }

<CPP1>attribute		{ BEGIN CPP_INLINE; /* apollo */}
<CPP1>options		{ BEGIN CPP_INLINE; /* apollo */}
<CPP_INLINE>[^;]*	FinishCpp();

<CPP1>define{WS}+{ID}	{
			    char name[MAX_TEXT_SIZE], value[MAX_TEXT_SIZE];

			    save_text();
			    sscanf(yytext, "define %s", name);
			    get_cpp_directive(buf, sizeof(buf));
			    sscanf(buf, "%s", value);
			    new_symbol(define_names, name, value, DS_NONE);
			}

<CPP1>include{WS}*	{
			    save_text();
			    get_cpp_directive(buf, sizeof(buf));
			    if (buf[0] != '"' && buf[0] != '<') {
				Symbol *sym = find_symbol(define_names, buf);
				if (sym != NULL && sym->value != NULL) {
				    strcpy(buf, sym->value);
				} else {
				    buf[0] = '\0';
				}
			    }
			    if (buf[0] != '\0')
				do_include(buf);
			}

<CPP1>line{WS}+[0-9]+{WS}+\".*$  {
			    save_text();
			    sscanf(yytext, "line %d \"%[^\"]\"",
			     &cur_file->line_num, cur_file->file_name);
			    cur_file->line_num--;
			    track_in();
			    FinishCpp();
			}
<CPP1>[0-9]+{WS}+\".*$  {
			    save_text();
			    sscanf(yytext, "%d \"%[^\"]\"", &cur_file->line_num,
			     cur_file->file_name);
			    cur_file->line_num--;
			    track_in();
			    FinishCpp();
			}
<CPP1>[0-9]+.*$ 	{
			    save_text();
			    sscanf(yytext, "%d ", &cur_file->line_num);
			    cur_file->line_num--;
			    track_in();
			    FinishCpp();
			}

<CPP1>. 		{ save_text(); get_cpp_directive(NULL, 0); }

<INITIAL>"("		{ save_text_offset(); return '('; }
<INITIAL>")"		{
			    save_text();
			    if (cur_file->convert)
				cur_file->begin_comment =
				 ftell(cur_file->tmp_file);
			    return ')';
			}
<INITIAL>"*"		{ save_text_offset(); return '*'; }
<INITIAL>","		{ save_text(); return ','; }
<INITIAL>";"		{
			    save_text();
			    if (cur_file->convert)
				cur_file->begin_comment =
				 ftell(cur_file->tmp_file);
			    return ';';
			}
<INITIAL>"..."		{ save_text(); return T_ELLIPSIS; }
<INITIAL>{STRING}	{
			    save_text();
			    update_line_num();
			    return T_STRING_LITERAL;
			}

<INITIAL>asm		{ save_text(); BEGIN ASM; return T_ASM; }
<ASM>"("		save_text();
<ASM>")"		{ save_text(); BEGIN INITIAL; return T_ASMARG; }
<ASM>{QUOTED} 		{ save_text(); update_line_num(); }
<ASM>. 			save_text();

<INITIAL>__?based[^(]*\([^)]*\)	{ save_text_offset(); return T_TYPE_QUALIFIER; }

<INITIAL>auto		{ save_text_offset(); return T_AUTO; }
<INITIAL>extern 	{ save_text_offset(); return T_EXTERN; }
<INITIAL>register	{ save_text_offset(); return T_REGISTER; }
<INITIAL>static 	{ save_text_offset(); return T_STATIC; }
<INITIAL>typedef	{ save_text_offset(); return T_TYPEDEF; }
<INITIAL>inline 	{ save_text_offset(); return T_INLINE; }
<INITIAL>char		{ save_text_offset(); return T_CHAR; }
<INITIAL>double 	{ save_text_offset(); return T_DOUBLE; }
<INITIAL>float		{ save_text_offset(); return T_FLOAT; }
<INITIAL>int		{ save_text_offset(); return T_INT; }
<INITIAL>void		{ save_text_offset(); return T_VOID; }
<INITIAL>long		{ save_text_offset(); return T_LONG; }
<INITIAL>short		{ save_text_offset(); return T_SHORT; }
<INITIAL>signed 	{ save_text_offset(); return T_SIGNED; }
<INITIAL>unsigned	{ save_text_offset(); return T_UNSIGNED; }
<INITIAL>enum		{ save_text_offset(); return T_ENUM; }
<INITIAL>struct 	{ save_text_offset(); return T_STRUCT; }
<INITIAL>union		{ save_text_offset(); return T_UNION; }
<INITIAL>va_dcl		{ save_text_offset(); return T_VA_DCL; }

<INITIAL>globalvalue	{ vms_extern; }
<INITIAL>globalref	{ vms_extern; }
<INITIAL>globaldef	{ vms_extern; }

<INITIAL>"std_$call"	{ apollo_keyword; }
<INITIAL>"__attribute"	{ apollo_special; }

<INITIAL>{ID}		{ is_IDENTIFIER }

<INITIAL>\[[^\]]*\]	{
			    /* This can't handle the case where a comment
			     * containing a ] appears between the brackets.
			     */
			    save_text_offset();
			    update_line_num();
			    return T_BRACKETS;
			}
<INITIAL>"??("[^?]*"??)"	{
			    save_text_offset();
			    update_line_num();
			    return T_BRACKETS;
			}

<INITIAL>"="		{ save_text(); BEGIN INIT1; return '='; }
<INIT1>"{"		{ save_text(); curly = 1; BEGIN INIT2; }
<INIT1>[,;]		{
			    unput(yytext[yyleng-1]);
			    BEGIN INITIAL;
			    return T_INITIALIZER;
			}
<INIT1>{QUOTED}		{ save_text(); update_line_num(); }
<INIT1>.		save_text();

<INIT2>"{"		{ save_text(); ++curly; }
<INIT2>"}"		{
			    save_text();
			    if (--curly == 0) {
				BEGIN INITIAL;
				return T_INITIALIZER;
			    }
			}
<INIT2>{QUOTED}		{ save_text(); update_line_num(); }
<INIT2>.		save_text();

<INITIAL>"{"		{
			    save_text();
			    curly = 1;
			    return_val =
			    returned_at = FALSE;
			    BEGIN CURLY;
			    return T_LBRACE;
			}
<CURLY>"{"		{ save_text(); ++curly; }
<CURLY>"}"		{
			    save_text();
			    if (--curly == 0) {
				BEGIN INITIAL;
				return T_MATCHRBRACE;
			    }
			}
<CURLY>{QUOTED}		{ save_text(); update_line_num(); }
<CURLY>"return"		{ save_text(); returned_at = TRUE; }
<CURLY>";"		{ save_text(); returned_at = FALSE; }
<CURLY>#{WS}* 		{ save_text(); StartCpp(1); }
<CURLY>"??="{WS}* 	{ save_text(); StartCpp(1); }
<CURLY>.		{ save_text(); return_val |= returned_at; }

[ \t\f]+		save_text();
.			{
			    save_text();
			    put_error();
			    fprintf(stderr, "bad character '%c'\n", yytext[0]);
			}
%%

static void
StartCpp(level)
int	level;
{
    save_cpp = level;
    in_cpp = TRUE;
    BEGIN CPP1;
}

static void
FinishCpp()
{
    in_cpp = FALSE;
    if (save_cpp)
    	BEGIN CURLY;
    else
	BEGIN INITIAL;
}

#ifdef	apollo
static
absorb_special()
{
	int	c;
	int	nest	= 0;
	while ((c = input()) > 0) {
		if (c == '(')
			nest++;
		else if (c == ')') {
			if (--nest <= 0)
				break;
		}
	}
}
#endif

/* Decode the current token according to the type-of-name
 */
static int
type_of_name(name)
char *name;
{
    if (find_symbol(type_qualifiers, name) != NULL)
	return T_TYPE_QUALIFIER;
    else if (find_symbol(typedef_names, name) != NULL)
	return T_TYPEDEF_NAME;
    else if (find_symbol(define_names, name) != NULL)
	return T_DEFINE_NAME;
    else
	return T_IDENTIFIER;
}

boolean	is_typedef_name (name)
	char	*name;
{
    return (find_symbol(typedef_names, name) != NULL);
}

/* If the matched text contains any new line characters, then update the
 * current line number.
 */
static void
update_line_num ()
{
    char *p = yytext;
    while (*p != '\0') {
	if (*p++ == '\n')
	    cur_file->line_num++;
    }
}

/* Save the matched text in the temporary file.
 */
static void
save_text ()
{
    if (!in_cpp)
    	copy_typedef(yytext);
    if (cur_file->convert) {
	fputs(yytext, cur_file->tmp_file);
    }
}

/* Record the current position in the temporary file and write the matched text
 * to the file.
 */
static void
save_text_offset ()
{
    copy_typedef(strcpy(yylval.text.text, yytext));
    if (cur_file->convert) {
	yylval.text.begin = ftell(cur_file->tmp_file);
	fputs(yytext, cur_file->tmp_file);
    }
}

/* Decipher comments that are useful for lint (and making lint-libraries)
 */
static	struct	{
	int	Varargs;
	int	Externs;
	int	Preproz;
	} CmtVal;

static int
decipher_comment (keyword, len)
char	*keyword;
int	len;
{
	if (len != 0) {
		keyword[len] = '\0';

		if (!strcmp(keyword, "VARARGS"))
			CmtVal.Varargs = -1;
		else if (sscanf(keyword, "VARARGS%d", &CmtVal.Varargs) == 1)
			;
		else if (!strcmp(keyword, "LINT_EXTERN"))
			CmtVal.Externs = MAX_INC_DEPTH;
		else if (sscanf(keyword, "LINT_EXTERN%d", &CmtVal.Externs) == 1)
			;
		else if (!strcmp(keyword, "LINT_PREPRO"))
			CmtVal.Preproz = 1;
		else if (sscanf(keyword, "LINT_PREPRO%d", &CmtVal.Preproz) == 1)
			;
	}
	return 0;
}

/* Scan to end of comment.
 */
static void
get_comment ()
{
    char keyword[BUFSIZ];
    int c, lastc = '\0', len = 0;

    keyword[len] = '\0';
    CmtVal.Varargs = 0;
    CmtVal.Externs = -1;
    CmtVal.Preproz = 0;

    while ((c = input()) != 0) {
	if (cur_file->convert)
	    fputc(c, cur_file->tmp_file);

	if (CmtVal.Preproz > 0)
	    fputc(c, stdout); 
	if (isspace(c))
	    len = decipher_comment(keyword, len);
	else if (len+1 < sizeof(keyword))
	    keyword[len++] = c;

	switch (c) {
	case '\n':
	    cur_file->line_num++;
	    CmtVal.Preproz -= 1;
	    break;
	case '/':
	    if (lastc == '*') {
		if (cur_file->convert) {
		    if (func_params && cur_declarator) {
			cur_declarator->begin_comment = cur_file->begin_comment;
			cur_file->begin_comment = ftell(cur_file->tmp_file);
			cur_declarator->end_comment = cur_file->begin_comment;
			cur_declarator = NULL;
		    } else {
			cur_file->end_comment = ftell(cur_file->tmp_file);
		    }
		}
		(void)decipher_comment(keyword, len);
		if (CmtVal.Varargs != 0)
		    varargs_num = CmtVal.Varargs;
		if (CmtVal.Externs != -1)
		    extern_in = CmtVal.Externs;
		return;
	    }
	}
	lastc = c;
    }
}

/* Scan rest of preprocessor directive.  If <dest> is not NULL, then store
 * the text in the buffer pointed to by <dest> having size <n>.
 */
static void
get_cpp_directive (dest, n)
char *dest;		/* buffer to store directive text */
unsigned n;		/* size of buffer to store directive text */
{
    char c, lastc[4];
    
    lastc[0] = lastc[1] = lastc[2] = lastc[3] = '\0';
    if (dest != NULL)
	*dest = '\0';

    while ((c = input()) != 0) {
	if (cur_file->convert)
	    fputc(c, cur_file->tmp_file);

	switch (c) {
	case '\n':
	    cur_file->line_num++;
	    if (lastc[2] != '\\' && strcmp(lastc, "?\?/") != 0) {
		FinishCpp();
		return;
	    }
	    break;
	case '*':
	    if (lastc[2] == '/')
		get_comment();
	    break;
	}
	lastc[0] = lastc[1];
	lastc[1] = lastc[2];
	lastc[2] = c;

	if (dest != NULL && n > 1) {
	    *dest++ = c;
	    *dest = '\0';
	    --n;
	}
    }
}

/* Return a pointer to the current file name.
 */
char *
cur_file_name ()
{
    return cur_file->file_name;
}

/* Return the current line number.
 */
unsigned
cur_line_num ()
{
    return cur_file->line_num;
}

/* Return the current temporary output file.
 */
FILE *
cur_tmp_file ()
{
    return cur_file->tmp_file;
}

/* Set the modify flag for the current file.
 */
void
cur_file_changed ()
{
    cur_file->changed = TRUE;
}

/* Return the temporary file offset of beginning of the current comment.
 */
long
cur_begin_comment ()
{
    return cur_file->begin_comment;
}

/* Return the text of the current lexical token.
 */
char *
cur_text ()
{
    return yytext;
}

#ifdef NEED_tmpfile
/*
 * tmpfile() - return a FILE* for a temporary file that will be
 * removed automatically when the program exits.
 * 
 * Not all systems have the ANSI tmpfile() function yet...
 *
 * DaviD W. Sanderson (dws@cs.wisc.edu)
 */
FILE *
tmpfile ()
{
    char name[MAX_TEXT_SIZE];
    char *tmpdir;
    FILE *f;

    if ((tmpdir = getenv("TMPDIR")) == (char *)0)
    {
	tmpdir = "/tmp";
    }
    sprintf(name, "%s/TfXXXXXX", tmpdir);
    mktemp(name);

    if ((f = fopen(name, "w+")) == (FILE *)0)
    {
	return (FILE *)0;
    }

    if (unlink(name) == -1)
    {
	fclose(f);
	return (FILE *)0;
    }

    return f;
}
#endif /* NEED_tmpfile */

/* Push a file onto the include stack.	The stream yyin must already
 * point to the file.
 */
static void
include_file (name, convert)
char *name;		/* file name */
boolean convert;	/* if TRUE, convert function definitions */
{
    ++inc_depth;
    cur_file = inc_stack + inc_depth;
    cur_file->file = yyin;
    cur_file->base_name = xstrdup(name);
    cur_file->file_name = strcpy(xmalloc(MAX_TEXT_SIZE), name);
    cur_file->line_num = 1;
    cur_file->convert = convert;
    cur_file->changed = FALSE;

#ifdef FLEX_SCANNER
    buffer_stack[inc_depth] = yy_create_buffer(yyin, YY_BUF_SIZE);
    yy_switch_to_buffer(buffer_stack[inc_depth]);
#endif

    if (convert) {
	cur_file->begin_comment = cur_file->end_comment = 0;
	cur_file->tmp_file = tmpfile();
	if (cur_file->tmp_file == NULL) {
	    fprintf(stderr, "%s: cannot create temporary file\n", progname);
	    cur_file->convert = FALSE;
	}
    }
}

#define BLOCK_SIZE 2048

/* Copy converted C source from the temporary file to the output stream.
 */
static void
put_file (outf)
FILE *outf;
{
    char block[BLOCK_SIZE];
    long filesize;
    unsigned nread, count;

    filesize = ftell(cur_file->tmp_file);
    fseek(cur_file->tmp_file, 0L, 0);
    while (filesize > 0) {
	count = (filesize < BLOCK_SIZE) ? (unsigned)filesize : BLOCK_SIZE;
	nread = fread(block, sizeof(char), count, cur_file->tmp_file);
	if (nread == 0)
	    break;
	fwrite(block, sizeof(char), nread, outf);
	filesize -= nread;
    }
}

/* Remove the top of the include stack.
 */
void
pop_file ()
{
    FILE *outf;

    if (yyin != stdin)
	fclose(yyin);

    if (cur_file->convert) {
	if (yyin == stdin) {
	    put_file(stdout);
	} else if (cur_file->changed) {
	    if ((outf = fopen(cur_file->base_name, "w")) != NULL) {
		put_file(outf);
		fclose(outf);
	    } else {
		fprintf(stderr, "%s: cannot create file %s\n", progname,
		 cur_file->base_name);
	    }
	}

	fclose(cur_file->tmp_file);
    }
    free(cur_file->base_name);
    free(cur_file->file_name);

#ifdef FLEX_SCANNER
    yy_delete_buffer(YY_CURRENT_BUFFER);
#endif

    if (--inc_depth >= 0) {
	cur_file = inc_stack + inc_depth;
	yyin = cur_file->file;

#ifdef FLEX_SCANNER
	yy_switch_to_buffer(buffer_stack[inc_depth]);
#endif
    }
}

/* Process include directive.
 */
static void
do_include (file_spec)
char *file_spec; 	/* path surrounded by "" or <> */
{
    int stdinc; 	/* 1 = path surrounded by <> */
    char file[MAX_TEXT_SIZE], path[MAX_TEXT_SIZE];
    char match, *s;
    int i;
    unsigned n;
    FILE *fp;

    if (inc_depth >= MAX_INC_DEPTH-1) {
	put_error();
	fprintf(stderr, "includes too deeply nested\n");
	return;
    }

    if (file_spec[0] == '"') {
	match = '"';
	stdinc = 0;
    } else if (file_spec[0] == '<') {
	match = '>';
	stdinc = 1;
    } else {
	return;
    }
    s = strchr(file_spec+1, match);
    n = (s != NULL) ? (unsigned)(s - file_spec - 1) : 0;
    strncpy(file, file_spec+1, (SIZE_T)n);
    file[n] = '\0';

    /* Do nothing if the file was already included. */
    sprintf(path, stdinc ? "<%s>" : "\"%s\"", file);
    if (find_symbol(included_files, path) != NULL)
	return;
    new_symbol(included_files, path, NULL, DS_NONE);

    for (i = stdinc != 0; i < num_inc_dir; ++i) {
	if (strlen(inc_dir[i]) == 0) {
	    strcpy(path, file);
	} else {
	    sprintf(path, "%s/%s", inc_dir[i], file);
	}
	if ((fp = fopen(path, "r")) != NULL) {
	    yyin = fp;
	    include_file(path, func_style != FUNC_NONE && !stdinc);
	    return;
	}
    }

    if (!quiet) {
	put_error();
	fprintf(stderr, "cannot read file %s\n", file_spec);
    }
}

/* When the end of the current input file is reached, pop a
 * nested include file.
 */
int
yywrap ()
{
    if (inc_depth > 0) {
	pop_file();
	return 0;
    } else {
	return 1;
    }
}

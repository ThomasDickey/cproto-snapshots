/* $Id: cproto.c,v 3.20 1994/08/14 22:16:23 tom Exp $
 *
 * C function prototype generator and function definition converter
 */
static char rcsid[] = "$Id: cproto.c,v 3.20 1994/08/14 22:16:23 tom Exp $";

#include <stdio.h>
#include <ctype.h>
#include "cproto.h"
#include "patchlev.h"

/* getopt declarations */
#if HAVE_GETOPT_H
#include <getopt.h>
#else
extern int getopt ARGS((int argc, char *const *argv, const char *shortopts));
extern char *optarg;
extern int optind;
#endif

/* Name of the program */
char progname[] = "cproto";

/* Program options */

/* If nonzero, output variables declared "extern" in include-files */
int extern_in = 0;

/* If TRUE, output "extern" before global declarations */
boolean extern_out = FALSE;

/* If TRUE, generate static declarations */
boolean static_out = FALSE;

/* If TRUE, export typedef declarations (needed for lint-libs) */
#if OPT_LINTLIBRARY
boolean types_out = FALSE;
#endif

/* If TRUE, generate variable declarations */
boolean variables_out = FALSE;

/* If TRUE, enable formal parameter promotion */
boolean promote_param = TRUE;

/* Style of function prototype to generate */
PrototypeStyle proto_style = PROTO_ANSI;

/* Function definition style converted to */
FuncDefStyle func_style = FUNC_NONE;

/* If TRUE, put guard macro around prototype parameters */
boolean proto_macro = FALSE;

/* Name of macro to guard prototypes */
char *macro_name = "P_";

/* If TRUE, output prototype macro definition */
boolean define_macro = TRUE;

/* If TRUE, output comments in prototypes */
boolean proto_comments = FALSE;

/* If TRUE, output comments naming source files */
boolean file_comments = TRUE;

/* Conditional compilation directive output in front of function definitions */
char *func_directive = "#ifdef ANSI_FUNC";

/* Output formats for function declarators */
FuncFormat fmt[] = {
    /* miscellaneous function declarator */
    { "", " ", "", "", " ", "" },
    /* prototype */
    { "", " ", "", "", " ", "" },
    /* function definition */
    { "", "\n", " ", "", " ", "" },
    /* function definition with parameter comments */
    { "", "\n", " ", "\n    ", "\n    ", "\n" },
};

/* If TRUE, don't output message if unable to read an include file */
boolean quiet = FALSE;

/* Include file directories */
#ifdef MSDOS
# ifdef __TURBOC__
int num_inc_dir = 2;
char *inc_dir[MAX_INC_DIR] = { "" , "/tc/include" };
# else
int num_inc_dir = 1;
char *inc_dir[MAX_INC_DIR] = { "" };
# endif
#else
# ifdef vms
int num_inc_dir = 2;
char *inc_dir[MAX_INC_DIR] = { "[]", "sys$library:" };
# else
int num_inc_dir = 2;
char *inc_dir[MAX_INC_DIR] = { "", "/usr/include" };
# endif
#endif

/* Run the C preprocessor */
#ifdef CPP
extern	FILE *	popen  ARGS((const char *c, const char *m));
extern	int	pclose ARGS((FILE *p));
static char *cpp = CPP, *cpp_opt, *cpp_cmd;
#endif

static	char *	escape_string   ARGS((char *src));
static	void	usage           ARGS((void));
static	void	process_options ARGS((int *pargc, char ***pargv));
static	void	parse_options   ARGS((char *src, int maxargc, int *pargc, char **argv));
	int	main            ARGS((int argc, char **argv));

/* Try to allocate some memory.
 * If unsuccessful, output an error message and exit.
 */
#ifdef NO_LEAKS
char *xMalloc(n,f,l) unsigned n; char *f; int l;
#else
char *xmalloc (n) unsigned n;
#endif
{
    char *p;
#if HAVE_LIBDBMALLOC
    p = debug_malloc(f, l, n);
#else
    p = malloc(n);
#endif

    if (p == NULL) {
	fprintf(stderr, "%s: out of memory (cannot allocate %d bytes)\n",
		progname, n);
	exit(FAIL);
    }
    *p = '\0';
    return p;
}

/* Copy the string into allocated memory.
 * If unsuccessful, output an error message and exit.
 */
#ifdef NO_LEAKS
char *xStrdup(src, f, l) char *src; char *f; int l;
#else
char *xstrdup (src) char *src;
#endif
{
#if defined(NO_LEAKS)
    return strcpy(xMalloc(strlen(src)+1, f, l), src);
#else
    return strcpy(xmalloc(strlen(src)+1), src);
#endif
}

/* Output the current source file name and line number.
 */
void
put_error ()
{
    fprintf(stderr, "\"%s\", line %u: ", cur_file_name(), cur_line_num());
}

/* Scan for options from a string.
 */
static void
parse_options (src, maxargc, pargc, argv)
char *src;
int maxargc, *pargc;
char **argv;
{
    char *g, *p, c;
    int argc;

    argc = 0;
    g = xstrdup(src);
    c = *g;
    while (c != '\0' && argc < maxargc) {
	while (c == ' ' || c == '\t')
	    c = *++g;
	if (c == '\0')
	    break;
	argv[argc++] = g;

	p = g;
	while (1) {
	    if (c == ' ' || c == '\t' || c == '\0') {
		*p = '\0';
		break;
	    } else if (c == '"') {
		while (1) {
		    c = *++g;
		    if (c == '"') {
			c = *++g;
			break;
		    } else if (c == '\0') {
			break;
		    } else {
			*p++ = c;
		    }
		}
	    } else {
		*p++ = c;
		c = *++g;
	    }
	}
	if (c != '\0')
	    c = *++g;
    }

    *pargc = argc;
}

/* Replace any character escape sequences in a string with the actual
 * characters.	Return a pointer to malloc'ed memory containing the result.
 * This function knows only a few escape sequences.
 */
static char *
escape_string (src)
char *src;
{
    char *result, *get, *put;

    result = xstrdup(src);
    put = result;
    get = src;
    while (*get != '\0') {
	if (*get == '\\') {
	    switch (*(++get)) {
	    case 'n':
		*put++ = '\n';
		++get;
		break;
	    case 's':
		*put++ = ' ';
		++get;
		break;
	    case 't':
		*put++ = '\t';
		++get;
		break;
	    default:
		if (*get != '\0')
		    *put++ = *get++;
	    }
	} else {
	    *put++ = *get++;
	}
    }
    *put = *get;
    return result;
}

/*
 * Returns true iff the character is a pathleaf separator
 */
int
is_path_sep (ch)
int ch;
{
    return (ch == '/' || ch == '\\');
}

/* Trim any path name separator from the end of the string.
 * Return a pointer to the string.
 */
char *
trim_path_sep (s)
char *s;
{
    int n;

    n = strlen(s);
    if (n > 0) {
	if (is_path_sep(s[n-1]))
	    s[n-1] = '\0';
    }
    return s;
}

/* Output usage message and exit.
 */
static void
usage ()
{
    fprintf(stderr, "usage: %s [ option ... ] [ file ... ]\n", progname);
    fputs("Options:\n", stderr);
    fputs("  -a, -t           Convert function definitions to ANSI or traditional style\n", stderr);
    fputs("  -b               Rewrite function definitions in both styles\n", stderr);
    fputs("  -c               Enable comments in prototype parameters\n", stderr);
    fputs("  -e               Output \"extern\" keyword before global declarations\n", stderr);
    fputs("  -f n             Set function prototype style (0 to 3)\n", stderr);
#if OPT_LINTLIBRARY
    fputs("  -l               Generate output in lint-library style\n", stderr);
#endif
    fputs("  -o file          Redirect output to file\n", stderr);
    fputs("  -O file          Redirect errors to file\n", stderr);
    fputs("  -p               Disable formal parameter promotion\n", stderr);
    fputs("  -q               Disable include file read failure messages\n", stderr);
    fputs("  -s               Output static declarations\n", stderr);
#if OPT_LINTLIBRARY
    fputs("  -T               Output type definitions\n", stderr);
#endif
    fputs("  -v               Output variable declarations\n", stderr);
    fputs("  -x               Output variables and functions declared \"extern\"\n", stderr);
    fputs("  -m               Put macro around prototype parameters\n", stderr);
    fputs("  -M name          Set name of prototype macro\n", stderr);
    fputs("  -d               Omit prototype macro definition\n", stderr);
    fputs("  -P template      Set prototype format template \" int f (a, b)\"\n", stderr);
    fputs("  -F template      Set function definition format template \" int f (a, b)\"\n", stderr);
    fputs("  -C template      Set format for function definition with parameter comments\n", stderr);
    fputs("  -D name[=value]  Define C preprocessor symbol\n", stderr);
    fputs("  -U name          Undefine C preprocessor symbol\n", stderr);
    fputs("  -I directory     Add #include search directory\n", stderr);
    fputs("  -E command       Run specified C preprocessor command\n", stderr);
    fputs("  -E 0             Do not run any C preprocessor\n", stderr);
    fputs("  -V               Print version information\n", stderr);
    exit(FAIL);
}

#ifdef	vms
static	char	*cpp_defines;
static	char	*cpp_include;
static	char	*cpp_undefns;

static	void	add2list ARGS((char *dst, char *src));
static	void	add_option ARGS((char *keyword, char *src));

static void
add2list(dst, src)
	char	*dst;
	char	*src;
{
	if (*dst)
		strcat(dst, ",");
	strcat(dst, src);
}

static void
add_option(keyword, src)
	char	*keyword;
	char	*src;
{
	if (*src)
		(void)sprintf(cpp_opt + strlen(cpp_opt),
			" /%s=(%s)", keyword, src);
}
#endif	/* vms */

#define MAX_OPTIONS 40

/* Process the command line options.
 */
static void
process_options (pargc, pargv)
int *pargc;
char ***pargv;
{
    int argc, eargc, nargc;
    char **argv, *eargv[MAX_OPTIONS], **nargv;
    int i, c;
    char *s;
#if defined(CPP)
    unsigned n;
#endif
#if defined(CPP) && !defined(vms)
    char tmp[MAX_TEXT_SIZE];
#endif

    argc = *pargc;
    argv = *pargv;
    if ((s = getenv("CPROTO")) != NULL) {
	parse_options(s, MAX_OPTIONS, &eargc, eargv);
	nargv = (char **)xmalloc((eargc+argc+1)*sizeof(char *));
	nargv[0] = argv[0];
	nargc = 1;
	for (i = 0; i < eargc; ++i)
	    nargv[nargc++] = eargv[i];
	for (i = 1; i < argc; ++i)
	    nargv[nargc++] = argv[i];
	nargv[nargc] = NULL;
	argc = nargc;
	argv = nargv;
    }

#ifdef CPP
    /* Allocate buffer for C preprocessor command line. */
    n = strlen(cpp) + 1;
    for (i = 0; i < argc; ++i) {
	n += strlen(argv[i]) + 1;
    }
#ifdef	vms
    cpp_include = xmalloc(n+argc);
    cpp_defines = xmalloc(n+argc);
    cpp_undefns = xmalloc(n+argc);
    n += 30;	/* for keywords */
#endif
    cpp_opt = xmalloc(n);
    cpp_cmd = xmalloc(n);
#endif

    while ((c = getopt(argc, argv, "aB:bC:cD:dE:eF:f:I:mM:P:pqstU:Vvo:Tlx")) != EOF) {
	switch (c) {
	case 'I':
#ifdef	vms
	    add2list(cpp_include, optarg);
	    break;
#else	/* unix */
	    if (num_inc_dir < MAX_INC_DIR) {
		inc_dir[num_inc_dir++] = trim_path_sep(xstrdup(optarg));
	    } else {
		fprintf(stderr, "%s: too many include directories\n",
		    progname);
	    }
#endif
		/*FALLTHRU*/
	case 'D':
#ifdef	vms
	    add2list(cpp_defines, optarg);
	    break;
#endif
		/*FALLTHRU*/
	case 'U':
#ifdef	vms
	    add2list(cpp_undefns, optarg);
	    break;
#else	/* UNIX, etc. */
#ifdef CPP
	    sprintf(tmp, " -%c%s", c, optarg);
	    strcat(cpp_opt, tmp);
#endif
#endif
	    break;

	case 'a':
	    func_style = FUNC_ANSI;
	    break;
	case 'B':
	    func_directive = optarg;
	    break;
	case 'b':
	    func_style = FUNC_BOTH;
	    break;
	case 'c':
	    proto_comments = TRUE;
	    break;
	case 'd':
	    define_macro = FALSE;
	    break;
	case 'E':
#ifdef CPP
	    if (strcmp(optarg, "0") == 0) {
		cpp = NULL;
	    } else {
		cpp = optarg;
	    }
#endif
	    break;
	case 'e':
	    extern_out = TRUE;
	    break;
	case 'C':
	case 'F':
	case 'P':
	    s = escape_string(optarg);
	    i = (c == 'C') ? FMT_FUNC_COMMENT :
		((c == 'F') ? FMT_FUNC : FMT_PROTO);

	    fmt[i].decl_spec_prefix = s;
	    while (*s != '\0' && isascii(*s) && !isalnum(*s)) ++s;
	    if (*s == '\0') usage();
	    *s++ = '\0';
	    while (*s != '\0' && isascii(*s) && isalnum(*s)) ++s;
	    if (*s == '\0') usage();

	    fmt[i].declarator_prefix = s;
	    while (*s != '\0' && isascii(*s) && !isalnum(*s)) ++s;
	    if (*s == '\0') usage();
	    *s++ = '\0';
	    while (*s != '\0' && isascii(*s) && isalnum(*s)) ++s;
	    if (*s == '\0') usage();

	    fmt[i].declarator_suffix = s;
	    while (*s != '\0' && *s != '(') ++s;
	    if (*s == '\0') usage();
	    *s++ = '\0';

	    fmt[i].first_param_prefix = s;
	    while (*s != '\0' && isascii(*s) && !isalnum(*s)) ++s;
	    if (*s == '\0') usage();
	    *s++ = '\0';
	    while (*s != '\0' && *s != ',') ++s;
	    if (*s == '\0') usage();

	    fmt[i].middle_param_prefix = ++s;
	    while (*s != '\0' && isascii(*s) && !isalnum(*s)) ++s;
	    if (*s == '\0') usage();
	    *s++ = '\0';
	    while (*s != '\0' && isascii(*s) && isalnum(*s)) ++s;
	    if (*s == '\0') usage();

	    fmt[i].last_param_suffix = s;
	    while (*s != '\0' && *s != ')') ++s;
	    *s = '\0';

	    break;
	case 'f':
	    proto_style = atoi(optarg);
	    if (proto_style < 0 || proto_style > PROTO_ANSI)
		usage();
	    break;
	case 'm':
	    proto_macro = TRUE;
	    break;
	case 'M':
	    macro_name = optarg;
	    break;
	case 'p':
	    promote_param = FALSE;
	    break;
	case 'q':
	    quiet = TRUE;
	    break;
	case 's':
	    static_out = TRUE;
	    break;
	case 't':
	    func_style = FUNC_TRADITIONAL;
	    break;
	case 'V':
	    fprintf(stderr, "%s patchlevel %d\n", rcsid, PATCHLEVEL);
	    exit(FAIL);
	    break;
	case 'v':
	    variables_out = TRUE;
	    break;
	/* options added by dickey */
	case 'o':
	    if (freopen(optarg, "w", stdout) == 0) {
		perror(optarg);
		exit(FAIL);
  	    }
	    break;
	case 'O':
	    if (freopen(optarg, "w", stderr) == 0) {
		perror(optarg);
		exit(FAIL);
  	    }
	    break;
#if OPT_LINTLIBRARY
	case 'T':	/* emit typedefs */
	    types_out = TRUE;
	    break;
	case 'l':
	    func_style    = FUNC_NONE;
	    proto_style   = PROTO_LINTLIBRARY;
	    extern_out    = FALSE;
	    types_out     = TRUE;
	    variables_out = TRUE;
#ifdef unix
	    (void)strcat(cpp_opt, " -C");	/* pass-through comments */
#endif
	    break;
#endif
	case 'x':
	    extern_in = MAX_INC_DEPTH;
	    break;
	default:
	    usage();
	}
    }

#ifdef	vms
    add_option("includes", cpp_include);
    add_option("define",   cpp_defines);
    add_option("undefine", cpp_undefns);
#endif

    *pargc = argc;
    *pargv = argv;
}

int
main (argc, argv)
int argc;
char **argv;
{
    int i;
    FILE *inf;

    process_options(&argc, &argv);

#if OPT_LINTLIBRARY
    if (LintLibrary())
    	put_string(stdout, "/* LINTLIBRARY */\n");
#endif

    if (proto_macro && define_macro) {
	printf("#if __STDC__ || defined(__cplusplus)\n");
	printf("#define %s(s) s\n", macro_name);
	printf("#else\n");
	printf("#define %s(s) ()\n", macro_name);
	printf("#endif\n\n");
    }

    init_parser();
    if (optind == argc) {
	if (func_style != FUNC_NONE) {
	    proto_style = PROTO_NONE;
	    variables_out = FALSE;
	    file_comments = FALSE;
	}
	process_file(stdin, "stdin");
	pop_file();
    } else {
	for (i = optind; i < argc; ++i) {
#ifdef CPP
# if CPP_DOES_ONLY_C_FILES
	    /*
	     * GCC (and possibly other compilers) don't pass-through the ".l"
	     * and ".y" files to the C preprocessor stage.  Fix this by
	     * temporarily linking the input file to a temporary-file with a
	     * ".c" suffix.
	     *
	     * Of course, this solution assumes that the input directory is
	     * writeable.
	     */
	    char temp[BUFSIZ];
	    char *s = strcpy(temp, argv[i]);
	    int len = strlen(temp);
	    s += len - 1;
	    if ((len > 2)
	     && (s[-1] == '.')
	     && (*s == 'l' || *s == 'y')) {
		while (s != temp && s[-1] != '/')
		    s--;
		(void)strcpy(s, "XXXXXX.c");
	    	mktemp(temp);
	    	if (link(argv[i], temp) < 0)
		    (void)strcpy(temp, argv[i]);
	    }
#  define FileName temp
# else
#  define FileName argv[i]
#  ifdef vms
	    char temp[BUFSIZ];
	    (void)strcpy(temp, FileName);
#  endif
# endif
	    if (func_style == FUNC_NONE && cpp != NULL) {
#ifdef vms
		/*
		 * Assume the 'cpp' command contains a "%s" for the name of
		 * the file that we're writing to.
		 */
		sprintf(cpp_cmd, cpp,
			mktemp(strcpy(temp, "sys$scratch:XXXXXX")));
		sprintf(cpp_cmd + strlen(cpp_cmd), "%s %s", cpp_opt, FileName);
		system(cpp_cmd);
		inf = fopen(temp, "r");
#else
		sprintf(cpp_cmd, "%s%s %s", cpp, cpp_opt, FileName);
		inf = popen(cpp_cmd, "r");
#endif
		if (inf == NULL) {
		    fprintf(stderr, "%s: error running %s\n", progname,
		     cpp_cmd);
		    continue;
		}
	    } else {
		if ((inf = fopen(argv[i], "r")) == NULL) {
		    fprintf(stderr, "%s: cannot read file %s\n", progname,
		     argv[i]);
		    continue;
		}
	    }
#else
	    if ((inf = fopen(argv[i], "r")) == NULL) {
		fprintf(stderr, "%s: cannot read file %s\n", progname, argv[i]);
		continue;
	    }
#endif
	    process_file(inf, argv[i]);
#ifdef CPP
	    if (func_style == FUNC_NONE && cpp != NULL) {
		pclose(inf);
#if CPP_DOES_ONLY_C_FILES || defined(vms)
		if (strcmp(argv[i], temp)) {
			(void)unlink(temp);
		}
		pop_file();
#endif
	    } else {
		pop_file();
	    }
#else	/* no CPP defined */
	    pop_file();
#endif
	}
    }

    if (proto_macro && define_macro) {
	printf("\n#undef %s\n", macro_name);
    }

#ifdef NO_LEAKS
# ifdef CPP
    if (cpp_opt != 0) free(cpp_opt);
    if (cpp_cmd != 0) free(cpp_cmd);
#  ifdef vms
    if (cpp_include != 0) free(cpp_include);
    if (cpp_defines != 0) free(cpp_defines);
    if (cpp_undefns != 0) free(cpp_undefns);
#  endif
# endif
    while (num_inc_dir-- > 2) {
	free(inc_dir[num_inc_dir]);
    }
    free_parser();
# if OPT_LINTLIBRARY
    free_lintlibs();
# endif

# ifdef DOALLOC
    show_alloc();
# endif
#endif

#if HAVE_LIBDBMALLOC
    malloc_dump(fileno(stderr));
#endif
    exit(SUCCESS);
    return SUCCESS;
}

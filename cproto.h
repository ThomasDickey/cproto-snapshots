/* $Id: cproto.h,v 4.1 1994/10/12 14:12:48 cthuang Exp $
 *
 * Declarations for C function prototype generator
 */
#ifdef	HAVE_CONFIG_H
#include "config.h"
#endif

#include "system.h"

#if HAVE_LIBDBMALLOC || defined(DOALLOC)
#undef  NO_LEAKS
#define NO_LEAKS 1
#endif

#ifdef	lint
#define	ALLOC(cast)	(cast *)0
#else
#define	ALLOC(cast)	(cast *)xmalloc(sizeof(cast))
#endif

/* Useful constants (mainly to avoid problems balancing parentheses...) */
#define PAREN_L       '('
#define PAREN_R       ')'
#define SQUARE_L      '['
#define SQUARE_R      ']'
#define CURL_L        '{'
#define CURL_R        '}'
#define	COMMENT_BEGIN "/*"
#define COMMENT_END   "*/"

/* Boolean type */
typedef char boolean;

/* Source file text */
typedef struct text {
    char text[MAX_TEXT_SIZE];	/* source text */
    long begin; 		/* offset in temporary file */
} Text;

/* This is a list of function parameters. */
typedef struct parameter_list {
    struct parameter *first;	/* pointer to first parameter in list */
    struct parameter *last;	/* pointer to last parameter in list */  
    long begin_comment; 	/* begin offset of comment */
    long end_comment;		/* end offset of comment */
    char *comment;		/* comment at start of parameter list */
} ParameterList;

/* Declaration specifier flags */
#define DS_NONE 	0	/* default */
#define DS_EXTERN	1	/* contains "extern" specifier */
#define DS_STATIC	2	/* contains "static" specifier */
#define DS_CHAR 	4	/* contains "char" type specifier */
#define DS_SHORT	8	/* contains "short" type specifier */
#define DS_FLOAT	16	/* contains "float" type specifier */
#define DS_JUNK 	32	/* we're not interested in this declaration */

/* This structure stores information about a declaration specifier. */
typedef struct decl_spec {
    unsigned short flags;	/* flags defined above */
    char *text; 		/* source text */
    long begin; 		/* offset in temporary file */
} DeclSpec;

/* Styles of function definitions */
#define FUNC_NONE		0	/* not a function definition */
#define FUNC_TRADITIONAL	1	/* traditional style */
#define FUNC_ANSI		2	/* ANSI style */
#define FUNC_BOTH		3	/* both styles */
typedef int FuncDefStyle;

/* This structure stores information about a declarator. */
typedef struct declarator {
    char *name; 			/* name of variable or function */
    char *text; 			/* source text */
    long begin; 			/* offset in temporary file */
    long begin_comment; 		/* begin offset of comment */
    long end_comment;			/* end offset of comment */
    FuncDefStyle func_def;		/* style of function definition */
    ParameterList params;		/* function parameters */
    boolean pointer;			/* TRUE if it declares a pointer */
    struct declarator *head;		/* head function declarator */
    struct declarator *func_stack;	/* stack of function declarators */
    struct declarator *next;		/* next declarator in list */
} Declarator;

/* This is a list of declarators. */
typedef struct declarator_list {
    Declarator *first;		/* pointer to first declarator in list */
    Declarator *last;		/* pointer to last declarator in list */  
} DeclaratorList;

/* This structure stores information about a function parameter. */
typedef struct parameter {
    struct parameter *next;	/* next parameter in list */
    DeclSpec decl_spec;
    Declarator *declarator;
    char *comment;		/* comment following the parameter */
} Parameter;

/* parser stack entry type */
typedef union {
    Text text;
    DeclSpec decl_spec;
    Parameter *parameter;
    ParameterList param_list;
    Declarator *declarator;
    DeclaratorList decl_list;
} YYSTYPE;

/* Prototype styles */
#if OPT_LINTLIBRARY
#define PROTO_LINTLIBRARY	-1	/* form lint-library source */
#endif
#define PROTO_NONE		0	/* do not output any prototypes */
#define PROTO_TRADITIONAL	1	/* comment out parameters */
#define PROTO_ABSTRACT		2	/* comment out parameter names */
#define PROTO_ANSI		3	/* ANSI C prototype */
typedef int PrototypeStyle;

#define lintLibrary() (proto_style == PROTO_LINTLIBRARY)

/* The role of a function declarator */
#define FUNC_OTHER	0	/* miscellaneous declaration */
#define FUNC_PROTO	1	/* prototype */
#define FUNC_DEF	2	/* function definition */
typedef int FuncDeclRole;

/* Prototype/function definition output formats */
#define FMT_OTHER		0	/* miscellaneous */
#define FMT_PROTO		1	/* prototype */
#define FMT_FUNC		2	/* function definition */
#define FMT_FUNC_COMMENT	3	/* func. def. with parameter comments */
typedef int FuncFormatType;

/* Prototype/function definition output format */
typedef struct func_format {
    char *decl_spec_prefix;	/* output before declaration specifier */
    char *declarator_prefix;	/* output before declarator name */
    char *declarator_suffix;	/* output before '(' of parameter list */
    char *first_param_prefix;	/* output before first parameter */
    char *middle_param_prefix;	/* output before each subsequent parameter */
    char *last_param_suffix;	/* output after last parameter */
} FuncFormat;

/* Program options */
extern boolean extern_out;
extern boolean static_out;
#if OPT_LINTLIBRARY
extern boolean types_out;
extern boolean lint_shadowed;
#endif
extern boolean variables_out;
extern boolean promote_param;
extern PrototypeStyle proto_style;
extern FuncDefStyle func_style;
extern boolean proto_macro;
extern boolean define_macro;
extern char *macro_name;
extern boolean proto_comments;
extern boolean file_comments;
extern boolean quiet;
extern char *func_directive;
extern int num_inc_dir;
extern char *inc_dir[];
extern FuncFormat fmt[4];

/* Global declarations */
extern char progname[];
extern int varargs_num;		/* supports varargs-comment */
extern char *varargs_str;	/* additional info, such as PRINTFLIKEnn */
extern int extern_in;		/* supports "LINT_EXTERNnn" */
extern int exitlike_func;	/* supports noreturn-attribute */
extern int in_include;		/* current include-level */
extern int debug_trace;
extern char base_file[];

/* cproto.c */
#ifdef NO_LEAKS
extern char *xMalloc        ARGS((unsigned n, char *f, int l));
extern char *xStrdup        ARGS((char *s,    char *f, int l));
#define xmalloc(n)          xMalloc(n, __FILE__, __LINE__)
#define xstrdup(s)          xStrdup(s, __FILE__, __LINE__)
#else
extern char *xmalloc        ARGS((unsigned n));
extern char *xstrdup        ARGS((char *src));
#endif
extern void put_error       ARGS((void));
extern int is_path_sep      ARGS((int ch));
extern char *trim_path_sep  ARGS((char *s));

/* lintlibs.c */
#if OPT_LINTLIBRARY
extern void put_string      ARGS((FILE *outf, char *s));
extern void put_char        ARGS((FILE *outf, int c));
extern void put_newline     ARGS((FILE *outf));
extern void put_blankline   ARGS((FILE *outf));
extern void put_padded      ARGS((FILE *outf, char *s));
extern void fmt_library     ARGS((int code));
extern void begin_tracking  ARGS((void));
extern int already_declared ARGS((char *name));
extern void track_in        ARGS((void));
extern int want_typedef     ARGS((void));
extern void begin_typedef   ARGS((void));
extern void copy_typedef    ARGS((char *s));
extern void end_typedef     ARGS((void));
extern void imply_typedef   ARGS((char *s));
extern char *implied_typedef ARGS((void));
extern void indent          ARGS((FILE *outf));
extern int lint_ellipsis    ARGS((Parameter *p));
extern void ellipsis_varargs ARGS((Declarator *d));
extern char *supply_parm    ARGS((int count));
extern int is_actual_func   ARGS((Declarator *d));
extern void put_body        ARGS((FILE *outf, DeclSpec *decl_spec, Declarator *declarator));
# ifdef NO_LEAKS
extern void free_lintlibs   ARGS((void));
# endif
#else
#define put_string(fp,S)    fputs(S, fp)
#define put_char(fp,C)      fputc(C, fp)
#define put_padded(fp,S)    fprintf(fp, "%s ", S)
#define put_body(fp,s,d)    put_string(fp,";\n")
#define track_in()
#define begin_typedef()
#define copy_typedef()
#define end_typedef()
#define imply_typedef(s)
#define implied_typedef()   ((char *)0)
#endif

/* strkey.c */
extern char *strkey         ARGS((char *src, char *key));
extern void strcut          ARGS((char *src, char *key));

/* grammar.y */
extern boolean is_typedef_name ARGS((char *name));
extern char *cur_file_name  ARGS((void));
extern unsigned cur_line_num ARGS((void));
extern FILE *cur_tmp_file   ARGS((void));
extern void cur_file_changed ARGS((void));
extern long cur_begin_comment ARGS((void));
extern char *cur_text       ARGS((void));
extern void pop_file        ARGS((void));
extern void init_parser     ARGS((void));
extern void process_file    ARGS((FILE *infile, char *name));
#ifdef NO_LEAKS
extern void free_parser     ARGS((void));
#endif

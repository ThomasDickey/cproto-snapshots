/* $Id: cproto.h,v 3.8 1993/06/09 18:15:02 tom Exp $
 *
 * Declarations for C function prototype generator
 */
#include "config.h"

#ifdef	lint
#define	ALLOC(cast)	(cast *)0
#else
#define	ALLOC(cast)	(cast *)xmalloc(sizeof(cast))
#endif

#if __STDC__ && (defined(apollo) || defined(sun))
#define	SIZE_T	size_t
#else
#define	SIZE_T	int
#endif

/* Boolean type */
typedef char boolean;
#define FALSE	0
#define TRUE	1

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
#define PROTO_LINTLIBRARY	-1	/* form lint-library source */
#define PROTO_NONE		0	/* do not output any prototypes */
#define PROTO_TRADITIONAL	1	/* comment out parameters */
#define PROTO_ABSTRACT		2	/* comment out parameter names */
#define PROTO_ANSI		3	/* ANSI C prototype */
typedef int PrototypeStyle;

#define LintLibrary() (proto_style == PROTO_LINTLIBRARY)

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
extern boolean types_out;
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
extern int extern_in;		/* supports "LINT_EXTERNnn" */
extern int in_include;		/* current include-level */
extern int debug_trace;
extern char base_file[];

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* cproto.c */
extern char *xmalloc P_((unsigned n));
extern char *xstrdup P_((char *src));
extern void put_error P_((void));
extern int is_path_sep P_((int ch));
extern char *trim_path_sep P_((char *s));

/* lintlibs.c */
extern void put_string P_((FILE *outf, char *s));
extern void put_char P_((FILE *outf, int c));
extern void put_newline P_((FILE *outf));
extern void put_blankline P_((FILE *outf));
extern void put_padded P_((FILE *outf, char *s));
extern void fmt_library P_((int code));
extern void begin_tracking P_((void));
extern void track_in P_((void));
extern int want_typedef P_((void));
extern void begin_typedef P_((void));
extern void copy_typedef P_((char *s));
extern void end_typedef P_((void));
extern void imply_typedef P_((char *s));
extern char *implied_typedef P_((void));
extern void indent P_((FILE *outf));
extern int lint_ellipsis P_((Parameter *p));
extern void ellipsis_varargs P_((Declarator *d));
extern char *supply_parm P_((int count));
extern void put_body P_((FILE *outf, DeclSpec *decl_spec, Declarator *declarator));

/* strkey.c */
extern char *strkey P_((char *src, char *key));
extern void strcut P_((char *src, char *key));

/* grammar.y */
extern boolean is_typedef_name P_((char *name));
extern char *cur_file_name P_((void));
extern unsigned cur_line_num P_((void));
extern FILE *cur_tmp_file P_((void));
extern void cur_file_changed P_((void));
extern long cur_begin_comment P_((void));
extern char *cur_text P_((void));
extern void pop_file P_((void));
extern void init_parser P_((void));
extern void process_file P_((FILE *infile, char *name));

#undef P_

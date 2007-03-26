#ifndef lint
static char yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93 (BSDI)";
#endif
#include <stdlib.h>
#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define YYEMPTY (-1)
#define YYLEX yylex()
#define yyclearin (yychar=YYEMPTY)
#define yyerrok (yyerrflag=0)
#define YYRECOVERING (yyerrflag!=0)
#define YYPREFIX "yy"
#line 65 "grammar.y"
#include <stdio.h>
#include <ctype.h>
#include "cproto.h"
#include "symbol.h"
#include "semantic.h"

#define YYMAXDEPTH 150

extern	int	yylex ARGS((void));

/* declaration specifier attributes for the typedef statement currently being
 * scanned
 */
static int cur_decl_spec_flags;

/* pointer to parameter list for the current function definition */
static ParameterList *func_params;

/* A parser semantic action sets this pointer to the current declarator in
 * a function parameter declaration in order to catch any comments following
 * the parameter declaration on the same line.  If the lexer scans a comment
 * and <cur_declarator> is not NULL, then the comment is attached to the
 * declarator.  To ignore subsequent comments, the lexer sets this to NULL
 * after scanning a comment or end of line.
 */
static Declarator *cur_declarator;

/* temporary string buffer */
static char buf[MAX_TEXT_SIZE];

/* table of typedef names */
static SymbolTable *typedef_names;

/* table of define names */
static SymbolTable *define_names;

/* table of type qualifiers */
static SymbolTable *type_qualifiers;

/* information about the current input file */
typedef struct {
    char *base_name;		/* base input file name */
    char *file_name;		/* current file name */
    FILE *file; 		/* input file */
    unsigned line_num;		/* current line number in input file */
    FILE *tmp_file;		/* temporary file */
    long begin_comment; 	/* tmp file offset after last written ) or ; */
    long end_comment;		/* tmp file offset after last comment */
    boolean convert;		/* if TRUE, convert function definitions */
    boolean changed;		/* TRUE if conversion done in this file */
} IncludeStack;

static IncludeStack *cur_file;	/* current input file */

#include "yyerror.c"

static int haveAnsiParam ARGS((void));


/* Flags to enable us to find if a procedure returns a value.
 */
static int return_val,	/* nonzero on BRACES iff return-expression found */
	   returned_at;	/* marker for token-number to set 'return_val' */

#if OPT_LINTLIBRARY
static char *dft_decl_spec ARGS((void));

static char *
dft_decl_spec ()
{
    return (lintLibrary() && !return_val) ? "void" : "int";
}

#else
#define dft_decl_spec() "int"
#endif

static int
haveAnsiParam ()
{
    Parameter *p;
    if (func_params != 0) {
	for (p = func_params->first; p != 0; p = p->next) {
	    if (p->declarator->func_def == FUNC_ANSI) {
		return TRUE;
	    }
	}
    }
    return FALSE;
}
#line 106 "y.tab.c"
#define T_IDENTIFIER 257
#define T_TYPEDEF_NAME 258
#define T_DEFINE_NAME 259
#define T_AUTO 260
#define T_EXTERN 261
#define T_REGISTER 262
#define T_STATIC 263
#define T_TYPEDEF 264
#define T_INLINE 265
#define T_CHAR 266
#define T_DOUBLE 267
#define T_FLOAT 268
#define T_INT 269
#define T_VOID 270
#define T_LONG 271
#define T_SHORT 272
#define T_SIGNED 273
#define T_UNSIGNED 274
#define T_ENUM 275
#define T_STRUCT 276
#define T_UNION 277
#define T_TYPE_QUALIFIER 278
#define T_BRACKETS 279
#define T_LBRACE 280
#define T_MATCHRBRACE 281
#define T_ELLIPSIS 282
#define T_INITIALIZER 283
#define T_STRING_LITERAL 284
#define T_ASM 285
#define T_ASMARG 286
#define T_VA_DCL 287
#define YYERRCODE 256
short yylhs[] = {                                        -1,
    0,    0,   26,   26,   27,   27,   27,   27,   27,   27,
   27,   31,   30,   30,   28,   28,   34,   28,   32,   33,
   33,   35,   35,   37,   38,   29,   39,   29,   36,   36,
   36,   40,   40,    1,    1,    2,    2,    2,    3,    3,
    3,    3,    3,    4,    4,    4,    4,    4,    4,    4,
    4,    4,    4,    4,    4,    5,    5,    6,    6,    6,
   19,   19,    8,    8,    9,   41,    9,    7,    7,    7,
   25,   23,   23,   10,   10,   11,   11,   11,   11,   11,
   20,   20,   21,   21,   22,   22,   14,   14,   15,   15,
   16,   16,   16,   17,   17,   18,   18,   24,   24,   12,
   12,   12,   13,   13,   13,   13,   13,   13,   13,
};
short yylen[] = {                                         2,
    0,    1,    1,    2,    1,    1,    1,    1,    3,    2,
    2,    2,    3,    3,    2,    3,    0,    5,    1,    0,
    1,    1,    3,    0,    0,    7,    0,    5,    0,    1,
    1,    1,    2,    1,    2,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    3,    2,    2,
    1,    1,    1,    3,    1,    0,    4,    3,    2,    2,
    1,    1,    1,    2,    1,    1,    3,    2,    4,    4,
    2,    3,    0,    1,    1,    2,    1,    3,    1,    3,
    2,    2,    1,    0,    1,    1,    3,    1,    2,    1,
    2,    1,    3,    2,    1,    4,    3,    3,    2,
};
short yydefred[] = {                                      0,
    0,    0,    0,    0,   98,   53,   57,   39,    0,   41,
   42,   19,   43,   44,   45,   46,   47,   52,   48,   49,
   50,   51,   71,   61,   62,   56,    0,    7,    0,    0,
   34,   36,   37,   38,   54,   55,   27,    0,    0,    0,
   76,    0,    0,    3,    5,    6,    8,    0,   10,   11,
    0,   85,    0,    0,   99,    0,    0,   40,   15,   35,
    0,   63,    0,    0,    0,   78,   72,   73,    0,    0,
   59,    0,    0,   69,    4,    0,   77,   82,   86,    0,
   14,   13,    9,   16,    0,   66,    0,   30,   32,    0,
    0,   96,    0,    0,    0,   89,    0,    0,   12,   58,
   68,    0,    0,   64,    0,    0,    0,   33,    0,  105,
   91,   92,    0,    0,   79,    0,   80,    0,   22,    0,
    0,   67,   25,   28,  109,    0,    0,    0,  104,    0,
   88,   90,   97,   18,    0,    0,  103,  108,  107,    0,
   23,   26,  106,
};
short yydgoto[] = {                                      29,
   80,   31,   32,   33,   34,   35,   36,   61,   62,   37,
   38,  112,  113,   94,   95,   96,   97,   98,   39,   40,
   53,   54,   70,   41,   42,   43,   44,   45,   46,   47,
   71,   48,  120,  102,  121,   90,   87,  136,   64,   91,
  105,
};
short yysindex[] = {                                    -38,
  -36,   17, -248, -241,    0,    0,    0,    0, -264,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, -261,    0,    0,  395,
    0,    0,    0,    0,    0,    0,    0,  -37, -245,  -11,
    0, -245,  -38,    0,    0,    0,    0,  438,    0,    0,
  -15,    0,   -5, -248,    0,  327,  -27,    0,    0,    0,
  -16,    0,  -22,  279,  351,    0,    0,    0, -239, -234,
    0,  -37, -234,    0,    0,  438,    0,    0,    0,  395,
    0,    0,    0,    0,   17,    0,  279,    0,    0, -232,
  372,    0,  150,   24,   27,    0,   33,   32,    0,    0,
    0,   17,  -22,    0, -206, -202, -199,    0,  127,    0,
    0,    0,  -35,  203,    0,  302,    0, -174,    0,   28,
   44,    0,    0,    0,    0,   48,   49,  258,    0,  -35,
    0,    0,    0,    0,   17, -189,    0,    0,    0,   52,
    0,    0,    0,
};
short yyrindex[] = {                                     95,
    0,    0,  157,    0,    0,    0,    0,    0,  417,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  -10,    0,    0,
    0,    0,   96,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  231,  173,    0,    0,    0,    0,    0,    0,
    0,    0,   50, -183,   57,    0,    0,    0,    0,   81,
    0,   20,  104,    0,    0,   16,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, -183,    0,    0,    0,
 -180,    0,  -26,    0,   60,    0,    0,   61,    0,    0,
    0,   45,    1,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    9,   25,    0,    0,    0,    0,    0,    0,
   46,    0,    0,    0,    0,    0,    0,    0,    0,   29,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,
};
short yygindex[] = {                                      0,
   19,   -8,    0,    0,   -2,    0,    0,    0,   18,    6,
  -34,   -3,   -7,  -95,    0,   -6,    0,    0,    0,  -46,
    0,    0,   69,    0,    0,    0,   70,  -47,    0,    0,
  -32,    0,    0,    0,    0,   30,    0,    0,    0,    0,
    0,
};
#define YYTABLESIZE 716
short yytable[] = {                                       4,
   52,    2,   65,    3,  128,   72,   78,   51,   81,   74,
    7,   67,   68,  127,   93,   55,   89,   93,   30,   56,
   28,   60,   50,   82,   57,   77,    4,   85,    2,   26,
   75,   83,  140,   75,   69,   63,    3,  100,   86,   89,
  101,   99,   84,  108,   65,   69,  114,  107,   75,  102,
   75,   79,  102,   17,    4,   17,    2,   17,    3,   65,
   74,   30,  114,   74,  115,  100,   76,   60,  100,  101,
  116,   60,  101,  117,   17,  118,  122,  123,   74,   72,
   74,  124,  133,   93,   60,  103,  134,  135,  137,  138,
  103,  142,  143,   65,    1,    2,   29,   94,  111,   31,
   87,   95,  104,   20,   21,  126,  130,  119,   65,  132,
   73,    0,   75,    0,   51,    0,  106,    0,   60,    0,
   60,   60,   60,    0,   60,    0,    0,   93,    0,    0,
    0,    0,    0,    0,   93,    0,    0,    0,    0,   60,
  141,   70,    0,   70,   70,   70,   93,   70,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   70,    0,    4,    0,  109,  125,    3,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    4,    0,  109,
    0,    3,    0,    0,   83,    0,   83,   83,   83,    0,
   83,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   84,    0,   84,   84,   84,    0,   84,    1,    5,    6,
    7,    8,    9,   10,   11,   12,   13,   14,   15,   16,
   17,   18,   19,   20,   21,   22,   23,   24,   25,   26,
    4,   66,  109,  129,   49,    5,   27,   75,   75,   75,
   75,   75,   75,   75,   75,   75,   75,   75,   75,   75,
   75,   75,   75,   75,   75,   75,   75,   75,   81,   75,
   81,   81,   17,    5,   81,    0,   75,   74,   74,   74,
   74,   74,   74,   74,   74,   74,   74,   74,   74,   74,
   74,   74,   74,   74,   74,   74,   74,   74,  139,   74,
    0,    0,    0,    0,    0,    0,   74,   24,   24,   24,
   24,   24,   24,   24,   24,   24,   24,   24,   24,   24,
   24,   24,   24,   24,   24,   24,   24,   24,    0,   24,
    0,    0,    0,    0,    0,    0,   24,   60,   60,   60,
   60,   60,   60,   60,    0,   60,   60,   60,   60,   60,
   60,   60,   60,   60,   60,   60,   60,   60,   60,   60,
   70,   70,   70,   70,   70,   70,   70,    0,   70,   70,
   70,   70,   70,   70,   70,   70,   70,   70,   70,   70,
   70,   70,   70,    5,    6,    7,    8,   58,   10,   11,
    0,   13,   14,   15,   16,   17,   18,   19,   20,   21,
   22,   23,   24,   25,   26,  110,    5,    6,    7,    8,
   58,   10,   11,   83,   13,   14,   15,   16,   17,   18,
   19,   20,   21,   22,   23,   24,   25,   26,  110,   84,
    0,    0,    4,    0,    2,   83,    3,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   84,    0,   59,   40,    0,   40,    0,   40,    5,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   40,    0,    0,    0,    0,
    0,  110,    0,    0,    0,    0,    0,   81,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   81,
    0,    0,    0,    0,    0,    6,    7,    8,   58,   10,
   11,    0,   13,   14,   15,   16,   17,   18,   19,   20,
   21,   22,   23,   24,   25,   26,    6,    7,    8,   58,
   10,   11,   12,   13,   14,   15,   16,   17,   18,   19,
   20,   21,   22,   23,   24,   25,   26,    0,    0,    6,
    7,    8,   58,   10,   11,   88,   13,   14,   15,   16,
   17,   18,   19,   20,   21,   22,   23,   24,   25,   26,
    0,    0,    0,  131,    6,    7,    8,   58,   10,   11,
   12,   13,   14,   15,   16,   17,   18,   19,   20,   21,
   22,   23,   24,   25,   26,    0,   69,   92,    6,    7,
    8,   58,   10,   11,    0,   13,   14,   15,   16,   17,
   18,   19,   20,   21,   22,   23,   24,   25,   26,    6,
    7,    8,   58,   10,   11,   12,   13,   14,   15,   16,
   17,   18,   19,   20,   21,   22,   23,   24,   25,   26,
    0,    5,    6,    7,    8,   58,   10,   11,    0,   13,
   14,   15,   16,   17,   18,   19,   20,   21,   22,   23,
   24,   25,   26,   40,   40,   40,   40,   40,   40,   40,
    0,   40,   40,   40,   40,   40,   40,   40,   40,   40,
   40,   40,   40,   40,   40,    6,    7,    8,   58,   10,
   11,    0,   13,   14,   15,   16,   17,   18,   19,   20,
   21,   22,   23,   24,   25,   26,
};
short yycheck[] = {                                      38,
    3,   40,   40,   42,   40,   40,   53,    2,   56,   42,
  259,  257,  258,  109,   41,  257,   64,   44,    0,  284,
   59,   30,   59,   56,  286,   41,   38,   44,   40,  278,
   41,   59,  128,   44,  280,   30,   42,   70,   61,   87,
   73,  281,   59,   91,   44,  280,   93,  280,   59,   41,
   61,   54,   44,   38,   38,   40,   40,   42,   42,   59,
   41,   43,  109,   44,   41,   41,   48,   76,   44,   41,
   44,   80,   44,   41,   59,   44,  283,  280,   59,  114,
   61,  281,  257,   65,   93,   80,   59,   44,   41,   41,
   85,  281,   41,   44,    0,    0,  280,   41,   93,  280,
   41,   41,   85,   59,   59,  109,  114,  102,   59,  116,
   42,   -1,   43,   -1,  109,   -1,   87,   -1,   38,   -1,
   40,   41,   42,   -1,   44,   -1,   -1,  109,   -1,   -1,
   -1,   -1,   -1,   -1,  116,   -1,   -1,   -1,   -1,   59,
  135,   38,   -1,   40,   41,   42,  128,   44,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   59,   -1,   38,   -1,   40,   41,   42,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   38,   -1,   40,
   -1,   42,   -1,   -1,   38,   -1,   40,   41,   42,   -1,
   44,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   38,   -1,   40,   41,   42,   -1,   44,  256,  257,  258,
  259,  260,  261,  262,  263,  264,  265,  266,  267,  268,
  269,  270,  271,  272,  273,  274,  275,  276,  277,  278,
   38,  279,   40,  279,  281,  257,  285,  258,  259,  260,
  261,  262,  263,  264,  265,  266,  267,  268,  269,  270,
  271,  272,  273,  274,  275,  276,  277,  278,   38,  280,
   40,   41,  257,  257,   44,   -1,  287,  258,  259,  260,
  261,  262,  263,  264,  265,  266,  267,  268,  269,  270,
  271,  272,  273,  274,  275,  276,  277,  278,   41,  280,
   -1,   -1,   -1,   -1,   -1,   -1,  287,  258,  259,  260,
  261,  262,  263,  264,  265,  266,  267,  268,  269,  270,
  271,  272,  273,  274,  275,  276,  277,  278,   -1,  280,
   -1,   -1,   -1,   -1,   -1,   -1,  287,  257,  258,  259,
  260,  261,  262,  263,   -1,  265,  266,  267,  268,  269,
  270,  271,  272,  273,  274,  275,  276,  277,  278,  279,
  257,  258,  259,  260,  261,  262,  263,   -1,  265,  266,
  267,  268,  269,  270,  271,  272,  273,  274,  275,  276,
  277,  278,  279,  257,  258,  259,  260,  261,  262,  263,
   -1,  265,  266,  267,  268,  269,  270,  271,  272,  273,
  274,  275,  276,  277,  278,  279,  257,  258,  259,  260,
  261,  262,  263,  257,  265,  266,  267,  268,  269,  270,
  271,  272,  273,  274,  275,  276,  277,  278,  279,  257,
   -1,   -1,   38,   -1,   40,  279,   42,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  279,   -1,   59,   38,   -1,   40,   -1,   42,  257,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   59,   -1,   -1,   -1,   -1,
   -1,  279,   -1,   -1,   -1,   -1,   -1,  257,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  279,
   -1,   -1,   -1,   -1,   -1,  258,  259,  260,  261,  262,
  263,   -1,  265,  266,  267,  268,  269,  270,  271,  272,
  273,  274,  275,  276,  277,  278,  258,  259,  260,  261,
  262,  263,  264,  265,  266,  267,  268,  269,  270,  271,
  272,  273,  274,  275,  276,  277,  278,   -1,   -1,  258,
  259,  260,  261,  262,  263,  287,  265,  266,  267,  268,
  269,  270,  271,  272,  273,  274,  275,  276,  277,  278,
   -1,   -1,   -1,  282,  258,  259,  260,  261,  262,  263,
  264,  265,  266,  267,  268,  269,  270,  271,  272,  273,
  274,  275,  276,  277,  278,   -1,  280,  257,  258,  259,
  260,  261,  262,  263,   -1,  265,  266,  267,  268,  269,
  270,  271,  272,  273,  274,  275,  276,  277,  278,  258,
  259,  260,  261,  262,  263,  264,  265,  266,  267,  268,
  269,  270,  271,  272,  273,  274,  275,  276,  277,  278,
   -1,  257,  258,  259,  260,  261,  262,  263,   -1,  265,
  266,  267,  268,  269,  270,  271,  272,  273,  274,  275,
  276,  277,  278,  257,  258,  259,  260,  261,  262,  263,
   -1,  265,  266,  267,  268,  269,  270,  271,  272,  273,
  274,  275,  276,  277,  278,  258,  259,  260,  261,  262,
  263,   -1,  265,  266,  267,  268,  269,  270,  271,  272,
  273,  274,  275,  276,  277,  278,
};
#define YYFINAL 29
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 287
#if YYDEBUG
char *yyname[] = {
"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,"'&'",0,"'('","')'","'*'",0,"','",0,0,0,0,0,0,0,0,0,0,0,0,0,0,"';'",0,
"'='",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
"T_IDENTIFIER","T_TYPEDEF_NAME","T_DEFINE_NAME","T_AUTO","T_EXTERN",
"T_REGISTER","T_STATIC","T_TYPEDEF","T_INLINE","T_CHAR","T_DOUBLE","T_FLOAT",
"T_INT","T_VOID","T_LONG","T_SHORT","T_SIGNED","T_UNSIGNED","T_ENUM","T_STRUCT",
"T_UNION","T_TYPE_QUALIFIER","T_BRACKETS","T_LBRACE","T_MATCHRBRACE",
"T_ELLIPSIS","T_INITIALIZER","T_STRING_LITERAL","T_ASM","T_ASMARG","T_VA_DCL",
};
char *yyrule[] = {
"$accept : program",
"program :",
"program : translation_unit",
"translation_unit : external_declaration",
"translation_unit : translation_unit external_declaration",
"external_declaration : declaration",
"external_declaration : function_definition",
"external_declaration : ';'",
"external_declaration : linkage_specification",
"external_declaration : T_ASM T_ASMARG ';'",
"external_declaration : error T_MATCHRBRACE",
"external_declaration : error ';'",
"braces : T_LBRACE T_MATCHRBRACE",
"linkage_specification : T_EXTERN T_STRING_LITERAL braces",
"linkage_specification : T_EXTERN T_STRING_LITERAL declaration",
"declaration : decl_specifiers ';'",
"declaration : decl_specifiers init_declarator_list ';'",
"$$1 :",
"declaration : any_typedef decl_specifiers $$1 opt_declarator_list ';'",
"any_typedef : T_TYPEDEF",
"opt_declarator_list :",
"opt_declarator_list : declarator_list",
"declarator_list : declarator",
"declarator_list : declarator_list ',' declarator",
"$$2 :",
"$$3 :",
"function_definition : decl_specifiers declarator $$2 opt_declaration_list T_LBRACE $$3 T_MATCHRBRACE",
"$$4 :",
"function_definition : declarator $$4 opt_declaration_list T_LBRACE T_MATCHRBRACE",
"opt_declaration_list :",
"opt_declaration_list : T_VA_DCL",
"opt_declaration_list : declaration_list",
"declaration_list : declaration",
"declaration_list : declaration_list declaration",
"decl_specifiers : decl_specifier",
"decl_specifiers : decl_specifiers decl_specifier",
"decl_specifier : storage_class",
"decl_specifier : type_specifier",
"decl_specifier : type_qualifier",
"storage_class : T_AUTO",
"storage_class : T_EXTERN",
"storage_class : T_REGISTER",
"storage_class : T_STATIC",
"storage_class : T_INLINE",
"type_specifier : T_CHAR",
"type_specifier : T_DOUBLE",
"type_specifier : T_FLOAT",
"type_specifier : T_INT",
"type_specifier : T_LONG",
"type_specifier : T_SHORT",
"type_specifier : T_SIGNED",
"type_specifier : T_UNSIGNED",
"type_specifier : T_VOID",
"type_specifier : T_TYPEDEF_NAME",
"type_specifier : struct_or_union_specifier",
"type_specifier : enum_specifier",
"type_qualifier : T_TYPE_QUALIFIER",
"type_qualifier : T_DEFINE_NAME",
"struct_or_union_specifier : struct_or_union any_id braces",
"struct_or_union_specifier : struct_or_union braces",
"struct_or_union_specifier : struct_or_union any_id",
"struct_or_union : T_STRUCT",
"struct_or_union : T_UNION",
"init_declarator_list : init_declarator",
"init_declarator_list : init_declarator_list ',' init_declarator",
"init_declarator : declarator",
"$$5 :",
"init_declarator : declarator '=' $$5 T_INITIALIZER",
"enum_specifier : enumeration any_id braces",
"enum_specifier : enumeration braces",
"enum_specifier : enumeration any_id",
"enumeration : T_ENUM",
"any_id : T_IDENTIFIER",
"any_id : T_TYPEDEF_NAME",
"declarator : pointer direct_declarator",
"declarator : direct_declarator",
"direct_declarator : identifier_or_ref",
"direct_declarator : '(' declarator ')'",
"direct_declarator : direct_declarator T_BRACKETS",
"direct_declarator : direct_declarator '(' parameter_type_list ')'",
"direct_declarator : direct_declarator '(' opt_identifier_list ')'",
"pointer : '*' opt_type_qualifiers",
"pointer : '*' opt_type_qualifiers pointer",
"opt_type_qualifiers :",
"opt_type_qualifiers : type_qualifier_list",
"type_qualifier_list : type_qualifier",
"type_qualifier_list : type_qualifier_list type_qualifier",
"parameter_type_list : parameter_list",
"parameter_type_list : parameter_list ',' T_ELLIPSIS",
"parameter_list : parameter_declaration",
"parameter_list : parameter_list ',' parameter_declaration",
"parameter_declaration : decl_specifiers declarator",
"parameter_declaration : decl_specifiers abs_declarator",
"parameter_declaration : decl_specifiers",
"opt_identifier_list :",
"opt_identifier_list : identifier_list",
"identifier_list : T_IDENTIFIER",
"identifier_list : identifier_list ',' T_IDENTIFIER",
"identifier_or_ref : T_IDENTIFIER",
"identifier_or_ref : '&' T_IDENTIFIER",
"abs_declarator : pointer",
"abs_declarator : pointer direct_abs_declarator",
"abs_declarator : direct_abs_declarator",
"direct_abs_declarator : '(' abs_declarator ')'",
"direct_abs_declarator : direct_abs_declarator T_BRACKETS",
"direct_abs_declarator : T_BRACKETS",
"direct_abs_declarator : direct_abs_declarator '(' parameter_type_list ')'",
"direct_abs_declarator : direct_abs_declarator '(' ')'",
"direct_abs_declarator : '(' parameter_type_list ')'",
"direct_abs_declarator : '(' ')'",
};
#endif
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 10000
#define YYMAXDEPTH 10000
#endif
#endif
#define YYINITSTACKSIZE 200
int yydebug;
int yynerrs;
struct yystack {
    short *ssp;
    YYSTYPE *vsp;
    short *ss;
    YYSTYPE *vs;
    int stacksize;
    short *sslim;
};
int yychar; /* some people use this, so we copy it in & out */
int yyerrflag; /* must be global for yyerrok & YYRECOVERING */
YYSTYPE yylval;
#line 792 "grammar.y"

#if defined(__EMX__) || defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(vms)
# ifdef USE_flex
#  include "lexyy.c"
# else
#  include "lex_yy.c"
# endif
#else
# include "lex.yy.c"
#endif

static void
yaccError (msg)
char *msg;
{
    func_params = NULL;
    put_error();		/* tell what line we're on, and what file */
    fprintf(stderr, "%s at token '%s'\n", msg, yytext);
}

/* Initialize the table of type qualifier keywords recognized by the lexical
 * analyzer.
 */
void
init_parser ()
{
    static char *keywords[] = {
	"const", "volatile", "interrupt",
#ifdef vms
	"noshare", "readonly",
#endif
#if defined(MSDOS) || defined(OS2)
	"cdecl", "far", "huge", "near", "pascal",
	"_cdecl", "_export", "_far", "_fastcall", "_fortran", "_huge",
	"_interrupt", "_loadds", "_near", "_pascal", "_saveregs", "_segment",
	"_cs", "_ds", "_es", "_ss", "_seg",
	"__cdecl", "__export", "__far", "__fastcall", "__fortran", "__huge",
	"__inline", "__interrupt", "__loadds", "__near", "__pascal",
	"__saveregs", "__segment", "__stdcall", "__syscall",
#ifdef OS2
	"__far16",
#endif
#else
	"__const__",    "__const",
	"__volatile__", "__volatile",
	"__inline__",   "__inline",
#endif
    };
    int i;

    /* Initialize type qualifier table. */
    type_qualifiers = new_symbol_table();
    for (i = 0; i < sizeof(keywords)/sizeof(keywords[0]); ++i) {
	new_symbol(type_qualifiers, keywords[i], NULL, DS_NONE);
    }
}

/* Process the C source file.  Write function prototypes to the standard
 * output.  Convert function definitions and write the converted source
 * code to a temporary file.
 */
void
process_file (infile, name)
FILE *infile;
char *name;
{
    char *s;

    if (strlen(name) > 2) {
	s = name + strlen(name) - 2;
	if (*s == '.') {
	    ++s;
	    if (*s == 'l' || *s == 'y')
		BEGIN LEXYACC;
#if defined(MSDOS) || defined(OS2)
	    if (*s == 'L' || *s == 'Y')
		BEGIN LEXYACC;
#endif
	}
    }

    included_files = new_symbol_table();
    typedef_names = new_symbol_table();
    define_names = new_symbol_table();
    inc_depth = -1;
    curly = 0;
    ly_count = 0;
    func_params = NULL;
    yyin = infile;
    include_file(strcpy(base_file, name), func_style != FUNC_NONE);
    if (file_comments) {
#if OPT_LINTLIBRARY
    	if (lintLibrary()) {
	    put_blankline(stdout);
	    begin_tracking();
	}
#endif
	put_string(stdout, "/* ");
	put_string(stdout, cur_file_name());
	put_string(stdout, " */\n");
    }
    yyparse();
    free_symbol_table(define_names);
    free_symbol_table(typedef_names);
    free_symbol_table(included_files);
}

#ifdef NO_LEAKS
void
free_parser()
{
    free_symbol_table (type_qualifiers);
#ifdef FLEX_SCANNER
    if (yy_current_buffer != 0)
	yy_delete_buffer(yy_current_buffer);
#endif
}
#endif
#line 656 "y.tab.c"
/* allocate initial stack */
#if defined(__STDC__) || defined(__cplusplus)
static int yyinitstack(struct yystack *sp)
#else
static int yyinitstack(sp)
    struct yystack *sp;
#endif
{
    int newsize;
    short *newss;
    YYSTYPE *newvs;

    newsize = YYINITSTACKSIZE;
    newss = (short *)malloc(newsize * sizeof *newss);
    newvs = (YYSTYPE *)malloc(newsize * sizeof *newvs);
    sp->ss = sp->ssp = newss;
    sp->vs = sp->vsp = newvs;
    if (newss == NULL || newvs == NULL) return -1;
    sp->stacksize = newsize;
    sp->sslim = newss + newsize - 1;
    return 0;
}

/* double stack size, up to YYMAXDEPTH */
#if defined(__STDC__) || defined(__cplusplus)
static int yygrowstack(struct yystack *sp)
#else
static int yygrowstack(sp)
    struct yystack *sp;
#endif
{
    int newsize, i;
    short *newss;
    YYSTYPE *newvs;

    if ((newsize = sp->stacksize) >= YYMAXDEPTH) return -1;
    if ((newsize *= 2) > YYMAXDEPTH) newsize = YYMAXDEPTH;
    i = sp->ssp - sp->ss;
    if ((newss = (short *)realloc(sp->ss, newsize * sizeof *newss)) == NULL)
        return -1;
    sp->ss = newss;
    sp->ssp = newss + i;
    if ((newvs = (YYSTYPE *)realloc(sp->vs, newsize * sizeof *newvs)) == NULL)
        return -1;
    sp->vs = newvs;
    sp->vsp = newvs + i;
    sp->stacksize = newsize;
    sp->sslim = newss + newsize - 1;
    return 0;
}

#define YYFREESTACK(sp) { free((sp)->ss); free((sp)->vs); }

#define YYABORT goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR goto yyerrlab
int
yyparse()
{
    register int yym, yyn, yystate, yych;
    register YYSTYPE *yyvsp;
    YYSTYPE yyval;
    struct yystack yystk;
#if YYDEBUG
    register char *yys;
    extern char *getenv();

    if (yys = getenv("YYDEBUG"))
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = yych = YYEMPTY;

    if (yyinitstack(&yystk)) goto yyoverflow;
    *yystk.ssp = yystate = 0;

yyloop:
    if (yyn = yydefred[yystate]) goto yyreduce;
    if (yych < 0)
    {
        if ((yych = YYLEX) < 0) yych = 0;
        yychar = yych;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yych <= YYMAXTOKEN) yys = yyname[yych];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yych, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yych) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yych)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yystk.ssp >= yystk.sslim && yygrowstack(&yystk))
            goto yyoverflow;
        *++yystk.ssp = yystate = yytable[yyn];
        *++yystk.vsp = yylval;
        yychar = yych = YYEMPTY;
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yych) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yych)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;
#ifdef lint
    goto yynewerror;
#endif
yynewerror:
    yyerror("syntax error");
#ifdef lint
    goto yyerrlab;
#endif
yyerrlab:
    ++yynerrs;
yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yystk.ssp]) &&
                    (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yystk.ssp, yytable[yyn]);
#endif
                if (yystk.ssp >= yystk.sslim && yygrowstack(&yystk))
                    goto yyoverflow;
                *++yystk.ssp = yystate = yytable[yyn];
                *++yystk.vsp = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yystk.ssp);
#endif
                if (yystk.ssp <= yystk.ss) goto yyabort;
                --yystk.ssp;
                --yystk.vsp;
            }
        }
    }
    else
    {
        if (yych == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yych <= YYMAXTOKEN) yys = yyname[yych];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yych, yys);
        }
#endif
        yychar = yych = YYEMPTY;
        goto yyloop;
    }
yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    yyvsp = yystk.vsp; /* for speed in code under switch() */
    yyval = yyvsp[1-yym];
    switch (yyn)
    {
case 10:
#line 175 "grammar.y"
{
	    yyerrok;
	}
break;
case 11:
#line 179 "grammar.y"
{
	    yyerrok;
	}
break;
case 13:
#line 190 "grammar.y"
{
	    /* Provide an empty action here so bison will not complain about
	     * incompatible types in the default action it normally would
	     * have generated.
	     */
	}
break;
case 14:
#line 197 "grammar.y"
{
	    /* empty */
	}
break;
case 15:
#line 204 "grammar.y"
{
#if OPT_LINTLIBRARY
	    if (types_out && want_typedef()) {
		gen_declarations(&yyvsp[-1].decl_spec, (DeclaratorList *)0);
		flush_varargs();
	    }
#endif
	    free_decl_spec(&yyvsp[-1].decl_spec);
	    end_typedef();
	}
break;
case 16:
#line 215 "grammar.y"
{
	    if (func_params != NULL) {
		set_param_types(func_params, &yyvsp[-2].decl_spec, &yyvsp[-1].decl_list);
	    } else {
		gen_declarations(&yyvsp[-2].decl_spec, &yyvsp[-1].decl_list);
#if OPT_LINTLIBRARY
		flush_varargs();
#endif
		free_decl_list(&yyvsp[-1].decl_list);
	    }
	    free_decl_spec(&yyvsp[-2].decl_spec);
	    end_typedef();
	}
break;
case 17:
#line 229 "grammar.y"
{
	    cur_decl_spec_flags = yyvsp[0].decl_spec.flags;
	    free_decl_spec(&yyvsp[0].decl_spec);
	}
break;
case 18:
#line 234 "grammar.y"
{
	    end_typedef();
	}
break;
case 19:
#line 241 "grammar.y"
{
	    begin_typedef();
	}
break;
case 22:
#line 253 "grammar.y"
{
	    int flags = cur_decl_spec_flags;

	    /* If the typedef is a pointer type, then reset the short type
	     * flags so it does not get promoted.
	     */
	    if (strcmp(yyvsp[0].declarator->text, yyvsp[0].declarator->name) != 0)
		flags &= ~(DS_CHAR | DS_SHORT | DS_FLOAT);
	    new_symbol(typedef_names, yyvsp[0].declarator->name, NULL, flags);
	    free_declarator(yyvsp[0].declarator);
	}
break;
case 23:
#line 265 "grammar.y"
{
	    int flags = cur_decl_spec_flags;

	    if (strcmp(yyvsp[0].declarator->text, yyvsp[0].declarator->name) != 0)
		flags &= ~(DS_CHAR | DS_SHORT | DS_FLOAT);
	    new_symbol(typedef_names, yyvsp[0].declarator->name, NULL, flags);
	    free_declarator(yyvsp[0].declarator);
	}
break;
case 24:
#line 277 "grammar.y"
{
	    check_untagged(&yyvsp[-1].decl_spec);
	    if (yyvsp[0].declarator->func_def == FUNC_NONE) {
		yyerror("syntax error");
		YYERROR;
	    }
	    func_params = &(yyvsp[0].declarator->head->params);
	    func_params->begin_comment = cur_file->begin_comment;
	    func_params->end_comment = cur_file->end_comment;
	}
break;
case 25:
#line 288 "grammar.y"
{
	    /* If we're converting to K&R and we've got a nominally K&R
	     * function which has a parameter which is ANSI (i.e., a prototyped
	     * function pointer), then we must override the deciphered value of
	     * 'func_def' so that the parameter will be converted.
	     */
	    if (func_style == FUNC_TRADITIONAL
	     && haveAnsiParam()
	     && yyvsp[-3].declarator->head->func_def == func_style) {
		yyvsp[-3].declarator->head->func_def = FUNC_BOTH;
	    }

	    func_params = NULL;

	    if (cur_file->convert)
		gen_func_definition(&yyvsp[-4].decl_spec, yyvsp[-3].declarator);
	    gen_prototype(&yyvsp[-4].decl_spec, yyvsp[-3].declarator);
#if OPT_LINTLIBRARY
	    flush_varargs();
#endif
	    free_decl_spec(&yyvsp[-4].decl_spec);
	    free_declarator(yyvsp[-3].declarator);
	}
break;
case 27:
#line 313 "grammar.y"
{
	    if (yyvsp[0].declarator->func_def == FUNC_NONE) {
		yyerror("syntax error");
		YYERROR;
	    }
	    func_params = &(yyvsp[0].declarator->head->params);
	    func_params->begin_comment = cur_file->begin_comment;
	    func_params->end_comment = cur_file->end_comment;
	}
break;
case 28:
#line 323 "grammar.y"
{
	    DeclSpec decl_spec;

	    func_params = NULL;

	    new_decl_spec(&decl_spec, dft_decl_spec(), yyvsp[-4].declarator->begin, DS_NONE);
	    if (cur_file->convert)
		gen_func_definition(&decl_spec, yyvsp[-4].declarator);
	    gen_prototype(&decl_spec, yyvsp[-4].declarator);
#if OPT_LINTLIBRARY
	    flush_varargs();
#endif
	    free_decl_spec(&decl_spec);
	    free_declarator(yyvsp[-4].declarator);
	}
break;
case 35:
#line 354 "grammar.y"
{
	    join_decl_specs(&yyval.decl_spec, &yyvsp[-1].decl_spec, &yyvsp[0].decl_spec);
	    free(yyvsp[-1].decl_spec.text);
	    free(yyvsp[0].decl_spec.text);
	}
break;
case 39:
#line 369 "grammar.y"
{
	    new_decl_spec(&yyval.decl_spec, yyvsp[0].text.text, yyvsp[0].text.begin, DS_NONE);
	}
break;
case 40:
#line 373 "grammar.y"
{
	    new_decl_spec(&yyval.decl_spec, yyvsp[0].text.text, yyvsp[0].text.begin, DS_EXTERN);
	}
break;
case 41:
#line 377 "grammar.y"
{
	    new_decl_spec(&yyval.decl_spec, yyvsp[0].text.text, yyvsp[0].text.begin, DS_NONE);
	}
break;
case 42:
#line 381 "grammar.y"
{
	    new_decl_spec(&yyval.decl_spec, yyvsp[0].text.text, yyvsp[0].text.begin, DS_STATIC);
	}
break;
case 43:
#line 385 "grammar.y"
{
	    new_decl_spec(&yyval.decl_spec, yyvsp[0].text.text, yyvsp[0].text.begin, DS_JUNK);
	}
break;
case 44:
#line 392 "grammar.y"
{
	    new_decl_spec(&yyval.decl_spec, yyvsp[0].text.text, yyvsp[0].text.begin, DS_CHAR);
	}
break;
case 45:
#line 396 "grammar.y"
{
	    new_decl_spec(&yyval.decl_spec, yyvsp[0].text.text, yyvsp[0].text.begin, DS_NONE);
	}
break;
case 46:
#line 400 "grammar.y"
{
	    new_decl_spec(&yyval.decl_spec, yyvsp[0].text.text, yyvsp[0].text.begin, DS_FLOAT);
	}
break;
case 47:
#line 404 "grammar.y"
{
	    new_decl_spec(&yyval.decl_spec, yyvsp[0].text.text, yyvsp[0].text.begin, DS_NONE);
	}
break;
case 48:
#line 408 "grammar.y"
{
	    new_decl_spec(&yyval.decl_spec, yyvsp[0].text.text, yyvsp[0].text.begin, DS_NONE);
	}
break;
case 49:
#line 412 "grammar.y"
{
	    new_decl_spec(&yyval.decl_spec, yyvsp[0].text.text, yyvsp[0].text.begin, DS_SHORT);
	}
break;
case 50:
#line 416 "grammar.y"
{
	    new_decl_spec(&yyval.decl_spec, yyvsp[0].text.text, yyvsp[0].text.begin, DS_NONE);
	}
break;
case 51:
#line 420 "grammar.y"
{
	    new_decl_spec(&yyval.decl_spec, yyvsp[0].text.text, yyvsp[0].text.begin, DS_NONE);
	}
break;
case 52:
#line 424 "grammar.y"
{
	    new_decl_spec(&yyval.decl_spec, yyvsp[0].text.text, yyvsp[0].text.begin, DS_NONE);
	}
break;
case 53:
#line 428 "grammar.y"
{
	    Symbol *s;
	    s = find_symbol(typedef_names, yyvsp[0].text.text);
	    if (s != NULL)
		new_decl_spec(&yyval.decl_spec, yyvsp[0].text.text, yyvsp[0].text.begin, s->flags);
	}
break;
case 56:
#line 440 "grammar.y"
{
	    new_decl_spec(&yyval.decl_spec, yyvsp[0].text.text, yyvsp[0].text.begin, DS_NONE);
	}
break;
case 57:
#line 444 "grammar.y"
{
	    /* This rule allows the <pointer> nonterminal to scan #define
	     * names as if they were type modifiers.
	     */
	    Symbol *s;
	    s = find_symbol(define_names, yyvsp[0].text.text);
	    if (s != NULL)
		new_decl_spec(&yyval.decl_spec, yyvsp[0].text.text, yyvsp[0].text.begin, s->flags);
	}
break;
case 58:
#line 457 "grammar.y"
{
	    char *s;
	    if ((s = implied_typedef()) == 0)
	        (void)sprintf(s = buf, "%s %s", yyvsp[-2].text.text, yyvsp[-1].text.text);
	    new_decl_spec(&yyval.decl_spec, s, yyvsp[-2].text.begin, DS_NONE);
	}
break;
case 59:
#line 464 "grammar.y"
{
	    char *s;
	    if ((s = implied_typedef()) == 0)
		(void)sprintf(s = buf, "%s {}", yyvsp[-1].text.text);
	    new_decl_spec(&yyval.decl_spec, s, yyvsp[-1].text.begin, DS_NONE);
	}
break;
case 60:
#line 471 "grammar.y"
{
	    (void)sprintf(buf, "%s %s", yyvsp[-1].text.text, yyvsp[0].text.text);
	    new_decl_spec(&yyval.decl_spec, buf, yyvsp[-1].text.begin, DS_NONE);
	}
break;
case 61:
#line 479 "grammar.y"
{
	    imply_typedef(yyval.text.text);
	}
break;
case 62:
#line 483 "grammar.y"
{
	    imply_typedef(yyval.text.text);
	}
break;
case 63:
#line 490 "grammar.y"
{
	    new_decl_list(&yyval.decl_list, yyvsp[0].declarator);
	}
break;
case 64:
#line 494 "grammar.y"
{
	    add_decl_list(&yyval.decl_list, &yyvsp[-2].decl_list, yyvsp[0].declarator);
	}
break;
case 65:
#line 501 "grammar.y"
{
	    if (yyvsp[0].declarator->func_def != FUNC_NONE && func_params == NULL &&
		func_style == FUNC_TRADITIONAL && cur_file->convert) {
		gen_func_declarator(yyvsp[0].declarator);
		fputs(cur_text(), cur_file->tmp_file);
	    }
	    cur_declarator = yyval.declarator;
	}
break;
case 66:
#line 510 "grammar.y"
{
	    if (yyvsp[-1].declarator->func_def != FUNC_NONE && func_params == NULL &&
		func_style == FUNC_TRADITIONAL && cur_file->convert) {
		gen_func_declarator(yyvsp[-1].declarator);
		fputs(" =", cur_file->tmp_file);
	    }
	}
break;
case 68:
#line 522 "grammar.y"
{
	    char *s;
	    if ((s = implied_typedef()) == 0)
		(void)sprintf(s = buf, "enum %s", yyvsp[-1].text.text);
	    new_decl_spec(&yyval.decl_spec, s, yyvsp[-2].text.begin, DS_NONE);
	}
break;
case 69:
#line 529 "grammar.y"
{
	    char *s;
	    if ((s = implied_typedef()) == 0)
		(void)sprintf(s = buf, "%s {}", yyvsp[-1].text.text);
	    new_decl_spec(&yyval.decl_spec, s, yyvsp[-1].text.begin, DS_NONE);
	}
break;
case 70:
#line 536 "grammar.y"
{
	    (void)sprintf(buf, "enum %s", yyvsp[0].text.text);
	    new_decl_spec(&yyval.decl_spec, buf, yyvsp[-1].text.begin, DS_NONE);
	}
break;
case 71:
#line 544 "grammar.y"
{
	    imply_typedef("enum");
	    yyval.text = yyvsp[0].text;
	}
break;
case 74:
#line 557 "grammar.y"
{
	    yyval.declarator = yyvsp[0].declarator;
	    (void)sprintf(buf, "%s%s", yyvsp[-1].text.text, yyval.declarator->text);
	    free(yyval.declarator->text);
	    yyval.declarator->text = xstrdup(buf);
	    yyval.declarator->begin = yyvsp[-1].text.begin;
	    yyval.declarator->pointer = TRUE;
	}
break;
case 76:
#line 570 "grammar.y"
{
	    yyval.declarator = new_declarator(yyvsp[0].text.text, yyvsp[0].text.text, yyvsp[0].text.begin);
	}
break;
case 77:
#line 574 "grammar.y"
{
	    yyval.declarator = yyvsp[-1].declarator;
	    (void)sprintf(buf, "(%s)", yyval.declarator->text);
	    free(yyval.declarator->text);
	    yyval.declarator->text = xstrdup(buf);
	    yyval.declarator->begin = yyvsp[-2].text.begin;
	}
break;
case 78:
#line 582 "grammar.y"
{
	    yyval.declarator = yyvsp[-1].declarator;
	    (void)sprintf(buf, "%s%s", yyval.declarator->text, yyvsp[0].text.text);
	    free(yyval.declarator->text);
	    yyval.declarator->text = xstrdup(buf);
	}
break;
case 79:
#line 589 "grammar.y"
{
	    yyval.declarator = new_declarator("%s()", yyvsp[-3].declarator->name, yyvsp[-3].declarator->begin);
	    yyval.declarator->params = yyvsp[-1].param_list;
	    yyval.declarator->func_stack = yyvsp[-3].declarator;
	    yyval.declarator->head = (yyvsp[-3].declarator->func_stack == NULL) ? yyval.declarator : yyvsp[-3].declarator->head;
	    yyval.declarator->func_def = FUNC_ANSI;
	}
break;
case 80:
#line 597 "grammar.y"
{
	    yyval.declarator = new_declarator("%s()", yyvsp[-3].declarator->name, yyvsp[-3].declarator->begin);
	    yyval.declarator->params = yyvsp[-1].param_list;
	    yyval.declarator->func_stack = yyvsp[-3].declarator;
	    yyval.declarator->head = (yyvsp[-3].declarator->func_stack == NULL) ? yyval.declarator : yyvsp[-3].declarator->head;
	    yyval.declarator->func_def = FUNC_TRADITIONAL;
	}
break;
case 81:
#line 608 "grammar.y"
{
	    (void)sprintf(yyval.text.text, "*%s", yyvsp[0].text.text);
	    yyval.text.begin = yyvsp[-1].text.begin;
	}
break;
case 82:
#line 613 "grammar.y"
{
	    (void)sprintf(yyval.text.text, "*%s%s", yyvsp[-1].text.text, yyvsp[0].text.text);
	    yyval.text.begin = yyvsp[-2].text.begin;
	}
break;
case 83:
#line 621 "grammar.y"
{
	    strcpy(yyval.text.text, "");
	    yyval.text.begin = 0L;
	}
break;
case 85:
#line 630 "grammar.y"
{
	    (void)sprintf(yyval.text.text, "%s ", yyvsp[0].decl_spec.text);
	    yyval.text.begin = yyvsp[0].decl_spec.begin;
	    free(yyvsp[0].decl_spec.text);
	}
break;
case 86:
#line 636 "grammar.y"
{
	    (void)sprintf(yyval.text.text, "%s%s ", yyvsp[-1].text.text, yyvsp[0].decl_spec.text);
	    yyval.text.begin = yyvsp[-1].text.begin;
	    free(yyvsp[0].decl_spec.text);
	}
break;
case 88:
#line 646 "grammar.y"
{
	    add_ident_list(&yyval.param_list, &yyvsp[-2].param_list, "...");
	}
break;
case 89:
#line 653 "grammar.y"
{
	    new_param_list(&yyval.param_list, yyvsp[0].parameter);
	}
break;
case 90:
#line 657 "grammar.y"
{
	    add_param_list(&yyval.param_list, &yyvsp[-2].param_list, yyvsp[0].parameter);
	}
break;
case 91:
#line 664 "grammar.y"
{
	    check_untagged(&yyvsp[-1].decl_spec);
	    yyval.parameter = new_parameter(&yyvsp[-1].decl_spec, yyvsp[0].declarator);
	}
break;
case 92:
#line 669 "grammar.y"
{
	    check_untagged(&yyvsp[-1].decl_spec);
	    yyval.parameter = new_parameter(&yyvsp[-1].decl_spec, yyvsp[0].declarator);
	}
break;
case 93:
#line 674 "grammar.y"
{
	    check_untagged(&yyvsp[0].decl_spec);
	    yyval.parameter = new_parameter(&yyvsp[0].decl_spec, (Declarator *)0);
	}
break;
case 94:
#line 682 "grammar.y"
{
	    new_ident_list(&yyval.param_list);
	}
break;
case 96:
#line 690 "grammar.y"
{
	    new_ident_list(&yyval.param_list);
	    add_ident_list(&yyval.param_list, &yyval.param_list, yyvsp[0].text.text);
	}
break;
case 97:
#line 695 "grammar.y"
{
	    add_ident_list(&yyval.param_list, &yyvsp[-2].param_list, yyvsp[0].text.text);
	}
break;
case 98:
#line 702 "grammar.y"
{
	    yyval.text = yyvsp[0].text;
	}
break;
case 99:
#line 706 "grammar.y"
{
#if OPT_LINTLIBRARY
	    if (lintLibrary()) { /* Lint doesn't grok C++ ref variables */
		yyval.text = yyvsp[0].text;
	    } else
#endif
		(void)sprintf(yyval.text.text, "&%s", yyvsp[0].text.text);
	    yyval.text.begin = yyvsp[-1].text.begin;
	}
break;
case 100:
#line 719 "grammar.y"
{
	    yyval.declarator = new_declarator(yyvsp[0].text.text, "", yyvsp[0].text.begin);
	}
break;
case 101:
#line 723 "grammar.y"
{
	    yyval.declarator = yyvsp[0].declarator;
	    (void)sprintf(buf, "%s%s", yyvsp[-1].text.text, yyval.declarator->text);
	    free(yyval.declarator->text);
	    yyval.declarator->text = xstrdup(buf);
	    yyval.declarator->begin = yyvsp[-1].text.begin;
	}
break;
case 103:
#line 735 "grammar.y"
{
	    yyval.declarator = yyvsp[-1].declarator;
	    (void)sprintf(buf, "(%s)", yyval.declarator->text);
	    free(yyval.declarator->text);
	    yyval.declarator->text = xstrdup(buf);
	    yyval.declarator->begin = yyvsp[-2].text.begin;
	}
break;
case 104:
#line 743 "grammar.y"
{
	    yyval.declarator = yyvsp[-1].declarator;
	    (void)sprintf(buf, "%s%s", yyval.declarator->text, yyvsp[0].text.text);
	    free(yyval.declarator->text);
	    yyval.declarator->text = xstrdup(buf);
	}
break;
case 105:
#line 750 "grammar.y"
{
	    yyval.declarator = new_declarator(yyvsp[0].text.text, "", yyvsp[0].text.begin);
	}
break;
case 106:
#line 754 "grammar.y"
{
	    yyval.declarator = new_declarator("%s()", "", yyvsp[-3].declarator->begin);
	    yyval.declarator->params = yyvsp[-1].param_list;
	    yyval.declarator->func_stack = yyvsp[-3].declarator;
	    yyval.declarator->head = (yyvsp[-3].declarator->func_stack == NULL) ? yyval.declarator : yyvsp[-3].declarator->head;
	    yyval.declarator->func_def = FUNC_ANSI;
	}
break;
case 107:
#line 762 "grammar.y"
{
	    yyval.declarator = new_declarator("%s()", "", yyvsp[-2].declarator->begin);
	    yyval.declarator->func_stack = yyvsp[-2].declarator;
	    yyval.declarator->head = (yyvsp[-2].declarator->func_stack == NULL) ? yyval.declarator : yyvsp[-2].declarator->head;
	    yyval.declarator->func_def = FUNC_ANSI;
	}
break;
case 108:
#line 769 "grammar.y"
{
	    Declarator *d;
	    
	    d = new_declarator("", "", yyvsp[-2].text.begin);
	    yyval.declarator = new_declarator("%s()", "", yyvsp[-2].text.begin);
	    yyval.declarator->params = yyvsp[-1].param_list;
	    yyval.declarator->func_stack = d;
	    yyval.declarator->head = yyval.declarator;
	    yyval.declarator->func_def = FUNC_ANSI;
	}
break;
case 109:
#line 780 "grammar.y"
{
	    Declarator *d;
	    
	    d = new_declarator("", "", yyvsp[-1].text.begin);
	    yyval.declarator = new_declarator("%s()", "", yyvsp[-1].text.begin);
	    yyval.declarator->func_stack = d;
	    yyval.declarator->head = yyval.declarator;
	    yyval.declarator->func_def = FUNC_ANSI;
	}
break;
#line 1495 "y.tab.c"
    }
    yystk.ssp -= yym;
    yystate = *yystk.ssp;
    yystk.vsp -= yym;
    yym = yylhs[yyn];
    yych = yychar;
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yystk.ssp = YYFINAL;
        *++yystk.vsp = yyval;
        if (yych < 0)
        {
            if ((yych = YYLEX) < 0) yych = 0;
            yychar = yych;
#if YYDEBUG
            if (yydebug)
            {
                yys = 0;
                if (yych <= YYMAXTOKEN) yys = yyname[yych];
                if (!yys) yys = "illegal-symbol";
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yych, yys);
            }
#endif
        }
        if (yych == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yystk.ssp, yystate);
#endif
    if (yystk.ssp >= yystk.sslim && yygrowstack(&yystk))
        goto yyoverflow;
    *++yystk.ssp = yystate;
    *++yystk.vsp = yyval;
    goto yyloop;
yyoverflow:
    yyerror("yacc stack overflow");
yyabort:
    YYFREESTACK(&yystk);
    return (1);
yyaccept:
    YYFREESTACK(&yystk);
    return (0);
}

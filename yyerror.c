/* $Id: yyerror.c,v 4.4 2000/10/29 00:47:16 cthuang Exp $
 *
 * This file is included into grammar.y to provide the 'yyerror()' function. 
 * If the yacc/bison parser is one that we know how to backtrack, we'll augment
 * the "syntax error" message with information that shows what type of token we
 * expected.
 */

/* 'yyerror()' has to be a macro, since it must be expanded inline to subvert
 * the internal state of 'yyparse()'.
 */
#if BISON_HAS_YYTNAME	/* bison 1.22 */
#if YYDEBUG
/* this is better than defining YYERROR_VERBOSE */
#define	yyerror(text) {\
    register int n, c1, count = 0;\
    yaccError(text);\
    if (yypact[yystate] != YYFLAG) { \
	for (c1 = 0; c1 < YYLAST; c1++) {\
	    n = yypact[yystate] + c1;\
	    if (n >= 0 && n < YYLAST\
	     && yycheck[n] == c1 && yytname[c1] != 0)\
		yaccExpected(yytname[c1], count++);\
	}\
    }\
    yaccExpected("", -1);\
}
#endif
#endif	/* BISON_HAS_YYTNAME */

#if YACC_HAS_YYTOKS_2
#undef  YACC_HAS_YYTOKS
#define YACC_HAS_YYTOKS 1
#define YaccState yy_state
#endif

#if YACC_HAS_YYTOKS		/* Solaris, Gould */
#if YYDEBUG
#ifndef YaccState
#define YaccState yystate	/* sometimes 'yy_state' */
#endif
#define	yyerror(text) {\
    register int	n, x, c1, y, count = 0;\
    yaccError(text);\
    if (((n = yypact[YaccState]) > YYFLAG) && (n < YYLAST)) {\
	for (x = ((n > 0) ? n : 0); x < YYLAST; ++x) {\
	    c1 = x - n;\
	    if ((yychk[yyact[x]] == c1) && (c1 != YYERRCODE)) {\
		if (isascii(c1)) {\
		    static char tmp[] = "'%'";\
		    tmp[1] = c1;\
		    yaccExpected(tmp, count++);\
		} else {\
		    for (y = 0; yytoks[y].t_val >= 0; y++) {\
			if (yytoks[y].t_val == c1) {\
			    yaccExpected(yytoks[y].t_name, count++);\
			    break;\
			}\
		    }\
		}\
	    }\
	}\
    }\
    yaccExpected("", -1);\
}
#endif
#endif	/* YACC_HAS_YYTOKS */

#if YACC_HAS_YYNAME	/* Linux's yacc */
#if YYDEBUG
#define	yyerror(text) {\
    register int n, x, c1, count = 0;\
    yaccError(text);\
    if (((n = yysindex[yystate]) != 0) && (n < YYTABLESIZE)) {\
	for (x = ((n > 0) ? n : 0); x < YYTABLESIZE; ++x) {\
	    c1 = x - n;\
	    if ((yycheck[x] == c1) && (c1 != YYERRCODE)) {\
		yaccExpected(yyname[c1] ? yyname[c1] : "illegal symbol",\
		    count++);\
	    }\
	}\
    }\
    yaccExpected("", -1);\
} 
#endif
#endif	/* YACC_HAS_YYNAME */


/*
 * Any way we define it, 'yyerror()' is a real function (that we provide,
 * rather than use the one from a library).
 */
static void yaccError    ARGS((char *));

#ifdef yyerror
static int  compar       ARGS((const void *a, const void *b));
static void yaccExpected ARGS((const char *, int count));

static
int compar(p1, p2)
const void *p1;
const void *p2;
{
    return (strcmp(*(char **)p1, *(char **)p2));
}

#define MSGLEN 80

static
void yaccExpected (s, count)
const char *s;
int count;
{
    static struct {
	char *actual, *name;
    } tbl[] = {
	{"...",	"T_ELLIPSIS"},
	{"[]",	"T_BRACKETS"},
	{"{",	"T_LBRACE"},
	{"}",	"T_MATCHRBRACE"},
    };
    register int j, k, x;
    unsigned n;
    char *t = (char *)s;
    char *tag;
    char tmp[MSGLEN];
    static unsigned have;
    static unsigned used;
    static char	**vec;

    if (count < 0) {
	if (used != 0) {
	    if (used > 1)
		qsort((char *)vec, used, sizeof(vec[0]), compar);
	    /* limit length of error message */
	    k = MSGLEN - (strlen(vec[used-1]) + 2);
	    for (j = 0; j < used; j++) {
		tag = j ? " " : "Expected: ";
		s = vec[j];
		if (j != (used - 1)) {
		    x = strlen(s) + strlen(tag);
		    if (k <= 0)
			continue;
		    else if ((k - x) <= 0)
			s = "...";
		    k -= x;
		}
		fprintf(stderr, "%s%s", tag, s);
	    }
	    fprintf(stderr, "\n");
	    while (used-- != 0) {
	    	free(vec[used]);
		vec[used] = 0;
	    }
	}
	used = 0;
    } else {
	if (!strncmp(t, "T_", 2)) {
	    int	found = FALSE;
	    for (j = 0; j < sizeof(tbl)/sizeof(tbl[0]); j++) {
		if (!strcmp(t, tbl[j].name)) {
		    t = tbl[j].actual;
		    found = TRUE;
		    break;
		}
	    }
	    if (!found) {
		t = strncpy(tmp, t + 2, sizeof(tmp)-1);
		for (k = 0; t[k] != '\0'; k++) {
		    if (t[k] == '_')
			t[k] = '-';
		    else if (isalpha(t[k]) && isupper(t[k]))
			t[k] = tolower(t[k]);
		}
	    }
	}
	if (count >= have) {
	    have = (count + 10);
	    if (vec == 0) {
		vec = malloc(have * sizeof(*vec));
	    } else {
		vec = realloc(vec, have * sizeof(*vec));
	    }
	    for (n = used; n < have; n++)
		vec[n] = 0;
	}
	if (vec[count] != 0) {
	    free(vec[count]);
	}
	vec[count] = xstrdup(t);
	used = count + 1;
    }
}
#else
#define yyerror(s) yaccError(s)
#endif	/* yyerror */

/* $Id: yyerror.c,v 3.5 1994/08/09 01:07:32 tom Exp $
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
#define	yyerror(text)	\
	{ register int	n, c1, count = 0;\
		YaccError(text);\
		if (yypact[yystate] != YYFLAG)\
			for (c1 = 0; c1 < YYLAST; c1++) {\
				n = yypact[yystate] + c1;\
				if (n >= 0 && n < YYLAST\
				&& yycheck[n] == c1 && yytname[c1] != 0)\
					YaccExpected(yytname[c1], count++);\
			}\
		YaccExpected("", -1);\
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
#define	yyerror(text)	{\
	register int	n, x, c1, y, count = 0;\
	YaccError(text);\
	if (((n = yypact[YaccState]) > YYFLAG) && (n < YYLAST)) {\
	    for (x = ((n > 0) ? n : 0); x < YYLAST; ++x) {\
		c1 = x - n;\
		if ((yychk[yyact[x]] == c1) && (c1 != YYERRCODE)) {\
		    if (isascii(c1)) {\
			static char tmp[] = "'%'";\
			tmp[1] = c1;\
			YaccExpected(tmp, count++);\
		    } else {\
			for (y = 0; yytoks[y].t_val >= 0; y++) {\
			    if (yytoks[y].t_val == c1) {\
				YaccExpected(yytoks[y].t_name, count++);\
				break;\
			    }\
			}\
		    }\
		}\
	    }\
	}\
	YaccExpected("", -1);\
    }
#endif
#endif	/* YACC_HAS_YYTOKS */

#if YACC_HAS_YYNAME	/* Linux's yacc */
#if YYDEBUG
#define	yyerror(text)	{\
	register int	n, x, c1, count = 0;\
	YaccError(text);\
	if (((n = yysindex[yystate]) != 0) && (n < YYTABLESIZE)) {\
	    for (x = ((n > 0) ? n : 0); x < YYTABLESIZE; ++x) {\
		c1 = x - n;\
		if ((yycheck[x] == c1) && (c1 != YYERRCODE)) {\
		    YaccExpected(yyname[c1] ? yyname[c1] : "illegal symbol",\
			count++);\
		}\
	    }\
	}\
	YaccExpected("", -1);\
    } 
#endif
#endif	/* YACC_HAS_YYNAME */


/*
 * Any way we define it, 'yyerror()' is a real function (that we provide,
 * rather than use the one from a library).
 */
static void YaccError    ARGS((char *));

#ifdef	yyerror
static int  compar       ARGS((const void *a, const void *b));
static void YaccExpected ARGS((const char *, int count));

static
int compar(p1, p2)
const void *p1;
const void *p2;
{
	return (strcmp(*(char **)p1, *(char **)p2));
}

static
void	YaccExpected(s, count)
	const char *s;
	int	count;
{
	static	struct	{
		char	*actual, *name;
		} tbl[] = {
		{"...",	"T_ELLIPSIS"},
		{"[]",	"T_BRACKETS"},
		{"{",	"T_LBRACE"},
		{"}",	"T_MATCHRBRACE"},
		};
	register int	j, k, x;
	auto	char	*t = (char *)s;
	auto	char	*tag;
	auto	char	tmp[80];
	static	int	last;
	static	char	*vec[10];	/* patch: don't know how big */

	if (count < 0) {
		if (last++ >= 0) {
			qsort((char *)vec, (size_t)last, sizeof(vec[0]), compar);
			k = 80	/* limit length of error message */
				- (strlen(vec[last-1]) + 2);
			for (j = 0; j < last; j++) {
				tag = j ? " " : "Expected: ";
				s = vec[j];
				if (j != (last - 1)) {
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
		}
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
		if (count >= (sizeof(vec)/sizeof(vec[0]))-1)
			count = (sizeof(vec)/sizeof(vec[0]))-1;
		vec[count] = xstrdup(t);
	}
	last = count;
}
#else
#define yyerror(s) YaccError(s)
#endif	/* yyerror */

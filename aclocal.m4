dnl $Id: aclocal.m4,v 3.4 1994/09/24 19:42:22 tom Exp $
dnl
dnl	Test the supplied version of yacc to see which (if any) of the
dnl	error-reporting enhancements will work.
dnl
dnl	Also, test the preprocessor to see if it will handle non-C files.
dnl	(gcc 2.5.8 doesn't).
dnl
define([TD_YACC_ERROR],
[
rm -f yacctest.y
cat >yacctest.y <<EOF
%{
#define xstrdup(s) s
#include "system.h"
#include <stdio.h>
#include "yyerror.c"
static void YaccError(s) char *s; { exit(0); }
int yylex ARGS((void))
{ return 1; }
%}
%%
dummy
	: /* empty */
	| error '.'
	;
%%
EOF
$CPP yacctest.y >yacctest.out 2>/dev/null
if test ! -s yacctest.out
then
	AC_DEFINE(CPP_DOES_ONLY_C_FILES)
fi
$YACC yacctest.y
rm -f yacctest.*
td_incl='#include "y.tab.c"'
for COND in BISON_HAS_YYTNAME YACC_HAS_YYTOKS YACC_HAS_YYTOKS_2 YACC_HAS_YYNAME
do
AC_COMPILE_CHECK(error-reporting with $COND,
[#define $COND 1
#define YYDEBUG 1
$td_incl],,[
AC_DEFINE($COND)
break
])
done
rm -f y.tab.c
])dnl
dnl
dnl	Check to ensure that our prototype for 'popen()' doesn't conflict
dnl	with the system's (this is a problem on AIX and CLIX).
dnl
define([TD_POPEN_TEST],
[AC_COMPILE_CHECK(conflicting prototype for popen,
[#include <stdio.h>
#include "system.h"
extern FILE *popen ARGS((const char *c, const char *m));],,
[AC_DEFINE(OK_POPEN_PROTOTYPE)])])dnl
dnl
dnl	This should have been in autoconf 1.11
dnl
define([TD_SIZE_T],
[AC_COMPILE_CHECK(size_t in <sys/types.h> or <stdio.h>,
[#include <sys/types.h>
#include <stdio.h>],[size_t f],,[AC_DEFINE(size_t,unsigned)])])dnl

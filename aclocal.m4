dnl $Id: aclocal.m4,v 3.1 1994/08/09 01:06:16 tom Exp $
dnl
dnl	Test the supplied version of yacc to see which (if any) of the
dnl	error-reporting enhancements will work.
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
$YACC yacctest.y
rm -f yacctest.y
td_incl='#include "y.tab.c"'
for COND in BISON_HAS_YYTNAME YACC_HAS_YYTOKS YACC_HAS_YYTOKS_2 YACC_HAS_YYNAME
do
AC_COMPILE_CHECK(error-reporting with $COND,
[#define $COND 1
$td_incl],,[
AC_DEFINE($COND)
break
])
done
rm -f y.tab.c
])dnl
define([TD_SIZE_T],
[AC_COMPILE_CHECK(size_t in <sys/types.h> or <stdio.h>,
[#include <sys/types.h>
#include <stdio.h>],[size_t f],,[AC_DEFINE(size_t,unsigned)])])dnl

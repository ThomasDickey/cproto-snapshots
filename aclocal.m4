dnl $Id: aclocal.m4,v 4.2.1.1 1995/02/24 11:19:33 tom Exp $
dnl
dnl	Test the supplied version of yacc to see which (if any) of the
dnl	error-reporting enhancements will work.
dnl
dnl	Also, test the preprocessor to see if it will handle non-C files.
dnl	(gcc 2.5.8 doesn't).
dnl
AC_DEFUN([TD_YACC_ERROR],
[
rm -f yacctest.y
cat >yacctest.y <<EOF
%{
#define xstrdup(s) s
#include "system.h"
#include <stdio.h>
#include <ctype.h>
#include "yyerror.c"
static void yaccError(s) char *s; { exit(0); }
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
AC_MSG_CHECKING(for error-reporting with $COND)
AC_CACHE_VAL(ac_cv_$COND,
AC_TRY_LINK([
#define $COND 1
#define YYDEBUG 1
$td_incl],,
eval 'ac_cv_'$COND'=yes',
eval 'ac_cv_'$COND'=no'))
eval 'td_result=$ac_cv_'$COND
AC_MSG_RESULT($td_result)
if test $td_result = yes; then
	AC_DEFINE_UNQUOTED($COND)
	break
fi
done
rm -f y.tab.c
])dnl
dnl
dnl	Check to ensure that our prototype for 'popen()' doesn't conflict
dnl	with the system's (this is a problem on AIX and CLIX).
dnl
AC_DEFUN([TD_POPEN_TEST],
[AC_MSG_CHECKING(for conflicting prototype for popen)
AC_CACHE_VAL(ac_cv_td_popen,
AC_TRY_LINK([
#include <stdio.h>
#include "system.h"
extern int pclose ARGS((FILE *p));
extern FILE *popen ARGS((const char *c, const char *m));],,
ac_cv_td_popen=no,
ac_cv_td_popen=yes))
AC_MSG_RESULT($ac_cv_td_popen)
if test $ac_cv_td_popen = yes; then
	AC_DEFINE(HAVE_POPEN_PROTOTYPE)
fi
])dnl
dnl
dnl	This is a more stringent test for size_t than the one distributed with
dnl	autoconf 2.1: some systems (CLIX, Ultrix) define size_t in <stdio.h>
dnl
AC_DEFUN([TD_SIZE_T],
[AC_REQUIRE([AC_HEADER_STDC])dnl
AC_MSG_CHECKING(for size_t in <sys/types.h> or <stdio.h>)
AC_CACHE_VAL(ac_cv_td_size_t,
AC_TRY_LINK([
#include <sys/types.h>
#if STDC_HEADERS
#include <stdlib.h>
#endif
#include <stdio.h>],[size_t f = 0],
ac_cv_td_size_t=yes,
ac_cv_td_size_t=no))
AC_MSG_RESULT($ac_cv_td_size_t)
if test $ac_cv_td_size_t = no; then
	AC_DEFINE(size_t, unsigned)
fi
])dnl

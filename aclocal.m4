dnl $Id: aclocal.m4,v 4.4 2000/10/29 00:47:16 cthuang Exp $
dnl
dnl Macros for cproto configure script (T.Dickey)
dnl ---------------------------------------------------------------------------
dnl	Test the supplied version of yacc to see which (if any) of the
dnl	error-reporting enhancements will work.
dnl
dnl	Also, test the preprocessor to see if it will handle non-C files.
dnl	(gcc 2.5.8 doesn't).
dnl
AC_DEFUN([CF_YACC_ERROR],
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
dnl ---------------------------------------------------------------------------
dnl	Check to ensure that our prototype for 'popen()' doesn't conflict
dnl	with the system's (this is a problem on AIX and CLIX).
dnl
AC_DEFUN([CF_POPEN_TEST],
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
dnl ---------------------------------------------------------------------------
dnl	On both Ultrix and CLIX, I find size_t defined in <stdio.h>
AC_DEFUN([CF_SIZE_T],
[
AC_MSG_CHECKING(for size_t in <sys/types.h> or <stdio.h>)
AC_CACHE_VAL(cf_cv_type_size_t,[
	AC_TRY_COMPILE([
#include <sys/types.h>
#ifdef STDC_HEADERS
#include <stdlib.h>
#include <stddef.h>
#endif
#include <stdio.h>],
		[size_t x],
		[cf_cv_type_size_t=yes],
		[cf_cv_type_size_t=no])
	])
AC_MSG_RESULT($cf_cv_type_size_t)
test $cf_cv_type_size_t = no && AC_DEFINE(size_t, unsigned)
])dnl

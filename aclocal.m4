dnl $Id: aclocal.m4,v 4.4.1.2 2004/03/24 19:32:49 tom Exp $
dnl
dnl Macros for cproto configure script (T.Dickey)
dnl ---------------------------------------------------------------------------
dnl ---------------------------------------------------------------------------
dnl CF_ADD_CFLAGS version: 5 updated: 2002/12/01 00:12:15
dnl -------------
dnl Copy non-preprocessor flags to $CFLAGS, preprocessor flags to $CPPFLAGS
dnl The second parameter if given makes this macro verbose.
AC_DEFUN([CF_ADD_CFLAGS],
[
cf_new_cflags=
cf_new_cppflags=
for cf_add_cflags in $1
do
	case $cf_add_cflags in #(vi
	-undef|-nostdinc*|-I*|-D*|-U*|-E|-P|-C) #(vi
		case "$CPPFLAGS" in
		*$cf_add_cflags) #(vi
			;;
		*) #(vi
			cf_new_cppflags="$cf_new_cppflags $cf_add_cflags"
			;;
		esac
		;;
	*)
		cf_new_cflags="$cf_new_cflags $cf_add_cflags"
		;;
	esac
done

if test -n "$cf_new_cflags" ; then
	ifelse($2,,,[CF_VERBOSE(add to \$CFLAGS $cf_new_cflags)])
	CFLAGS="$CFLAGS $cf_new_cflags"
fi

if test -n "$cf_new_cppflags" ; then
	ifelse($2,,,[CF_VERBOSE(add to \$CPPFLAGS $cf_new_cppflags)])
	CPPFLAGS="$cf_new_cppflags $CPPFLAGS"
fi

])dnl
dnl ---------------------------------------------------------------------------
dnl CF_ANSI_CC_CHECK version: 9 updated: 2001/12/30 17:53:34
dnl ----------------
dnl This is adapted from the macros 'fp_PROG_CC_STDC' and 'fp_C_PROTOTYPES'
dnl in the sharutils 4.2 distribution.
AC_DEFUN([CF_ANSI_CC_CHECK],
[
AC_CACHE_CHECK(for ${CC-cc} option to accept ANSI C, cf_cv_ansi_cc,[
cf_cv_ansi_cc=no
cf_save_CFLAGS="$CFLAGS"
cf_save_CPPFLAGS="$CPPFLAGS"
# Don't try gcc -ansi; that turns off useful extensions and
# breaks some systems' header files.
# AIX			-qlanglvl=ansi
# Ultrix and OSF/1	-std1
# HP-UX			-Aa -D_HPUX_SOURCE
# SVR4			-Xc
# UnixWare 1.2		(cannot use -Xc, since ANSI/POSIX clashes)
for cf_arg in "-DCC_HAS_PROTOS" \
	"" \
	-qlanglvl=ansi \
	-std1 \
	-Ae \
	"-Aa -D_HPUX_SOURCE" \
	-Xc
do
	CF_ADD_CFLAGS($cf_arg)
	AC_TRY_COMPILE(
[
#ifndef CC_HAS_PROTOS
#if !defined(__STDC__) || (__STDC__ != 1)
choke me
#endif
#endif
],[
	int test (int i, double x);
	struct s1 {int (*f) (int a);};
	struct s2 {int (*f) (double a);};],
	[cf_cv_ansi_cc="$cf_arg"; break])
done
CFLAGS="$cf_save_CFLAGS"
CPPFLAGS="$cf_save_CPPFLAGS"
])

if test "$cf_cv_ansi_cc" != "no"; then
if test ".$cf_cv_ansi_cc" != ".-DCC_HAS_PROTOS"; then
	CF_ADD_CFLAGS($cf_cv_ansi_cc)
else
	AC_DEFINE(CC_HAS_PROTOS)
fi
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_ANSI_CC_REQD version: 3 updated: 1997/09/06 13:40:44
dnl ---------------
dnl For programs that must use an ANSI compiler, obtain compiler options that
dnl will make it recognize prototypes.  We'll do preprocessor checks in other
dnl macros, since tools such as unproto can fake prototypes, but only part of
dnl the preprocessor.
AC_DEFUN([CF_ANSI_CC_REQD],
[AC_REQUIRE([CF_ANSI_CC_CHECK])
if test "$cf_cv_ansi_cc" = "no"; then
	AC_ERROR(
[Your compiler does not appear to recognize prototypes.
You have the following choices:
	a. adjust your compiler options
	b. get an up-to-date compiler
	c. use a wrapper such as unproto])
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_ARG_DISABLE version: 3 updated: 1999/03/30 17:24:31
dnl --------------
dnl Allow user to disable a normally-on option.
AC_DEFUN([CF_ARG_DISABLE],
[CF_ARG_OPTION($1,[$2],[$3],[$4],yes)])dnl
dnl ---------------------------------------------------------------------------
dnl CF_ARG_OPTION version: 3 updated: 1997/10/18 14:42:41
dnl -------------
dnl Restricted form of AC_ARG_ENABLE that ensures user doesn't give bogus
dnl values.
dnl
dnl Parameters:
dnl $1 = option name
dnl $2 = help-string
dnl $3 = action to perform if option is not default
dnl $4 = action if perform if option is default
dnl $5 = default option value (either 'yes' or 'no')
AC_DEFUN([CF_ARG_OPTION],
[AC_ARG_ENABLE($1,[$2],[test "$enableval" != ifelse($5,no,yes,no) && enableval=ifelse($5,no,no,yes)
  if test "$enableval" != "$5" ; then
ifelse($3,,[    :]dnl
,[    $3]) ifelse($4,,,[
  else
    $4])
  fi],[enableval=$5 ifelse($4,,,[
  $4
])dnl
  ])])dnl
dnl ---------------------------------------------------------------------------
dnl CF_DISABLE_ECHO version: 10 updated: 2003/04/17 22:27:11
dnl ---------------
dnl You can always use "make -n" to see the actual options, but it's hard to
dnl pick out/analyze warning messages when the compile-line is long.
dnl
dnl Sets:
dnl	ECHO_LT - symbol to control if libtool is verbose
dnl	ECHO_LD - symbol to prefix "cc -o" lines
dnl	RULE_CC - symbol to put before implicit "cc -c" lines (e.g., .c.o)
dnl	SHOW_CC - symbol to put before explicit "cc -c" lines
dnl	ECHO_CC - symbol to put before any "cc" line
dnl
AC_DEFUN([CF_DISABLE_ECHO],[
AC_MSG_CHECKING(if you want to see long compiling messages)
CF_ARG_DISABLE(echo,
	[  --disable-echo          display "compiling" commands],
	[
    ECHO_LT='--silent'
    ECHO_LD='@echo linking [$]@;'
    RULE_CC='	@echo compiling [$]<'
    SHOW_CC='	@echo compiling [$]@'
    ECHO_CC='@'
],[
    ECHO_LT=''
    ECHO_LD=''
    RULE_CC='# compiling'
    SHOW_CC='# compiling'
    ECHO_CC=''
])
AC_MSG_RESULT($enableval)
AC_SUBST(ECHO_LT)
AC_SUBST(ECHO_LD)
AC_SUBST(RULE_CC)
AC_SUBST(SHOW_CC)
AC_SUBST(ECHO_CC)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_GCC_ATTRIBUTES version: 9 updated: 2002/12/21 19:25:52
dnl -----------------
dnl Test for availability of useful gcc __attribute__ directives to quiet
dnl compiler warnings.  Though useful, not all are supported -- and contrary
dnl to documentation, unrecognized directives cause older compilers to barf.
AC_DEFUN([CF_GCC_ATTRIBUTES],
[
if test "$GCC" = yes
then
cat > conftest.i <<EOF
#ifndef GCC_PRINTF
#define GCC_PRINTF 0
#endif
#ifndef GCC_SCANF
#define GCC_SCANF 0
#endif
#ifndef GCC_NORETURN
#define GCC_NORETURN /* nothing */
#endif
#ifndef GCC_UNUSED
#define GCC_UNUSED /* nothing */
#endif
EOF
if test "$GCC" = yes
then
	AC_CHECKING([for $CC __attribute__ directives])
cat > conftest.$ac_ext <<EOF
#line __oline__ "configure"
#include "confdefs.h"
#include "conftest.h"
#include "conftest.i"
#if	GCC_PRINTF
#define GCC_PRINTFLIKE(fmt,var) __attribute__((format(printf,fmt,var)))
#else
#define GCC_PRINTFLIKE(fmt,var) /*nothing*/
#endif
#if	GCC_SCANF
#define GCC_SCANFLIKE(fmt,var)  __attribute__((format(scanf,fmt,var)))
#else
#define GCC_SCANFLIKE(fmt,var)  /*nothing*/
#endif
extern void wow(char *,...) GCC_SCANFLIKE(1,2);
extern void oops(char *,...) GCC_PRINTFLIKE(1,2) GCC_NORETURN;
extern void foo(void) GCC_NORETURN;
int main(int argc GCC_UNUSED, char *argv[[]] GCC_UNUSED) { return 0; }
EOF
	for cf_attribute in scanf printf unused noreturn
	do
		CF_UPPER(CF_ATTRIBUTE,$cf_attribute)
		cf_directive="__attribute__(($cf_attribute))"
		echo "checking for $CC $cf_directive" 1>&AC_FD_CC
		case $cf_attribute in
		scanf|printf)
		cat >conftest.h <<EOF
#define GCC_$CF_ATTRIBUTE 1
EOF
			;;
		*)
		cat >conftest.h <<EOF
#define GCC_$CF_ATTRIBUTE $cf_directive
EOF
			;;
		esac
		if AC_TRY_EVAL(ac_compile); then
			test -n "$verbose" && AC_MSG_RESULT(... $cf_attribute)
			cat conftest.h >>confdefs.h
		fi
	done
else
	fgrep define conftest.i >>confdefs.h
fi
rm -rf conftest*
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_GCC_VERSION version: 3 updated: 2003/09/06 19:16:57
dnl --------------
dnl Find version of gcc
AC_DEFUN([CF_GCC_VERSION],[
AC_REQUIRE([AC_PROG_CC])
GCC_VERSION=none
if test "$GCC" = yes ; then
	AC_MSG_CHECKING(version of $CC)
	GCC_VERSION="`${CC} --version|sed -e '2,$d' -e 's/^[[^0-9.]]*//' -e 's/[[^0-9.]].*//'`"
	test -z "$GCC_VERSION" && GCC_VERSION=unknown
	AC_MSG_RESULT($GCC_VERSION)
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_GCC_WARNINGS version: 15 updated: 2003/07/05 18:42:30
dnl ---------------
dnl Check if the compiler supports useful warning options.  There's a few that
dnl we don't use, simply because they're too noisy:
dnl
dnl	-Wconversion (useful in older versions of gcc, but not in gcc 2.7.x)
dnl	-Wredundant-decls (system headers make this too noisy)
dnl	-Wtraditional (combines too many unrelated messages, only a few useful)
dnl	-Wwrite-strings (too noisy, but should review occasionally).  This
dnl		is enabled for ncurses using "--enable-const".
dnl	-pedantic
dnl
AC_DEFUN([CF_GCC_WARNINGS],
[
AC_REQUIRE([CF_GCC_VERSION])
if test "$GCC" = yes
then
	cat > conftest.$ac_ext <<EOF
#line __oline__ "configure"
int main(int argc, char *argv[[]]) { return (argv[[argc-1]] == 0) ; }
EOF
	AC_CHECKING([for $CC warning options])
	cf_save_CFLAGS="$CFLAGS"
	EXTRA_CFLAGS="-W -Wall"
	cf_warn_CONST=""
	test "$with_ext_const" = yes && cf_warn_CONST="Wwrite-strings"
	for cf_opt in \
		Wbad-function-cast \
		Wcast-align \
		Wcast-qual \
		Winline \
		Wmissing-declarations \
		Wmissing-prototypes \
		Wnested-externs \
		Wpointer-arith \
		Wshadow \
		Wstrict-prototypes \
		Wundef $cf_warn_CONST
	do
		CFLAGS="$cf_save_CFLAGS $EXTRA_CFLAGS -$cf_opt"
		if AC_TRY_EVAL(ac_compile); then
			test -n "$verbose" && AC_MSG_RESULT(... -$cf_opt)
			case $cf_opt in #(vi
			Wcast-qual) #(vi
				CPPFLAGS="$CPPFLAGS -DXTSTRINGDEFINES"
				;;
			Winline) #(vi
				case $GCC_VERSION in
				3.3*)
					CF_VERBOSE(feature is broken in gcc $GCC_VERSION)
					continue;;
				esac
				;;
			esac
			EXTRA_CFLAGS="$EXTRA_CFLAGS -$cf_opt"
		fi
	done
	rm -f conftest*
	CFLAGS="$cf_save_CFLAGS"
fi
AC_SUBST(EXTRA_CFLAGS)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_MKSTEMP version: 3 updated: 2001/11/08 20:59:59
dnl ----------
dnl Check for a working mkstemp.  This creates two files, checks that they are
dnl successfully created and distinct (AmigaOS apparently fails on the last).
AC_DEFUN([CF_MKSTEMP],[
AC_CACHE_CHECK(for working mkstemp, cf_cv_func_mkstemp,[
rm -f conftest*
AC_TRY_RUN([
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
int main()
{
	char *tmpl = "conftestXXXXXX";
	char name[2][80];
	int n;
	int result = 0;
	int fd;
	struct stat sb;

	umask(077);
	for (n = 0; n < 2; ++n) {
		strcpy(name[n], tmpl);
		if ((fd = mkstemp(name[n])) >= 0) {
			if (!strcmp(name[n], tmpl)
			 || stat(name[n], &sb) != 0
			 || (sb.st_mode & S_IFMT) != S_IFREG
			 || (sb.st_mode & 077) != 0) {
				result = 1;
			}
			close(fd);
		}
	}
	if (result == 0
	 && !strcmp(name[0], name[1]))
		result = 1;
	exit(result);
}
],[cf_cv_func_mkstemp=yes
],[cf_cv_func_mkstemp=no
],[AC_CHECK_FUNC(mkstemp)
])
])
if test "$cf_cv_func_mkstemp" = yes ; then
	AC_DEFINE(HAVE_MKSTEMP)
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_POPEN_TEST version: 2 updated: 1995/08/23 21:29:04
dnl -------------
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
dnl CF_SIZE_T version: 4 updated: 2000/01/22 00:19:54
dnl ---------
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
dnl ---------------------------------------------------------------------------
dnl CF_UPPER version: 5 updated: 2001/01/29 23:40:59
dnl --------
dnl Make an uppercase version of a variable
dnl $1=uppercase($2)
AC_DEFUN([CF_UPPER],
[
$1=`echo "$2" | sed y%abcdefghijklmnopqrstuvwxyz./-%ABCDEFGHIJKLMNOPQRSTUVWXYZ___%`
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_VERBOSE version: 2 updated: 1997/09/05 10:45:14
dnl ----------
dnl Use AC_VERBOSE w/o the warnings
AC_DEFUN([CF_VERBOSE],
[test -n "$verbose" && echo "	$1" 1>&AC_FD_MSG
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_WITH_DBMALLOC version: 4 updated: 2004/02/28 05:49:27
dnl ----------------
dnl Configure-option for dbmalloc.  The optional parameter is used to override
dnl the updating of $LIBS, e.g., to avoid conflict with subsequent tests.
AC_DEFUN([CF_WITH_DBMALLOC],[
AC_MSG_CHECKING(if you want to link with dbmalloc for testing)
AC_ARG_WITH(dbmalloc,
	[  --with-dbmalloc         use Conor Cahill's dbmalloc library],
	[with_dbmalloc=$withval],
	[with_dbmalloc=no])
AC_MSG_RESULT($with_dbmalloc)
if test "$with_dbmalloc" = yes ; then
	AC_CHECK_HEADER(dbmalloc.h,
		[AC_CHECK_LIB(dbmalloc,[debug_malloc]ifelse($1,,[],[,$1]))])
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_WITH_DMALLOC version: 4 updated: 2004/02/28 05:49:27
dnl ---------------
dnl Configure-option for dmalloc.  The optional parameter is used to override
dnl the updating of $LIBS, e.g., to avoid conflict with subsequent tests.
AC_DEFUN([CF_WITH_DMALLOC],[
AC_MSG_CHECKING(if you want to link with dmalloc for testing)
AC_ARG_WITH(dmalloc,
	[  --with-dmalloc          use Gray Watson's dmalloc library],
	[with_dmalloc=$withval],
	[with_dmalloc=no])
AC_MSG_RESULT($with_dmalloc)
if test "$with_dmalloc" = yes ; then
	AC_CHECK_HEADER(dmalloc.h,
		[AC_CHECK_LIB(dmalloc,[dmalloc_debug]ifelse($1,,[],[,$1]))])
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_WITH_WARNINGS version: 4 updated: 2003/05/24 15:01:41
dnl ----------------
dnl Combine the checks for gcc features into a configure-script option
AC_DEFUN([CF_WITH_WARNINGS],
[
if ( test "$GCC" = yes || test "$GXX" = yes )
then
AC_MSG_CHECKING(if you want to check for gcc warnings)
AC_ARG_WITH(warnings,
	[  --with-warnings         test: turn on gcc warnings],
	[cf_opt_with_warnings=$withval],
	[cf_opt_with_warnings=no])
AC_MSG_RESULT($cf_opt_with_warnings)
if test "$cf_opt_with_warnings" != no ; then
	CF_GCC_ATTRIBUTES
	CF_GCC_WARNINGS
fi
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_YACC_ERROR version: 3 updated: 2003/04/05 12:23:48
dnl -------------
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

AC_MSG_CHECKING(if $CPP can handle only .c files)
$CPP yacctest.y >yacctest.out 2>/dev/null
if test ! -s yacctest.out
then
	AC_DEFINE(CPP_DOES_ONLY_C_FILES)
	AC_MSG_RESULT(yes)
else
	AC_MSG_RESULT(no)
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

/* $Id: system.h,v 4.7 1998/01/24 01:42:09 cthuang Exp $
 *
 * cproto configuration and system dependencies
 */
#ifndef	SYSTEM_H
#define	SYSTEM_H
 
#if !defined(TRUE) || (TRUE != 1)
#undef  TRUE
#define	TRUE	(1)
#endif

#if !defined(FALSE) || (FALSE != 0)
#undef  FALSE
#define	FALSE	(0)
#endif

/* Watcom C++ predefines __DOS__ when the target platform is MS-DOS */
/* Borland C++ for MS-DOS predefines __MSDOS__ */
#if defined(__DOS__) || defined(__MSDOS__)
#ifndef MSDOS
#define MSDOS
#endif
#endif

/* Watcom C++ predefines __OS2__ when the target platform is OS/2 */
#ifdef __OS2__
#ifndef OS2
#define OS2
#endif
#endif

/* Watcom C++ predefines __NT__ when the target platform is Windows NT */
#ifdef __NT__
#ifndef WIN32
#define WIN32
#endif
#endif

/* don't use continuation-lines -- breaks on VAXC */
#if defined(__STDC__) || defined(__GNUC__) || defined(__WATCOMC__) || defined(vms)
#define ARGS(p) p
#else
#define ARGS(p) ()
#endif

/* Turbo C preprocessor */
#ifdef __TURBOC__
#define YY_READ_BUF_SIZE 256	/* patch */
#define HAVE_TMPFILE 1
#define HAVE_GETOPT_H 1	/* use the one from porting-directory */
#include <io.h>		/* declares 'read()' for flex */
#endif

#ifdef TURBO_CPP
#define CPP "cpp -P-"
#endif

/* EMX C preprocessor */
#ifdef __EMX__
#ifndef CPP
#define CPP "cpp"
#endif
#endif

/* Watcom C preprocessor */
#ifdef __WATCOMC__
#ifndef CPP
#define CPP "wcl386 /p"
#endif
#define HAVE_POPEN_PROTOTYPE 1
#define popen _popen
#define pclose _pclose
#define HAVE_TMPFILE 1
#endif

/* Microsoft C preprocessor */
#ifdef M_I86
#ifndef CPP
#define CPP "cl /E /nologo"
#endif
#define HAVE_TMPFILE 1
#endif

/* Vax C */
#ifdef vms
#ifndef CPP
#define CPP "cc /preprocess_only=%s"
#endif
#define unlink remove
#define HAVE_TMPFILE 1
#define HAVE_GETOPT_H 1
#define USE_flex 1
#endif

/* Set configuration parameters for systems on which we cannot run autoconf.
 * (Assumes Posix unless overridden).
 */
#ifndef HAVE_STDLIB_H
#define HAVE_STDLIB_H 1
#endif

#ifndef HAVE_STRING_H
#define HAVE_STRING_H 1
#endif

#ifndef HAVE_STRSTR
#define HAVE_STRSTR 1
#endif

#ifndef HAVE_GETOPT_H
#define HAVE_GETOPT_H 0
#endif 

/* Default C preprocessor on UNIX systems */
#ifndef MSDOS
#ifndef CPP
#define CPP "/lib/cpp"
#endif
#endif

/* maximum include file nesting */
#ifndef MAX_INC_DEPTH
#define MAX_INC_DEPTH 15
#endif

/* maximum number of include directories */
#ifndef MAX_INC_DIR
#define MAX_INC_DIR 15
#endif

/* maximum text buffer size */
#ifndef MAX_TEXT_SIZE
#define MAX_TEXT_SIZE 256
#endif

#if HAVE_STDLIB_H
#include <stdlib.h>
#else
extern char *malloc  ARGS((size_t n));
extern char *realloc ARGS((char *p, size_t n));
#endif

/* Declare argument for exit() function */
#ifdef vms
#include <stsdef.h>
#define	EXIT_SUCCESS	(STS$M_INHIB_MSG | STS$K_SUCCESS)
#define	EXIT_FAILURE	(STS$M_INHIB_MSG | STS$K_ERROR)
#else
#ifndef EXIT_SUCCESS
#define	EXIT_SUCCESS	(0)
#endif
#ifndef EXIT_FAILURE
#define	EXIT_FAILURE	(1)
#endif
#endif /* vms */

#if HAVE_UNISTD_H
#include <unistd.h>
#endif

#if STDC_HEADERS || HAVE_STRING_H
#  include <string.h>
/* An ANSI string.h and pre-ANSI memory.h might conflict.  */
#  if !STDC_HEADERS && HAVE_MEMORY_H
#    include <memory.h>
#  endif /* not STDC_HEADERS and HAVE_MEMORY_H */
#else /* not STDC_HEADERS and not HAVE_STRING_H */
#  include <strings.h>
#  define strchr index
#  define strrchr rindex
/* memory.h and strings.h conflict on some systems.  */
#endif /* not STDC_HEADERS and not HAVE_STRING_H */

extern char *getenv  ARGS((const char *v));
extern char *strstr  ARGS((const char *s, const char *p));

/*
 * The DOALLOC symbol controls whether we compile in the simple memory tests
 * in 'trace.c' (use dbmalloc for more rigorous testing).
 */
#ifdef DOALLOC
#include <trace.h>
#endif /* DOALLOC */

/*
 * Lint libraries are useful only on systems that are likely to have lint.
 * The OPT_LINTLIBRARY symbol controls whether we compile in the lint library
 * support.
 */
#ifndef OPT_LINTLIBRARY
# if HAVE_PROG_LINT || defined(unix)
#  define OPT_LINTLIBRARY 1
# endif
#endif

#if BISON_HAS_YYTNAME || YACC_HAS_YYTOKS || YACC_HAS_YYTOKS_2 || YACC_HAS_YYNAME
# define YYDEBUG 1
#endif

#if HAVE_LIBDMALLOC
#include <dmalloc.h>	/* Gray Watson's library */
#endif

#if HAVE_LIBDBMALLOC
#include <dbmalloc.h>	/* Conor Cahill's library */
#endif

#endif /* SYSTEM_H */

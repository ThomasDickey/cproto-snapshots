/* $Id: system.h,v 3.6 1994/07/30 19:36:45 tom Exp $
 *
 * cproto configuration and system dependencies
 */

#ifndef	SYSTEM_H
#define	SYSTEM_H
 
/* don't use continuation-lines -- breaks on VAXC */
#if defined(__STDC__) || defined(__TURBOC__) || defined(vms) || defined(__cplusplus)
#define ARGS(p) p
#else
#define ARGS(p) ()
#endif

/* Declare argument for 'exit()' and '_exit()': */
#ifdef	vms
#include	<stsdef.h>
#define	SUCCESS	(STS$M_INHIB_MSG | STS$K_SUCCESS)
#define	FAIL	(STS$M_INHIB_MSG | STS$K_ERROR)
#else	/* unix */
#define	SUCCESS	(0)		/* if no error */
#define	FAIL	(1)		/* if any error */
#endif	/* vms/unix */

/* Borland C++ for MS-DOS predefines __MSDOS__ */
#ifdef __MSDOS__
#ifndef MSDOS
#define MSDOS
#endif
#endif

/* Borland C++ for OS/2 predefines __OS2__ */
#ifdef __OS2__
#ifndef OS2
#define OS2
#endif
#endif

/* Turbo C preprocessor */
#ifdef TURBO_CPP
#define CPP "cpp -P-"
#else
#ifdef OS2
#define popen _popen
#define pclose _pclose
#endif
#endif

/* Microsoft C preprocessor */
#ifdef M_I86
#define CPP "cl /E /nologo"
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
#define HAVE_GETOPT_H 1
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
#include	<stdlib.h>
#else
extern char *malloc  ARGS((size_t n));
extern char *realloc ARGS((char *p, size_t n));
#endif

#if HAVE_UNISTD_H
#include	<unistd.h>
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

#ifdef DOALLOC
extern char *doalloc(char *,unsigned);
extern void dofree(char *);

#define malloc(n)    doalloc(0,n)
#define realloc(p,n) doalloc(p,n)
#define free(n) dofree(n)
#endif	/* DOALLOC */

/*
 * Lint libraries are useful only on systems that are likely to have lint.
 * The OPT_LINTLIBRARY symbol controls whether we compile in the lint library
 * support.
 */
#ifndef OPT_LINTLIBRARY
# ifdef unix
#  define OPT_LINTLIBRARY 1
# endif
#endif

#endif	/* SYSTEM_H */

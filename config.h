/* $Id: config.h,v 3.7 1993/06/07 17:57:10 tom Exp $
 *
 * cproto configuration and system dependencies
 */

 
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

#if __STDC__ || defined(MSDOS) || defined(OS2)
#include <stdlib.h>
#include <string.h>
#else
#if defined(BSD) && !defined(apollo)
#include <strings.h>
#define strchr index
#define strrchr rindex
#else
#include <string.h>
#endif
extern char *getenv(), *malloc(), *realloc(), *strstr();
#endif

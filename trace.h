/* $Id: trace.h,v 4.5 2022/10/13 23:32:42 tom Exp $ */

#ifndef TRACE_H
#define TRACE_H

#include <cproto.h>

#ifdef DOALLOC			/* FIXME: want OPT_DUMP or OPT_TRACE */
extern void Where(char *f, int n);
extern void Trace(char *f, ...) GCC_PRINTFLIKE(1,2);
extern void show_alloc(void);
extern void Elapsed(char *msg);
extern void WalkBack(void);
extern void fail_alloc(char *msg, char *ptr);
#define TRACE(p)	Trace p
#else
#define TRACE(p)		/* thing */
#endif

#ifdef DOALLOC
extern void *doalloc(void *, unsigned);
extern void dofree(void *);

#define malloc(n)    doalloc(0,n)
#define realloc(p,n) doalloc(p,n)
#define free(n)      dofree(n)
#endif

#define NonNull(s)	((s) ? (s) : "<null>")

#endif /* TRACE_H */

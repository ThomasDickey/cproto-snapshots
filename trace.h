/* $Id: trace.h,v 4.2 1998/01/19 00:49:35 cthuang Exp $ */

#ifndef TRACE_H
#define TRACE_H

#if !defined(__GNUC__) && !defined(__attribute__)
#define __attribute__(p)
#endif

extern void Trace(char *f, ...) __attribute__((format (printf,1,2)));
extern void show_alloc(void);
extern void Elapsed(char *msg);
extern void WalkBack(void);
extern void fail_alloc(char *msg, char *ptr);

#ifdef DOALLOC
extern void *doalloc(void *,unsigned);
extern void dofree(void *);

#define malloc(n)    doalloc(0,n)
#define realloc(p,n) doalloc(p,n)
#define free(n)      dofree(n)
#endif

#endif /* TRACE_H */

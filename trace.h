/* $Id: trace.h,v 4.1 1994/09/23 23:09:59 tom Exp $ */

#ifndef TRACE_H
#define TRACE_H

#ifndef __attribute__
#define __attribute(p)
#endif

extern void Trace(char *f, ...) __attribute__((format (printf,1,2)));
extern void show_alloc(void);
extern void Elapsed(char *msg);
extern void WalkBack(void);
extern void fail_alloc(char *msg, char *ptr);

extern void dump_parameter(Parameter *p, int level);
extern void dump_param_list(ParameterList *p, int level);
extern void dump_declarator(Declarator *d, int level);
extern void dump_decl_spec(DeclSpec *d, int level);

#endif /* TRACE_H */

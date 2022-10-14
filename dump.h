/* $Id: dump.h,v 4.7 2022/10/13 23:32:45 tom Exp $ */

#ifndef DUMP_H
#define DUMP_H

#include <cproto.h>

#ifdef DOALLOC			/* FIXME: want OPT_DUMP or OPT_TRACE */
extern void dump_parameter(Parameter * p, int level);
extern void dump_param_list(ParameterList * p, int level);
extern void dump_declarator(Declarator * d, int level);
extern void dump_decl_spec(DeclSpec * d, int level);
extern void dump_declarator_list(DeclaratorList * d, int level);
extern char *flagsDeclSpec(int flags);
#else
#define dump_parameter(p, level)	/* nothing */
#define dump_param_list(pl, level)	/* nothing */
#define dump_declarator(d, level)	/* nothing */
#define dump_decl_spec(ds, level)	/* nothing */
#define dump_declarator_list(dl, level)		/* nothing */
#define flagsDeclSpec(flags)	/* nothing */
#endif

#endif /* DUMP_H */

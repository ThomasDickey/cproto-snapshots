#include <stdio.h>
#include "cproto.h"
#include "trace.h"

static char *whatFuncDefStyle (FuncDefStyle func_def);

static
char *whatFuncDefStyle(func_def)
	FuncDefStyle func_def;		/* style of function definition */
{
	switch (func_def) {
	case FUNC_NONE:		return "FUNC_NONE";
	case FUNC_TRADITIONAL:	return "FUNC_TRADITIONAL";
	case FUNC_ANSI:		return "FUNC_ANSI";
	case FUNC_BOTH:		return "FUNC_BOTH";
	}
	return "?";
}

void dump_declarator(d, level)
	Declarator *d;
	int	level;
{
	char	pad[80];
	sprintf(pad, "%-*s", level * 3, ".");
	Trace("%sdeclarator %p\n",		pad, d);
	Trace("%s   name /%s/\n",		pad, d->name);
	Trace("%s   text /%s/\n",		pad, d->text);
	Trace("%s   begin %ld\n",		pad, d->begin);
	Trace("%s   begin_comment %ld\n",	pad, d->begin_comment);
	Trace("%s   end_comment %ld\n",		pad, d->end_comment);
	Trace("%s   func_dec %s\n",		pad, whatFuncDefStyle(d->func_def));
#if 0
    ParameterList params;		/* function parameters */
#endif
	Trace("%s   pointer %s\n",		pad, d->pointer ? "YES" : "NO");

	if (d->head != 0 && d != d->head) {
		Trace("%s >HEAD of %p\n", pad, d);
		dump_declarator(d->head, level+1);
	}
	if (d->func_stack != 0) {
		Trace("%s >FUNC_STACK of %p\n", pad, d);
		dump_declarator(d->func_stack, level+1);
	}
	if (d->next != 0) {
		Trace("%s >NEXT of %p\n", pad,  d);
		dump_declarator(d->next, level+1);
	}
}

static
char *flagsDeclSpec(flags)
	int flags;
{
	static	char	temp[100];
	static	struct	{
		int	mask;
		char	*text;
	} table [] = {
		{DS_EXTERN,	"extern"},
		{DS_STATIC,	"static"},
		{DS_CHAR, 	"char"},
		{DS_SHORT,	"short"},
		{DS_FLOAT,	"float"},
		{DS_JUNK ,	"junk"},
	};
	register int j;
	*temp = 0;
	for (j = 0; j < sizeof(table)/sizeof(table[0]); j++) {
		if (flags & table[j].mask) {
			if (*temp)
				(void)strcat(temp, ",");
			(void)strcat(temp, table[j].text);
		}
	}
	return temp;
}

void dump_decl_spec(d, level)
	DeclSpec *d;
	int level;
{
	Trace("decl_spec %p\n", d);
	Trace("  flags %s\n", flagsDeclSpec(d->flags));
	Trace("  text /%s/\n", d->text);
	Trace("  begin %ld\n", d->begin);
}

/* $Id: symbol.h,v 3.4 1993/05/26 01:36:04 cthuang Exp $
 *
 * A symbol table is a collection of string identifiers stored in a
 * hash table.
 */
#ifndef SYMBOL_H
#define SYMBOL_H

typedef struct symbol {
    struct symbol *next;	/* next symbol in list */
    char *name; 		/* name of symbol */
    char *value;		/* value of symbol (for defines) */
    short flags;		/* symbol attributes */
} Symbol;

/* The hash table length should be a prime number. */
#define SYM_MAX_HASH 251

typedef struct symbol_table {
    Symbol *bucket[SYM_MAX_HASH];	/* hash buckets */
} SymbolTable;

extern SymbolTable *new_symbol_table(); /* Create symbol table */
extern void free_symbol_table();	/* Destroy symbol table */
extern Symbol *find_symbol();		/* Lookup symbol name */
extern Symbol *new_symbol();		/* Define new symbol */

#endif

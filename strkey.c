/* $Id: strkey.c,v 4.1.1.1 1996/05/30 15:27:42 tom Exp $
 *
 * Some string handling routines
 */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "cproto.h"

#define	LETTER(c) (isalnum(c) || (c == '_') || (c == '$'))

/*
 * Return a pointer to the first occurence of the given keyword in the string
 * or NULL if not found.  Unlike 'strstr()', which verifies that the match is
 * against an identifier-token.
 */
char *
strkey (src, key)
char *src, *key;
{
    if (src != 0 && key != 0) {
	register char *s  = src, *d;
	register size_t len = strlen(key);

	while (*s) {
	    if (!LETTER(*s)) {
		s++;
	    } else {
		for (d = s; LETTER(*d); d++)
		    ;
		if ((d - s) == len && !strncmp(s, key, len))
		    return s;
		s = d;
	    }
	}
    }
    return NULL;
}

/*
 * Delete a specified keyword from a string if it appears there
 */
void
strcut (src, key)
char *src, *key;
{
    register char *s, *d;

    if ((s = strkey(src, key)) != '\0') {
	d = s + strlen(key);
	while (*d != '\0' && !LETTER(*d))
	    d++;
	while ((*s++ = *d++) != '\0')
	    ;
    }
}

/* $Id: strkey.c,v 4.2.1.1 2004/03/24 20:05:44 tom Exp $
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
strkey (char *src, char *key)
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
		if ((d - s) == (int) len && !strncmp(s, key, len))
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
strcut (char *src, char *key)
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

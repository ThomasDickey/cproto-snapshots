/* $Id: strstr.c,v 3.4 1994/02/07 20:11:30 tom Exp $
 *
 * Simple implementation of the ANSI strstr() function
 */
#include <stdio.h>
#include "config.h"

/* Search for a substring within the given string.
 * Return a pointer to the first occurence within the string,
 * or NULL if not found.
 */
#ifndef strstr
char *
strstr (src, key)
char *src, *key;
{
    char *s;
    int keylen;

    if ((keylen = strlen(key)) == 0)
	return src;

    s = strchr(src, *key);
    while (s != NULL) {
	if (strncmp(s, key, keylen) == 0)
	    return s;
	s = strchr(s+1, *key);
    }
    return NULL;
}
#endif

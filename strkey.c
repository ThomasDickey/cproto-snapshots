/* $Id: strkey.c,v 3.1 1993/06/09 18:07:18 tom Exp $
 *
 * Some string handling routines
 */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "cproto.h"

#define	LETTER(c)	(isalnum(c) || (c == '_') || (c == '$'))

/*
 * Return a pointer to the first occurence of the given keyword in the string
 * or NULL if not found.  Unlike 'strstr()', which verifies that the match is
 * against an identifier-token.
 */
char *	strkey (src, key)
	char	*src, *key;
{
	register char	*s  = src,
			*d;
	register SIZE_T	len = strlen(key);

	while (*s) {
		if (!LETTER(*s))
			s++;
		else {
			for (d = s; LETTER(*d); d++);
			if ((d - s) == len
			&&  !strncmp(s, key, len))
				return (s);
			s = d;
		}
	}
	return NULL;
}

/*
 * Delete a specified keyword from a string if it appears there
 */
void	strcut (src, key)
	char	*src, *key;
{
	register char	*s, *d;

	if ((s = strkey(src, key)) != '\0') {
		d = s + strlen(key);
		while (*d != '\0'
		&&	!LETTER(*d))
			d++;
		while ((*s++ = *d++) != '\0');
	}
}

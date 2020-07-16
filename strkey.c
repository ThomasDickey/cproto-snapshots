/* $Id: strkey.c,v 4.11 2020/07/16 13:00:27 tom Exp $
 *
 * Some string handling routines
 */
#include <string.h>
#include <cproto.h>

#define	LETTER(c) (isalnum(UCH(c)) || (c == '_') || (c == '$'))

/*
 * Return a pointer to the first occurrence of the given keyword in the string
 * or NULL if not found.  Unlike 'strstr()', which verifies that the match is
 * against an identifier-token.
 */
char *
strkey(char *src, const char *key)
{
    if (src != 0 && key != 0) {
	char *s = src, *d;
	size_t len = strlen(key);

	while (*s) {
	    if (!LETTER(*s)) {
		s++;
	    } else {
		for (d = s; LETTER(*d); d++) ;
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
strcut(char *src, const char *key)
{
    char *s, *d;

    if ((s = strkey(src, key)) != 0) {
	d = s + strlen(key);
	while (*d != '\0' && !LETTER(*d))
	    d++;
	while ((*s++ = *d++) != '\0') ;
    }
}

/* Search for a substring within the given string.
 * Return a pointer to the first occurrence within the string,
 * or NULL if not found.
 */
#if !HAVE_STRSTR
char *
strstr(char *src, char *key)
{
    char *s;
    int keylen;

    if ((keylen = strlen(key)) == 0)
	return src;

    s = strchr(src, *key);
    while (s != NULL) {
	if (strncmp(s, key, keylen) == 0)
	    return s;
	s = strchr(s + 1, *key);
    }
    return NULL;
}
#endif

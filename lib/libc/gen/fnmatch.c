/*
 * Copyright (c) 1989, 1993, 1994
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Guido van Rossum.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * This implementation uses recursion.  It should be rewritten to avoid
 * it due to stack limitations on the IIgs.
 *
 * $Id: fnmatch.c,v 1.2 1997/09/21 06:05:00 gdr Exp $
 *
 * This file is formatted for tab stops every 8 characters.
 */
 
#ifdef __ORCAC__
segment "libc_gen__";
#endif

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)fnmatch.c	8.2 (Berkeley) 4/16/94";
#endif /* LIBC_SCCS and not lint */

/*
 * Function fnmatch() as specified in POSIX 1003.2-1992, section B.6.
 * Compares a filename or pathname to a pattern.
 */

#include <fnmatch.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <err.h>

#define	EOS	'\0'

static const char *rangematch __P((const char *, int, int));
static void _fnmatch_map (const char *, const char *, char **, char **, int);

int
fnmatch(const char *opattern, const char *ostring, int flags)
{
	const char *stringstart;
	char c, test;
	char *pattern, *string;

	_fnmatch_map(ostring, opattern, &string, &pattern, flags);

	for (stringstart = string;;)
		switch (c = *pattern++) {
		case EOS:
			return (*string == EOS ? 0 : FNM_NOMATCH);
		case '?':
			if (*string == EOS)
				return (FNM_NOMATCH);
			if (*string == '/' && (flags & FNM_PATHNAME))
				return (FNM_NOMATCH);
			if (*string == '.' && (flags & FNM_PERIOD) &&
			    (string == stringstart ||
			    ((flags & FNM_PATHNAME) && *(string - 1) == '/')))
				return (FNM_NOMATCH);
			++string;
			break;
		case '*':
			c = *pattern;
			/* Collapse multiple stars. */
			while (c == '*')
				c = *++pattern;

			if (*string == '.' && (flags & FNM_PERIOD) &&
			    (string == stringstart ||
			    ((flags & FNM_PATHNAME) && *(string - 1) == '/')))
				return (FNM_NOMATCH);

			/* Optimize for pattern with * at end or before /. */
			if (c == EOS)
				if (flags & FNM_PATHNAME)
					return (strchr(string, '/') == NULL ?
					    0 : FNM_NOMATCH);
				else
					return (0);
			else if (c == '/' && flags & FNM_PATHNAME) {
				if ((string = strchr(string, '/')) == NULL)
					return (FNM_NOMATCH);
				break;
			}

			/* General case, use recursion. */
			while ((test = *string) != EOS) {
				if (!fnmatch(pattern, string, flags & ~FNM_PERIOD))
					return (0);
				if (test == '/' && flags & FNM_PATHNAME)
					break;
				++string;
			}
			return (FNM_NOMATCH);
		case '[':
			if (*string == EOS)
				return (FNM_NOMATCH);
			if (*string == '/' && flags & FNM_PATHNAME)
				return (FNM_NOMATCH);
			if ((pattern =
			    rangematch(pattern, *string, flags)) == NULL)
				return (FNM_NOMATCH);
			++string;
			break;
		case '\\':
			if (!(flags & FNM_NOESCAPE)) {
				if ((c = *pattern++) == EOS) {
					c = '\\';
					--pattern;
				}
			}
			/* FALLTHROUGH */
		default:
			if (c != *string++)
				return (FNM_NOMATCH);
			break;
		}
	/* NOTREACHED */
}

static const char *
rangematch(const char *pattern, int test, int flags)
{
	int negate, ok;
	char c, c2;

	/*
	 * A bracket expression starting with an unquoted circumflex
	 * character produces unspecified results (IEEE 1003.2-1992,
	 * 3.13.2).  This implementation treats it like '!', for
	 * consistency with the regular expression syntax.
	 * J.T. Conklin (conklin@ngai.kaleida.com)
	 */
	if (negate = (*pattern == '!' || *pattern == '^'))
		++pattern;

	for (ok = 0; (c = *pattern++) != ']';) {
		if (c == '\\' && !(flags & FNM_NOESCAPE))
			c = *pattern++;
		if (c == EOS)
			return (NULL);
		if (*pattern == '-'
		    && (c2 = *(pattern+1)) != EOS && c2 != ']') {
			pattern += 2;
			if (c2 == '\\' && !(flags & FNM_NOESCAPE))
				c2 = *pattern++;
			if (c2 == EOS)
				return (NULL);
			if (c <= test && test <= c2)
				ok = 1;
		} else if (c == test)
			ok = 1;
	}
	return (ok == negate ? NULL : pattern);
}

static void
_fnmatch_map (const char *opath, const char *orex, char **npath, char **nrex,
	      int flags) {
	static char *path = NULL;
	static char *rex = NULL;
	char *p;
	int i;
	
	/*
	 * create copies of opath and orex; this depends on an
	 * ANSI implementation of realloc (accepts NULL pointer)
	 */
	path = realloc(path, strlen(opath) + 1);
	rex = realloc(rex, strlen(orex) + 1);
	if (path == NULL || rex == NULL) {
		err (1, "fnmatch could not allocate internal buffer");
		/*NOTREACHED*/
	}
	strcpy(path, opath);
	strcpy(rex, orex);

	/* fold case if necessary */
	if (flags & FNM_CASEFOLD) {
		for (p = path; *p != '\0'; p++) {
			if (isupper(*p)) {
				*p = _tolower(*p);
			}
		}
		for (p = rex; *p != '\0'; p++) {
			if (isupper(*p)) {
				*p = _tolower(*p);
			}
		}
	}

	/*
	 * If either pattern or string contain _both_ a ':' and a '/',
	 * then we leave them exactly as they are.  Otherwise, all colons
	 * are mapped to '/'.
	 */
	if (!((strchr(path, ':') && strchr(path, '/')) ||
	     (strchr(rex,  ':') && strchr(rex,  '/')))) {
		for (i=0; i<2; i++) {
			switch (i) {
			case 0:
				p = path;
				break;
			case 1:
				p = rex;
				break;
			}
			while (*p) {
				if (*p == ':') {
					*p = '/';
				}
				p++;
			}
		}
	}

	/* give the caller pointers to our buffer */	     
	*npath = path;
	*nrex = rex;
	return;
}
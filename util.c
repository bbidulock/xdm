/* $Xorg: util.c,v 1.4 2001/02/09 02:05:41 xorgcvs Exp $ */
/*

Copyright 1989, 1998  The Open Group

Permission to use, copy, modify, distribute, and sell this software and its
documentation for any purpose is hereby granted without fee, provided that
the above copyright notice appear in all copies and that both that
copyright notice and this permission notice appear in supporting
documentation.

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE OPEN GROUP BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of The Open Group shall
not be used in advertising or otherwise to promote the sale, use or
other dealings in this Software without prior written authorization
from The Open Group.

*/

/*
 * xdm - display manager daemon
 * Author:  Keith Packard, MIT X Consortium
 *
 * util.c
 *
 * various utility routines
 */

# include   "dm.h"
#ifdef X_POSIX_C_SOURCE
#define _POSIX_C_SOURCE X_POSIX_C_SOURCE
#include <signal.h>
#undef _POSIX_C_SOURCE
#else
#if defined(X_NOT_POSIX) || defined(_POSIX_SOURCE)
#include <signal.h>
#else
#define _POSIX_SOURCE
#include <signal.h>
#undef _POSIX_SOURCE
#endif
#endif
#if defined(__osf__) || defined(linux)
#define setpgrp setpgid
#endif

printEnv (e)
char	**e;
{
	while (*e)
		Debug ("%s\n", *e++);
}

static char *
makeEnv (name, value)
char	*name;
char	*value;
{
	char	*result;

	result = malloc ((unsigned) (strlen (name) + strlen (value) + 2));
	if (!result) {
		LogOutOfMem ("makeEnv");
		return 0;
	}
	sprintf (result, "%s=%s", name, value);
	return result;
}

char *
getEnv (e, name)
	char	**e;
	char	*name;
{
	int	l = strlen (name);

	if (!e) return 0;

	while (*e) {
		if ((int)strlen (*e) > l && !strncmp (*e, name, l) &&
			(*e)[l] == '=')
			return (*e) + l + 1;
		++e;
	}
	return 0;
}

char **
setEnv (e, name, value)
	char	**e;
	char	*name;
	char	*value;
{
	char	**new, **old;
	char	*newe;
	int	envsize;
	int	l;

	l = strlen (name);
	newe = makeEnv (name, value);
	if (!newe) {
		LogOutOfMem ("setEnv");
		return e;
	}
	if (e) {
		for (old = e; *old; old++)
			if ((int)strlen (*old) > l && !strncmp (*old, name, l) && (*old)[l] == '=')
				break;
		if (*old) {
			free (*old);
			*old = newe;
			return e;
		}
		envsize = old - e;
		new = (char **) realloc ((char *) e,
				(unsigned) ((envsize + 2) * sizeof (char *)));
	} else {
		envsize = 0;
		new = (char **) malloc (2 * sizeof (char *));
	}
	if (!new) {
		LogOutOfMem ("setEnv");
		free (newe);
		return e;
	}
	new[envsize] = newe;
	new[envsize+1] = 0;
	return new;
}

freeEnv (env)
    char    **env;
{
    char    **e;

    if (env)
    {
    	for (e = env; *e; e++)
	    free (*e);
    	free (env);
    }
}

# define isblank(c)	((c) == ' ' || c == '\t')

char **
parseArgs (argv, string)
char	**argv;
char	*string;
{
	char	*word;
	char	*save;
	int	i;

	i = 0;
	while (argv && argv[i])
		++i;
	if (!argv) {
		argv = (char **) malloc (sizeof (char *));
		if (!argv) {
			LogOutOfMem ("parseArgs");
			return 0;
		}
	}
	word = string;
	for (;;) {
		if (!*string || isblank (*string)) {
			if (word != string) {
				argv = (char **) realloc ((char *) argv,
					(unsigned) ((i + 2) * sizeof (char *)));
				save = malloc ((unsigned) (string - word + 1));
				if (!argv || !save) {
					LogOutOfMem ("parseArgs");
					if (argv)
						free ((char *) argv);
					if (save)
						free (save);
					return 0;
				}
				argv[i] = strncpy (save, word, string-word);
				argv[i][string-word] = '\0';
				i++;
			}
			if (!*string)
				break;
			word = string + 1;
		}
		++string;
	}
	argv[i] = 0;
	return argv;
}

freeArgs (argv)
    char    **argv;
{
    char    **a;

    if (!argv)
	return;

    for (a = argv; *a; a++)
	free (*a);
    free ((char *) argv);
}

CleanUpChild ()
{
#if defined(SYSV) || defined(SVR4)
	setpgrp ();
#else
	setpgrp (0, getpid ());
	sigsetmask (0);
#endif
#ifdef SIGCHLD
	(void) Signal (SIGCHLD, SIG_DFL);
#endif
	(void) Signal (SIGTERM, SIG_DFL);
	(void) Signal (SIGPIPE, SIG_DFL);
	(void) Signal (SIGALRM, SIG_DFL);
	(void) Signal (SIGHUP, SIG_DFL);
	CloseOnFork ();
}

static char localHostbuf[256];
static int  gotLocalHostname;

char *
localHostname ()
{
    if (!gotLocalHostname)
    {
	XmuGetHostname (localHostbuf, sizeof (localHostbuf) - 1);
	gotLocalHostname = 1;
    }
    return localHostbuf;
}

SIGVAL (*Signal (sig, handler))()
    int sig;
    SIGVAL (*handler)();
{
#ifndef X_NOT_POSIX
    struct sigaction sigact, osigact;
    sigact.sa_handler = handler;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = 0;
    sigaction(sig, &sigact, &osigact);
    return osigact.sa_handler;
#else
    return signal(sig, handler);
#endif
}

/* $Xorg: verify.c,v 1.4 2001/02/09 02:05:41 xorgcvs Exp $ */
/*

Copyright 1988, 1998  The Open Group

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
 * verify.c
 *
 * typical unix verification routine.
 */

# include	"dm.h"
# include	<pwd.h>
#ifdef USESHADOW
# include	<shadow.h>
# include	<errno.h>
#ifdef X_NOT_STDC_ENV
extern int errno;
#endif
#endif

# include	"greet.h"

#ifdef X_NOT_STDC_ENV
char *getenv();
#endif

static char *envvars[] = {
    "TZ",			/* SYSV and SVR4, but never hurts */
#if defined(sony) && !defined(SYSTYPE_SYSV) && !defined(_SYSTYPE_SYSV)
    "bootdev",
    "boothowto",
    "cputype",
    "ioptype",
    "machine",
    "model",
    "CONSDEVTYPE",
    "SYS_LANGUAGE",
    "SYS_CODE",
#endif
#if (defined(SVR4) || defined(SYSV)) && defined(i386) && !defined(sun)
    "XLOCAL",
#endif
    NULL
};

static char **
userEnv (d, useSystemPath, user, home, shell)
struct display	*d;
int	useSystemPath;
char	*user, *home, *shell;
{
    char	**env;
    char	**envvar;
    char	*str;
    extern char **defaultEnv (), **setEnv ();
    
    env = defaultEnv ();
    env = setEnv (env, "DISPLAY", d->name);
    env = setEnv (env, "HOME", home);
    env = setEnv (env, "LOGNAME", user); /* POSIX, System V */
    env = setEnv (env, "USER", user);    /* BSD */
    env = setEnv (env, "PATH", useSystemPath ? d->systemPath : d->userPath);
    env = setEnv (env, "SHELL", shell);
    for (envvar = envvars; *envvar; envvar++)
    {
	str = getenv(*envvar);
	if (str)
	    env = setEnv (env, *envvar, str);
    }
    return env;
}

int
Verify (d, greet, verify)
struct display		*d;
struct greet_info	*greet;
struct verify_info	*verify;
{
	struct passwd	*p;
#ifdef USESHADOW
	struct spwd	*sp;
#endif
	char		*user_pass;
#if !defined(SVR4) || !defined(GREET_LIB) /* shared lib decls handle this */
	char		*crypt ();
	char		**systemEnv (), **parseArgs ();
#endif
	char		*shell, *home;
	char		**argv;

	Debug ("Verify %s ...\n", greet->name);
	p = getpwnam (greet->name);
	if (!p || strlen (greet->name) == 0) {
		Debug ("getpwnam() failed.\n");
		bzero(greet->password, strlen(greet->password));
		return 0;
	} else {
	    user_pass = p->pw_passwd;
	}
#ifdef USESHADOW
	errno = 0;
	sp = getspnam(greet->name);
	if (sp == NULL) {
	    Debug ("getspnam() failed, errno=%d.  Are you root?\n", errno);
	} else {
	    user_pass = sp->sp_pwdp;
	}
	endspent();
#endif
#if defined(ultrix) || defined(__ultrix__)
	if (authenticate_user(p, greet->password, NULL) < 0)
#else
	if (strcmp (crypt (greet->password, user_pass), user_pass))
#endif
	{
		Debug ("password verify failed\n");
		bzero(greet->password, strlen(greet->password));
		return 0;
	}
	Debug ("verify succeeded\n");
	bzero(user_pass, strlen(user_pass)); /* in case shadow password */
	/* The password is passed to StartClient() for use by user-based
	   authorization schemes.  It is zeroed there. */
	verify->uid = p->pw_uid;
	verify->gid = p->pw_gid;
	home = p->pw_dir;
	shell = p->pw_shell;
	argv = 0;
	if (d->session)
		argv = parseArgs (argv, d->session);
	if (greet->string)
		argv = parseArgs (argv, greet->string);
	if (!argv)
		argv = parseArgs (argv, "xsession");
	verify->argv = argv;
	verify->userEnviron = userEnv (d, p->pw_uid == 0,
				       greet->name, home, shell);
	Debug ("user environment:\n");
	printEnv (verify->userEnviron);
	verify->systemEnviron = systemEnv (d, greet->name, home);
	Debug ("system environment:\n");
	printEnv (verify->systemEnviron);
	Debug ("end of environments\n");
	return 1;
}

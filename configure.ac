
dnl  Copyright 2005 Red Hat, Inc.
dnl 
dnl  Permission to use, copy, modify, distribute, and sell this software and its
dnl  documentation for any purpose is hereby granted without fee, provided that
dnl  the above copyright notice appear in all copies and that both that
dnl  copyright notice and this permission notice appear in supporting
dnl  documentation, and that the name of Red Hat not be used in
dnl  advertising or publicity pertaining to distribution of the software without
dnl  specific, written prior permission.  Red Hat makes no
dnl  representations about the suitability of this software for any purpose.  It
dnl  is provided "as is" without express or implied warranty.
dnl 
dnl  RED HAT DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
dnl  INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
dnl  EVENT SHALL RED HAT BE LIABLE FOR ANY SPECIAL, INDIRECT OR
dnl  CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
dnl  DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
dnl  TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
dnl  PERFORMANCE OF THIS SOFTWARE.
dnl
dnl Process this file with autoconf to create configure.

AC_PREREQ([2.57])
AC_INIT(xdm,[0.99.2], [https://bugs.freedesktop.org/enter_bug.cgi?product=xorg],xdm)
AM_INIT_AUTOMAKE([dist-bzip2])
AM_MAINTAINER_MODE

AM_CONFIG_HEADER(config.h)

AC_PROG_CC
AC_LIBTOOL_WIN32_DLL
AM_PROG_LIBTOOL
AC_PROG_INSTALL

XORG_PROG_RAWCPP

AC_FUNC_FORK

dnl If you're looking here because you got a message that said
dnl 'error: possibly undefined macro: AC_DEFINE', the source of
dnl your problem is not here.   You need to upgrade to pkgconfig 0.18
dnl or later to get an updated pkg.m4 with PKG_CHECK_EXISTS defined.
dnl See http://www.mail-archive.com/autoconf@gnu.org/msg13413.html
AC_CHECK_FUNC(mkstemp, AC_DEFINE(HAS_MKSTEMP,1,
	[Define to 1 if you have the 'mkstemp' function.]))
AC_CHECK_FUNC(getifaddrs, AC_DEFINE(HAS_GETIFADDRS,1,
	[Define to 1 if you have the 'getifaddrs' function.]))
AC_CHECK_FUNC(getspnam, AC_DEFINE(USESHADOW,1,
	[Define to 1 if you have the shadow password functions.]))


# According to the old Imakefile, FreeBSD has this in libutil
AC_CHECK_FUNC(setproctitle, [HAS_SETPROCTITLE="yes"],
	AC_CHECK_LIB(util, setproctitle, 
		     [HAS_SETPROCTITLE="yes" ; XDM_LIBS="$XDM_LIBS -lutil"]))
if test "x$HAS_SETPROCTITLE" = "xyes" ; then
	AC_DEFINE(HAS_SETPROCTITLE,1,
		[Define to 1 if you have the 'setproctitle' function.])
fi

# According to the old Imakefile, BSD/OS has this in libutil
AC_CHECK_FUNC(setusercontext, [HAS_SETUSERCONTEXT="yes"],
	AC_CHECK_LIB(util, setusercontext, 
		    [HAS_SETUSERCONTEXT="yes" ; XDM_LIBS="$XDM_LIBS -lutil"]))
if test "x$HAS_SETUSERCONTEXT" = "xyes" ; then
	AC_DEFINE(HAS_SETUSERCONTEXT,1,
		[Define to 1 if you have the 'setusercontext' function.])
fi

XTRANS_CONNECTION_FLAGS
dnl Sets $SECURE_RPC to "yes" or "no"
XTRANS_SECURE_RPC_FLAGS
AM_CONDITIONAL(HAS_SECURE_RPC, test x$SECURE_RPC = xyes)

# darwin could have PAM support, but not yet
case $host_os in
    darwin*)
        use_pam_default=no
        ;;
    *)
        use_pam_default=try
        ;;
esac

# Check for PAM support
AC_ARG_WITH(pam, AC_HELP_STRING([--with-pam],[Use PAM for authentication]),
	[USE_PAM=$withval], [USE_PAM=$use_pam_default])
if test "x$USE_PAM" != "xno" ; then
	AC_SEARCH_LIBS(pam_open_session,[pam])
	AC_CHECK_FUNC(pam_open_session, 
		[AC_DEFINE(USE_PAM,1,[Use PAM for authentication])],
		[if test "x$USE_PAM" != "xtry" ; then
			AC_MSG_ERROR(["PAM support requested, but pam_open_session not found."])
		 fi])
fi

# FIXME: Find better test for which OS'es use su -m  - for now, just try to
# mirror the Imakefile setting of:
# if  defined(OpenBSDArchitecture) || defined(NetBSDArchitecture) || defined(FreeBSDArchitecture) || defined(DarwinArchitecture)


case $host_os in
    linux*)
        OS_CFLAGS="-D_XOPEN_SOURCE"
	SU="su"
        ;;
    darwin*)
        OS_CFLAGS="-D__DARWIN__ -DNO_LASTLOG"
	SU="su -m"
        ;;
    *bsd*)
        OS_CFLAGS="-DCSRG_BASED"
	SU="su -m"
        ;;
    solaris*|sysv4*)
	OS_CFLAGS=""
	GREETER_CFLAGS="$GREETER_CFLAGS -DGREET_LIB"
	SU="su"
	;;
    *)
        OS_CFLAGS=
	SU="su"
        ;;
esac
AC_SUBST(SU)

# Check for /dev/random or /dev/urandom
AC_ARG_WITH(random-device, 
	AC_HELP_STRING([--with-random-device\[=<pathname>\]],
		[Use <pathname> as a source of randomness]),
	RANDOM_DEVICE="$withval", RANDOM_DEVICE="try")

if test x$RANDOM_DEVICE = xyes -o x$RANDOM_DEVICE = xtry ; then
	AC_CHECK_FILE([/dev/urandom], [RANDOM_DEVICE=/dev/urandom],
		AC_CHECK_FILE([/dev/random], [RANDOM_DEVICE=/dev/random]))
	if test x$RANDOM_DEVICE = xyes ; then
		AC_MSG_ERROR(["random device support requested, but no random device was found."])		
	else 
		if test x$RANDOM_DEVICE = xtry ; then
			RANDOM_DEVICE="no"
		fi
	fi
fi

if test x$RANDOM_DEVICE != xno ; then
	AC_DEFINE_UNQUOTED(DEV_RANDOM,"$RANDOM_DEVICE",
		[Define to device that provides random data source])
fi


# FIXME: find out how to check for these things. Note that Kerberos was
# turned off everywhere but on OpenBSD.
AM_CONDITIONAL(HAS_KERBEROS_FIVE, false)
AM_CONDITIONAL(HAS_ARC4_RANDOM, false)

# Figure out where everything goes in the filesystem

AC_ARG_WITH(xdmlibdir,
	AC_HELP_STRING([--with-xdmlibdir=<pathname>],
	  [specify directory for xdm scripts and modules (default=${libdir}/X11/xdm)]),
	[XDMLIBDIR="$withval"], [XDMLIBDIR="${libdir}/X11/xdm"])
AC_SUBST(XDMLIBDIR)

AC_ARG_WITH(xdmconfigdir,
	AC_HELP_STRING([--with-xdmconfigdir=<pathname>],
	  [specify directory for xdm config files (default is same as xdmlibdir)]),
	[XDMCONFIGDIR="$withval"], [XDMCONFIGDIR="${XDMLIBDIR}"])
AC_SUBST(XDMCONFIGDIR)

AC_ARG_WITH(logdir,
	AC_HELP_STRING([--with-logdir=<pathname>],
	  [specify directory for xdm log files (default=/var/log)]),
	[XDMLOGDIR="$withval"], [XDMLOGDIR="/var/log"])
AC_SUBST(XDMLOGDIR)

AC_ARG_WITH(piddir,
	AC_HELP_STRING([--with-piddir=<pathname>],
	  [specify directory for xdm pid files (default=/var/run)]),
	[XDMPIDDIR="$withval"], [XDMPIDDIR="/var/run"])
AC_SUBST(XDMPIDDIR)

AC_ARG_WITH(default-vt,
	AC_HELP_STRING([--with-default-vt=<vtname>],
	  [specify default vt to start xdm on (default=none)]),
	[DEFAULTVT="$withval"], [[DEFAULTVT=""]])
AC_SUBST(DEFAULTVT)

AC_ARG_WITH(config-type,
	AC_HELP_STRING([--with-config-type={ws|fs}],
	 [default config file type to use - workstation (ws) or fileserver (fs) (default=ws)]),
	[SERVERSTYPE="$withval"], [SERVERSTYPE="ws"])
AC_SUBST(SERVERSTYPE)

# Checks for pkg-config packages

PKG_CHECK_EXISTS(xinerama, [
	AC_DEFINE([USE_XINERAMA], 1, 
		[Define to 1 to use XINERAMA in greeter & chooser])
	PKG_CHECK_MODULES(XINERAMA, xinerama xineramaproto)
	XDM_CFLAGS="$XDM_CFLAGS $XINERAMA_CFLAGS"
	XDM_LIBS="$XDM_LIBS $XINERAMA_LIBS"
	CHOOSER_CFLAGS="$CHOOSER_CFLAGS $XINERAMA_CFLAGS"
	CHOOSER_LIBS="$CHOOSER_LIBS $XINERAMA_LIBS"
	GREETER_CFLAGS="$GREETER_CFLAGS $XINERAMA_CFLAGS"
	GREETER_LIBS="$GREETER_LIBS $XINERAMA_LIBS"
])

# XPM logos for the greeter screen
AC_ARG_ENABLE(xpm-logos, 
    	AC_HELP_STRING([--enable-xpm-logos], 
			[Display xpm logos in greeter (default is YES)]),
	[USE_XPM="$enableval"], 
	PKG_CHECK_EXISTS(xpm, [USE_XPM="yes"], [USE_XPM="no"]))
if test "x$USE_XPM" = "xyes" ; then
	PKG_CHECK_MODULES(XPM, xpm)
	XDM_CFLAGS="$XDM_CFLAGS $XPM_CFLAGS"
	XDM_LIBS="$XDM_LIBS $XPM_LIBS"
	GREETER_CFLAGS="$GREETER_CFLAGS $XPM_CFLAGS"
	GREETER_LIBS="$GREETER_LIBS $XPM_LIBS"
	AC_DEFINE([XPM], 1, 
		[Define to 1 to display XPM logos on greeter screen])
fi
AM_CONDITIONAL(USE_XPM, test "x$USE_XPM" = "xyes")

AC_ARG_WITH(color-pixmap,
	AC_HELP_STRING([--with-color-pixmap=<filename>],
		[specify xpm file to use as color logo (default=xorg.xpm)]),
	[XDM_PIXMAP="$withval"], [XDM_PIXMAP="xorg.xpm"])
AC_ARG_WITH(bw-pixmap,
	AC_HELP_STRING([--with-bw-pixmap=<filename>],
      [specify xpm file to use as black-and-white logo (default=xorg-bw.xpm)]),
	[XDM_BWPIXMAP="$withval"], [XDM_BWPIXMAP="xorg-bw.xpm"])
AC_ARG_WITH(pixmapdir,
	AC_HELP_STRING([--with-pixmapdir=<pathname>],
	[specify directory for xpm files (default=xdmlibdir/pixmaps)]),
	[XDM_PIXMAPDIR="$withval"], 
	[XDM_PIXMAPDIR='${XDMLIBDIR}/pixmaps'])

AC_SUBST(XDM_PIXMAP)
AC_SUBST(XDM_BWPIXMAP)
AC_SUBST(XDM_PIXMAPDIR)


#
#  XDM
#

XAW_CHECK_XPRINT_SUPPORT(XDM_PRINT)

PKG_CHECK_MODULES(DMCP, xdmcp)
PKG_CHECK_MODULES(XPROTO, xproto)
XDM_CFLAGS="$XDM_CFLAGS $XDM_PRINT_CFLAGS $DMCP_CFLAGS $XPROTO_CFLAGS $OS_CFLAGS"
XDM_LIBS="$XDM_LIBS $XDM_PRINT_LIBS $DMCP_LIBS"

AC_CHECK_LIB(Xdmcp, XdmcpWrap, [xdmauth="yes"], [xdmauth="no"], [$DMCP_LIBS])
AC_SEARCH_LIBS(crypt, crypt)

AM_CONDITIONAL(HAS_XDM_AUTH, test x$xdmauth = xyes)

AC_SUBST(XDM_CFLAGS)
AC_SUBST(XDM_LIBS)

#
# xmdshell
#

PKG_CHECK_MODULES(AUTH, xau)
PKG_CHECK_MODULES(XT, xt)
XDMSHELL_CFLAGS="$XDM_CFLAGS $XDM_PRINT_CFLAGS $AUTH_CFLAGS $DMCP_CFLAGS $XPROTO_CFLAGS $XT_CFLAGS $OS_CFLAGS"
XDMSHELL_LIBS="$XDM_LIBS $XDM_PRINT_LIBS $DMCP_LIBS"

AC_SUBST(XDMSHELL_CFLAGS)
AC_SUBST(XDMSHELL_LIBS)

#
# Greeter
#

PKG_CHECK_MODULES(XDMGREET, xt x11)

XDMGREET_CFLAGS="$XDMGREET_CFLAGS $DMCP_CFLAGS $XPROTO_CFLAGS $OS_CFLAGS $GREETER_CFLAGS"
XDMGREET_LIBS="$XDMGREET_LIBS $DMCP_LIBS $GREETER_LIBS"

GREETERLIB="${XDMLIBDIR}/libXdmGreet.so"
AC_DEFINE_DIR(DEF_GREETER_LIB, GREETERLIB, [Location of Xdm Greeter library])

AC_SUBST(XDMGREET_CFLAGS)
AC_SUBST(XDMGREET_LIBS)

#
# Chooser
#

CHOOSER_CFLAGS="-D_BSD_SOURCE $CHOOSER_CFLAGS $XDM_PRINT_CFLAGS $XPROTO_CFLAGS $DMCP_CFLAGS"
CHOOSER_LIBS="$CHOOSER_LIBS $XDM_PRINT_LIBS $DMCP_LIBS"

AC_SUBST(CHOOSER_CFLAGS)
AC_SUBST(CHOOSER_LIBS)

# Defaults for X session - should probably be configure --with-* flags
# Currently just trying to mirror what defaults were set in Imake

# -DDEF_SERVER_LINE=":0 local $(DEF_SERVER) :0"
if test -z "$DEF_SERVER" ; then 
	DEF_SERVER="${bindir}/X"
fi
if test -z "$DEF_SERVER_LINE" ; then 
	DEF_SERVER_LINE=":0 local ${DEF_SERVER} :0"
fi
AC_DEFINE_DIR(DEF_SERVER_LINE, DEF_SERVER_LINE,
	[Define to default Xserver file entry for local X server])

# -DXRDB_PROGRAM="$(BINDIR)/xrdb"
AC_DEFINE_DIR(XRDB_PROGRAM, bindir/xrdb, 
	[Define to pathname for xrdb program])

# -DDEF_SESSION="$(BINDIR)/xterm -ls"
AC_DEFINE_DIR(DEF_SESSION, bindir/xterm -ls,
	[Define to default command to start X session])

# -DDEF_USER_PATH="$(DEF_USER_PATH)"
# Default values taken from DefaultUserPath setting in the
# old Imake *.cf files for each platform
if test -z "$DEF_USER_PATH" ; then
	case $host_os in
	    dragonfly*)
		DEF_USER_PATH="/bin:/usr/bin:/sbin:/usr/sbin:${bindir}:/usr/local/bin"
		;;
	    freebsd*)
		DEF_USER_PATH="/bin:/usr/bin:/sbin:/usr/sbin:${bindir}:/usr/gnu/bin:/usr/local/bin"
		;;
	    netbsd*)
		DEF_USER_PATH="/bin:/usr/bin:/usr/pkg/bin:/usr/local/bin:${bindir}"
		;;
	    openbsd*|bsdi*)
		DEF_USER_PATH="/bin:/usr/bin:/usr/local/bin:${bindir}"
		;;
	    solaris*)
		DEF_USER_PATH="/usr/bin:${bindir}"
		;;
	    *)
		DEF_USER_PATH="/bin:/usr/bin:${bindir}:/usr/ucb"
		;;
	esac
fi
AC_DEFINE_DIR(DEF_USER_PATH, DEF_USER_PATH,
	[Define to default PATH environment variable for user sessions])

# -DDEF_SYSTEM_PATH="$(DEF_SYSTEM_PATH)"
# Default values taken from DefaultSystemPath setting in the
# old Imake *.cf files for each platform
if test -z "$DEF_SYSTEM_PATH" ; then
	case $host_os in
	    freebsd*|dragonfly*)
		DEF_SYSTEM_PATH="/bin:/usr/bin:/sbin:/usr/sbin:${bindir}"
		;;
	    netbsd*|openbsd*|bsdi*)
		DEF_SYSTEM_PATH="/sbin:/usr/sbin:/bin:/usr/bin:${bindir}"
		;;
	    solaris*)
		DEF_SYSTEM_PATH="/sbin:/usr/sbin:/usr/bin:${bindir}"
		;;
	    *)
		DEF_SYSTEM_PATH="/etc:/bin:/usr/bin:${bindir}:/usr/ucb"
		;;
	esac
fi
AC_DEFINE_DIR(DEF_SYSTEM_PATH, DEF_SYSTEM_PATH,
	[Define to default PATH environment variable for system commands])

# -DDEF_SYSTEM_SHELL="$(BOURNE_SHELL)"
if test -z "$BOURNE_SHELL" ; then
	BOURNE_SHELL=/bin/sh
fi
AC_DEFINE_DIR(DEF_SYSTEM_SHELL, BOURNE_SHELL,
	[Define to default shell])

# -DDEF_FAILSAFE_CLIENT="$(BINDIR)/xterm"

AC_DEFINE_DIR(DEF_FAILSAFE_CLIENT, bindir/xterm,
	[Define to default command to start failsafe X session])

# -DDEF_XDM_CONFIG="$(XDMDIR)/xdm-config"

AC_DEFINE_DIR(DEF_XDM_CONFIG, XDMCONFIGDIR/xdm-config, 
	[Define to pathname of default xdm-config file])

# -DDEF_CHOOSER="$(CHOOSERPATH)"

AC_DEFINE_DIR(DEF_CHOOSER, XDMLIBDIR/chooser, 
	[Define to pathname of where chooser binary is installed])

# -DDEF_AUTH_DIR="$(XDMDIR)"

AC_DEFINE_DIR(DEF_AUTH_DIR, XDMCONFIGDIR, 
	[Define to pathname where authentication cookie files should be stored])

# -DDEF_GREETER_LIB="$(XDMDIR)/libXdmGreet.so"

AC_DEFINE_DIR(DEF_GREETER_LIB, XDMLIBDIR/libXdmGreet.so, 
	[Define to pathname where greeter shared object is installed (if used)])

XORG_MANPAGE_SECTIONS
XORG_RELEASE_VERSION

AC_OUTPUT([Makefile config/Makefile greeter/Makefile])
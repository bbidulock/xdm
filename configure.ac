
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
AC_INIT(xdm,[0.99.0], [https://bugs.freedesktop.org/enter_bug.cgi?product=xorg],xdm)
AM_INIT_AUTOMAKE([dist-bzip2])
AM_MAINTAINER_MODE

AM_CONFIG_HEADER(config.h)

AC_PROG_CC
AC_PROG_INSTALL
AC_CANONICAL_HOST

AC_FUNC_FORK
AC_CHECK_FUNC(mkstemp, 
   AC_DEFINE(HAS_MKSTEMP,1,[Define to 1 if you have the 'mkstemp' function.]))

XTRANS_TCP_FLAGS
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

case $host_os in
    linux*)
        OS_CFLAGS="-D_XOPEN_SOURCE"
        ;;
    darwin*)
        OS_CFLAGS="-D__DARWIN__ -DNO_LASTLOG"
        ;;
    *)
        OS_CFLAGS=
        ;;
esac

# FIXME: find out how to check for these things. Note that Kerberos was
# turned off everywhere but on OpenBSD.
AM_CONDITIONAL(HAS_KERBEROS_FIVE, false)
AM_CONDITIONAL(HAS_ARC4_RANDOM, false)

# Checks for pkg-config packages

#
#  XDM
#

XAW_CHECK_XPRINT_SUPPORT(XDM)

PKG_CHECK_MODULES(DMCP, xdmcp)
PKG_CHECK_MODULES(XPROTO, xproto)
XDM_CFLAGS="$XDM_CFLAGS $DMCP_CFLAGS $XPROTO_CFLAGS $OS_CFLAGS"
XDM_LIBS="$XDM_LIBS $DMCP_LIBS"


AC_CHECK_LIB(Xdmcp, XdmcpWrap, [xdmauth="yes"], [xdmauth="no"], [$DMCP_LIBS])
AC_SEARCH_LIBS(crypt, crypt)

AM_CONDITIONAL(HAS_XDM_AUTH, test x$xdmauth = xyes)

AC_SUBST(XDM_CFLAGS)
AC_SUBST(XDM_LIBS)

#
# Chooser
#
XAW_CHECK_XPRINT_SUPPORT(CHOOSER)

CHOOSER_CFLAGS="-D_BSD_SOURCE $XPROTO_CFLAGS"
CHOOSER_LIBS="$CHOOSER_LIBS $DMCP_LIBS"

AC_SUBST(CHOOSER_CFLAGS)
AC_SUBST(CHOOSER_LIBS)

XORG_RELEASE_VERSION

AC_OUTPUT([Makefile])

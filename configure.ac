
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
AC_INIT(xdm,[7.0], [xorg@freedesktop.org],xdm)
AM_INIT_AUTOMAKE([dist-bzip2])
AM_MAINTAINER_MODE

AM_CONFIG_HEADER(config.h)

AC_PROG_CC
AC_PROG_INSTALL


AC_CHECK_LIB(rpcsvc, xdr_status, has_secure_rpc=yes, has_secure_rpc=no)
AM_CONDITIONAL(HAS_SECURE_RPC, test x$has_secure_rpc = xyes)

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
XDM_CFLAGS="$XDM_CFLAGS $DMCP_CFLAGS"
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

AC_SUBST(CHOOSER_CFLAGS)
AC_SUBST(CHOOSER_LIBS)

AC_OUTPUT([Makefile])

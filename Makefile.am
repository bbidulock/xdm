#
#  Copyright 2005  Red Hat, Inc.
#
#  Permission to use, copy, modify, distribute, and sell this software and its
#  documentation for any purpose is hereby granted without fee, provided that
#  the above copyright notice appear in all copies and that both that
#  copyright notice and this permission notice appear in supporting
#  documentation, and that the name of Red Hat not be used in
#  advertising or publicity pertaining to distribution of the software without
#  specific, written prior permission.  Red Hat makes no
#  representations about the suitability of this software for any purpose.  It
#  is provided "as is" without express or implied warranty.
#
#  RED HAT DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
#  INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
#  EVENT SHALL RED HAT BE LIABLE FOR ANY SPECIAL, INDIRECT OR
#  CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
#  DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
#  TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
#  PERFORMANCE OF THIS SOFTWARE.

SUBDIRS = chooser config greeter man

bin_PROGRAMS = xdm
noinst_PROGRAMS =

if INSTALL_XDMSHELL
bin_PROGRAMS += xdmshell
else
noinst_PROGRAMS += xdmshell
endif

AM_CFLAGS = $(CWARNFLAGS)

#
# xdm
#

xdm_CFLAGS = $(XDM_CFLAGS)
# The xdm binary needs to export symbols so that they can be used from
# libXdmGreet.so loaded through a dlopen call from session.c
xdm_LDFLAGS =  -export-dynamic
xdm_LDADD = $(XDM_LIBS)

xdm_SOURCES =	\
        auth.c \
        daemon.c \
        server.c \
        dpylist.c \
        dm.c \
        error.c \
        file.c \
        netaddr.c \
        reset.c \
        resource.c \
        protodpy.c \
        policy.c \
        session.c \
        socket.c \
        streams.c \
        util.c \
        xdmcp.c \
        mitauth.c \
        genauth.c \
        access.c \
        choose.c \
        dm_auth.h \
        dm_error.h \
        dm.h \
        dm_socket.h \
        greet.h

if HAS_XDM_AUTH
xdm_CFLAGS +=  -DHASXDMAUTH
xdm_SOURCES += xdmauth.c
endif

if !HAVE_ARC4RANDOM
xdm_SOURCES += prngc.c
endif

if HAS_SECURE_RPC
xdm_CFLAGS += -DSECURE_RPC
xdm_SOURCES += rpcauth.c
endif

if HAS_KERBEROS_FIVE
xdm_SOURCES += krb5auth.c
endif

#
#  xdmshell
#
xdmshell_CFLAGS = $(XDMSHELL_CFLAGS) -DBINDIR="\"$(bindir)\""


xdmshell_SOURCES =	\
        xdmshell.c

# App default files

# This ensures distcheck is performed in a location where user has write access
DISTCHECK_CONFIGURE_FLAGS = --with-appdefaultdir=\$${datadir}/X11/app-defaults

dist_appdefault_DATA = app-defaults/Chooser

if LINT
ALL_LINT_FLAGS=$(LINT_FLAGS) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) \
		$(AM_CPPFLAGS) $(CPPFLAGS)

lint:
	$(LINT) $(ALL_LINT_FLAGS) $(xdm_CFLAGS) $(xdm_SOURCES) $(xdm_LIBS)
	$(LINT) $(ALL_LINT_FLAGS) $(xdmshell_CFLAGS) $(xdmshell_SOURCES)
	(cd greeter && $(MAKE) $(AM_MAKEFLAGS) lint)
	(cd chooser && $(MAKE) $(AM_MAKEFLAGS) lint)
endif LINT

MAINTAINERCLEANFILES = ChangeLog INSTALL

.PHONY: ChangeLog INSTALL

INSTALL:
	$(INSTALL_CMD)

ChangeLog:
	$(CHANGELOG_CMD)

dist-hook: ChangeLog INSTALL

ACLOCAL_AMFLAGS = -I m4

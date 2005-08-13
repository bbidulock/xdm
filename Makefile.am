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

bin_PROGRAMS = xdm xdmshell

xdmdir = $(libdir)/xdm
xdm_PROGRAMS = chooser

#
# xdm
#

xdm_CFLAGS = $(XDM_CFLAGS) -D_BSD_SOURCE
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
	greeter/greet.c		\
	greeter/Login.c		\
	greeter/Login.h		\
	greeter/LoginP.h	\
	greeter/verify.c	\
        dm_auth.h \
        dm_error.h \
        dm.h \
        dm_socket.h \
        greet.h

if HAS_XDM_AUTH
xdm_CFLAGS +=  -DHASXDMAUTH
xdm_SOURCES += xdmauth.c
endif

if !HAS_ARC4_RANDOM
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
xdmshell_CFLAGS = $(XPROTO_CFLAGS)

xdmshell_SOURCES =	\
        xdmshell.c

#
#  chooser
#

chooser_CFLAGS = $(CHOOSER_CFLAGS)
chooser_LDADD = $(CHOOSER_LIBS)

chooser_SOURCES =	\
	chooser.c

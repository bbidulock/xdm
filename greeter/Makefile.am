# $XdotOrg: app/xdm/greeter/Makefile.am,v $

xdmdir = $(libdir)/X11/xdm

xdm_LTLIBRARIES = libXdmGreet.la

libXdmGreet_la_SOURCES = \
		  Login.c \
		  greet.c \
		  verify.c

libXdmGreet_la_LIBADD = $(XDMGREET_LIBS)

AM_CFLAGS = $(XDMGREET_CFLAGS)

libXdmGreet_la_LDFLAGS = -module -avoid-version

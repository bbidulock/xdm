xdmlibdir = $(XDMLIBDIR)

xdmlib_LTLIBRARIES = libXdmGreet.la

libXdmGreet_la_SOURCES = \
		  Login.c \
		  Login.h \
		  LoginP.h \
		  greet.c \
		  verify.c

libXdmGreet_la_LIBADD = $(XDMGREET_LIBS)

AM_CFLAGS = $(CWARNFLAGS) $(XDMGREET_CFLAGS) -DGREET_LIB -I$(top_srcdir)

libXdmGreet_la_LDFLAGS = -module -avoid-version

if LINT
ALL_LINT_FLAGS=$(LINT_FLAGS) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) \
		$(AM_CPPFLAGS) $(CPPFLAGS)

lint:
	$(LINT) $(ALL_LINT_FLAGS) $(libXdmGreet_la_SOURCES)

endif LINT

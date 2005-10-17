# $Id$
#

include $(top_srcdir)/cpprules.in

xdmdir = $(libdir)/X11/xdm

dist_xdm_DATA = \
	Xaccess

xdm_DATA = \
	xdm-config \
	Xresources \
	Xservers

dist_xdm_SCRIPTS = \
	Xsession \
	GiveConsole \
	TakeConsole \
	Xsetup_0 \
	Xstartup \
	Xreset \
	Xwilling

pixmapdir = $(XDM_PIXMAPDIR)

dist_pixmap_DATA = xorg-bw.xpm xorg.xpm

BUILT_SOURCES = Xservers.ws xdm-config Xresources

CLEANFILES = $(BUILT_SOURCES) Xservers Xservers.ws xdm-config Xresources

EXTRA_DIST = Xservers.ws.cpp xdm-config.cpp Xresources.cpp Xservers.fs

Xservers: Xservers.$(SERVERSTYPE)
	ln -s Xservers.$(SERVERSTYPE) Xservers

# Defines used when cpp'ing config files to replace settings in the files

#if USE_XPM
XPMDEFINES = -DXPM -DBITMAPDIR=$(XDM_PIXMAPDIR) -DXDM_PIXMAP=$(XDM_PIXMAP) \
        -DXDM_BWPIXMAP=$(XDM_BWPIXMAP)
#endif

CPP_FILES_FLAGS = -DBINDIR=$(bindir) -DDEFAULTVT=$(DEFAULTVT) \
    -DXDMDIR=$(xdmdir) -DXDMLOGDIR=$(XDMLOGDIR) -DXDMPIDDIR=$(XDMPIDDIR) \
    -DSU=$(SU) -DCHOOSERPATH=$(xdmdir)/chooser $(XPMDEFINES)

Xservers.ws: $(srcdir)/Xservers.ws.cpp
	$(RAWCPP) $(RAWCPPFLAGS) $(CPP_FILES_FLAGS) < $(srcdir)/Xservers.ws.cpp | $(CPP_SED_MAGIC) > $@

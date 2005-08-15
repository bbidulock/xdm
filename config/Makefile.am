# $Id$
#

include $(top_srcdir)/cpprules.in

pkglib_DATA = \
	xdm-config \
	Xresources \
	Xservers \
	Xaccess

pkglib_SCRIPTS = \
	Xsession \
	GiveConsole \
	TakeConsole \
	Xsetup_0 \
	Xstartup \
	Xreset \
	Xwilling

pixmapdir = $(pkglibdir)/pixmaps

pixmap_DATA = xorg-bw.xpm xorg.xpm

BUILT_SOURCES = Xservers.ws

CLEANFILES = $(BUILT_SOURCES) Xservers Xservers.ws xdm-config Xresources

Xservers:
	ln -s Xservers.$(SERVERSTYPE) Xservers

# Defines used when cpp'ing config files to replace settings in the files

#if USE_XPM
XPMDEFINES = -DXPM -DBITMAPDIR=$(XDM_PIXMAPDIR) -DXDM_PIXMAP=$(XDM_PIXMAP) \
        -DXDM_BWPIXMAP=$(XDM_BWPIXMAP)
#endif

CPP_FILES_FLAGS = -DBINDIR=$(bindir) -DDEFAULTVT=$(DEFAULTVT) \
    -DXDMDIR=$(pkglibdir) -DXDMLOGDIR=$(XDMLOGDIR) -DXDMPIDDIR=$(XDMPIDDIR) \
    -DSU=$(SU) -DCHOOSERPATH=$(pkglibdir)/chooser $(XPMDEFINES)
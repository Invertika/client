
iconsdir = $(pkgdatadir)/data/icons

icons_DATA = ivk.png \
	     ivk.xpm

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(datarootdir)/pixmaps
	$(INSTALL) $(srcdir)/invertika.png $(DESTDIR)$(datarootdir)/pixmaps/ivk.png

uninstall-local:
	rm -f $(DESTDIR)$(datarootdir)/pixmaps/ivk.png

EXTRA_DIST =		\
	$(icons_DATA)

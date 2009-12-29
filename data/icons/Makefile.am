
iconsdir = $(pkgdatadir)/data/icons

icons_DATA = invertika.png \
	     invertika.xpm

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(datarootdir)/pixmaps
	$(INSTALL) $(srcdir)/invertika.png $(DESTDIR)$(datarootdir)/pixmaps/invertika.png

uninstall-local:
	rm -f $(DESTDIR)$(datarootdir)/pixmaps/invertika.png

EXTRA_DIST =		\
	$(icons_DATA)

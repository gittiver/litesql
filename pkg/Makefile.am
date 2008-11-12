bin_SCRIPTS = litesql-config

CLEANFILES = $(bin_SCRIPTS) litesql.pc

install-data-local: litesql.pc
	mkdir -p $(libdir)/pkgconfig && \
	cp -p litesql.pc $(libdir)/pkgconfig

uninstall-local:
	-rm -f $(libdir)/pkgconfig/litesql.pc


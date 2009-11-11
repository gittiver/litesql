SUBDIRS=src include docs rpm pkg

EXTRA_DIST=CMakeLists.txt \
	   Copyright.txt \
	   README \
	   readme.txt \
	   config.h.in \
	   config.h.cmake \
	   cmake/FindPostgreSQL.cmake \
	   cmake/FindMySQL.cmake \
	   cmake/FindSQLite3.cmake \
	   win32

ACLOCAL_AMFLAGS=-I m4
AUTOMAKE_OPTIONS=foreign

dist-hook:
	find $(distdir)/win32 -name \.svn
	rm -rf `find $(distdir)/win32 -name .svn`

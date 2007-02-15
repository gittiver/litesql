AM_CXXFLAGS=-I$(srcdir)/../../include
lib_LTLIBRARIES=liblitesql.la
liblitesql_la_SOURCES=database.cpp expr.cpp selectquery.cpp persistent.cpp split.cpp string.cpp backend.cpp updatequery.cpp field.cpp datasource.cpp sqlite3.cpp postgresql.cpp mysql.cpp compatibility.cpp datetime.cpp dbtypes.cpp
EXTRA_DIST=compatibility.hpp
liblitesql_la_LDFLAGS = -version-info `echo $(VERSION)|sed -e 's/\./:/g'`

AM_CXXFLAGS=-I$(srcdir)/../../include
lib_LTLIBRARIES=liblitesql.la
liblitesql_la_SOURCES=database.cpp expr.cpp selectquery.cpp persistent.cpp split.cpp string.cpp backend.cpp updatequery.cpp field.cpp datasource.cpp sqlite3_backend.cpp postgresql.cpp mysql.cpp compatibility.cpp datetime.cpp
EXTRA_DIST=compatibility.hpp odbc_backend.hpp mysql.hpp postgresql.hpp sqlite3.hpp
liblitesql_la_LDFLAGS = -version-info 3:4 
#`echo $(VERSION)|sed -e 's/\./:/g'`

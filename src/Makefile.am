AM_CXXFLAGS=-I../include
bin_PROGRAMS=litesql-gen
noinst_PROGRAMS=example
example_SOURCES=example.cpp exampledatabase.cpp exampledatabase.hpp
litesql_gen_SOURCES=litesql-gen.cpp litesql-gen-actions.cpp xmlobjects.cpp md5.cpp litesql-gen-cpp.cpp
litesql_gen_LDADD=liblitesql.la
example_LDADD=liblitesql.la
lib_LTLIBRARIES=liblitesql.la
liblitesql_la_SOURCES=database.cpp expr.cpp selectquery.cpp persistent.cpp split.cpp string.cpp backend.cpp updatequery.cpp field.cpp datasource.cpp sqlite3.cpp postgresql.cpp mysql.cpp compatibility.cpp
liblitesql_la_LDFLAGS = -version-info 0:3:0



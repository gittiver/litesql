AM_CXXFLAGS=-I$(srcdir)/../../include

noinst_PROGRAMS=example

example_SOURCES=example.cpp exampledatabase.cpp exampledatabase.hpp

example_LDADD=../library/liblitesql.la

EXTRA_DIST=CMakeLists.txt \
	   exampledatabase.xml \
	   exampledatabase.dot \
	   exampledatabase.png \
	   example_output.txt \
	   example_queries.txt

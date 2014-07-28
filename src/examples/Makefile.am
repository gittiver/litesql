AM_CXXFLAGS=-I$(srcdir)/../../include

noinst_PROGRAMS=example

example_SOURCES=example.cpp exampledatabase.cpp exampledatabase.hpp

example_LDADD=../library/liblitesql.la ../library/liblitesql-util.la

EXTRA_DIST=CMakeLists.txt \
	   exampledatabase.xml \
	   auth.xml \
	   exampledatabase.dot \
	   example_output.txt \
	   example_queries.txt

$(srcdir)/exampledatabase.cpp \
$(srcdir)/exampledatabase.hpp: $(srcdir)/exampledatabase.xml
	litesql-gen -t c++ $(srcdir)/exampledatabase.xml  

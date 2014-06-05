AM_CXXFLAGS=-I$(srcdir)/../../include

noinst_PROGRAMS=example

example_SOURCES=example.cpp ExampleDatabase.cpp ExampleDatabase.hpp

example_LDADD=../library/liblitesql.la ../library/liblitesql-util.la

EXTRA_DIST=CMakeLists.txt \
	   exampledatabase.xml \
	   auth.xml \
	   ExampleDatabase.dot \
	   exampledatabase.png \
	   example_output.txt \
	   example_queries.txt

$(srcdir)/ExampleDatabase.cpp \
$(srcdir)/ExampleDatabase.hpp: $(srcdir)/exampledatabase.xml
	litesql-gen -t c++ $(srcdir)/exampledatabase.xml  

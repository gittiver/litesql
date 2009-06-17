AM_CXXFLAGS=-I$(srcdir)/../../include

noinst_PROGRAMS=test-split test-selectquery test-datetime.cpp

test_split_SOURCES=test-split.cpp
test_split_LDADD=../library/liblitesql.la

test_selectquery_SOURCES=test-selectquery.cpp
test_selectquery_LDADD=../library/liblitesql.la

test_datetime_SOURCES=test-datetime.cpp
test_datetime_LDADD=../library/liblitesql.la

EXTRA_DIST=CMakeLists.txt

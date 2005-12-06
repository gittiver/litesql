#!/bin/sh
flexml -LSH -a litesql-gen.xml litesql.dtd
mv litesql.h litesql-gen.hpp
cat litesql.l|sed -e 's/#include "litesql\.h"/#include "litesql-gen\.hpp"/' > litesql-gen.l
rm litesql.l
cat litesql-gen.c|sed -e 's/#include "litesql\.h"/#include "litesql-gen\.hpp"/' > litesql-gen-actions.cpp
rm litesql-gen.c
flex -olitesql-gen.cpp litesql-gen.l
make

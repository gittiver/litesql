#!/bin/sh
flexml -L -S -H -a litesql-gen.xml litesql.dtd
mv litesql.h litesql-gen.hpp
cat litesql.l\
|sed -e 's/#include "litesql\.h"/#include "litesql-gen\.hpp"\n#include "common.hpp"/'\
|sed -e "s/FAIL(\"Premature EOF: \`<\/database>' expected\.\");/{\nyypop_buffer_state();\nif (!posStack.empty()) {xml::Position p = *posStack.begin(); yylineno = p.line; currentFile = p.file; posStack.pop_front(); } \nif (!YY_CURRENT_BUFFER) FAIL(\"Premature EOF: \`<\/database>' expected.\");}/"\
|sed -e 's/YY_START);/YY_START, currentFile.c_str());/'\
|sed -e 's/state %d/state %d, file \\"%s\\"/'\
|sed -e 's/<<EOF>>.*SUCCEED/<<EOF>>  { if (posStack.empty()) { SUCCEED; } else FAIL(\"Premature <\/database>.\"); }/'\
> litesql-gen.l
rm litesql.l*

cat litesql-gen.c|sed -e 's/#include "litesql\.h"/#include "litesql-gen\.hpp"/' > litesql-gen-actions.cpp
rm litesql-gen.c
flex -olitesql-gen.cpp --header-file=scanner.hpp litesql-gen.l 
make

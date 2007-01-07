#!/bin/sh
flexml -L -S -H flexml-header.hpp litesql.dtd
cp litesql.l scanner.l
#cat litesql.l\
#|sed -e 's/#include "flexml-header\.hpp"/#include "scanner-globals\.hpp"\n/'\
#> scanner.l
#|sed -e "s/FAIL(\"Premature EOF: \`<\/database>' expected\.\");/{\nyypop_buffer_state();\nif (!posStack.empty()) {xml::Position p = *posStack.begin(); yylineno = p.line; currentFile = p.file; posStack.pop_front(); } \nif (!YY_CURRENT_BUFFER) FAIL(\"Premature EOF: \`<\/database>' expected.\");}/"\
#|sed -e 's/YY_START);/YY_START, currentFile.c_str());/'\
#|sed -e 's/state %d/state %d, file \\"%s\\"/'\
#|sed -e 's/<<EOF>>.*SUCCEED/<<EOF>>  { if (posStack.empty()) { SUCCEED; } else FAIL(\"Premature <\/database>.\"); }/'\
#> scanner.l
rm litesql.l

flex -oscanner.cpp --header-file=scanner.hpp scanner.l 
make

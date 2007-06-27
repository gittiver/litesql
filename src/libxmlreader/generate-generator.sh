#!/bin/sh
flexml -L -S -H flexml-header.hpp litesql.dtd
cat litesql.l\
|sed -e 's/#include "flexml-header\.hpp"/#include "scanner-globals\.hpp"\n/'\
|sed -e "s/FAIL(\"Premature EOF: \`<\/database>' expected\.\");/{\nyypop_buffer_state();\nif (!xml::posStack.empty()) {xml::Position p = *xml::posStack.begin(); yylineno = p.line; xml::currentFile = p.file; xml::posStack.pop_front(); } \nif (!YY_CURRENT_BUFFER) FAIL(\"Premature EOF: \`<\/database>' expected.\");}/"\
|sed -e 's/YY_START);/YY_START, xml::currentFile.c_str());/'\
|sed -e 's/state %d/state %d, file \\"%s\\"/'\
|sed -e 's/<<EOF>>.*SUCCEED/<<EOF>>  { if (xml::posStack.empty()) { SUCCEED; } else FAIL(\"Premature <\/database>.\"); }/'\
> scanner.l
rm litesql.l

flex -oscanner.cpp --header-file=scanner.hpp scanner.l 
make

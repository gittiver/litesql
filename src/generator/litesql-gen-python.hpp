#ifndef _litesql_gen_python_hpp
#define _litesql_gen_python_hpp
#include <cstdio>
#include "xmlobjects.hpp"

void writePython(xml::Database& db,
                 std::vector<xml::Object*>& objects,
                 std::vector<xml::Relation*>& relations);
#endif

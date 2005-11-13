#ifndef _litesql_gen_graphviz_hpp
#define _litesql_gen_graphviz_hpp
#include <cstdio>
#include "xmlobjects.hpp"

void writeGraphviz(xml::Database& db,
                   std::vector<xml::Object>& objects,
                   std::vector<xml::Relation>& relations);

#endif

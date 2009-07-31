#ifndef _litesql_gen_main_hpp
#define _litesql_gen_main_hpp
#include "config.h"

#include "xmlobjects.hpp"
#include <vector>
#include <string>
#include <cstdio>
void generateCode(xml::Database& db,
                  std::vector<xml::Object*>& objects,
                  std::vector<xml::Relation*>& relations);

#ifndef WITH_SAX_PARSER
int litesql_gen_main(int argc, char **argv, std::FILE ** yyin);
#endif // #ifndef WITH_SAX_PARSER

void report(const std::string& msg);
#endif

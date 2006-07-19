#ifndef _litesql_gen_main_hpp
#define _litesql_gen_main_hpp
#include "xmlobjects.hpp"
#include <vector>
#include <string>
#include <cstdio>
void generateCode(xml::Database& db);
int litesql_gen_main(int argc, char **argv, std::FILE ** yyin);
void report(const std::string& msg);
#endif

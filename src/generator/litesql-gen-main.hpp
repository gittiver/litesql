#ifndef _litesql_gen_main_hpp
#define _litesql_gen_main_hpp
#include "xmlobjects.hpp"
#include <vector>
#include <string>
#include <cstdio>
void generateCode(xml::Database& db,
                  std::vector<xml::Object*>& objects,
                  std::vector<xml::Relation*>& relations);
int litesql_gen_main(int argc, char **argv, std::FILE ** yyin);

void report(const std::string& msg);

std::string quote(const std::string& s);
std::string brackets(const std::string& s);
std::string sqbrackets(const std::string& s);
std::string braces(const std::string& s);
#endif

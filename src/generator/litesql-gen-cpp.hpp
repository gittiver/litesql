#ifndef _litesql_gen_cpp_hpp
#define _litesql_gen_cpp_hpp
#include <cstdio>
#include "xmlobjects.hpp"

void writeCPPClasses(std::FILE* hpp, std::FILE* cpp, 
                     const std::string& dbName,
                     std::vector<xml::Object>& objects,
                     std::vector<xml::Relation>& relations);

#endif

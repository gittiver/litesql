#ifndef _litesql_gen_common_hpp
#define _litesql_gen_common_hpp
#include <string>
#include <list>
#include "args.hpp"
#include "xmlreader.hpp"

std::string quote(const std::string& s);
std::string brackets(const std::string& s);
std::string sqbrackets(const std::string& s);
std::string braces(const std::string& s);

#endif

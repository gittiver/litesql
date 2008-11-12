/* LiteSQL 
 * 
 * By Tero Laitinen 
 * 
 * See LICENSE for copyright information. */

/** \file string.hpp
    helpful string utils */
#ifndef _litesql_string_hpp
#define _litesql_string_hpp

#include <string>
#include <sstream>
namespace litesql {
/** returns string representation of passed parameter if it can be 
    written to ostringstream */
template <class T> 
std::string toString(T a) {
    std::ostringstream ost;
    ost << a;
    return ost.str();
}
/** returns true if 'what' starts with 'with' */
bool startsWith(std::string what, std::string with);
/** returns true if 'what' end with 'with' */
bool endsWith(std::string what, std::string with);
/** returns lower-case version of the string */
std::string toLower(std::string s);
/** returns upper-case version of the string */
std::string toUpper(std::string s);
/** returns a string with no empty characters at the end of the string */
std::string rstrip(std::string s);
/** returns a string with no empty characters at the beginning of the string */
std::string lstrip(std::string s);
/** returns a copy string 's' where 'what' is replaced with 'with' */
std::string replace(std::string s, std::string what, std::string with);
/** converts string representation of a hex number to integer */
int hexToInt(std::string s);
/** string version of atoi */
int atoi(const std::string &s);
/** returns a string which is duplicated 'amount' times */
std::string operator*(int amount, const std::string &s);
/** returns a string which is duplicated 'amount' times */
std::string operator*(const std::string &s, int amount);
/** escapes ' characters so that they do not break SQL statements.
    Note: 'NULL' is escaped to NULL */
std::string escapeSQL(const std::string &str);
}

#endif
















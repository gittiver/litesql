/* LiteSQL - string utilities' implementation
 * 
 * The list of contributors at http://litesql.sf.net/ 
 * 
 * See LICENSE for copyright information. */

#include "litesql/utils.hpp"

#include <ctype.h>
#include <cstdlib>

namespace litesql {
using namespace std; 

bool startsWith(const string& what, const string& with) {
    if (what.size() < with.size())
        return false;
    return (what.substr(0, with.size()) == with);
}

bool endsWith(const string& what, const string& with) {
    if (what.size() < with.size())
        return false;
    return (what.substr(what.size()-with.size(), what.size()) == with);
}

string toLower(const string& s) {
    string result(s);
    for (unsigned int i = 0; i < s.size(); i++)
        result[i] = static_cast<char>(tolower(s[i]));
    return result;
}
string toUpper(const string& s) {
    string result(s);
    for (unsigned int i = 0; i < s.size(); i++)
        result[i] = static_cast<char>(toupper(s[i]));
    return result;
}

string capitalize(const string& s) {
   string result(s); 
   if (!s.empty())
   {
      result[0]=static_cast<char>(toupper(s[0]));
   }
   return result;
}
string decapitalize(const string& s) {
   string result(s); 
   if (!s.empty())
   {
      result[0]=static_cast<char>(tolower(s[0]));
   }
   return result;
}

string rstrip(const string& s) {
    if (s.empty())
        return s;
    int pos = s.size()-1;
    while (1) {
        if (isspace(s[pos]) && pos > 0)
            pos--;
        else
            break;
    }
    return s.substr(0, pos+1);
}
string lstrip(const string& s) {
    unsigned int pos = 0;
    while (1) {
        if (isspace(s[pos]) && pos < s.size()-1)
            pos++;
        else
            break;
    }
    return s.substr(pos, s.size());
}
string replace(const string& s, const string& what, const string& with) {
    Split parts(s, what);
    return parts.join(with);
}
int hexToInt(const string& s) {
    int res = 0;
	
    for (size_t i = 0; i < s.size(); i++) {
        int multiplier = 1;
        int exp = (s.size() - 1 - i);
        while (exp-- > 0)
            multiplier *= 16;
        int ch = s[i];
        if (ch >= '0' && ch <= '9')
            res += multiplier * (ch - '0');
        else if (ch >= 'a' && ch <= 'z')
            res += multiplier * (ch - 'a');
        else if (ch >= 'A' && ch <= 'Z')
            res += multiplier * (ch - 'A');
    }
    return res;
}
int atoi(const string &s) {
    return ::atoi(s.c_str());
}
std::string operator*(int amount, const std::string &s) {
    std::string res;
    for(;amount>0;amount--)
        res += s;
    return res;
}	
std::string operator*(const std::string &s, int amount) {
    return amount * s;
}
string escapeSQL(const string &str)
{ 
    string tmp;
    if (str == "NULL")
        return "NULL";
    
    tmp = replace(str, "'NULL'", "NULL");
    return "'" + replace(tmp, "'", "''") + "'";
} 
}



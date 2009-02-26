/* LiteSQL - string utilities' implementation
 * 
 * By Tero Laitinen 
 * 
 * See LICENSE for copyright information. */

#include "compatibility.hpp"
#include "litesql/utils.hpp"
#include "litesql/field.hpp"
#include <ctype.h>
#include <cstdlib>

namespace litesql {
using namespace std;    
bool startsWith(string what, string with) {
    if (what.size() < with.size())
        return false;
    if (what.substr(0, with.size()) == with)
        return true;
    return false;
}
bool endsWith(string what, string with) {
    if (what.size() < with.size())
        return false;
    if (what.substr(what.size()-with.size(), what.size()) == with)
        return true;
    return false;
}

string toLower(string s) {
    for (unsigned int i = 0; i < s.size(); i++)
        s[i] = tolower(s[i]);
    return s;	
}
string toUpper(string s) {
    for (unsigned int i = 0; i < s.size(); i++)
        s[i] = toupper(s[i]);
    return s;	
}
string rstrip(string s) {
    if (s.size() == 0)
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
string lstrip(string s) {
    unsigned int pos = 0;
    while (1) {
        if (isspace(s[pos]) && pos < s.size()-1)
            pos++;
        else
            break;
    }
    return s.substr(pos, s.size());
}
string replace(string s, string what, string with) {
    Split parts(s, what);
    return parts.join(with);
}
int hexToInt(string s) {
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



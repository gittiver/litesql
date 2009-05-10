/* LiteSQL - Split implementation
 * 
 * By Tero Laitinen 
 * 
 * See LICENSE for copyright information. */
#include "compatibility.hpp"
#include "litesql/split.hpp"
#include <string>
#include <string.h>
#include <cstring>
#include <stdlib.h>

namespace litesql {    
using namespace std;
Split::Split(const string& s, const string& delim) {
    char * buf = strdup((char*) s.c_str());
    char * ptr = buf;
    int len = delim.size();
    vector<char*> pointers;
    pointers.push_back(ptr);
    while((ptr = strstr(ptr, delim.c_str()))) {
        *ptr = '\0';
        ptr += len;
        pointers.push_back(ptr);
    }
    for (vector<char*>::iterator i = pointers.begin();
         i != pointers.end();
         ++i)
        push_back(string(*i));

    free(buf);
}
Split Split::slice(int start, int end) const {
    vector<string> data;
    if (start < 0)
        start = start+size();
    if (end < 0)
        end = end+size();
    if (start >= static_cast<int>(size()))
        start = size()-1;
    if (end >= static_cast<int>(size()))
        end = size()-1;
    if (start >= end)
        return data;
    for (int i = start; i < end; i++)
        data.push_back(this->operator[](i));
    return data;
}
string Split::join(const string& delim) const {
    string res;
    for (const_iterator i = begin(); i != end(); i++) {
        if (i != begin())
            res += delim;
        res += *i;
    }
    return res;
}
Split & Split::extend(const Split & s) {
    for (size_t i = 0; i < s.size(); i++)
        push_back(s[i]);
    return *this;
}
}


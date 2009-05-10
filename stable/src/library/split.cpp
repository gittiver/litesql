/* LiteSQL - Split implementation
 * 
 * The list of contributors at http://litesql.sf.net/ 
 * 
 * See LICENSE for copyright information. */
#include "compatibility.hpp"
#include "litesql/split.hpp"
#include <string>
#include <string.h>
#include <cstring>
#include <cstdlib>

namespace litesql {    
using namespace std;
Split::Split(const string& s, const string& delim) {
    string::const_iterator i;
    string::const_iterator m = delim.begin();
    string::const_iterator b = s.begin();

    if (delim.empty()) {
        push_back(s);
        return;
    }


    for (i = s.begin(); i != s.end(); i++) {
        if (*m == *i) {
            m++;

            if (m == delim.end()) {
                string ne;
                ne.resize(i - delim.size() - b + 1);
                copy(b, i - delim.size() + 1, ne.begin());

                push_back(ne);
                m = delim.begin();
                b = i + 1;
            }

        } else 
            m = delim.begin();

    }
    string ne;
    ne.resize(s.end() - b);
    copy(b, s.end(), ne.begin());
    push_back(ne);
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


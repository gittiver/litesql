#ifndef _litesql_pythongenerator_hpp
#define _litesql_pythongenerator_hpp
#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include "litesql/string.hpp"
namespace gen {
using namespace litesql;
using namespace std;

class Block {
    vector<string> body;
    int indent;
public:    
    Block() : indent(0) {}
    Block& operator()(const string& s) {
        string i = indent * string(" ");
        
        body.push_back(i + replace(s, "\n", "\n" + i) + "\n");
        return *this;
    }
    Block& operator++(int) {
        indent += 2;
        return *this;
    }
    Block& operator--(int) {
        indent -= 2;
        return *this;
    }
    void write(FILE* f) {
        for (size_t i = 0; i < body.size(); i++) 
            fprintf(f, "%s", body[i].c_str());
    }
};

}
#endif

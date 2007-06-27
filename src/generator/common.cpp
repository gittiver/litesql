#include "common.hpp"
#include <fstream>
using namespace std;
string quote(const string& s) {
    return "\"" + s + "\"";
}
string brackets(const string& s) {
    return "(" + s + ")";
}
string sqbrackets(const string& s) {
    return "[" + s + "]";
}
string braces(const string& s) {
    return "{" + s + "}";
}
string readFile(const string& fName) {
    ifstream f(fName.c_str(), ifstream::in);


    string contents;
    char buf[16834];

    while (f.rdstate() & (ifstream::failbit 
                          | ifstream::eofbit 
                          | ifstream::badbit) == 0) {
        f.read(buf, 16834);
        contents += string(buf, f.gcount());
    }
    f.close();
    return contents;
}


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
    char buf[1024];

    while (!f.eof()) {
        f.read(buf, 1024);
        contents += string(buf, f.gcount());
    }
    f.close();
    return contents;
}


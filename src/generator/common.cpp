#include "common.hpp"
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

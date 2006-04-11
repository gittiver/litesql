/* LiteSQL - Syntactic sugar for Expr-API
 * 
 * By Tero Laitinen 
 * 
 * See LICENSE for copyright information. */
#include "compatibility.hpp"
#include "litesql.hpp"
namespace litesql {
using namespace std;
In FieldType::in(const string& set) const {
    return In(*this, set);
}
In FieldType::in(const SelectQuery& sel) const {
    return In(*this, sel);
}
Like FieldType::like(const string& s) {
    return Like(*this, s);
}
template <> 
string convert<int, string>(int value) {
    return toString(value);
}
template <>
float convert<int, float>(int value) {
    return value;
}
template <>
string convert<float, string>(float value) {
    return toString(value);
}
template <> 
string convert<const int&, string>(const int& value) {
    return toString(value);
}
template <>
string convert<const float&, string>(const float& value) {
    return toString(value);
}
template <>
string convert<const bool&, string>(const bool& value) {
    return toString(value);
}

template <>
int convert<const string&, int>(const string& value) {
    return strtol(value.c_str(), NULL, 10);
}
template <>
bool convert<int, bool>(int value) {
    return value;
}
template <>
bool convert<const string&, bool>(const string& value) {
    return convert<const string&, int>(value);
}
template <>
long long convert<const string&, long long>(const string& value) {
    return strtoll(value.c_str(), NULL, 10);
}
template <>
float convert<const string&, float>(const string& value) {
    return strtof(value.c_str(), NULL);
}
template <>
string convert<const string&, string>(const string& value) {
    return value;
}
}

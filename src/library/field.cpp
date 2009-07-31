/* LiteSQL - Syntactic sugar for Expr-API
 * 
 * The list of contributors at http://litesql.sf.net/ 
 * 
 * See LICENSE for copyright information. */
#include <cstdlib>
#include "compatibility.hpp"
#include "litesql.hpp"
#include "litesql/field.hpp"

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
    return value!=0;
}
template <> 
float convert<int, float>(int value) {
    return (float)value;
}
template <> 
float convert<double, float>(double value) {
    return (float)value;
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



const char hexDigits[] = "0123456789abcdef";

string Blob::toHex(void) const
{
  string result;
  if (!m_data) 
  {
    result ="NULL";  
  }
  else
  {
    result.reserve(m_length);
    for (size_t i = 0; i < m_length;i++)
    {
      char c = m_data[i];
      char n1 = m_data[i]&0x0f;
      char n2 = (m_data[i]&0xf0) >>4;

      result.push_back( hexDigits[(m_data[i]&0xf0) >>4]);
      result.push_back( hexDigits[m_data[i]&0x0f]);
    }
  }
  return result;
}


ostream& operator << (ostream& os, const Blob& blob)
{
  return os << convert<const Blob&, string>(blob);
}


template <>
Blob convert<const string&, Blob>(const string& value) 
{
  return Blob(value);
}

template <>
string convert<const Blob&, string>(const Blob& value)
{
  if (value.isNull())
  {
    return "NULL";
  }
  else 
  {
    string hexVal;
    hexVal.append("X'");
    hexVal.append(value.toHex());
    hexVal.append("'");
    return hexVal;
  }
}


}

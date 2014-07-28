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
Like FieldType::like(const string& s) const {
    return Like(*this, s);
}

template <> 
string convert<int, string>(int value) {
    return toString(value);
}

template <> 
string convert<bigint, string>(bigint value) {
    return toString(value);
}

template <>
string convert<float, string>(float value) {
    return toString(value);
}

template <>
string convert<double, string>(double value) {
    return toString(value);
}

template <> 
string convert<const int&, string>(const int& value) {
    return toString(value);
}

template <> 
string convert<const bigint&, string>(const bigint& value) {
    return toString(value);
}

template <>
string convert<const float&, string>(const float& value) {
    return toString(value);
}

template <>
string convert<const double&, string>(const double& value) {
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
bigint convert<const string&, bigint>(const string& value) {
    return strtoll(value.c_str(), NULL, 10);
}

template <>
bool convert<int, bool>(int value) {
    return value!=0;
}

template <> 
bigint convert<int, bigint>(int value) {
    return (bigint)value;
}

template <> 
float convert<int, float>(int value) {
    return (float)value; 
}

template <> 
double convert<int,double>(int value) {
    return (double)value;
}

template <>
bool convert<const string&, bool>(const string& value) {
    return convert<const string&, int>(value);
}

template <>
float convert<const string&, float>(const string& value) {
    return strtof(value.c_str(), NULL);
}

template <>
double convert<const string&, double>(const string& value) {
    return strtod(value.c_str(), NULL);
}

template <>
string convert<const string&, string>(const string& value) {
    return value;
}

const char hexDigits[] = "0123456789ABCDEF";

string Blob::toHex(void) const
{
	return toHex(m_data,m_length);
}

string Blob::toHex(const u8_t *data, size_t length)
{
  string result;
  if (!data) 
  {
    result ="NULL";  
  }
  else
  {
    result.reserve(2*length);
    for (size_t i = 0; i < length;i++)
    {
      result.push_back( hexDigits[(data[i]&0xf0) >>4]);
      result.push_back( hexDigits[data[i]&0x0f]);
    }
  }
  return result;
}

u8_t hex(char c)
{
  switch(c)
  {
    case '0':
      return 0;
    case '1':
      return 1;
    case '2':
      return 2;
    case '3':
      return 3;
    case '4':
      return 4;
    case '5':
      return 5;
    case '6':
      return 6;
    case '7':
      return 7;
    case '8':
      return 8;
    case '9':
      return 9;
    case 'a':
    case 'A':
      return 0xa;
    case 'b':
    case 'B':
      return 0xb;
    case 'c':
    case 'C':
      return 0xc;
    case 'd':
    case 'D':
      return 0xd;
    case 'e':
    case 'E':
      return 0xe;
    case 'f':
    case 'F':
      return 0xf;
    default:
      throw("invalid digit");
  }
}

Blob::~Blob()
{ 
  if (m_data!=NULL) 
    free(m_data); 
}

void Blob::initWithHexString(const string& hexString)
{
  if ("NULL"==hexString)
  {
    m_data = NULL;
    m_length = 0;
  }
  else
  {
    m_length = (hexString.size()+1)/2;
    m_data = (u8_t*) malloc(m_length);
    for (size_t i = 0; i < hexString.size();i+=2)
    {
      m_data[i/2] = (hex(hexString[i])<<4) | hex(hexString[i+1]);
    }
  }
}

void   Blob::data(const char* pszData) 
{
  if (m_data!=NULL)
  {
    free(m_data);
  }
  initWithData( (u8_t*)pszData,
		  pszData!=NULL ? strlen(pszData)+1 : 0 );
}

void Blob::getData(u8_t* pBuffer,size_t& length, size_t offset)
{
    if (!pBuffer)
    {
        length=0;
    }
    else if ( offset>m_length )
    {
        length = 0;
    }
    else 
    {
        length = min(length,m_length-offset);
        memcpy(pBuffer,m_data+offset,length);
    }
}

void Blob::initWithData(const u8_t* data, size_t length)
{
  if (data!=m_data)
  {
    if (m_data!=NULL)
    {
      free(m_data);
    }

    if (data==NULL)
    {
      m_data = NULL;
      m_length = 0;
    }
    else
    {
      m_data = (u8_t*)malloc(length); 
      memcpy(m_data,data,length);
      m_length = length;
    }
  }
}

ostream& operator << (ostream& os, const Blob& blob)
{
  return os << convert<const Blob&, string>(blob);
}


template <>
Blob convert<const string&, Blob>(const string& value) 
{
  if ("NULL"==value)
    return Blob(NULL,0);
  else
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

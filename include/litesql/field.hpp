/* LiteSQL 
 * 
 * The list of contributors at http://litesql.sf.net/ 
 * 
 * See LICENSE for copyright information. */

#ifndef litesql_field_hpp
#define litesql_field_hpp
#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include "litesql/string.hpp"
#include "litesql/commontypes.h"

/** \file field.hpp
    contains FieldType- and Field-classes */
namespace litesql {

/** holds field name, type and table information */
class In;
class Like;
class SelectQuery;

class FieldType {
protected:
    typedef std::vector< std::pair<std::string, std::string> > Values;
public:
    FieldType() {};
    FieldType(const std::string& n, 
              AT_field_type t, 
              const std::string& tbl,
              const Values& vals = Values())
        : _name(n), _table(tbl), _type(t), _values(vals) {}
    std::string fullName() const { return  table() + "." + name(); }
    std::string name() const { return _name; }
    AT_field_type type() const { return _type; }
    std::string table() const { return _table; }
    std::vector< std::pair<std::string, std::string> > values() { return _values; }
    /** syntactic sugar to Expr-API, Object::field_.in(set) */
    In in(const std::string& set) const;
    /** syntactic sugar to Expr-API, Object::field_.in(sel) */
    In in(const SelectQuery& sel) const;
    /** syntactic sugar to Expr-API, Object::field_.like(s) */
    Like like(const std::string& s) const;
    bool operator==(const FieldType & fd) const {
        return fd.fullName() == fullName();
    }
    bool operator!=(const FieldType & fd) const {
        return ! (*this == fd);
    }
private:
    std::string _name, _table;
    AT_field_type _type;
    Values _values;
};

/** convert function */
template <class From, class To>
To convert(From value);

/** store function */
template <class T>
std::string store(const T& value) {
    return litesql::toString(value);
//  return convert<T,std::string>(value);
}

template <class T>
T load(const std::string& value) {
    return convert<const std::string&, T>(value);
}
/** holds field value */
template <class T>
class Field {
    const FieldType * field; 
    bool _modified;
    T _value;
public:
    Field(const FieldType & f) : field(&f), _modified(true) {}
    std::string fullName() const { return field->fullName(); }   
    std::string name() const { return field->name(); }
    AT_field_type type() const { return field->type(); }
    std::string table() const { return field->table(); }
    T value() const { return _value; }
    const FieldType & fieldType() const { return *field; } 
    bool modified() const { return _modified; }
    void setModified(bool state) { _modified = state; }
    const Field & operator=(const std::string& v) { 
        _value = convert<const std::string&, T>(v);
        _modified = true;
        return *this;
    }
    const Field & operator=(const T& v) {
        _value = v;
        _modified = true;
        return *this;
    }
    template <class T2>
    const Field & operator=(T2 v) { 
        _modified = true;
        _value = litesql::convert<T2, T>(v); 
        return *this;
    }
    template <class T2>
    bool operator==(const T2& v) const {
        return litesql::toString(_value) == litesql::toString(v);
    }
	template <class T2>
	bool operator!=(const T2& v) const { return !(*this == v); }

    operator std::string() const { return toString(value()); }

    operator T() const { return value(); }
};

template <>
class Field<std::string> {
    const FieldType * field; 
    bool _modified;
    std::string _value;
public:
    Field(const FieldType & f) : field(&f), _modified(true) {}
    std::string fullName() const { return field->fullName(); }   
    std::string name() const { return field->name(); }
    AT_field_type type() const { return field->type(); }
    std::string table() const { return field->table(); }
    std::string value() const { return _value; }
    const FieldType & fieldType() const { return *field; } 
    bool modified() const { return _modified; }
    void setModified(bool state) { _modified = state; }
    const Field & operator=(std::string v) { 
        _value = v;
        _modified = true;
        return *this;
    }
    const Field& operator=(const char * v) {
        _value = v;
        _modified = true;
        return *this;
    }
    template <class T2>
    const Field & operator=(T2 v) { 
        _modified = true;
        _value = litesql::convert<T2, std::string>(v); 
        return *this;
    }
    template <class T2>
    bool operator==(const T2& v) const {
        return litesql::toString(_value) == litesql::toString(v);
    }
    template <class T2>
    bool operator!=(const T2& v) const { return !(*this == v); }

    operator std::string() const { return value(); }
};

typedef unsigned char u8_t;
typedef long long bigint;

class Blob {
public:
  Blob()                            : m_data(NULL),m_length(0)               {};
  Blob(const std::string & value) : m_data(NULL),m_length(0)
  {
    initWithHexString(value);
  };


  Blob(const Blob & b) : m_data(NULL)
  { 
    initWithData(b.m_data,b.m_length);
  };

  Blob(const void* data, size_t length) : m_data(NULL), m_length(0)
  {
    initWithData((u8_t*)data,length);
  };
  
  virtual ~Blob();
  const Blob& operator=(const Blob& v) {
    initWithData(v.m_data,v.m_length);
    return *this;
  }

  static std::string toHex(const u8_t* data, size_t length);
  std::string toHex()            const ;
  size_t length()           const  { return m_length;      };
  bool   isNull()           const  { return m_data==NULL;  }; 
  u8_t   data(size_t index) const  { return m_data[index]; };
  void   data(const char* pszData);
  void	 getData(u8_t* pData,size_t &length, size_t offset=0);

private:
  u8_t* m_data;
  size_t m_length;

  void initWithData(const u8_t* data, size_t length);
  void initWithHexString(const std::string& hexString);
};

std::ostream& operator << (std::ostream& os, const Blob& blob);
template <>
Blob convert<const std::string&, Blob>(const std::string& value);
template <>
std::string convert<const Blob&, std::string>(const Blob& value);

template <>
class Field<Blob> {
    const FieldType * field; 
    bool _modified;
    Blob _value;
public:
    Field(const FieldType & f) : field(&f), _modified(true) {}
    std::string fullName() const { return field->fullName(); }   
    std::string name() const { return field->name(); }
//    std::string type() const { return field->type(); }
    std::string table() const { return field->table(); }
    Blob value() const { return _value; }
    const FieldType & fieldType() const { return *field; } 
    bool modified() const { return _modified; }
    void setModified(bool state) { _modified = state; }
    const Field & operator=(const Blob& v) { 
        _value = v;
        _modified = true;
        return *this;
    }

/*
const Field& operator=(const char * v) {
        _value = v;
        _modified = true;
        return *this;
    }
    template <class T2>
    const Field & operator=(T2 v) { 
        _modified = true;
        _value = litesql::convert<T2, Blob>(v); 
        return *this;
    }
    template <class T2>
    bool operator==(const T2& v) const {
        return litesql::toString(_value) == litesql::toString(v);
    }
    template <class T2>
    bool operator!=(const T2& v) const { return !(*this == v); }
*/

    operator std::string() const { return _value.toHex(); }
};

template <class T>
std::string operator+(std::string a, litesql::Field<T> f) {
    return a + std::string(f);
}
template <class T>
std::string operator+(litesql::Field<T> f, std::string a) {
    return std::string(f) + a;    
}
template <class T>
std::ostream & operator << (std::ostream & os, const litesql::Field<T> & f) {
    return os << f.value();
}

}
#endif

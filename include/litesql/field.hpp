/* LiteSQL 
 * 
 * By Tero Laitinen 
 * 
 * See LICENSE for copyright information. */

#ifndef __litesql_field_hpp
#define __litesql_field_hpp
#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include "litesql/string.hpp"
#include "litesql/fieldtype.hpp"

/** \file field.hpp
    contains FieldType- and Field-classes */
namespace litesql {
using namespace std;



/** convert function */
template <class From, class To>
To convert(From value);

/** store function */
template <class T>
std::string store(const T& value) {
    return litesql::toString(value);
}

template <class T>
T load(const std::string& value) {
    return convert<const std::string&, T>(value);
}
/** holds field value */
template <class T>
class Field {
    const FieldType * field; 
    bool _modified; // TODO: int flags
    T _value;
public:
    Field(const FieldType & f) : field(&f), _modified(true) {}
    string fullName() const { return field->fullName(); }   
    string name() const { return field->name(); }
    string type() const { return field->type(); }
    string table() const { return field->table(); }
    T value() const { return _value; }
    const FieldType & fieldType() const { return *field; } 
    bool modified() const { return _modified; }
    void setModified(bool state) { _modified = state; }
    const Field & operator=(const string& v) { 
        _value = convert<const string&, T>(v);
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
    

    operator string() const { return toString(value()); }

    operator T() const { return value(); }

    bool isNULL() const { return false; }
    void setNULL() {}
};

template <>
class Field<string> {
    const FieldType * field; 
    bool _modified;
    string _value;
public:
    Field(const FieldType & f) : field(&f), _modified(true) {}
    string fullName() const { return field->fullName(); }   
    string name() const { return field->name(); }
    db::Type* type() const { return field->type(); }
    db::Table* table() const { return field->table(); }
    string value() const { return _value; }
    const FieldType & fieldType() const { return *field; } 
    bool modified() const { return _modified; }
    void setModified(bool state) { _modified = state; }
    const Field & operator=(string v) { 
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
        _value = litesql::convert<T2, string>(v); 
        return *this;
    }
    template <class T2>
    bool operator==(const T2& v) const {
        return litesql::toString(_value) == litesql::toString(v);
    }
    template <class T2>
    bool operator!=(const T2& v) const { return !(*this == v); }

    operator string() const { return value(); }
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

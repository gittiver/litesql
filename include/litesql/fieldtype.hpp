/* LiteSQL 
 * 
 * By Tero Laitinen 
 * 
 * See LICENSE for copyright information. */

#ifndef _litesql_fieldtype_hpp
#define _litesql_fieldtype_hpp

#include "litesql/dbtypes.hpp"

#include <string>
#include <vector>
namespace litesql {

    class Value {

        std::string name_, value_;
    public:
        Value(const std::string& n,
              const std::string& v)
            : name_(n), value_(v) {}
        
        std::string name() const { return name_; }
        std::string value() const { return value_; }

    };

    /** holds field name, type and table information */
    class In;
    class Like;
    class SelectQuery;

    class FieldType {
        db::Field* field_;

    public:
        typedef std::vector< Value > Values;
    private:
        Values values_;
    public:

        FieldType(db::Field* fld,
                  const Values& vals = Values())
            : field_(fld), values_(vals) {}

        std::string fullName() const { return  table()->name() + "." + name(); }

        std::string name() const { return field()->name(); }
        db::Field* field() const { return field_; }
        db::Type* type() const { return field()->type(); }
        db::Table* table() const { return field()->table(); }
        Values values() const { return values_; }

        /** syntactic sugar to Expr-API, Object::field_.in(set) */
        In in(const std::string& set) const;
        /** syntactic sugar to Expr-API, Object::field_.in(sel) */
        In in(const SelectQuery& sel) const;
        /** syntactic sugar to Expr-API, Object::field_.like(s) */
        Like like(const std::string& s);

        bool operator==(const FieldType & fd) const {
            return fd.fullName() == fullName();
        }
        bool operator!=(const FieldType & fd) const {
            return ! (*this == fd);
        }
    };
}
#endif

/* LiteSQL - Expr-API implementation
 * 
 * By Tero Laitinen 
 * 
 * See LICENSE for copyright information. */

#include "compatibility.hpp"
#include "litesql/expr.hpp"
#include "litesql/selectquery.hpp"
namespace litesql {
In::In(const FieldType & fld, const SelectQuery& s)
    : Oper(fld, "in", "(" + s.asString() + ")") {}
And operator&&(const Expr &o1, const Expr &o2) {
    return And(o1,o2);
}
Or operator||(const Expr &o1, const Expr &o2) {
    return Or(o1,o2);
}
Not operator!(const Expr & exp) {
    return Not(exp);
}
}

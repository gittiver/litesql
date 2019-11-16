/* LiteSQL - SelectQuery implementation
 * 
 * The list of contributors at http://litesql.sf.net/ 
 * 
 * See LICENSE for copyright information. */
#include "compatibility.hpp"
#include "litesql/selectquery.hpp"

using std::string;

using namespace litesql;
SelectQuery & SelectQuery::distinct(bool d) { 
    _distinct = d; 
    return *this;
}
SelectQuery & SelectQuery::limit(int value) { 
    _limit = value;	
    return *this;
}
SelectQuery & SelectQuery::offset(int value) { 
    _offset = value; 
    return *this;
}
SelectQuery & SelectQuery::result(const std::string& r) { 
    _results.push_back(r); 
    return *this;
}
SelectQuery & SelectQuery::clearResults() {
    _results.clear();
    return *this;
}
SelectQuery & SelectQuery::source(const std::string& s, const std::string& alias) {
    string a(s);
    if (!alias.empty())
        a += " AS " + alias;
    _sources.push_back(a);
    return *this;
}
SelectQuery & SelectQuery::where(const Expr & w) { 
    _where = (RawExpr(_where) && w).asString();	
    return *this;
}
SelectQuery & SelectQuery::where(const std::string& w) {
    _where = (RawExpr(_where) && RawExpr(w)).asString();
    return *this;
}
SelectQuery & SelectQuery::groupBy(const std::string& gb) {
    _groupBy.push_back(gb);	
    return *this;
}
SelectQuery & SelectQuery::having(const Expr & h) { 
    _having = h.asString(); 
    return *this;
}
SelectQuery & SelectQuery::having(const std::string& h) {
    _having = h;
    return *this;
}
SelectQuery & SelectQuery::orderBy(const std::string& ob, bool ascending) {
    std::string value = ob;
    if (!ascending)
        value += " DESC";
    _orderBy.push_back(value); 
    return *this;
}
SelectQuery::operator string() const {
    std::string res = "SELECT ";
    if (_distinct)
        res += "DISTINCT ";
    res += join(_results,",");
    res += " FROM ";
    res += join(_sources,",");
    if (_where != "True")
        res += " WHERE " + _where;
    if (_groupBy.size() > 0)
        res += " GROUP BY " + join(_groupBy,",");
    if (!_having.empty())
        res += " HAVING " + _having;
    if (_orderBy.size() > 0)
        res += " ORDER BY " + join(_orderBy,",");
    if (_limit) 
        res += " LIMIT " + toString(_limit);
    if (_offset) 
            res += " OFFSET " + toString(_offset);
    return res;
}

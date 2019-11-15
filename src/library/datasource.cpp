/* LiteSQL - selectObjectQuery - implementation
 * 
 * The list of contributors at http://litesql.sf.net/ 
 * 
 * See LICENSE for copyright information. */

#include <set>

//#include "compatibility.hpp"
#include "litesql/datasource.hpp"
namespace litesql {
using std::string;
using std::vector;
using std::set;

SelectQuery selectObjectQuery(const vector<FieldType>& fdatas,
                              const Expr& e) {
    SelectQuery sel;
    vector<string> tables;
    set<string> tableSet;

    for (size_t i = 0; i < fdatas.size(); i++)
        if (tableSet.find(fdatas[i].table()) == tableSet.end()) {
            tables.push_back(fdatas[i].table());
            tableSet.insert(fdatas[i].table());
        }

    vector<string> tableFilters;
    tableFilters.resize(tables.size()-1);
    for (size_t i = 1; i < tables.size(); i++)
        tableFilters[i-1] = tables[i-1] + ".id_ = " + tables[i] + ".id_";
    tableSet.clear();
    for (size_t i = 0; i < tables.size(); i++) {
        sel.source(tables[i]);
        tableSet.insert(tables[i]);
    }
    if (tables.size() > 1)
      sel.where((e && RawExpr(join(tableFilters," AND "))).asString());
    else
        sel.where(e.asString());
    
    for (size_t i = 0; i < fdatas.size(); i++)
        sel.result(fdatas[i].table() + "." + fdatas[i].name());

    return sel;
}
}

/* LiteSQL - Common routines for backends
 * 
 * By Tero Laitinen 
 * 
 * See LICENSE for copyright information. */

#include "compatibility.hpp"
#include "litesql/backend.hpp"
#include "litesql/string.hpp"
#include "litesql/types.hpp"
namespace litesql {
using namespace std;    
string Backend::groupInsert(Record tables, Records fields, Records values,
                   string sequence) const {
    string id = values[0][0];
    
    if (supportsSequences() && values[0][0] == "NULL")
        id = execute("SELECT nextval('" + sequence + "');")->records()[0][0];
    for (size_t i = 0; i < tables.size(); i++) {
        string fieldString = Split(fields[i]).join(",");
        string valueString;
        Split valueSplit(values[i]);
        for (size_t i2 = 0; i2 < valueSplit.size(); i2++)
            valueSplit[i2] = escapeSQL(valueSplit[i2]);
        valueString = valueSplit.join(",");
        string query = "INSERT INTO " + tables[i] + " (" + fieldString
            + ") VALUES (" + valueString + ");";
        execute(query);
        if (!supportsSequences() && id == "NULL")
            id = getInsertID();
    }
    return id;
}
}

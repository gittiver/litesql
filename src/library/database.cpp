/* LiteSQL - Database implementation
 * 
 * By Tero Laitinen 
 * 
 * See LICENSE for copyright information. */

#include "compatibility.hpp"
#include "litesql/database.hpp"
#include "litesql/cursor.hpp"
#include "litesql/except.hpp"
#include "litesql/selectquery.hpp"
#include <map>
namespace litesql {
using namespace std;

void Database::openDatabase() {
   backend = Backend::getBackend(backendType,connInfo);
   if (backend==NULL)
      throw DatabaseError("Unknown backend: " + backendType);
}

void Database::storeSchemaItem(const SchemaItem& s) const {
    delete_("schema_", 
            RawExpr("name_='" + s.name 
                + "' and type_='" + s.type + "'"));
    Record values;
    values.push_back(s.name);
    values.push_back(s.type);
    values.push_back(s.sql);
    insert("schema_", values);
}

vector<Database::SchemaItem> Database::getCurrentSchema() const {
    SelectQuery sel;
    sel.result("name_").result("type_").result("sql_").source("schema_");
    vector<SchemaItem> s;
    Records recs;
    try {
        recs = query(sel);
    } catch (Except e) {
        return vector<Database::SchemaItem>();
    }
    for (size_t i = 0; i < recs.size(); i++) {
        s.push_back(SchemaItem(recs[i][0], recs[i][1], recs[i][2]));
    }
    return s;
}
static Split getFields(string schema) {
    Split s;
    int start = schema.find("(");
    int end = schema.find(")");
    if (start == -1 || end == -1)
        return s;
    Split tmp(replace(schema.substr(start+1, end), ", ", ","), ",");
    for (size_t i = 0; i < tmp.size(); i++)
        s.push_back(Split(tmp[i])[0]);
    return s;
}
void Database::upgradeTable(string name, 
                            string oldSchema, string newSchema) const {
    Split oldFields = getFields(oldSchema);
    Split newFields = getFields(newSchema);
    SelectQuery sel;
    sel.source(name);
    for (size_t i = 0; i < oldFields.size(); i++)
        sel.result(name + "." + oldFields[i]);
    Split q(oldSchema);
    q[2] += "_old";
    query(q.join(" "));
    for (Cursor<Record> cur(cursor<Record>(sel)); cur.rowsLeft(); cur++) 
        insert(name + "_old", *cur);
    query("DROP TABLE " + name);
    query(newSchema);

    sel = SelectQuery();
    sel.source(name+"_old");
    for (size_t i = 0; i < oldFields.size(); i++)
        sel.result(name + "_old." + oldFields[i]);
    vector<pair<int, int> > transFields;
    for (size_t i = 0; i < oldFields.size(); i++) {
        for (size_t i2 = 0; i2 < newFields.size(); i2++) 
            if (oldFields[i] == newFields[i2]) {
                transFields.push_back(make_pair(i, i2));             
                break;  
            }
    }
    for (Cursor<Record> cur(cursor<Record>(sel)); cur.rowsLeft(); cur++) {
        Record fields;
        Record values;
        Record data = *cur;
        for (size_t i = 0; i < transFields.size(); i++) {
            fields.push_back(newFields[transFields[i].second]);
            values.push_back(data[transFields[i].first]);
        }
        insert(name, values, fields);
    }
    
    
    query("DROP TABLE " + name + "_old");
}
    Database::Database(string backend, string conn) 
     : backendType(backend), connInfo(conn), backend(NULL), verbose(false) {
        openDatabase();
    }
    Database::Database(const Database &op) 
      : backendType(op.backendType), connInfo(op.connInfo), 
        verbose(op.verbose) {
        openDatabase();
    }
    Database::~Database() {
        delete backend;
    }
    void Database::create() const { 
        vector<SchemaItem> s = getSchema(); 
        begin();
        for (size_t i = 0; i < s.size(); i++) {
            query(s[i].sql);
            storeSchemaItem(s[i]);
        }
        commit();
    } 
    void Database::drop() const { 
        vector<SchemaItem> s = getSchema(); 

        for (size_t i = 0; i < s.size(); i++) {
            try {
                begin();
                if (s[i].type == "table")
                    query("DROP TABLE " + s[i].name);
                else if (s[i].type == "sequence")
                    query("DROP SEQUENCE " + s[i].name);
                commit();
            } catch (Except e) {
                rollback();
            }
        }
    }  
   
    bool Database::needsUpgrade() const {
        vector<SchemaItem> cs = getCurrentSchema();
        vector<SchemaItem> s = getSchema();
        map<string, int> items;
        for (size_t i = 0; i < cs.size(); i++) 
            items[cs[i].name] = i;
    
        for (size_t i = 0; i < s.size(); i++) {
            if (items.find(s[i].name) == items.end() 
                    || cs[items[s[i].name]].sql != s[i].sql) 
                return true;
        }
        return false;
    }
    void Database::upgrade() const {
        vector<SchemaItem> cs = getCurrentSchema();
        vector<SchemaItem> s = getSchema();
        map<string, int> items;
        for (size_t i = 0; i < cs.size(); i++) {
            items[cs[i].name] = i;
        }
        begin();
        for (size_t i = 0; i < s.size(); i++) {
            if (items.find(s[i].name) == items.end()) {
                query(s[i].sql);
                storeSchemaItem(s[i]);
                continue;
            }
            if (s[i].type == "table" && cs[items[s[i].name]].sql != s[i].sql) {
                upgradeTable(s[i].name, cs[items[s[i].name]].sql, s[i].sql);
                storeSchemaItem(s[i]);
            }
        }
        commit();
    }
    Records Database::query(const string &q) const {
        if (verbose)
            cerr << q << endl;
        auto_ptr<Backend::Result> r(backend->execute(q));
        return r->records();
    }

    void Database::insert(const string &table, const Record &r,
                          const Split& fields) const {
        string command = "INSERT INTO " + table;
        if (fields.size())
            command += " (" + fields.join(",") + ")";
        command += " VALUES (";
        unsigned int i;
        for (i=0; i < r.size() -1; i++) {
            command += escapeSQL(r[i]) + ",";
        }
        command += escapeSQL(r[i]) + ")";
        query(command);
    }
    string Database::groupInsert(Record tables, Records fields, Records values, 
                     string sequence) const {
        if (verbose) {
            cerr << "groupInsert" << endl;
            for (size_t i = 0; i < tables.size(); i++) {
                cerr << "\t" << tables[i] << endl;
                cerr << "\t\tfields : " << Split(fields[i]).join(",") << endl;
                cerr << "\t\tvalues : " << Split(values[i]).join("|") << endl;
            }
        }
        return backend->groupInsert(tables, fields, values, sequence);
    }

    void Database::delete_(const string& table, const Expr& e) const {
        string where = "";
        if (e.asString() != "True")
            where = " WHERE " + e.asString();
        query("DELETE FROM " + table + where);
    }

}
    

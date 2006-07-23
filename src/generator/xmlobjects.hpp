#ifndef _litesql_xmlobjects_hpp
#define _litesql_xmlobjects_hpp
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <map>
#include "litesql-gen.hpp"
#include "litesql/except.hpp"
#include "litesql/string.hpp"
#include "litesql/split.hpp"
#include "args.hpp"
extern int yylineno;
namespace xml {
using namespace std;
using namespace litesql;
string capitalize(const string& s);
string decapitalize(const string& s);
string safe(const char *s);
string makeDBName(const string& s);

template <class T> 
vector<T> joinVector(const vector<T>& v1, const vector<T>& v2) {
        vector<T> res;
        res.reserve(v1.size() + v2.size());
        copy(v1.begin(), v1.end(), back_inserter(res));
        copy(v2.begin(), v2.end(), back_inserter(res));
        return res;
    }

template <class T> 
void addVector(const vector<T>& from, vector<T>& to) {
        to.reserve(to.size() + from.size());
        copy(from.begin(), from.end(), back_inserter(to));
    }
    

class Position {
public:
    string file;
    int line;
    Position(const string& f, int l) : file(f), line(l) {}
};
class XMLExcept : public Except { 
public:
    XMLExcept(const Position& p, const string& m) 
      : Except("In file " + p.file + ", line " + toString(p.line) + " : " 
              + m) {}
};
class XML {
public:
    Position pos;
    XML(const Position& p) : pos(p) {}
};
class Value : public XML {
public:
    string name, value;
    Value(const Position& p, string n, string v) : XML(p), name(n), value(v) {}
};
class Type : public XML {
public:
    string name, class_, sqlType;
    vector<Value> values;
    Type(string n, string c, string st) : XML(Position("", 0)),
        name(n), class_(c), sqlType(st) {}
    Type(const Position& p, string n, string c, string st)
       : XML(p), name(n), class_(c), sqlType(st) {}
};
class IndexField : public XML {
public:
    string name;
    IndexField(const Position& p, string n) : XML(p), name(n) {}
};
class Index : public XML {
public:
    vector<IndexField> fields;
    AT_index_unique unique;
    Index(const Position& p, AT_index_unique u) : XML(p), unique(u) {}

    bool isUnique() const {
        return unique == A_index_unique_true;
    }
};
class Field : public XML {
public:
    string name;
    string fieldTypeName;
    string typeName;
    Type* type;
    string default_;
    AT_field_indexed indexed;
    AT_field_unique unique;
    vector<Value> values;
    int offset;
    Field(const Position& p, string n, string t, string d, AT_field_indexed i, AT_field_unique u) 
       : XML(p), name(n), fieldTypeName(capitalize(n)), typeName(t), type(NULL), default_(d), indexed(i), unique(u) {
    }
    void value(const Value& v) {
        values.push_back(v);
    }
    bool isIndexed() const {
        return indexed == A_field_indexed_true;
    }
    bool isUnique() const {
        return unique == A_field_unique_true;
    }
    bool hasQuotedValues() const {
/*       switch(type) {
       case A_field_type_integer:
       case A_field_type_float: 
       case A_field_type_boolean: 
       case A_field_type_date:
       case A_field_type_time:
       case A_field_type_datetime:
           return false;
       case A_field_type_string:
           return true;
       }*/
       return "";
    }
    string getQuotedDefaultValue() const {
        if (hasQuotedValues())
            return "\"" + default_ + "\"";
        if (default_.size() == 0)
            return "0";
        return default_;
    }
    string getSQLType() const {
        if (!type)
            return "";
        return type->sqlType;
    }
    string getClass() const {
        if (!type)
            return "";
        return type->class_;
    }

};
class Param : public XML {
public:
    string name;
    AT_param_type type;
    Param(const Position& p, string n, AT_param_type t) 
       : XML(p), name(n), type(t) {}
    
};
class Method : public XML {
public:
    string name, returnType;
    AT_method_const const_;
    vector<Param> params;
    Method(const Position& p, 
         string n, string rt, AT_method_const c)
        : XML(p), name(n), returnType(rt), const_(c) {}
    bool isConst() const {
        return const_ == A_method_const_true;
    }
    void param(const Param& p) {
        params.push_back(p);
    }
};

class Relation;
class Relate;
class Object;
class RelationHandle : public XML {
public:
    string name;
    Relation * relation;
    Relate * relate;
    Object * object;
    vector< pair<Object*,Relate*> > destObjects;

    RelationHandle(const Position& p, 
                   string n, Relation * r, Relate * rel, Object * o) 
        : XML(p), name(n), relation(r), relate(rel), object(o) {}
};
class Relate : public XML {
public:    
    string objectName;
    string fieldTypeName, fieldName;
    string getMethodName;
    int paramPos;
    AT_relate_limit limit;
    AT_relate_unique unique;
    string handle;
    string remoteHandle;
    Object* object;
    Relate(const Position& p, 
           string on, AT_relate_limit l, AT_relate_unique u, string h, string rh) 
        : XML(p), objectName(on), limit(l), unique(u), handle(h), remoteHandle(rh), object(NULL) {
        if (hasLimit() && isUnique())
            throw logic_error("both limit and unique specified in relate: line " + 
                              toString(yylineno));
    }
    bool hasLimit() const {
        return limit == A_relate_limit_one;
    }
    bool isUnique() const {
        return unique == A_relate_unique_true;
    }
    bool operator < (const Relate& r) const {
        return objectName < r.objectName;
    }

};
class Option : public XML {
public:
    string name, value;
    Option(const Position& p, const string& n, const string& v) 
      : XML(p), name(n), value(v) {}
};
class IfBackend : public XML {
public:
    string name;
    vector<Option*> options;
    vector<Type*> types;
    IfBackend(const Position& p, const string& n) 
       : XML(p), name(n) {}
};
class IfTarget : public XML {
public:
    string name;
    vector<Option*> options;
    vector<Type*> types;
    IfTarget(const string& n)
       : XML(Position("", 0)), name(n) {}
    IfTarget(const Position& p, const string& n) 
       : XML(p), name(n) {}
};

class Relation : public XML {
public:
    string id, name;
    string table;
    AT_relation_unidir unidir;
    vector<Relate*> related;
    vector<Field*> fields;
    vector<Index*> indices;
    vector<Option*> options;
    vector<IfBackend*> ifBackends;

    Relation(const Position& p, string i, string n, AT_relation_unidir ud) 
        : XML(p), id(i), name(n), unidir(ud) {}
    string getName() const {
        if (name.size() == 0) {
            string result;
            for (size_t i = 0; i < related.size(); i++) 
                result += related[i]->objectName;
            return result + "Relation" + id;
        }
        return name;
    }
    bool isUnidir() const {
        return unidir == A_relation_unidir_true;
    }
    int sameTypes() const {
        map<string, int> names;
        int max = 0;
        for (size_t i = 0; i < related.size(); i++) {
            if (names.find(related[i]->objectName) == names.end()) 
                names[related[i]->objectName] = 0;
            int value =	++names[related[i]->objectName];
            if (value > max)
                max = value;
        }
        return max;
    }
    int countTypes(string name) const {
        int res = 0;
        for (size_t i = 0; i < related.size(); i++)
            if (related[i]->objectName == name)
                res++;
        return res;
    }
    string getTable() const {
        Split res;
        for (size_t i = 0; i < related.size(); i++)
            res.push_back(related[i]->objectName);
        res.push_back(id);

        return makeDBName(res.join("_"));
    }
};
class Object : public XML {
public:
    string name, inherits;
    bool temporary;

    vector<Field*> fields;
    vector<Method*> methods;
    vector<Index*> indices;
    vector<RelationHandle*> handles;
    map<Relation*, vector<Relate*> > relations;
    vector<Object*> children;
    vector<Option*> options;
    vector<IfBackend*> ifBackends;
    vector<Relate*> related;

    Object* parentObject;

    Object(const Position& p, string n, string i, AT_object_temporary t) 
        : XML(p), name(n), inherits(i), temporary(t == A_object_temporary_true),
        parentObject(NULL) {
        if (i.size() == 0) {
            fields.push_back(new Field(pos, "id", "integer", "", 
                         A_field_indexed_false, A_field_unique_false));
            fields.push_back(new Field(pos, "type", "string", "", 
                        A_field_indexed_false, A_field_unique_false));
        }
    }
    string getInherits(const string& style) const {
        if (!inherits.empty())
            return inherits;
        if (style == "c++")
            return "litesql::Persistent";
        else if (style == "python")
            return "litesql.Persistent";
        
        throw litesql::Except("Unknown style: " + style);
    }

    int getLastFieldOffset() const {
        if (!parentObject)
            return fields.size();
        else return parentObject->getLastFieldOffset() + fields.size();
    }
    void getAllFields(vector<Field*>& flds) const {
        if (parentObject)
            parentObject->getAllFields(flds);
        for (size_t i = 0; i < fields.size(); i++)
            flds.push_back(fields[i]);
    }
    void getChildrenNames(Split & names) const {
        for (size_t i = 0; i < children.size(); i++) {
            names.push_back(children[i]->name);
            children[i]->getChildrenNames(names);
        }
    }
    const Object* getBaseObject() const{
        if (!parentObject)
            return this;
        else
            return parentObject->getBaseObject();
    }
    string getTable() const {
        return makeDBName(name + "_");
    }
    string getSequence() const {
        return makeDBName(name + "_seq");
    }
};
class Database : public XML {
public:
    class Sequence {
    public:
        string name, table;
        string getSQL() {
            return "CREATE SEQUENCE " + name + " START 1 INCREMENT 1";
        }
    };
    class DBField {
    public:
        string name, type, extra;
        bool primaryKey;
        Field* field;
        vector<DBField*> references;
        DBField() : primaryKey(false) {}
        string getSQL(string rowIDType) {
            if (primaryKey)
                type = rowIDType;
            return name + " " + type + extra;
        }
    };
    class DBIndex {
    public:
        string name, table;
        bool unique;
        vector<DBField*> fields;
        DBIndex() : unique(false) {}
        string getSQL() {
            litesql::Split flds;
            for (size_t i = 0; i < fields.size(); i++)
                flds.push_back(fields[i]->name);
            string uniqueS;
            if (unique)
                uniqueS = " UNIQUE";
            return "CREATE" + uniqueS + " INDEX " + name + " ON " + table + " (" + flds.join(",") + ")";
        }
    };
    class Table {
    public:
        string name;
        vector<DBField*> fields;
        string getSQL(string rowIDType) {
            litesql::Split flds;
            for (size_t i = 0; i < fields.size(); i++)
                flds.push_back(fields[i]->getSQL(rowIDType));
            return "CREATE TABLE " + name + " (" + flds.join(",") + ")";
        }

    };
    vector<Sequence*> sequences;
    vector<DBIndex*> indices;
    vector<Table*> tables;

    vector<Object*> objects;
    vector<Relation*> relations;
    vector<Option*> options;
    vector<IfBackend*> ifBackends;
    vector<IfTarget*> ifTargets;
    vector<Type*> types;

    string name, include, nspace;

    Database(const Position& p) : XML(p) {}

};
void initBaseTypes(Database& db);
void init(Database& db, Args& args);


}

#endif

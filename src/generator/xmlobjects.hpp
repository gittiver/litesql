#ifndef litesql_xmlobjects_hpp
#define litesql_xmlobjects_hpp

#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <map>
#include "litesql-gen.hpp"
#include "litesql/split.hpp"
#include "litesql/string.hpp"

#define NO_MEMBER_TEMPLATES
#include "litesql/counted_ptr.hpp"
#undef NO_MEMBER_TEMPLATES

namespace xml {
using namespace std;
using namespace litesql;
string safe(const char *s);
string attribute(const string& name, const string& value);
string endtag(const string& name);
string makeDBName(const string& s);

class Value {
public:
    static const char* TAG;

    string name;
    string value;
    
    Value(const string& n, const string& v) : name(n), value(v) {}
};

class IndexField {
public:
    static const char* TAG;

    string name;
    IndexField(const string& n) : name(n) {}
};

class Index {
public:
  typedef counted_ptr<Index> counted_ptr;
  typedef std::vector<counted_ptr> sequence;

  static const char* TAG;

    std::vector<IndexField> fields;
    AT_index_unique unique;
    
    Index(AT_index_unique u) : unique(u) {}

    bool isUnique() const {
        return unique == A_index_unique_true;
    }
};

class Field {
public:
  typedef counted_ptr<Field> counted_ptr;
  typedef std::vector<counted_ptr> sequence;
  
  static const char* TAG;

    string name;
    string fieldTypeName;
    AT_field_type type;
    string default_;
    AT_field_indexed indexed;
    AT_field_unique unique;
    vector<Value> values;
    Field(const string& n, AT_field_type t, const string& d, AT_field_indexed i, AT_field_unique u) 
        : name(n), fieldTypeName(capitalize(n)), type(t), default_(d), indexed(i), unique(u) {
    }
    void value(const Value& v) {
        values.push_back(v);
    }
    
    bool isEditable() {
        return (name!="id") && (name!="type");  
    }

    bool isIndexed() const {
        return indexed == A_field_indexed_true;
    }
    bool isUnique() const {
        return unique == A_field_unique_true;
    }
    bool hasQuotedValues() const {
       switch(type) {
       case A_field_type_integer:
       case A_field_type_float: 
       case A_field_type_double: 
       case A_field_type_boolean: 
       case A_field_type_date:
       case A_field_type_time:
       case A_field_type_datetime:
       case A_field_type_blob:
          return false;
       
       case A_field_type_string:
       default:
         return true;
       }
    }
    string getQuotedDefaultValue() const {
        if (hasQuotedValues())
            return "\"" + default_ + "\"";
        if (default_.size() == 0)
        {
          switch(type) {
            case A_field_type_float: 
            case A_field_type_double: 
              return "0.0";
            case A_field_type_blob: 
              return "Blob::nil";
            default:
              return "0";
          }
        }
        return default_;
    }
    string getSQLType() const {
       switch(type) {
           case A_field_type_integer: return "INTEGER";
           case A_field_type_string: return "TEXT";
           case A_field_type_float: return "FLOAT";
           case A_field_type_double: return "DOUBLE";
           case A_field_type_boolean: return "INTEGER";
           case A_field_type_date: return "INTEGER";
           case A_field_type_time: return "INTEGER";
           case A_field_type_datetime: return "INTEGER";
           case A_field_type_blob: return "BLOB";
           default: return "";
       }
    }
    string getCPPType() const {
       switch(type) {
           case A_field_type_integer: return "int";
           case A_field_type_string: return "std::string";
           case A_field_type_float: return "float";
           case A_field_type_double: return "double";
           case A_field_type_boolean: return "bool";
           case A_field_type_date: return "litesql::Date";
           case A_field_type_time: return "litesql::Time";
           case A_field_type_datetime: return "litesql::DateTime";
           case A_field_type_blob: return "litesql::Blob";
           default: return "";
       }
    }
};

inline bool operator==(const Field::counted_ptr& lhs,const Field::counted_ptr& rhs)
{
  return lhs.get()==rhs.get();
}

class Param {
public:
    static const char* TAG;

  string name;
    AT_param_type type;
    Param(const string& n, AT_param_type t) : name(n), type(t) {}
    
};

class Method {
public:
  typedef counted_ptr<Method> counted_ptr;
  typedef std::vector<counted_ptr> sequence;

  static const char* TAG;

    string name;
    string returnType;
    vector<Param> params;
    Method(const string& n, const string& rt) 
        : name(n), returnType(rt) {}
    void param(const Param& p) {
        params.push_back(p);
    }
};

inline bool operator==(const Method::counted_ptr& lhs,const Method::counted_ptr& rhs)
{
  return lhs.get()==rhs.get();
}

class Relation;
class Relate;
class Object;

typedef counted_ptr<Object> ObjectPtr;
typedef std::vector<ObjectPtr> ObjectSequence;

inline bool operator==(const ObjectPtr& lhs,const ObjectPtr& rhs)
{
  return lhs.get()==rhs.get();
}

class Relate {
public:
  typedef counted_ptr<Relate> counted_ptr;
  typedef std::vector<counted_ptr> sequence;

    static const char* TAG;
  
    string objectName;
    string fieldTypeName;
    string fieldName;
    string getMethodName;
    size_t paramPos;
    AT_relate_limit limit;
    AT_relate_unique unique;
    string handle;
    Relate(const string& on, AT_relate_limit l, AT_relate_unique u, const string& h) 
        : objectName(on), limit(l), unique(u), handle(h) {
        if (hasLimit() && isUnique())
            throw logic_error("both limit and unique specified in relate: line " /*+ 
                              toString(yylineno)*/);
    }
    bool hasLimit() const {
        return limit == A_relate_limit_one;
    }
    
    bool hasLimitOne() const {
        return limit == A_relate_limit_one;
    }
    bool isUnique() const {
        return unique == A_relate_unique_true;
    }
    
  struct CompareByObjectName
		: public binary_function<counted_ptr, counted_ptr, bool>
	{	// functor for operator<
	bool operator()(const counted_ptr& _Left, const counted_ptr& _Right) const
		{	// apply operator< to operands
		return (_Left->objectName < _Right->objectName);
		}
	};
};

inline bool operator==(const Relate::counted_ptr& lhs,const Relate::counted_ptr& rhs)
{ return lhs.get()==rhs.get();  }

class Relation {
public:
  typedef counted_ptr<Relation> counted_ptr;
  typedef std::vector<counted_ptr> sequence;

    static const char* TAG;

    string id, name;
    string table;
    AT_relation_unidir unidir;
    Relate::sequence related;
    Field::sequence fields;
    Index::sequence indices;
    Relation(const string& i, const string& n, AT_relation_unidir ud) 
        : id(i), name(n), unidir(ud) {}
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
    size_t countTypes(const string& name) const {
        size_t res = 0;
        for (size_t i = 0; i < related.size(); i++)
            if (related[i]->objectName == name)
                res++;
        return res;
    }
    string getTable() const {
        Split res(related.size());
        for (size_t i = 0; i < related.size(); i++)
            res.push_back(related[i]->objectName);
        res.push_back(id);

        return makeDBName(res.join("_"));
    }
};

inline bool operator==(const Relation::counted_ptr& lhs,const Relation::counted_ptr& rhs)
{
  return lhs.get()==rhs.get();
}

inline bool operator<(const Relation::counted_ptr& lhs,const Relation::counted_ptr& rhs)
{
  return lhs.get() < rhs.get();
}

class RelationHandle {
public:
  typedef counted_ptr<RelationHandle> counted_ptr;
  typedef std::vector<counted_ptr> sequence;

  string name;
  Relation::counted_ptr relation;
  Relate::counted_ptr relate;
  ObjectPtr object;
  std::vector< pair< ObjectPtr,Relate::counted_ptr > > destObjects;

  RelationHandle(const string& n, Relation::counted_ptr& r, Relate::counted_ptr& rel, ObjectPtr& o) 
    : name(n), relation(r), relate(rel), object(o) {}
};


class Object {
public:
  typedef counted_ptr<Object> counted_ptr;
  typedef std::vector<counted_ptr> sequence;


    static ObjectPtr DEFAULT_BASE;
    static Field::counted_ptr ID_FIELD;
    static Field::counted_ptr TYPE_FIELD;

    static const char* TAG;

    string name, inherits;
    Field::sequence fields;
    Method::sequence methods;
    Index::sequence indices;
    RelationHandle::sequence handles;
    map<Relation::counted_ptr, Relate::sequence > relations;
    ObjectPtr parentObject;
    ObjectSequence children;

    Object(const string& n, const string& i) 
      : name(n), 
        inherits(i),
        parentObject(NULL) {
        if (i.size() == 0) {
            inherits = "litesql::Persistent";
            fields.push_back(ID_FIELD);
            fields.push_back(TYPE_FIELD);
        }
    }

    bool inheritsFromDefault() const
    {
      return inherits == "litesql::Persistent";
    }

    size_t getLastFieldOffset() const {
        if (!parentObject.get())
            return fields.size();
        else return parentObject->getLastFieldOffset() + fields.size();
    }
    void getAllFields(Field::sequence& flds) const {
        if (parentObject.get())
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
    const ObjectPtr getBaseObject() const{
      if (!parentObject.get())
            return ObjectPtr(NULL);
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

class Database {
public:

    static const char* TAG;
    
    class Sequence {
    public:
      typedef counted_ptr<Sequence> counted_ptr;
      typedef std::vector<counted_ptr> sequence;
    
      string name;
      string table;
      
      string getSQL() {
            return "CREATE SEQUENCE " + name + " START 1 INCREMENT 1";
        }
    };
    
    class DBField {
    public:
      typedef counted_ptr<DBField> counted_ptr;
      typedef std::vector<counted_ptr> sequence;

        string name, type, extra;
        bool primaryKey;
        Field::counted_ptr field;
        sequence references;
        DBField() : primaryKey(false) {}
        string getSQL(const string& rowIDType) {
            if (primaryKey)
                type = rowIDType;
            return name + " " + type + extra;
        }
    };
    
    class DBIndex {
    public:
        typedef counted_ptr<DBIndex> counted_ptr;
        typedef std::vector<counted_ptr> sequence;

        string name;
        string table;
        bool unique;
        DBField::sequence fields;
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
      typedef counted_ptr<Table> counted_ptr;
      typedef std::vector<counted_ptr> sequence;

      string name;
        DBField::sequence fields;
        string getSQL(const string& rowIDType) {
            litesql::Split flds;
            for (size_t i = 0; i < fields.size(); i++)
                flds.push_back(fields[i]->getSQL(rowIDType));
            return "CREATE TABLE " + name + " (" + flds.join(",") + ")";
        }

    };
    
    Sequence::sequence sequences;
    DBIndex::sequence indices;
    Table::sequence tables;
    string name;
    string include;
    string nspace;

    bool hasNamespace() const { return !nspace.empty(); }
};

typedef counted_ptr<Database> DatabasePtr;

void init(DatabasePtr& db, 
          Object::sequence& objects,
          Relation::sequence& relations);


}

#endif

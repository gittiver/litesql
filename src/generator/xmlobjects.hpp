#ifndef litesql_xmlobjects_hpp
#define litesql_xmlobjects_hpp

#include "litesql/string.hpp"

#include <string>
#include <stdexcept>
#include <algorithm>
#include <map>


#include "litesql/commontypes.h"
#include "litesql/except.hpp"
#define NO_MEMBER_TEMPLATES
#include "litesql/counted_ptr.hpp"
#undef NO_MEMBER_TEMPLATES

typedef std::string AT_param_type;

namespace xml {

std::string safe(const char *s);
std::string attribute(const std::string& name, const std::string& value);
std::string endtag(const std::string& name);
std::string makeDBName(const std::string& s);

class Value {
public:
    static const char* TAG;

    std::string name;
    std::string value;
    
    Value(const std::string& n, const std::string& v) : name(n), value(v) {}
};

class IndexField {
public:
    static const char* TAG;

    std::string name;
    IndexField(const std::string& n) : name(n) {}
};

class Index {
public:
  typedef counted_ptr<Index> Ptr;
  typedef std::vector<Ptr> sequence;

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
  typedef counted_ptr<Field> Ptr;
  typedef std::vector<Ptr> sequence;
  
  static const char* TAG;

    std::string name;
    std::string fieldTypeName;
    AT_field_type type;
    std::string default_;
    AT_field_indexed indexed;
    AT_field_unique unique;
    std::vector<Value> values;
	std::string length;
    Field(const std::string& n, AT_field_type t, const std::string& d, AT_field_indexed i, AT_field_unique u, const std::string& l="")
        : name(n), fieldTypeName(litesql::capitalize(n)), type(t), default_(d), indexed(i), unique(u),length(l) {
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
       case A_field_type_bigint:
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
    std::string getQuotedDefaultValue() const {
        if (hasQuotedValues())
            return "\"" + default_ + "\"";
        if (default_.size() == 0)
        {
          switch(type) {
            case A_field_type_float: 
            case A_field_type_double: 
              return "0.0";
            case A_field_type_blob: 
              return "Blob()";
            default:
              return "0";
          }
        }
        return default_;
    }

    std::string getCPPType() const {
       switch(type) {
           case A_field_type_integer : return "int";
		   case A_field_type_bigint  : return "litesql::bigint";
           case A_field_type_string  : return "std::string";
           case A_field_type_float   : return "float";
           case A_field_type_double  : return "double";
           case A_field_type_boolean : return "bool";
           case A_field_type_date    : return "litesql::Date";
           case A_field_type_time    : return "litesql::Time";
           case A_field_type_datetime: return "litesql::DateTime";
           case A_field_type_blob    : return "litesql::Blob";
           default: return "";
       }
    }
};

inline bool operator==(const Field::Ptr& lhs,const Field::Ptr& rhs)
{
  return lhs.get()==rhs.get();
}

class Param {
public:
    static const char* TAG;

  std::string name;
    AT_param_type type;
    Param(const std::string& n, const AT_param_type& t) : name(n), type(t) {}
    
};

class Method {
public:
  typedef counted_ptr<Method> Ptr;
  typedef std::vector<Ptr> sequence;

  static const char* TAG;

    std::string name;
    std::string returnType;
    std::vector<Param> params;
    Method(const std::string& n, const std::string& rt)
        : name(n), returnType(rt) {}
    void param(const Param& p) {
        params.push_back(p);
    }
};

inline bool operator==(const Method::Ptr& lhs,const Method::Ptr& rhs)
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
  typedef counted_ptr<Relate> Ptr;
  typedef std::vector<Ptr> sequence;

    static const char* TAG;
  
    std::string objectName;
    std::string fieldTypeName;
    std::string fieldName;
    std::string getMethodName;
    size_t paramPos;
    AT_relate_limit limit;
    AT_relate_unique unique;
    std::string handle;
    Relate(const std::string& on, AT_relate_limit l, AT_relate_unique u, const std::string& h)
        : objectName(on), limit(l), unique(u), handle(h) {
        if (hasLimit() && isUnique())
            throw std::logic_error("both limit and unique specified in relate: line " /*+
                              toString(yylineno)*/);
    }

    // TODO check if this is correct
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
		: public std::binary_function<Ptr, Ptr, bool>
	{	// functor for operator<
	bool operator()(const Ptr& _Left, const Ptr& _Right) const
		{	// apply operator< to operands
		return (_Left->objectName < _Right->objectName);
		}
	};
};

inline bool operator==(const Relate::Ptr& lhs,const Relate::Ptr& rhs)
{ return lhs.get()==rhs.get();  }

class Relation {
public:
  typedef counted_ptr<Relation> Ptr;
  typedef std::vector<Ptr> sequence;

    static const char* TAG;

    std::string id, name;
    std::string table;
    AT_relation_unidir unidir;
    Relate::sequence related;
    Field::sequence fields;
    Index::sequence indices;
    Relation(const std::string& i, const std::string& n, AT_relation_unidir ud)
        : id(i), name(n), unidir(ud) {}
    std::string getName() const {
        if (name.size() == 0) {
            std::string result;
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
        std::map<std::string, int> names;
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
    size_t countTypes(const std::string& name_) const {
        size_t res = 0;
        for (size_t i = 0; i < related.size(); i++)
            if (related[i]->objectName == name_)
                res++;
        return res;
    }
    std::string getTable() const {
        std::vector<std::string> res;
        for (size_t i = 0; i < related.size(); i++)
            res.push_back(related[i]->objectName);
        res.push_back(id);

      return makeDBName(litesql::join(res,"_"));
    }
};

inline bool operator==(const Relation::Ptr& lhs,const Relation::Ptr& rhs)
{
  return lhs.get()==rhs.get();
}

inline bool operator<(const Relation::Ptr& lhs,const Relation::Ptr& rhs)
{
  return lhs.get() < rhs.get();
}

class RelationHandle {
public:
  typedef counted_ptr<RelationHandle> Ptr;
  typedef std::vector<Ptr> sequence;

  std::string name;
  Relation::Ptr relation;
  Relate::Ptr relate;
  ObjectPtr object;
  std::vector< std::pair< ObjectPtr,Relate::Ptr > > destObjects;

  RelationHandle(const std::string& n, Relation::Ptr& r, Relate::Ptr& rel, ObjectPtr& o)
    : name(n), relation(r), relate(rel), object(o) {}
};


class Object {
public:
  typedef counted_ptr<Object> Ptr;
  typedef std::vector<Ptr> sequence;


    static ObjectPtr DEFAULT_BASE;
    static Field::Ptr ID_FIELD;
    static Field::Ptr TYPE_FIELD;

    static const char* TAG;

    std::string name, inherits;
    Field::sequence fields;
    Method::sequence methods;
    Index::sequence indices;
    RelationHandle::sequence handles;
    std::map<Relation::Ptr, Relate::sequence > relations;
    ObjectPtr parentObject;
    ObjectSequence children;

    Object(const std::string& n, const std::string& i)
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
  void getChildrenNames(std::vector<std::string> & names) const {
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
    std::string getTable() const {
        return makeDBName(name + "_");
    }
    std::string getSequence() const {
        return makeDBName(name + "_seq");
    }
};

class Database {
public:

    static const char* TAG;
    
    class Sequence {
    public:
      typedef counted_ptr<Sequence> Ptr;
      typedef std::vector<Ptr> sequence;
    
      std::string name;
      std::string table;
    };
    
    class DBField {
    public:
      typedef counted_ptr<DBField> Ptr;
      typedef std::vector<Ptr> sequence;

      std::string name() const {return field->name +"_"; };
      void name(const std::string& fieldname) {field->name=fieldname;};
        bool primaryKey;
        Field::Ptr field;
        sequence references;
        DBField() : primaryKey(false), field(new Field("",A_field_type_integer,"",A_field_indexed_false,A_field_unique_false)) {}
    };
    
    class DBIndex {
    public:
        typedef counted_ptr<DBIndex> Ptr;
        typedef std::vector<Ptr> sequence;

        std::string name;
        std::string table;
        bool unique;
        DBField::sequence fields;
        DBIndex() : unique(false) {}
        std::string getSQL() {
            std::vector<std::string> flds;
            for (size_t i = 0; i < fields.size(); i++)
                flds.push_back(fields[i]->name());
            std::string uniqueS;
            if (unique)
                uniqueS = " UNIQUE";
          return "CREATE" + uniqueS + " INDEX " + name + " ON "
            + table + " (" + litesql::join(flds,",") + ")";
        }
    };
    
    class Table {
    public:
      typedef counted_ptr<Table> Ptr;
      typedef std::vector<Ptr> sequence;

      std::string name;
        DBField::sequence fields;
        std::string getSQL(const std::string& rowIDType) {
            std::vector<std::string> flds;
            for (size_t i = 0; i < fields.size(); i++)
            {
              if (fields[i]->primaryKey)
                flds.push_back(fields[i]->name() + " " + rowIDType);
              else
                flds.push_back(fields[i]->name() 
                              + " \" + backend->getSQLType(" + "A_field_type_"+toAttributeString(fields[i]->field->type) +",\""+fields[i]->field->length+ "\")" + " + \""
                              +(fields[i]->field->isUnique() ? " UNIQUE" : "") + + "\" +"
                              +"\"");
             }
          return "CREATE TABLE " + name + " (" + litesql::join(flds,",") + ")";
        }

    };
    
    Sequence::sequence sequences;
    DBIndex::sequence indices;
    Table::sequence tables;
    std::string name;
    std::string include;
    std::string nspace;

    bool hasNamespace() const { return !nspace.empty(); }
};

typedef counted_ptr<Database> DatabasePtr;

void init(DatabasePtr& db, 
          Object::sequence& objects,
          Relation::sequence& relations);


}

#endif

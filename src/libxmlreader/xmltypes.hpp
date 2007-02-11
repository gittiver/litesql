#ifndef _litesql_xmltypes_hpp
#define _litesql_xmltypes_hpp
#include <string>
#include <vector>
#include <map>
#include "litesql.hpp"
#include "stringutils.hpp"
#include "flexml-header.hpp"

namespace xml {

    class Position {
        public:

            std::string file;
            int line;

            Position(const std::string& f, int l) : file(f), line(l) {}
    };

    class XMLExcept : public litesql::Except { 
        public:

            XMLExcept(const Position& p, const std::string& m) 
                : litesql::Except("In file " + p.file + ", line " 
                     + litesql::toString(p.line) + " : " 
                        + m) {}
    };

    class XML {
        public:

            const Position pos;
            XML(const Position& p) : pos(p) {}
    };

    class Represent : public XML {
        public:

            const std::string as, target;

            Represent(const Position& p, 
                      const std::string& a,
                      const std::string& t)
                : XML(p), as(a), target(t) {}                      
            
    };

    class Store : public XML {
        public:

            const std::string as, backend;

            Store(const Position& p,
                  const std::string& a,
                  const std::string& b)
                : XML(p), as(a), backend(b) {}                  
    };

    class Value : public XML {
        public:

            const std::string name, value;

            Value(const Position& p, 
                  const std::string& n, 
                  const std::string& v) 
               : XML(p), name(n), value(v) {}
    };

    class Check : public XML {
        public:
            
            const std::string function, param;
            const bool onCreate, onDelete, onUpdate, onLink, onUnlink,
                       constrained;

            Check(const Position& p, 
                  const std::string& f,
                  const std::string& pa,
                  AT_check_oncreate oc, 
                  AT_check_ondelete od,
                  AT_check_onupdate ou,
                  AT_check_onlink ol,
                  AT_check_onunlink oun)
                : XML(p), function(f), param(pa), 
                  onCreate(oc == A_check_oncreate_true), 
                  onDelete(od == A_check_ondelete_true), 
                  onUpdate(ou == A_check_onupdate_true), 
                  onLink(ol == A_check_onlink_true), 
                  onUnlink(oun == A_check_onunlink_true),
                  constrained(onCreate || onDelete || onUpdate || onLink 
                              || onUnlink) {}
    };

    class Type : public XML {
        public:

            const std::string name;
            const bool quotedValue;
            std::vector<Represent*> represents;
            std::vector<Store*> stores;
            std::vector<Value*> values;
            std::vector<Check*> checks;

            Type(const std::string& n, bool qv) 
                : XML(Position("", 0)), name(n), quotedValue(qv) {}

            Type(const Position& p, 
                 const std::string& n)
                : XML(p), name(n), quotedValue(true) {}
    };

    class IndexField : public XML {
        public:

            const std::string name;

            IndexField(const Position& p, 
                       const std::string& n) 
                : XML(p), name(n) {}
    };

    class Index : public XML {
        public:

            std::vector<IndexField*> fields;
            const bool unique;

            Index(const Position& p, 
                  AT_index_unique u) 
              : XML(p), unique(u == A_index_unique_true) {}
    };

    class Param : public XML {
        public:

            const std::string name;
            const std::string type;

            Param(const Position& p, 
                  const std::string& n, 
                  const std::string& t) 
                : XML(p), name(n), type(t) {}
    };

    class Method : public XML {
        public:

            const std::string name, returnType;
            const bool const_;
            std::vector<Param*> params;

            Method(const Position& p, 
                   const std::string& n, 
                   const std::string& rt, 
                   AT_method_const c)
                : XML(p), name(n), returnType(rt), 
                  const_(c == A_method_const_true) {}

    };

    class Interface : public XML {
        public:

            const std::string name;
            std::vector<Method*> methods;

            Interface(const Position& p, 
                      const std::string& n)
                : XML(p), name(n) {}
                    
    };

    class Field : public XML {
        public:

            const std::string name;
            const std::string fieldTypeName;
            const std::string typeName;
            const std::string default_;
            const bool indexed, unique;
            Type* type;
            std::vector<Value*> values;
            std::vector<Check*> checks;
            int offset;

            Field(const Position& p, 
                  const std::string& n, 
                  const std::string& t, 
                  const std::string& d, 
                  AT_field_indexed i, 
                  AT_field_unique u) 
                : XML(p), 
                  name(n), 
                  fieldTypeName(capitalize(n)), typeName(t), 
                  type(NULL), default_(d), 
                  indexed(i == A_field_indexed_true), 
                  unique(u == A_field_unique_true) {}

            bool hasQuotedValues() const;
            std::string getQuotedDefaultValue() const;
            std::string getSQLType() const;
            std::string getClass() const;
    };


    class Relation;
    class Object;

    class Relate : public XML {
        public:

            const std::string objectName, interfaceName;
            std::string fieldTypeName, fieldName;
            const bool limit, unique;
            const std::string handle;
            int paramPos;
            Object* object;
            Interface* interface;

            Relate(const Position& p, 
                   const std::string& on, 
                   const std::string& in,
                   AT_relate_limit l, 
                   AT_relate_unique u, 
                   const std::string& h) 
                : XML(p), objectName(on), interfaceName(in),
                  limit(l == A_relate_limit_one), 
                  unique(u == A_relate_unique_true), 
                  handle(h), object(NULL), interface(NULL) {
                    if (limit && unique)
                        throw XMLExcept(p, "both limit and unique "
                                "specified in relate");
                }
    };

    
    class RelationHandle : public XML {
        public:

            const std::string name;
            Relation * relation;
            Relate * relate;
            Object * object;
            std::vector< std::pair<Object*,Relate*> > destObjects;

            RelationHandle(const Position& p, 
                           const std::string& n, 
                           Relation* r, 
                           Relate* rel, 
                           Object* o) 
                : XML(p), name(n), relation(r), relate(rel), object(o) {}
    };


    class Option : public XML {
        public:

            const std::string name, value, backend;
            

            Option(const Position& p, 
                   const std::string& n, 
                   const std::string& v, 
                   const std::string& be)
                : XML(p), name(n), value(v), backend(be) {}
    };


    class Relation : public XML {
        public:

            const std::string id;
            const std::string table;
            const bool unidir;
            std::string name;
            std::vector<Relate*> related;
            std::vector<Field*> fields;
            std::vector<Index*> indices;
            std::vector<Option*> options;

            Relation(const Position& p, 
                     const std::string& i, 
                     const std::string& n, 
                     AT_relation_unidir ud) 
                : XML(p), id(i), name(n), unidir(ud == A_relation_unidir_true) 
                  {}

            std::string getTable() const;
            int countTypes(const std::string& obj) const;
            int maxSameTypes() const;
    };

    class Implements : public XML {
        public:

            const std::string interfaceName;
            Interface* interface;

            Implements(const Position& p, 
                       const std::string& i)
                : XML(p), interfaceName(i), interface(NULL) {}
    };

    class Object : public XML {
        public:

            const std::string name, inherits;
            const bool temporary;

            std::vector<Implements*> implements;
            std::vector<Field*> fields;
            std::vector<Method*> methods;
            std::vector<Index*> indices;
            std::vector<RelationHandle*> handles;
            std::map<Relation*, std::vector<Relate*> > relations;
            std::vector<Object*> children;
            std::vector<Option*> options;
            std::vector<Check*> checks;
            Object* parentObject;

            Object(const Position& p, 
                   const std::string& n, 
                   const std::string& i, 
                   AT_object_temporary t) 
                : XML(p), name(n), inherits(i), 
                  temporary(t == A_object_temporary_true),
                  parentObject(NULL) {
                 }

            int getLastFieldOffset() const;
            void getAllFields(std::vector<Field*>& flds) const;
            void getChildrenNames(litesql::Split& names) const;
            const Object* getBaseObject() const;
            std::string getTable() const;
            std::string getSequence() const;
    };

    class DbSequence {
    public:
        std::string name;
    };

    class DbTable;
    class DbField {
    public:
        std::string name;
        bool primaryKey, unique;
        Type* type;
        Field* field;
        DbField* references; // TODO: m‰‰ritt‰minen
        DbTable* table;

        DbField() : primaryKey(false), unique(false) {}
    };

    class DbIndex {
    public:
        std::string name, table;
        bool unique;
        std::vector<DbField*> fields;
        DbIndex() : unique(false) {}
    };

    class DbTable {
    public:
        std::string name;
        std::vector<DbField*> fields;
        std::vector<Option*> options;
    };



    class Database : public XML {
        public:
            std::vector<Object*> objects;
            std::vector<Interface*> interfaces;
            std::vector<Relation*> relations;
            std::vector<Option*> options;
            std::vector<Type*> types;

            std::vector<DbSequence*> sequences;
            std::vector<DbIndex*> indices;
            std::vector<DbTable*> tables;

            const std::string name, include, nspace;

            Database(const Position& p, 
                     const std::string& n,
                     const std::string& i,
                     const std::string& ns) 
                : XML(p), name(n), include(i), nspace(ns) {}

    };

}

#endif

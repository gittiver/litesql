#ifndef _litesql_xmltypes_hpp
#define _litesql_xmltypes_hpp
#include <string>
#include <vector>
#include <map>

#include "litesql.hpp"

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
                  bool oc, 
                  bool od,
                  bool ou,
                  bool ol,
                  bool oun)
                : XML(p), function(f), param(pa), 
                  onCreate(oc), 
                  onDelete(od), 
                  onUpdate(ou), 
                  onLink(ol), 
                  onUnlink(oun),
                  constrained(onCreate || onDelete || onUpdate || onLink 
                              || onUnlink) {}
    };

    class Type : public XML {
        public:

            const std::string name;
            const bool quotedValue;
            const bool internal;
            std::vector<Represent*> represents;
            std::vector<Store*> stores;
            std::vector<Value*> values;
            std::vector<Check*> checks;

            Type(const std::string& n, bool qv) 
                : XML(Position("", 0)), name(n), quotedValue(qv), 
                  internal(true) {}

            Type(const Position& p, 
                 const std::string& n)
                : XML(p), name(n), quotedValue(true), internal(false) {}

            std::string getClass(const std::string& target) const;
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
                  bool u) 
              : XML(p), unique(u) {}
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
                   bool c)
                : XML(p), name(n), returnType(rt), const_(c) {}

    };

    class Relation;
    class Object;
    class Relate;
    class Interface;
    

    class RelationHandle : public XML {
        public:

            const std::string name;
            Relation* relation;
            Relate* relate;
            Object* object;

            std::vector< std::pair<Object*,Relate*> > destObjects;

            RelationHandle(const Position& p, 
                           const std::string& n, 
                           Relation* r, 
                           Relate* rel, 
                           Object* o)
                : XML(p), name(n), relation(r), relate(rel), object(o) {}
            
    };
   
    class Implementation;

    class Interface : public XML {
        public:

            const std::string name;
            std::vector<Method*> methods;
            std::vector<Implementation*> implementations;

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

            Field(const Position& pos, 
                  const std::string& name, 
                  const std::string& type, 
                  const std::string& default_, 
                  bool indexed, 
                  bool unique); 

            bool hasQuotedValues() const;
            std::string getQuotedDefaultValue() const;
            std::string getClass(const std::string& target) const;
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

    class Relate : public XML {
        public:

            std::string objectName, interfaceName;
            std::string fieldTypeName, fieldName;
            const bool limit, unique;
            const std::string handle;
            int paramPos;
            // in case there are more than one class of same type
            // sameObjectPos is the order number of references
            // to the same object
            int sameObjectPos; 
            Relation* relation;
            Object* object;
            Interface* interface;

            Relate(const Position& p, 
                   Relation* r,
                   const std::string& on, 
                   const std::string& in,
                   bool l, 
                   bool u, 
                   const std::string& h) 
                : XML(p), objectName(on), interfaceName(in),
                  limit(l), 
                  unique(u), 
                  handle(h), paramPos(-1), sameObjectPos(-1),
                  relation(r), object(NULL), interface(NULL) {}

            Relate* clone() const;                
    };


    class Relation : public XML {
        public:
            const std::string id;
            const std::string table;

            // true, if relation contains interfaces
            bool abstract;

            // if relation is stored in a table of a persistent object
            bool embedded;

            std::string name;
            std::vector<Relate*> related;
            std::vector<Field*> fields;
            std::vector<Index*> indices;
            std::vector<Option*> options;
            std::vector<Check*> checks;

            Relation(const Position& p, 
                     const std::string& i, 
                     const std::string& n)
                : XML(p), id(i), name(n), abstract(false), embedded(false) {}

            void sortRelated();
            std::string getTable() const;
            int countTypes(const std::string& obj) const;
            int maxSameTypes() const;
            Relation* clone() const;
    };

    class Implementation : public XML {
        public:

            const std::string interfaceName;
            Interface* interface;
            Object* object;

            Implementation(const Position& p, Object* o,
                       const std::string& i)
                : XML(p), interfaceName(i), interface(NULL), object(o) {}
    };

   
    class Object : public XML {
        public:

            const std::string name, inherits;
            const bool temporary;

            std::vector<Implementation*> implementations;
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
                   bool t) 
                : XML(p), name(n), inherits(i), 
                  temporary(t),
                  parentObject(NULL) {
                 }

            int getLastFieldOffset() const;
            void getAllFields(std::vector<Field*>& flds) const;
            void getChildrenNames(litesql::Split& names) const;
            Object* getBaseObject() const;
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
                : XML(p), name(n), include(i), nspace(ns) {
                initBaseTypes();
            }

            void initBaseTypes();                

    };

}

#endif

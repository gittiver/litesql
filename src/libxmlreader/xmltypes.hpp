#ifndef _litesql_xmltypes_hpp
#define _litesql_xmltypes_hpp
#include <string>
#include <vector>
#include <map>
#include "litesql.hpp"
#include "stringutils.hpp"
#include "xmlreader-actions.hpp"

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

            Position pos;
            XML(const Position& p) : pos(p) {}
    };

    class Value : public XML {
        public:

            std::string name, value;

            Value(const Position& p, std::string n, std::string v) 
               : XML(p), name(n), value(v) {}
    };

    class Type : public XML {
        public:

            std::string name, class_, sqlType;
            std::vector<Value> values;

            Type(std::string n, std::string c, std::string st) 
                : XML(Position("", 0)), name(n), class_(c), sqlType(st) {}

            Type(const Position& p, std::string n, std::string c, 
                 std::string st)
                : XML(p), name(n), class_(c), sqlType(st) {}
    };

    class IndexField : public XML {
        public:

            std::string name;
            IndexField(const Position& p, std::string n) : XML(p), name(n) {}
    };

    class Index : public XML {
        public:

            std::vector<IndexField> fields;
            bool unique;
            Index(const Position& p, AT_index_unique u) 
              : XML(p), unique(u == A_index_unique_true) {}
    };

    class Field : public XML {
        public:

            std::string name;
            std::string fieldTypeName;
            std::string typeName;
            Type* type;
            std::string default_;
            bool indexed, unique;
            std::vector<Value> values;
            int offset;

            Field(const Position& p, std::string n, std::string t, std::string d, AT_field_indexed i, AT_field_unique u) 
                : XML(p), name(n), fieldTypeName(capitalize(n)), typeName(t), 
                  type(NULL), default_(d), 
                  indexed(i == A_field_indexed_true), 
                  unique(u == A_field_unique_true) {
                }

            bool isIndexed() const {

                return indexed == A_field_indexed_true;
            }

            bool isUnique() const {

                return unique == A_field_unique_true;
            }



    };

    class Param : public XML {
        public:

            std::string name;
            AT_param_type type;
            Param(const Position& p, std::string n, AT_param_type t) 
                : XML(p), name(n), type(t) {}

    };

    class Method : public XML {
        public:

            std::string name, returnType;
            bool const_;
            std::vector<Param> params;

            Method(const Position& p, 
                    std::string n, std::string rt, AT_method_const c)
                : XML(p), name(n), returnType(rt), 
                const_(c == A_method_const_true) {}

    };

    class Relation;
    class Object;


    class Relate : public XML {
        public:

            std::string objectName;
            std::string fieldTypeName, fieldName;
            std::string getMethodName;
            int paramPos;
            bool limit, unique;
            std::string handle;
            std::string remoteHandle;
            Object* object;

            Relate(const Position& p, 
                    std::string on, AT_relate_limit l, AT_relate_unique u, 
                    std::string h, std::string rh) 
                : XML(p), objectName(on), 
                limit(l == A_relate_limit_one), 
                unique(u == A_relate_unique_true), 
                handle(h), remoteHandle(rh), object(NULL) {
                    if (limit && unique)
                        throw XMLExcept(p, "both limit and unique "
                                "specified in relate");
                }
    };
    class RelationHandle : public XML {
        public:

            std::string name;
            Relation * relation;
            Relate * relate;
            Object * object;
            std::vector< std::pair<Object*,Relate*> > destObjects;

            RelationHandle(const Position& p, 
                    std::string n, Relation * r, Relate * rel, Object * o) 
                : XML(p), name(n), relation(r), relate(rel), object(o) {}
    };



    class Option : public XML {
        public:

            std::string name, value;

            Option(const Position& p, const std::string& n, const std::string& v) 
                : XML(p), name(n), value(v) {}
    };

    class IfBackend : public XML {
        public:

            std::string name;
            std::vector<Option*> options;
            std::vector<Type*> types;

            IfBackend(const Position& p, const std::string& n) 
                : XML(p), name(n) {}
    };

    class IfTarget : public XML {
        public:

            std::string name;
            std::vector<Option*> options;
            std::vector<Type*> types;

            IfTarget(const std::string& n)
                : XML(Position("", 0)), name(n) {}

            IfTarget(const Position& p, const std::string& n) 
                : XML(p), name(n) {}
    };

    class Relation : public XML {
        public:

            std::string id, name;
            std::string table;
            bool unidir;
            std::vector<Relate*> related;
            std::vector<Field*> fields;
            std::vector<Index*> indices;
            std::vector<Option*> options;
            std::vector<IfBackend*> ifBackends;

            Relation(const Position& p, std::string i, std::string n, AT_relation_unidir ud) 
                : XML(p), id(i), name(n), unidir(ud == A_relation_unidir_true) 
                  {}

    };
    class Object : public XML {
        public:

            std::string name, inherits;
            bool temporary;

            std::vector<Field*> fields;
            std::vector<Method*> methods;
            std::vector<Index*> indices;
            std::vector<RelationHandle*> handles;
            std::map<Relation*, std::vector<Relate*> > relations;
            std::vector<Object*> children;
            std::vector<Option*> options;
            std::vector<IfBackend*> ifBackends;
            std::vector<Relate*> related;
            Object* parentObject;

            Object(const Position& p, std::string n, std::string i, AT_object_temporary t) 
                : XML(p), name(n), inherits(i), 
                  temporary(t == A_object_temporary_true),
                  parentObject(NULL) {
                 }

    };

    class Database : public XML {
        public:
            std::vector<Object*> objects;
            std::vector<Relation*> relations;
            std::vector<Option*> options;
            std::vector<IfBackend*> ifBackends;
            std::vector<IfTarget*> ifTargets;
            std::vector<Type*> types;

            std::string name, include, nspace;

            Database(const Position& p) : XML(p) {}

    };

}

#endif

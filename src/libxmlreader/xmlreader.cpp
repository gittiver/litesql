#include "xmlreader.hpp"
#include "flexml-header.hpp"
#include "scanner.hpp"
#include "stringutils.hpp"

namespace std {
    template <>
    struct less<xml::Relate*> {
        bool operator()(xml::Relate const* r1, xml::Relate const* r2) {
            if (!r1)
                return true;
            if (!r2)
                return false;
            return r1->objectName < r2->objectName;
        }
    };
}

namespace xml {
    extern Database* xmlReaderDb;

    using namespace std;

    static bool inWords(const string& s, char* words[]) {
        for (size_t i = 0; words[i]; i++)
            if (s == words[i])
                return true;
        return false; 

    }
    static bool isCPPReservedWord(const string& s) {

        static char* words[] = 
        {"asm","break","case","catch",
            "char","class","const","continue","default",
            "delete","do","double","else","enum","extern",
            "float","for","friend","goto","if","inline","int",
            "long","new","operator","private","protected",
            "public","register","return","short","signed",
            "sizeof","static","struct","switch","template",
            "this","throw","try","typedef","union","unsigned",
            "virtual","void","volatile","while",   
            // LiteSQL specific
            "update", "del", "upcast", "upcastCopy", NULL
        };
        return inWords(s, words);
    }

    static bool validTarget(string t) {
        static char* words[] = {"c++", "python", "graphviz", NULL};
        return inWords(t, words);
    }

    static bool validBackend(string t) {
        static char* words[] = {"sqlite3", "mysql", "postgresql", NULL};
        return inWords(t, words);
    }

    static string validId(string s) {

        if (s.size() == 0) 
            return "empty identifier";
        if (!isalpha(s[0])) 
            return "first character is not alphabet";
        for (size_t i = 1; i < s.size(); i++)
            if (!isalnum(s[i]) && !s[i] != '_')
                return "illegal character : " + s[i];
        if (s[s.size()-1] == '_')
            return "ends with an underscore";
        if (isCPPReservedWord(s))
            return "is C++ reserved word";
        return "";
    }

    static string validFieldId(string s) {

        s = validId(s);
        if (!s.empty())
            return s;
        if (toupper(s[0]) == s[0])
            return "does not begin with lower case letter";
        return "";
    }
    template <class Func>
    static void check(Func func, const string& tag, const Position& p, 
                      const std::string& value) {
        string err;                      
        if (!(err = func(value)).empty()) 
            throw XMLExcept(p, "invalid value: " + tag + ": " 
                            + value + " : " + err);
    }
    template <class T, class Func>
    static void checkNames(vector<T*>& objects, Func& func,
                           set<string>& usedNames, string tag,
                           string prefix="") {
        for (size_t i = 0; i < objects.size(); i++) {
            string name = objects[i]->name;
            string err;
            // name ok?
            if (!(err = func(name)).empty())
                throw XMLExcept(objects[i]->pos, 
                                "invalid name: " + tag + " : " + name);
            // name unique?

            if (!prefix.empty())
                name = prefix + "." + name;

            if (usedNames.find(name) != usedNames.end())                                        throw XMLExcept(objects[i]->pos,
                                "duplicate name: " + tag + " : " + name);
                
            usedNames.insert(name);                                
        }
    }
    
    template <class T>
    static void addNames(vector<T*> objects, set<string>& names) {
        for (size_t i = 0; i < objects.size(); i++)
            names.insert(objects[i]->name);
    }

    static void checkRelated(const string& relName,
                             vector<Relate*>& related, 
                             const set<string>& objectNames,
                             const set<string>& interfaceNames,
                             set<string>& usedNames) {

        bool uniques = false, limits = false;
        string err;
        for (size_t i = 0; i < related.size(); i++) {
            Relate& rel = *related[i];
            
            // valid relation handle name?
            if (!(err = validFieldId(rel.handle)).empty() 
                    && !rel.handle.empty())
                throw XMLExcept(rel.pos, 
                        "invalid id: relation.relate.handle : " 
                        + relName + "." + rel.handle + " : " + err);

            if (!rel.objectName.empty()) {

                // not both object and interface? 
                if (!rel.interfaceName.empty())
                    throw XMLExcept(rel.pos, "both object and interface "
                                             "specified in "
                                             "relation.relate");
            
                // valid object reference in relate?
                if (objectNames.find(rel.objectName) == objectNames.end())
                    throw XMLExcept(rel.pos, "unknown object: "
                                             "relation.relate.name : " 
                                             + relName + "." + rel.objectName);

                // object has id already?
                string id = rel.objectName + "." + rel.handle;
                if (usedNames.find(id) != usedNames.end())
                    throw XMLExcept(rel.pos, "object " + rel.objectName
                                             + " already has identifier "
                                             + rel.handle);
                usedNames.insert(id);                                           

            } else if (!rel.interfaceName.empty()) {

                // valid interface reference in relate?
                 if (interfaceNames.find(rel.interfaceName) 
                     == interfaceNames.end())
                    throw XMLExcept(rel.pos, "unknown interface: "
                                             "relation.relate.interface : " 
                                             + relName + "." + rel.objectName);
                // interface has id already?
                string id = rel.interfaceName + "." + rel.handle;
                if (usedNames.find(id) != usedNames.end())
                    throw XMLExcept(rel.pos, "interface " + rel.interfaceName
                                             + " already has identifier "
                                             + rel.handle);
                usedNames.insert(id);
                           
            } else // object or interface specified?
                throw XMLExcept(rel.pos, "no object or interface specified");                             
                
            if (!rel.handle.empty())
                usedNames.insert(relName + "." + rel.handle);

            if (rel.unique)
                uniques = true;
            if (rel.limit)
                limits = true;

            // only uniques or limits?
            
            if (uniques && limits)
                throw XMLExcept(rel.pos, "both 'unique' and 'limit' "
                                         "attributes used in relation " 
                                         + relName);
        }
    }

    static void checkTypes(vector<Type*>& types) {
        for (size_t i = 0; i < types.size(); i++) {
            Type* t = types[i];

            // represent target names ok?
            for (size_t i2 = 0; i2 < t->represents.size(); i2++) {
                Represent* r = t->represents[i2];
                if (!r->target.empty())
                    check(validTarget, "type.represent.target", r->pos, 
                          r->target);
            }

            // store backend names ok?
            for (size_t i2 = 0; i2 < t->stores.size(); i2++) {
                Store* s = t->stores[i2];
                if (!s->backend.empty())
                    check(validBackend, "type.store.backend", s->pos, 
                          s->backend);
            } 
            set<string> valueNames;
            checkNames(t->values, validId, valueNames, "type.value");

            // check function names ok?
            for (size_t i2 = 0; i2 < t->checks.size(); i2++) 
                check(validId, "type.check.function", 
                      t->checks[i2]->pos, t->checks[i2]->function);


        }
    }

    static void checkOptions(vector<Option*>& options) {    
        static char* optionNames[] = {"storagetype", NULL};

        for (size_t i = 0; i < options.size(); i++) {
            Option* o = options[i];
            if (!inWords(o->name, optionNames))
                throw XMLExcept(o->pos, "unknown option : " + o->name);

            check(validBackend, "option.backend", o->pos, o->backend);
        }
    }

    static void checkFields(vector<Field*>& fields) {
        for (size_t i = 0; i < fields.size(); i++) {
            Field* f = fields[i];

            // value names ok?
            set<string> names;
            checkNames(f->values, validId, names, "field.value");

            // check function names ok?
            for (size_t i2 = 0; i2 < f->checks.size(); i2++) 
                check(validId, "type.check.function", 
                      f->checks[i2]->pos, f->checks[i2]->function);

        }
    }
    static void sanityCheck(Database& db,
            vector<Object*>& objects,
            vector<Relation*>& relations) {

        using namespace litesql;
        set<string> usedNames, typeNames;
        set<string> objectNames;
        set<string> interfaceNames;
        string err;

        // database attributes ok?

        check(validId, "database.name", db.pos, db.name);
        check(validId, "database.nspace", db.pos, db.name);
        
        // object names ok?
        checkNames(objects, validId, usedNames, "object");

        // type names ok?
        checkNames(db.types, validId, typeNames, "type");
        checkTypes(db.types);

        addNames(objects, objectNames);
        addNames(db.interfaces, interfaceNames);

        checkOptions(db.options);

        for (size_t i = 0; i < objects.size(); i++) {
            Object& o = *objects[i];

            // object inherits ok?
            if (!o.inherits.empty() &&
                objectNames.find(o.inherits) == objectNames.end())
                throw XMLExcept(o.pos, "unknown object : object.inherits : "
                                       + o.inherits);
                              
            // object field names ok?
            checkNames(o.fields, validFieldId, usedNames, "object.field",
                       o.name);
            set<string> fieldNames;
            addNames(o.fields, fieldNames);

            checkFields(o.fields);

                               
            
            // object method names ok?
            checkNames(o.methods, validFieldId, usedNames, "object.method",
                       o.name);

            // object method parameter names ok?
            for (size_t i2 = 0; i2 < o.methods.size(); i2++)
                checkNames(o.methods[i2]->params, validFieldId, usedNames, 
                           "object.method.param", 
                           o.name + "." + o.methods[i2]->name);

            // implements references ok?                           
            for (size_t i2 = 0; i2 < o.implements.size(); i2++) {
                string iname = o.implements[i2]->interfaceName;
                if (interfaceNames.find(iname) == interfaceNames.end())
                    throw XMLExcept(o.implements[i2]->pos, 
                                    "unknown interface : object.implements : "
                                    + iname);
            }

            for (size_t i2 = 0; i2 < o.indices.size(); i2++) {
                Index* idx = o.indices[i2];
                set<string> names;

                // index field names ok?
                checkNames(idx->fields, validFieldId, names, 
                           "index.indexfield");

                // index field references ok?                           
                for (size_t i3 = 0; i3 < idx->fields.size(); i3++) {
                    IndexField* ifld = idx->fields[i3];
                    if (fieldNames.find(ifld->name) == fieldNames.end())
                        throw XMLExcept(ifld->pos, 
                                        "unknown field name "
                                        ": indexfield.name : " + ifld->name);
                }
            }


            checkOptions(o.options);

            // check function names ok?
            for (size_t i2 = 0; i2 < o.checks.size(); i2++) 
                check(validId, "object.check.function", 
                      o.checks[i2]->pos, o.checks[i2]->function);
        }
        
        // interface names ok?
        checkNames(db.interfaces, validId, usedNames, "interface");

        for (size_t i = 0; i < db.interfaces.size(); i++) {
            Interface& iface = *db.interfaces[i];

            // interface method names ok?
            checkNames(iface.methods, validFieldId, usedNames, 
                       "interface.method", iface.name);

            // interface method parameter names ok?
            for (size_t i2 = 0; i2 < iface.methods.size(); i2++)
                checkNames(iface.methods[i2]->params, validFieldId, usedNames, 
                           "interface.method.param", 
                           iface.name + "." + iface.methods[i2]->name);
                      
        }


        // relation names ok?
        checkNames(relations, validId, usedNames, "relation");

        for (size_t i = 0; i < relations.size(); i++) {
            Relation& r = *relations[i];

            // relation field names ok?
            checkNames(r.fields, validFieldId, usedNames, 
                       "relation.field", r.name);

            checkFields(r.fields);                       
            checkOptions(r.options);


            // relation field default flags ok?
            bool defaults = false;

            for (size_t i2 = 0; i2 < r.fields.size(); i2++) {
                Field& f = *r.fields[i2];
                if (f.default_.size() > 0)
                    defaults = true;
                else if (defaults)
                    throw XMLExcept(f.pos, "no default-value after "
                                           "field with default value : " 
                                           + r.name + "." + f.name);

            }
            
            bool limits = false;
            bool uniques = false;
            
            checkRelated(r.name, 
                         r.related, objectNames, interfaceNames, usedNames);

            // limit only in binary relations?
            if (r.related.size() != 2 && limits)
                throw XMLExcept(r.pos, "'limit' attribute "
                                       "used in relation of " 
                                       + toString(r.related.size()) 
                                       + " object(s) " + r.name);

        }   
    }

    static void initSchema(Database& db, map<string, Type*>& typeMap) {
        vector<Object*>& objects = db.objects;
        vector<Relation*>& relations = db.relations;

        for (size_t i = 0; i < objects.size(); i++) {
            Object& o = *objects[i];
            map<string, DbField*> fldMap;
            DbTable* tbl = new DbTable;
            tbl->name = o.getTable();
            db.tables.push_back(tbl);

            if (!o.parentObject) {
                DbSequence* seq = new DbSequence;
                seq->name = o.getSequence();
                db.sequences.push_back(seq);
            }  else {
                DbField *id = new DbField; 
                id->name = "id_";
                id->type = typeMap["integer"];
                id->primaryKey = true;
                tbl->fields.push_back(id);
            }

            for (size_t i2 = 0; i2 < o.fields.size(); i2++) {
                Field& f = *o.fields[i2];
                DbField* fld = new DbField;
                fld->name = f.name + "_";
                fldMap[f.name] = fld;
                fld->type = f.type;
                fld->primaryKey = (f.name == "id");
                fld->unique = f.unique;
                fld->field = o.fields[i2];
                tbl->fields.push_back(fld);

                if (f.indexed) {
                    DbIndex* idx = new DbIndex;
                    idx->name = makeDbName(tbl->name + fld->name + "idx");
                    idx->table = tbl->name;
                    idx->fields.push_back(fld);
                    db.indices.push_back(idx);
                }

            }
            for (size_t i2 = 0; i2 < o.indices.size(); i2++) {
                const Index& idx = *o.indices[i2];

                litesql::Split fldNames;
                DbIndex* index = new DbIndex;
                for (size_t i3 = 0; i3 < idx.fields.size(); i3++) {
                    index->fields.push_back(fldMap[idx.fields[i3]->name]);
                    fldNames.push_back(idx.fields[i3]->name);
                }

                index->name = makeDbName(tbl->name + "_" + fldNames.join("_") + "_idx");
                string unique = "";
                if (idx.unique)
                    index->unique = true;
                db.indices.push_back(index);
            }
        }
        for (size_t i = 0; i < relations.size(); i++) {
            Relation& r = *relations[i];
            DbTable* tbl = new DbTable;
            db.tables.push_back(tbl);
            tbl->name = r.getTable();
            vector<DbField*> objFields;
            map<string, DbField*> fldMap;
            for (size_t i2 = 0; i2 < r.related.size(); i2++) {
                const xml::Relate& relate = *r.related[i2];
                string extra;

                DbField* fld = new DbField;
                if (relate.unique)
                   fld->unique = true;

                else if (r.related.size() == 2) { 
                    if (i2 == 0 && r.related[1]->limit)
                        fld->unique = true;
                    if (i2 == 1 && r.related[0]->limit)
                        fld->unique = true;
                }
                fld->name = relate.fieldName;
                fld->type = typeMap["integer"];
                tbl->fields.push_back(fld);
                objFields.push_back(fld);

                DbIndex* idx = new DbIndex;
                idx->name = makeDbName(tbl->name + fld->name + "idx");
                idx->table = tbl->name;
                idx->fields.push_back(fld);
                db.indices.push_back(idx);

            }
            for (size_t i2 = 0; i2 < r.fields.size(); i2++) {
                Field& f = *r.fields[i2];
                DbField* fld = new DbField;
                fld->name = f.name + "_";
                fldMap[f.name] = fld;
                fld->type = f.type;
                fld->primaryKey = false;
                fld->unique = f.unique;
                fld->field = r.fields[i2];
                tbl->fields.push_back(fld);

                if (f.indexed) {
                    DbIndex* idx = new DbIndex;
                    idx->name = makeDbName(tbl->name + fld->name + "idx");
                    idx->table = tbl->name;
                    idx->fields.push_back(fld);
                    db.indices.push_back(idx);
                }

            }

            if (r.related.size() > 1) {

                DbIndex* idx = new DbIndex;
                idx->name = makeDbName(tbl->name + "_all_idx");
                idx->table = tbl->name;
                for (size_t i2 = 0; i2 < objFields.size(); i2++)
                    idx->fields.push_back(objFields[i2]);
                db.indices.push_back(idx);
            }
            for (size_t i2 = 0; i2 < r.indices.size(); i2++) {
                const Index& idx = *r.indices[i2];

                litesql::Split fldNames;
                DbIndex* index = new DbIndex;
                for (size_t i3 = 0; i3 < idx.fields.size(); i3++) {
                    DbField* fld = new DbField;
                    if (fldMap.find(idx.fields[i3]->name) == fldMap.end())
                        throw XMLExcept(idx.fields[i3]->pos,
                                "Indexfield " + r.name + "." + idx.fields[i3]->name + " is invalid.");
                    index->fields.push_back(fldMap[idx.fields[i3]->name]);
                    fldNames.push_back(idx.fields[i3]->name);
                }

                index->name = makeDbName(tbl->name + "_" + fldNames.join("_") + "_idx");
                string unique = "";
                if (idx.unique)
                    index->unique = true;
                db.indices.push_back(index);
            }
        }

    }
    static string makeRelationName(const Relation& r) {
        if (r.name.size() == 0) {
            string result;
            for (size_t i = 0; i < r.related.size(); i++)
                result += r.related[i]->objectName;
            return result + "Relation" + r.id;
        }
        return r.name;
    }

    static void initBaseTypes(Database& db) {
        Position p("", 0);
        Type* intType    = new Type(p, "integer");
        Type* floatType  = new Type(p, "float");
        Type* doubleType = new Type(p, "double");
        Type* stringType = new Type(p, "string");
        Type* dateType   = new Type(p, "date");
        Type* timeType   = new Type(p, "time");
        Type* dateTimeType = new Type(p, "datetime");
        Type* boolType = new Type(p, "boolean");

        intType->stores.push_back(new Store(p, "INTEGER", ""));
        floatType->stores.push_back(new Store(p, "FLOAT", ""));
        doubleType->stores.push_back(new Store(p, "FLOAT", ""));
        stringType->stores.push_back(new Store(p, "TEXT", ""));
        dateType->stores.push_back(new Store(p, "DATE", ""));
        timeType->stores.push_back(new Store(p, "TIME", ""));
        dateTimeType->stores.push_back(new Store(p, "DATETIME", ""));
        boolType->stores.push_back(new Store(p, "SMALLINT", ""));

        intType->represents.push_back(new Represent(p, "int", "c++"));
        floatType->represents.push_back(new Represent(p, "float", "c++"));
        doubleType->represents.push_back(new Represent(p, "double", "c++"));
        stringType->represents.push_back(new Represent(p, "std::string", 
                                                       "c++"));
        dateType->represents.push_back(new Represent(p, "litesql::Date","c++"));
        timeType->represents.push_back(new Represent(p,"litesql::Time", "c++"));
        dateTimeType->represents.push_back(new Represent(p,"litesql::DateTime", 
                                                         "c++"));

        intType->represents.push_back(new Represent(p, "int", "python"));
        floatType->represents.push_back(new Represent(p, "float", "python"));
        doubleType->represents.push_back(new Represent(p, "float", "python"));
        stringType->represents.push_back(new Represent(p, "str", "python"));
        dateType->represents.push_back(new Represent(p, "litesql.Date",
                                                        "python"));
        timeType->represents.push_back(new Represent(p,"litesql.Time", 
                                                        "python"));
        dateTimeType->represents.push_back(new Represent(p,"litesql.DateTime", 
                                                           "python"));
                                                         
    }

    static void init(Database& db) {
        vector<Object*>& objects = db.objects;
        vector<Relation*>& relations = db.relations;


        map<string, Object*> objMap;
        sanityCheck(db, objects, relations);
        // make string -> Object mapping

        for (size_t i = 0; i < objects.size(); i++)
            objMap[objects[i]->name] = objects[i];

        // make Object's class hierarchy mapping (parent and children)

        for (size_t i = 0; i < objects.size(); i++) 
            if (objMap.find(objects[i]->inherits) != objMap.end())
                objects[i]->parentObject = objMap[objects[i]->inherits];

        map<string, Type*> typeMap;

        initBaseTypes(db);

        for (size_t i = 0; i < db.types.size(); i++) {
            Type* type = db.types[i];
            typeMap[type->name] = type;
        }

        for (size_t i = 0; i < objects.size(); i++)  {
            if (objects[i]->parentObject)
                objects[i]->parentObject->children.push_back(objects[i]);
            int objOffset = objects[i]->getLastFieldOffset();
            for (size_t i2 = 0; i2 < objects[i]->fields.size(); i2++) {
                xml::Field* fld = objects[i]->fields[i2];
                if (typeMap.find(fld->typeName) == typeMap.end())
                    throw XMLExcept(fld->pos, "Type is invalid : " + fld->typeName);
                fld->type = typeMap[fld->typeName];

                fld->offset = objOffset - objects[i]->fields.size() + i2;
            }
        }

        // sort objects of relations alphabetically (ascii)

        for (size_t i = 0; i < relations.size(); i++) {
            sort(relations[i]->related.begin(), relations[i]->related.end(),
                    less<Relate*>());
        }

        for (size_t i = 0; i < relations.size(); i++) {
            Relation& rel = *relations[i];
            bool same = rel.maxSameTypes() > 1;
            rel.name = makeRelationName(rel);

            for (size_t i2 = 0; i2 < rel.related.size(); i2++) {
                Relate& relate = *rel.related[i2];
                Object* obj = objMap[relate.objectName];
                relate.paramPos = i2;

                string num;
                if (same)
                    num = litesql::toString(i2 + 1);
                relate.fieldTypeName = relate.objectName + num;
                relate.fieldName = relate.objectName + litesql::toString(i2 + 1);
                relate.object = obj;
                if (obj->relations.find(&rel) == obj->relations.end())
                    obj->relations[&rel] = vector<Relate*>();

                // make Object -> Relation mapping

                obj->relations[&rel].push_back(&relate);
                if (!relate.handle.empty()) {

                    // make Object's relation handles

                    RelationHandle* handle = new RelationHandle(relate.pos, 
                            relate.handle, &rel,
                            &relate, obj);
                    for (size_t i3 = 0; i3 < rel.related.size(); i3++) {
                        if (i2 != i3) {
                            Object* o = objMap[rel.related[i3]->objectName];
                            // make RelationHandle -> (Object,Relation) mapping
                            handle->destObjects.push_back(make_pair(o,rel.related[i3]));
                        }
                    }
                    obj->handles.push_back(handle);
                }
            }
        }


        initSchema(db, typeMap);

    }


    Database* parse(const string& fName) {
        yyin = fopen(fName.c_str(), "r");
        if (!yyin)
            throw litesql::Except("Could not open file: " + fName);


        yylex();
        fclose(yyin);

        init(*xmlReaderDb);
        return xmlReaderDb;
    }
}


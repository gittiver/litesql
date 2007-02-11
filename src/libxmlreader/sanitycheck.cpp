#include "sanitycheck.hpp"

namespace xml {
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

    static void checkRelated(Relation* r,
            vector<Relate*>& related, 
            const set<string>& objectNames,
            const set<string>& interfaceNames,
            set<string>& usedNames) {

        bool uniques = false, limits = false;
        string err;
        bool hasInterface = false, hasObjectHandles = false;
        for (size_t i = 0; i < related.size(); i++) {
            Relate& rel = *related[i];

            // valid relation handle name?
            if (!(err = validFieldId(rel.handle)).empty() 
                    && !rel.handle.empty())
                throw XMLExcept(rel.pos, 
                        "invalid id: relation.relate.handle : " 
                        + r->name + "." + rel.handle + " : " + err);

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
                            + r->name + "." + rel.objectName);

                // object has id already?
                string id = rel.objectName + "." + rel.handle;
                if (usedNames.find(id) != usedNames.end())
                    throw XMLExcept(rel.pos, "object " + rel.objectName
                            + " already has identifier "
                            + rel.handle);
                usedNames.insert(id); 

                if (!rel.handle.empty())
                    hasObjectHandles = true;

            } else if (!rel.interfaceName.empty()) {
    
                // already has an interface?
                if (hasInterface)
                    throw XMLExcept(rel.pos, 
                                    "multiple interfaces included in "
                                    "a relation");
                hasInterface = true;

                // valid interface reference in relate?
                if (interfaceNames.find(rel.interfaceName) 
                        == interfaceNames.end())
                    throw XMLExcept(rel.pos, "unknown interface: "
                            "relation.relate.interface : " 
                            + r->name + "." + rel.objectName);
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
                usedNames.insert(r->name + "." + rel.handle);

            if (rel.unique)
                uniques = true;
            if (rel.limit)
                limits = true;

        }

        // only uniques or limits?
        if (uniques && limits)
            throw XMLExcept(r->pos, "both 'unique' and 'limit' "
                    "attributes used in relation " 
                    + r->name);

        // object handles and interface?
        if (hasObjectHandles && hasInterface)
            throw XMLExcept(r->pos, "relation handles attached to "
                                   "objects cannot be used when "
                                   "an interface is included in the "
                                   "relation");
    }

    static void checkTypes(vector<Type*>& types) {
        for (size_t i = 0; i < types.size(); i++) {
            Type* t = types[i];
            set<string> tnames;
            // represent target names ok?
            for (size_t i2 = 0; i2 < t->represents.size(); i2++) {
                Represent* r = t->represents[i2];
                if (!r->target.empty())
                    check(validTarget, "type.represent.target", r->pos, 
                            r->target);
                // no duplicates?                            
                if (tnames.find(r->target) != tnames.end())
                    throw XMLExcept(r->pos, "type representation specified "
                                            "twice for target : " + r->target);

                tnames.insert(r->target);                            
            }

            // store backend names ok?
            set<string> bnames;
            for (size_t i2 = 0; i2 < t->stores.size(); i2++) {
                Store* s = t->stores[i2];
                if (!s->backend.empty())
                    check(validBackend, "type.store.backend", s->pos, 
                            s->backend);
                // no duplicates?                            
                if (bnames.find(s->backend) != bnames.end())
                    throw XMLExcept(s->pos, "type storage info specified "
                                            "twice for backend : " 
                                            + s->backend);

                bnames.insert(s->backend);                           
                           
            } 
            set<string> valueNames;
            checkNames(t->values, validId, valueNames, "type.value");

            // check function names ok?
            for (size_t i2 = 0; i2 < t->checks.size(); i2++) {
                check(validId, "type.check.function", 
                        t->checks[i2]->pos, t->checks[i2]->function);

                // no triggers?
                
                if (t->checks[i2]->constrained)
                    throw XMLExcept(t->checks[i2]->pos,
                                    "check functions attached to "
                                    "types cannot have triggers");
            }


        }
    }

    static void checkOptions(vector<Option*>& options) {    
        static char* optionNames[] = {"storagetype", NULL};
        
        set<string> used;
        for (size_t i = 0; i < options.size(); i++) {
            Option* o = options[i];
            if (!inWords(o->name, optionNames))
                throw XMLExcept(o->pos, "unknown option : " + o->name);

            if (used.find(o->name) != used.end())                
                throw XMLExcept(o->pos, "option specified twice : " + o->name);

            used.insert(o->name);

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

    static void checkInheritance(Object* o, map<string, Object*>& objMap) {

        set<Object*> parents;

        while (!o->inherits.empty()) {
            if (objMap.find(o->inherits) == objMap.end())
                throw XMLExcept(o->pos, "unknown object : object.inherits : "
                        + o->inherits);
            o = objMap[o->inherits];
            if (parents.find(o) != parents.end())
                throw XMLExcept(o->pos, "circular inheritance detected");
            parents.insert(o);
        }
    }


    void sanityCheck(Database* db_) {
        Database& db = *db_;
    
        using namespace litesql;
        set<string> usedNames, typeNames;
        set<string> objectNames;
        set<string> interfaceNames;
        string err;

        // database attributes ok?

        check(validId, "database.name", db.pos, db.name);
        check(validId, "database.nspace", db.pos, db.name);

        // object names ok?
        checkNames(db.objects, validId, usedNames, "object");

        // type names ok?
        checkNames(db.types, validId, typeNames, "type");
        checkTypes(db.types);

        addNames(db.objects, objectNames);
        addNames(db.interfaces, interfaceNames);

        checkOptions(db.options);

        map<string, Object*> objMap;
        for (size_t i = 0; i < db.objects.size(); i++)
            objMap[db.objects[i]->name] = db.objects[i];


        for (size_t i = 0; i < db.objects.size(); i++) {
            Object& o = *db.objects[i];

            // object inherits ok?

            checkInheritance(db.objects[i], objMap);                        

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
            // no duplicates?
            set<string> inames;
            for (size_t i2 = 0; i2 < o.implements.size(); i2++) {
                string iname = o.implements[i2]->interfaceName;
                if (interfaceNames.find(iname) == interfaceNames.end())
                    throw XMLExcept(o.implements[i2]->pos, 
                            "unknown interface : object.implements : "
                            + iname);
                if (inames.find(iname) != inames.end())
                    throw XMLExcept(o.implements[i2]->pos,
                            "interface implemented twice : " + iname);

                inames.insert(iname);                            
            }

            for (size_t i2 = 0; i2 < o.indices.size(); i2++) {
                Index* idx = o.indices[i2];
                set<string> names;

                // index field names ok?
                checkNames(idx->fields, validFieldId, names, 
                        "index.indexfield");

                // index field references ok?                           
                set<string> ifnames;
                for (size_t i3 = 0; i3 < idx->fields.size(); i3++) {
                    IndexField* ifld = idx->fields[i3];
                    if (fieldNames.find(ifld->name) == fieldNames.end())
                        throw XMLExcept(ifld->pos, 
                                "unknown field name "
                                ": indexfield.name : " + ifld->name);

                    // duplicates?
                    if (ifnames.find(ifld->name) != ifnames.end())
                        throw XMLExcept(ifld->pos,
                                        "field indexed twice : "
                                        + ifld->name);
                    ifnames.insert(ifld->name);                                
                }
            }


            checkOptions(o.options);

            // check function names ok?
            for (size_t i2 = 0; i2 < o.checks.size(); i2++)  {
                check(validId, "object.check.function", 
                        o.checks[i2]->pos, o.checks[i2]->function);
                if (o.checks[i2]->onLink || o.checks[i2]->onUnlink)
                    throw XMLExcept(o.checks[i2]->pos,
                                    "check functions attached to "
                                    "objects cannot have onlink or onunlink "
                                    "triggers");
            }
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
        checkNames(db.relations, validId, usedNames, "relation");

        for (size_t i = 0; i < db.relations.size(); i++) {
            Relation& r = *db.relations[i];

            // relation id ok?
            check(validId, "relation.id", r.pos, r.id);           

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

            checkRelated(&r, 
                    r.related, objectNames, interfaceNames, usedNames);

            // limit only in binary relations?
            if (r.related.size() != 2 && limits)
                throw XMLExcept(r.pos, "'limit' attribute "
                        "used in relation of " 
                        + toString(r.related.size()) 
                        + " object(s) " + r.name);

            // check function names ok?
            for (size_t i2 = 0; i2 < r.checks.size(); i2++)  {
                check(validId, "object.check.function", 
                        r.checks[i2]->pos, r.checks[i2]->function);
                if (r.checks[i2]->onUpdate || r.checks[i2]->onDelete
                     || r.checks[i2]->onCreate)
                    throw XMLExcept(r.checks[i2]->pos,
                                    "check functions attached to "
                                    "relations cannot have oncreate, ondelete "
                                    "or onupdate triggers");
            }
        }   
    }
}

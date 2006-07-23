#include "../library/compatibility.hpp"
#include "xmlobjects.hpp"
#include "litesql-gen-main.hpp"
#include "md5.hpp"
#include "litesql.hpp"
#include <algorithm>
namespace std {
    template <>
    struct less<xml::Relate*> {
        bool operator()(xml::Relate const* r1, xml::Relate const* r2) {
            if (!r1)
                return true;
            if (!r2)
                return false;
            return *r1 < *r2;
        }
    };
}
namespace xml {
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
         "initValues", "insert", "addUpdates", "addIDUpdates",
         "getFieldTypes", "delRecord", "delRelations",
         "update", "del", "typeIsCorrect", "upcast", "upcastCopy"
        };

    for (size_t i = 0; i < sizeof(words) / sizeof(words[0]); i++)
        if (s == words[i])
            return true;
    return false; 
}
bool validTarget(string t) {
    if (t == "c++")
        return true;
    if (t == "python")
        return true;
    if (t == "graphviz")
        return true;
    return false;
}
bool validBackend(string t) {
    if (t == "postgresql")
        return true;
    if (t == "sqlite3")
        return true;
    if (t == "mysql")
        return true;
    return false;
}
string validID(string s, string type="field") {
    if (s.size() == 0) 
        return "empty identifier";
    if (toupper(s[0]) == s[0] && type == "field")
        return "does not begin with lower case letter";
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
string capitalize(const string& s) {
    if (s.size() == 0)
        return s;
    char buf[2] = {toupper(s[0]), 0};
    return string(buf) + s.substr(1, s.size());
}
string decapitalize(const string& s) {
    if (s.size() == 0)
        return s;
    char buf[2] = {tolower(s[0]), 0};
    return string(buf) + s.substr(1, s.size());
}
string makeDBName(const string& s) {
    if (s.size() > 31)
        return "_" + md5HexDigest(s);
    return s;
}
void initBaseTypes(Database& db) {
    Type* types[] = {new Type("integer", "int", "INTEGER"),
                    new Type("float", "float", "FLOAT"),
                    new Type("double", "double", "FLOAT"),
                    new Type("string", "string", "TEXT"),
                    new Type("boolean", "bool", "SMALLINT"),
                    new Type("date", "Date", "DATE"),
                    new Type("time", "Time", "VARCHAR(10)"),
                    new Type("datetime", "DateTime", "TIMESTAMP")};

    Type* cppTypes[] = {new Type("string", "std::string", "TEXT"),
                    new Type("date", "litesql::Date", "DATE"),
                    new Type("time", "litesql::Time", "VARCHAR(10)"),
                    new Type("datetime", "litesql::DateTime", "TIMESTAMP")};
    Type* pythonTypes[] = {new Type("double", "float", "FLOAT"),
                    new Type("string", "str", "TEXT"),
                    new Type("date", "litesql.Date", "DATE"),
                    new Type("time", "litesql.Time", "VARCHAR(10)"),
                    new Type("datetime", "litesql.DateTime", "TIMESTAMP")};
    IfTarget* cpp = new IfTarget("c++");
    IfTarget* python = new IfTarget("python");

    for (size_t i = 0; i < sizeof(types) / sizeof(types[0]); i++)
        db.types.push_back(types[i]);

    for (size_t i = 0; i < sizeof(cppTypes) / sizeof(cppTypes[0]); i++)
        cpp->types.push_back(cppTypes[i]);

    for (size_t i = 0; i < sizeof(pythonTypes) / sizeof(pythonTypes[0]); i++)
        python->types.push_back(pythonTypes[i]);

    db.ifTargets.push_back(cpp);
    db.ifTargets.push_back(python);
}
static void sanityCheck(Database& db,
                        vector<Object*>& objects,
                        vector<Relation*>& relations) {
    using namespace litesql;
    map<string, bool> usedID;
    map<string, bool> objectName;
    string err;
        
    if (!(err = validID(db.name,"class")).empty()) 
        throw XMLExcept(db.pos, "invalid id: database.name : " + db.name + " : " + err);
    for (size_t i = 0; i < db.ifTargets.size(); i++) {
        IfTarget* it = db.ifTargets[i];
        if (!validTarget(it->name))
            throw XMLExcept(it->pos, "invalid target : " + it->name);
    }
    for (size_t i = 0; i < db.ifBackends.size(); i++) {
        IfBackend* it = db.ifBackends[i];
        if (!validBackend(it->name))
            throw XMLExcept(it->pos, "invalid backend : " + it->name);
    }
        
    for (size_t i = 0; i < objects.size(); i++) {
        Object& o = *objects[i];
        if (!(err = validID(o.name, "class")).empty())
            throw XMLExcept(o.pos, "invalid id: object.name : " + o.name + " : " + err);
        if (usedID.find(o.name) != usedID.end())
            throw XMLExcept(o.pos, "duplicate id: object.name : " + o.name);
        usedID[o.name] = true;
        objectName[o.name] = true;
        map<string, bool> usedField;
        usedField.clear();
        for (size_t i2 = 0; i2 < o.fields.size(); i2++) {
            Field& f = *o.fields[i2];

            if (!(err = validID(f.name)).empty())
                throw XMLExcept(o.pos, "invalid id: object.field.name : " + o.name + "." + f.name + " : " + err);
            if (usedField.find(f.name) != usedField.end())
                throw XMLExcept(o.pos, "duplicate id: object.field.name : " + o.name + "." + f.name);
            usedField[f.name] = true;
        }
    }
    for (size_t i = 0; i < relations.size(); i++) {
        Relation& r = *relations[i];
        string name = r.getName();
        if (!(err = validID(name,"class")).empty())
            throw XMLExcept(r.pos, "invalid id: relation.name : " + name + " : " + err);
        if (usedID.find(name) != usedID.end())
            throw XMLExcept(r.pos, "duplicate id: relation.name : " + name);
        usedID[name] = true;
        map<string, bool> usedField;
        usedField.clear();
  
        bool defaults = false;
        
        for (size_t i2 = 0; i2 < r.fields.size(); i2++) {
            Field& f = *r.fields[i2];
            if (!(err = validID(f.name)).empty())
                throw XMLExcept(f.pos, "invalid id: relation.field.name : " + name + "." + f.name + " : " + err);
            if (usedField.find(f.name) != usedField.end())
                throw XMLExcept(f.pos, "duplicate id: relation.field.name : " + name + "." + f.name);
            usedField[f.name] = true;
            if (f.default_.size() > 0)
                defaults = true;
            else if (defaults)
                throw XMLExcept(f.pos, "no default-value after field with default value : " + name + "." + f.name);

        }
        usedField.clear();
        bool limits = false;
        bool uniques = false;
        for (size_t i2 = 0; i2 < r.related.size(); i2++) {
            Relate& rel = *r.related[i2];
            if (!(err = validID(rel.handle)).empty() && !rel.handle.empty())
                throw XMLExcept(rel.pos, "invalid id: relation.relate.handle : " + name + "." + rel.handle + " : " + err);
            if (usedField.find(rel.handle) != usedField.end())
                throw XMLExcept(rel.pos, "duplicate id: relation.relate.handle : " + name + "." + rel.handle);
            if (objectName.find(rel.objectName) == objectName.end())
                throw XMLExcept(rel.pos, "unknown object: relation.relate.name : " + name + "." + rel.objectName);
            if (!rel.handle.empty())
                usedField[rel.handle] = true;
            if (rel.isUnique())
                uniques = true;
            if (rel.hasLimit())
                limits = true;
            if (uniques && limits)
                throw XMLExcept(rel.pos, "both 'unique' and 'limit' attributes used in relation " + name);
        }
        if (r.related.size() != 2 && limits)
            throw XMLExcept(r.pos, "'limit' attribute used in relation of " + toString(r.related.size()) 
                         + " object(s) " + name);
            
    }   
}

static void initSchema(Database& db) {
    vector<Object*>& objects = db.objects;
    vector<Relation*>& relations = db.relations;

    for (size_t i = 0; i < objects.size(); i++) {
        Object& o = *objects[i];
        map<string, Database::DBField*> fldMap;
        Database::Table* tbl = new Database::Table;
        tbl->name = o.getTable();
        db.tables.push_back(tbl);
        
        if (!o.parentObject) {
            Database::Sequence* seq = new Database::Sequence;
            seq->name = o.getSequence();
            seq->table = o.getTable();
            db.sequences.push_back(seq);
        }  else {
            Database::DBField *id = new Database::DBField; 
            id->name = "id_";
            id->type = "INTEGER";
            id->primaryKey = true;
            tbl->fields.push_back(id);
        }

        for (size_t i2 = 0; i2 < o.fields.size(); i2++) {
            Field& f = *o.fields[i2];
           Database::DBField* fld = new Database::DBField;
            fld->name = f.name + "_";
            fldMap[f.name] = fld;
            fld->type = f.getSQLType();
            fld->primaryKey = (f.name == "id");
            if (f.isUnique())
                fld->extra = " UNIQUE";
            fld->field = o.fields[i2];
            tbl->fields.push_back(fld);
            
            if (f.isIndexed()) {
                Database::DBIndex* idx = new Database::DBIndex;
                idx->name = makeDBName(tbl->name + fld->name + "idx");
                idx->table = tbl->name;
                idx->fields.push_back(fld);
                db.indices.push_back(idx);
            }
                
        }
        for (size_t i2 = 0; i2 < o.indices.size(); i2++) {
            const Index& idx = *o.indices[i2];

            litesql::Split fldNames;
            Database::DBIndex* index = new Database::DBIndex;
            for (size_t i3 = 0; i3 < idx.fields.size(); i3++) {
                if (fldMap.find(idx.fields[i3].name) == fldMap.end())
                    throw XMLExcept(idx.fields[i3].pos, 
                                  "Indexfield " + o.name + "." + idx.fields[i3].name + " is invalid.");
                index->fields.push_back(fldMap[idx.fields[i3].name]);
                fldNames.push_back(idx.fields[i3].name);
            }

            index->name = makeDBName(tbl->name + "_" + fldNames.join("_") + "_idx");
            string unique = "";
            if (idx.isUnique())
                index->unique = true;
            db.indices.push_back(index);
        }
    }
    for (size_t i = 0; i < relations.size(); i++) {
        Relation& r = *relations[i];
        Database::Table* tbl = new Database::Table;
        db.tables.push_back(tbl);
        tbl->name = r.getTable();
        vector<Database::DBField*> objFields;
        map<string, Database::DBField*> fldMap;
        for (size_t i2 = 0; i2 < r.related.size(); i2++) {
            const xml::Relate& relate = *r.related[i2];
            string extra;
            if (relate.isUnique())
                    extra = " UNIQUE";
            else if (r.related.size() == 2) { 
                if (i2 == 0 && r.related[1]->hasLimit())
                    extra = " UNIQUE";
                if (i2 == 1 && r.related[0]->hasLimit())
                    extra = " UNIQUE";
            }
            Database::DBField* fld = new Database::DBField;
            fld->name = relate.fieldName;
            fld->type = "INTEGER";
            fld->extra = extra;
            tbl->fields.push_back(fld);
            objFields.push_back(fld);
            
            Database::DBIndex* idx = new Database::DBIndex;
            idx->name = makeDBName(tbl->name + fld->name + "idx");
            idx->table = tbl->name;
            idx->fields.push_back(fld);
            db.indices.push_back(idx);
            
        }
        for (size_t i2 = 0; i2 < r.fields.size(); i2++) {
            Field& f = *r.fields[i2];
            Database::DBField* fld = new Database::DBField;
            fld->name = f.name + "_";
            fldMap[f.name] = fld;
            fld->type = f.getSQLType();
            fld->primaryKey = false;
            if (f.isUnique())
                fld->extra = " UNIQUE";
            fld->field = r.fields[i2];
            tbl->fields.push_back(fld);
            
            if (f.isIndexed()) {
                Database::DBIndex* idx = new Database::DBIndex;
                idx->name = makeDBName(tbl->name + fld->name + "idx");
                idx->table = tbl->name;
                idx->fields.push_back(fld);
                db.indices.push_back(idx);
            }
            
        }
    
        if (r.related.size() > 1) {

            Database::DBIndex* idx = new Database::DBIndex;
            idx->name = makeDBName(tbl->name + "_all_idx");
            idx->table = tbl->name;
            for (size_t i2 = 0; i2 < objFields.size(); i2++)
                idx->fields.push_back(objFields[i2]);
            db.indices.push_back(idx);
        }
        for (size_t i2 = 0; i2 < r.indices.size(); i2++) {
            const Index& idx = *r.indices[i2];

            litesql::Split fldNames;
            Database::DBIndex* index = new Database::DBIndex;
            for (size_t i3 = 0; i3 < idx.fields.size(); i3++) {
                Database::DBField* fld = new Database::DBField;
                if (fldMap.find(idx.fields[i3].name) == fldMap.end())
                    throw XMLExcept(idx.fields[i3].pos,
                               "Indexfield " + r.name + "." + idx.fields[i3].name + " is invalid.");
                index->fields.push_back(fldMap[idx.fields[i3].name]);
                fldNames.push_back(idx.fields[i3].name);
            }

            index->name = makeDBName(tbl->name + "_" + fldNames.join("_") + "_idx");
            string unique = "";
            if (idx.isUnique())
                index->unique = true;
            db.indices.push_back(index);
        }
    }

}
void init(Database& db, Args& args) {
    vector<Object*>& objects = db.objects;
    vector<Relation*>& relations = db.relations;


    map<string, Object*> objMap;
    report("validating XML file\n");
    sanityCheck(db, objects, relations);
    report("linking XML - objects\n"); 
    // make string -> Object mapping

    for (size_t i = 0; i < objects.size(); i++)
        objMap[objects[i]->name] = objects[i];

    // make Object's class hierarchy mapping (parent and children)

    for (size_t i = 0; i < objects.size(); i++) 
        if (objMap.find(objects[i]->inherits) != objMap.end())
            objects[i]->parentObject = objMap[objects[i]->inherits];

    for (size_t i = 0; i < db.ifTargets.size(); i++) {
        IfTarget* it = db.ifTargets[i];
        
        if (it->name == args["target"]) {
            addVector(it->options, db.options);
            addVector(it->types, db.types);
        }
    }


    map<string, Type*> typeMap;

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
        bool same = rel.sameTypes() > 1;
        
        for (size_t i2 = 0; i2 < rel.related.size(); i2++) {
            Relate& relate = *rel.related[i2];
            Object* obj = objMap[relate.objectName];
            relate.paramPos = i2;

            string num;
            if (same)
                num = toString(i2 + 1);
            relate.fieldTypeName = relate.objectName + num;
            relate.fieldName = relate.objectName + toString(i2 + 1);
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
    

    initSchema(db);

}
string safe(const char * s) {
    if (s)
        return s;
    return "";
}
}

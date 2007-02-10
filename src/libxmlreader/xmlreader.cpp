#include "xmlreader.hpp"
#include "xmlreader-actions.hpp"
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
            "update", "del", "upcast", "upcastCopy"
        };

        for (size_t i = 0; i < sizeof(words) / sizeof(words[0]); i++)
            if (s == words[i])
                return true;
        return false; 
    }
    static bool validTarget(string t) {
        if (t == "c++")
            return true;
        if (t == "python")
            return true;
        if (t == "graphviz")
            return true;
        return false;
    }
    static bool validBackend(string t) {
        if (t == "postgresql")
            return true;
        if (t == "sqlite3")
            return true;
        if (t == "mysql")
            return true;
        return false;
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

    static void sanityCheck(Database& db,
            vector<Object*>& objects,
            vector<Relation*>& relations) {
        using namespace litesql;
        map<string, bool> usedID;
        map<string, bool> objectName;
        string err;

        if (!(err = validId(db.name)).empty()) 
            throw XMLExcept(db.pos, "invalid id: database.name : " + db.name + " : " + err);

        for (size_t i = 0; i < objects.size(); i++) {
            Object& o = *objects[i];
            if (!(err = validId(o.name)).empty())
                throw XMLExcept(o.pos, "invalid id: object.name : " + o.name + " : " + err);
            if (usedID.find(o.name) != usedID.end())
                throw XMLExcept(o.pos, "duplicate id: object.name : " + o.name);
            usedID[o.name] = true;
            objectName[o.name] = true;
            map<string, bool> usedField;
            usedField.clear();
            for (size_t i2 = 0; i2 < o.fields.size(); i2++) {
                Field& f = *o.fields[i2];

                if (!(err = validFieldId(f.name)).empty())
                    throw XMLExcept(o.pos, "invalid id: object.field.name : " + o.name + "." + f.name + " : " + err);
                if (usedField.find(f.name) != usedField.end())
                    throw XMLExcept(o.pos, "duplicate id: object.field.name : " + o.name + "." + f.name);
                usedField[f.name] = true;
            }
        }
        for (size_t i = 0; i < relations.size(); i++) {
            Relation& r = *relations[i];
            string name = r.name;
            if (!(err = validId(name)).empty())
                throw XMLExcept(r.pos, "invalid id: relation.name : " + name + " : " + err);
            if (usedID.find(name) != usedID.end())
                throw XMLExcept(r.pos, "duplicate id: relation.name : " + name);
            usedID[name] = true;
            map<string, bool> usedField;
            usedField.clear();

            bool defaults = false;

            for (size_t i2 = 0; i2 < r.fields.size(); i2++) {
                Field& f = *r.fields[i2];
                if (!(err = validFieldId(f.name)).empty())
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
                if (!(err = validFieldId(rel.handle)).empty() && !rel.handle.empty())
                    throw XMLExcept(rel.pos, "invalid id: relation.relate.handle : " + name + "." + rel.handle + " : " + err);
                if (usedField.find(rel.handle) != usedField.end())
                    throw XMLExcept(rel.pos, "duplicate id: relation.relate.handle : " + name + "." + rel.handle);
                if (objectName.find(rel.objectName) == objectName.end())
                    throw XMLExcept(rel.pos, "unknown object: relation.relate.name : " + name + "." + rel.objectName);
                if (!rel.handle.empty())
                    usedField[rel.handle] = true;
                if (rel.unique)
                    uniques = true;
                if (rel.limit)
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
                id->type = "INTEGER";
                id->primaryKey = true;
                tbl->fields.push_back(id);
            }

            for (size_t i2 = 0; i2 < o.fields.size(); i2++) {
                Field& f = *o.fields[i2];
                DbField* fld = new DbField;
                fld->name = f.name + "_";
                fldMap[f.name] = fld;
                fld->type = f.getSQLType();
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
                    if (fldMap.find(idx.fields[i3]->name) == fldMap.end())
                        throw XMLExcept(idx.fields[i3]->pos, 
                                "Indexfield " + o.name + "." + idx.fields[i3]->name + " is invalid.");
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
                fld->type = "INTEGER";
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
                fld->type = f.getSQLType();
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


        initSchema(db);

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


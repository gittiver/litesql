#include "litesql-xmlreader.hpp"
#include "flexml-header.hpp"
#include "scanner.hpp"
#include "stringutils.hpp"
#include "sanitycheck.hpp"

namespace xml {
    using namespace std;

    extern Database* xmlReaderDb;
    extern string currentFile;


    
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

    static RelationHandle* makeRelationHandle(Relation* r,
                                              Relate* fromRelate,
                                              Object* obj) {

        RelationHandle* handle = new RelationHandle(fromRelate->pos, 
                       fromRelate->handle, r, fromRelate, obj);

        for (size_t i3 = 0; i3 < r->related.size(); i3++) {
            if (fromRelate != r->related[i3]) {
                Object* o = r->related[i3]->object;
                // make RelationHandle -> (Object,Relation) mapping

                handle->destObjects
                    .push_back(make_pair(o,r->related[i3]));
            }
        }
        return handle;
    }

    static Relation* createInterfaceRelation(Relation* old, Object* o) {
        // replaces interface parts with given object
        Relation* r = old->clone();
        for (size_t i = 0; i < r->related.size(); i++) {
            Relate* rel = r->related[i];
            if (rel->interface) {
                rel->interface = NULL;
                rel->interfaceName = "";
                rel->object = o;
                rel->objectName = o->name;
            }
        }

        r->abstract = false;
        r->sortRelated();

        return r;
    }

    static void registerRelation(Relation* rel,
                                 Relate* relate) {
        Object* obj = relate->object;

        if (obj->relations.find(rel) == obj->relations.end())
            obj->relations[rel] = vector<Relate*>();

        // make Object -> Relation mapping

        obj->relations[rel].push_back(relate);

        if (!relate->handle.empty())
            obj->handles.push_back(
                    makeRelationHandle(rel, relate, obj));
    }

    static void generateImplementation(Database& db,
                                       Relation* absRel,
                                       Implementation* imp) {

        Interface* iface = imp->interface;
        Object* obj = imp->object;
        Relation* rel = createInterfaceRelation(absRel, obj);
        db.relations.push_back(rel);

        for (size_t i = 0; i < rel->related.size(); i++) {
            Relate* relate = rel->related[i];
            registerRelation(rel, relate); 
        }
       
    }


    static void linkRelations(Database& db) {
               
        for (size_t i = 0; i < db.relations.size(); i++) {
            Relation& rel = *db.relations[i];
            Interface* iface = NULL;
            bool same = rel.maxSameTypes() > 1;

            for (size_t i2 = 0; i2 < rel.related.size(); i2++) {
                if (!rel.related[i2]->interfaceName.empty()) {
                    rel.abstract = true;
                    iface = rel.related[i2]->interface;
                    break;
                }
            }

            if (!rel.abstract) {
                rel.sortRelated();

                for (size_t i2 = 0; i2 < rel.related.size(); i2++) {
                    Relate& relate = *rel.related[i2];

                    registerRelation(&rel, &relate);    

                }

            } else {

                for (size_t i2 = 0; i2 < iface->implementations.size(); i2++) {
                    Implementation* imp = iface->implementations[i];
                    generateImplementation(db, &rel, imp);
                }
            }
        }
        
    }

    static void init(Database& db) {
        vector<Object*>& objects = db.objects;
        vector<Relation*>& relations = db.relations;


        map<string, Object*> objMap;
        map<string, Interface*> ifaceMap;
        for (size_t i = 0; i < relations.size(); i++)
            if (relations[i]->name.empty())
                relations[i]->name = makeRelationName(*relations[i]);

        sanityCheck(&db);
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
            Object* o = objects[i];

            // Interface -> Implementations mapping

            for (size_t i2 = 0; i2 < o->implementations.size(); i2++) {
                    Implementation* imp = o->implementations[i2];
                    Interface* iface = imp->interface;
                    iface->implementations.push_back(imp);
            }

            // Parentobject -> subclasses mapping
            if (o->parentObject)
                o->parentObject->children.push_back(objects[i]);

            int objOffset = o->getLastFieldOffset();
            for (size_t i2 = 0; i2 < o->fields.size(); i2++) {
                xml::Field* fld = o->fields[i2];
                if (typeMap.find(fld->typeName) == typeMap.end())
                    throw XMLExcept(fld->pos, "Type is invalid : " 
                                    + fld->typeName);
                fld->type = typeMap[fld->typeName];
                fld->offset = objOffset - o->fields.size() + i2;
            }
        }


        for (size_t i = 0; i < relations.size(); i++) {
            Relation*r = relations[i];


            // Relate -> Interface 
            // Relate -> Object        
            for (size_t i2 = 0; i2 < r->related.size(); i2++) {
                Relate* rel = r->related[i2];

                if (!rel->objectName.empty())
                    rel->object = objMap[rel->objectName];

                if (!rel->interfaceName.empty())
                    rel->interface = ifaceMap[rel->interfaceName];
            }

            for (size_t i2 = 0; i2 < r->fields.size(); i2++) {
                xml::Field* fld = r->fields[i2];

                if (typeMap.find(fld->typeName) == typeMap.end())
                    throw XMLExcept(fld->pos, "Type is invalid : " 
                                    + fld->typeName);
                                    
                fld->type = typeMap[fld->typeName];
                fld->offset = r->related.size() + i2;
            }

                
        }

        linkRelations(db);

        initSchema(db, typeMap);
    }


    Database* parse(const string& fName) {
        currentFile = fName;
        yyin = fopen(fName.c_str(), "r");

        if (!yyin)
            throw litesql::Except("Could not open file: " + fName);


        yylex();
        fclose(yyin);

        init(*xmlReaderDb);
        return xmlReaderDb;
    }
}


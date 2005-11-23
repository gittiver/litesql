#include "../library/compatibility.hpp"
#include "xmlobjects.hpp"
#include "litesql-gen-main.hpp"
#include "litesql.hpp"
#include <algorithm>
namespace xml {
bool validID(string s) {
    if (s.size() == 0) 
        return false;
    if (!isalpha(s[0])) 
        return false;
    for (size_t i = 1; i < s.size(); i++)
        if (!isalnum(s[i]) && !s[i] != '_')
            return false;
    if (s[s.size()-1] == '_')
        return false;
    return true;
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

static void sanityCheck(Database& db,
                        vector<Object>& objects,
                        vector<Relation>& relations) {
    using namespace litesql;
    map<string, bool> usedID;
    
    if (!validID(db.name)) 
        throw Except("invalid id: database.name : " + db.name);
    for (size_t i = 0; i < objects.size(); i++) {
        Object& o = objects[i];
        if (!validID(o.name))
            throw Except("invalid id: object.name : " + o.name);
        if (usedID.find(o.name) != usedID.end())
            throw Except("duplicate id: object.name : " + o.name);
        usedID[o.name] = true;
        map<string, bool> usedField;
        usedField.clear();
        for (size_t i2 = 0; i2 < o.fields.size(); i2++) {
            Field& f = o.fields[i2];
            if (!validID(f.name))
                throw Except("invalid id: object.field.name : " + o.name + "." + f.name);
            if (usedField.find(f.name) != usedField.end())
                throw Except("duplicate id: object.field.name : " + o.name + "." + f.name);
            usedField[f.name] = true;
        }
    }
    for (size_t i = 0; i < relations.size(); i++) {
        Relation& r = relations[i];
        string name = r.getName();
        if (!validID(name))
            throw Except("invalid id: relation.name : " + name);
        if (usedID.find(name) != usedID.end())
            throw Except("duplicate id: relation.name : " + name);
        usedID[name] = true;
        map<string, bool> usedField;
        usedField.clear();
  
        bool defaults = false;
        
        for (size_t i2 = 0; i2 < r.fields.size(); i2++) {
            Field& f = r.fields[i2];
            if (!validID(f.name))
                throw Except("invalid id: relation.field.name : " + name + "." + f.name);
            if (usedField.find(f.name) != usedField.end())
                throw Except("duplicate id: relation.field.name : " + name + "." + f.name);
            usedField[f.name] = true;
            if (f.default_.size() > 0)
                defaults = true;
            else if (defaults)
                throw Except("no default-value after field with default value : " + name + "." + f.name);

        }
        usedField.clear();
        bool limits = false;
        bool uniques = false;
        for (size_t i2 = 0; i2 < r.related.size(); i2++) {
            Relate& rel = r.related[i2];
            if (!validID(rel.handle) && !rel.handle.empty())
                throw Except("invalid id: relation.relate.handle : " + name + "." + rel.handle);
            if (usedField.find(rel.handle) != usedField.end())
                throw Except("duplicate id: relation.relate.handle : " + name + "." + rel.handle);
            usedField[rel.handle] = true;
            if (rel.isUnique())
                uniques = true;
            if (rel.hasLimit())
                limits = true;
            if (uniques && limits)
                throw Except("both 'unique' and 'limit' attributes used in relation " + name);
        }
        if (r.related.size() != 2 && limits)
            throw Except("'limit' attribute used in relation of " + toString(r.related.size()) 
                         + " object(s) " + name);
            
    }   
}
void init(Database& db,
          vector<Object>& objects,
          vector<Relation>& relations) {
    map<string, xml::Object*> objMap;
    report("validating XML file\n");
    sanityCheck(db, objects, relations);
    report("linking XML - objects\n"); 
    // make string -> xml::Object mapping

    for (size_t i = 0; i < objects.size(); i++)
        objMap[objects[i].name] = &objects[i];

    // make Object's class hierarchy mapping (parent and children)

    for (size_t i = 0; i < objects.size(); i++) 
        if (objMap.find(objects[i].inherits) != objMap.end())
            objects[i].parentObject = objMap[objects[i].inherits];
    for (size_t i = 0; i < objects.size(); i++) 
        if (objects[i].parentObject)
            objects[i].parentObject->children.push_back(&objects[i]);

    for (size_t i = 0; i < objects.size(); i++) {
        Object& o = objects[i];
    }

    // sort objects of relations alphabetically (ascii)

    for (size_t i = 0; i < relations.size(); i++) 
        sort(relations[i].related.begin(), relations[i].related.end());
            
    for (size_t i = 0; i < relations.size(); i++) {
        xml::Relation& rel = relations[i];
        

        for (size_t i2 = 0; i2 < rel.related.size(); i2++) {
            xml::Relate& relate = rel.related[i2];
            xml::Object* obj = objMap[relate.objectName];
            if (obj->relations.find(&rel) == obj->relations.end())
                obj->relations[&rel] = vector<xml::Relate*>();

            // make Object -> Relation mapping

            obj->relations[&rel].push_back(&relate);
            if (!relate.handle.empty()) {
                
                // make Object's relation handles

                xml::RelationHandle handle(relate.handle, &rel,
                                           &relate, obj);
                for (size_t i3 = 0; i3 < rel.related.size(); i3++) {
                    if (i2 != i3) {
                        xml::Object* o = objMap[rel.related[i3].objectName];
                        // make RelationHandle -> (Object,Relation) mapping
                        handle.destObjects.push_back(make_pair(o,&rel.related[i3]));
                    }
                }
                obj->handles.push_back(handle);
            }
        }
    }
}
string safe(const char * s) {
    if (s)
        return s;
    return "";
}
}

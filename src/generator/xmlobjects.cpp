#include "xmlobjects.hpp"
#include "md5.hpp"
#include "litesql.hpp"
#include "logger.hpp"

#include <algorithm>

namespace xml {

const char* Database::TAG="database";
const char* Object::TAG="object";
const char* Field::TAG="field";
const char* Relation::TAG="relation";
const char* Relate::TAG="relate";
const char* Value::TAG="value";
const char* IndexField::TAG="indexfield";
const char* Index::TAG="index";
const char* Param::TAG="param";
const char* Method::TAG="method";

ObjectPtr Object::DEFAULT_BASE(new Object("litesql::Persistent",""));

Field::Ptr Object::ID_FIELD(new Field("id", A_field_type_integer, "", A_field_indexed_true, A_field_unique_true));
Field::Ptr Object::TYPE_FIELD(new Field("type", A_field_type_string, "", A_field_indexed_false, A_field_unique_false));

   
string validID(const string& s, const string& type="field") {
  string result="";  
  
  if (s.empty()) 
    result = "empty identifier";
  else if (toupper(s[0]) == s[0] && type == "field")
    result =  "does not begin with lower case letter";
  else if (!isalpha(s[0])) 
    result = "first character is not alphabet";
  else if (s[s.size()-1] == '_')
        result = "ends with an underscore";
  else
  {
    for (size_t i = 1; i < s.size(); i++)
    {
      if (!isalnum(s[i]) && !(s[i] != '_')) 
      {
        result = "illegal character : ";
        result += s[i];
        break;
      }
    }
  }
  return result;
}

string makeDBName(const string& s) {
if(true)
{
	//ORACLE allows only 30 chars and leading alphabet is a must
	if (s.size() > 30)
		return "O" + md5HexDigest(s).substr(0,29);
    return s;
}
else
{
	if (s.size() > 31)
        return "_" + md5HexDigest(s);
    return s;
}
}
static void sanityCheck(DatabasePtr& db,
                        const ObjectSequence& objects,
                        const Relation::sequence& relations) {
    using namespace litesql;
    map<string, bool> usedID;
    map<string, bool> objectName;
    string err;
    if (!(err = validID(db->name,"class")).empty()) 
        throw Except("invalid id: database.name : " + db->name);
    for (size_t i = 0; i < objects.size(); i++) {
        Object& o = *objects[i];
        if (!(err = validID(o.name, "class")).empty())
            throw Except("invalid id: object.name : " + o.name);
        if (usedID.find(o.name) != usedID.end())
            throw Except("duplicate id: object.name : " + o.name);
        usedID[o.name] = true;
        objectName[o.name] = true;
        map<string, bool> usedField;
        usedField.clear();
        for (size_t i2 = 0; i2 < o.fields.size(); i2++) {
            Field& f = *o.fields[i2];
            if (!(err = validID(f.name)).empty())
              throw Except("invalid id: object.field.name : " + o.name + "." + f.name + " cause:" + err );
            if (usedField.find(f.name) != usedField.end())
                throw Except("duplicate id: object.field.name : " + o.name + "." + f.name);
            usedField[f.name] = true;
        }
    }
    for (size_t i = 0; i < relations.size(); i++) {
        Relation& r = *relations[i];
        string name = r.getName();
        if (!(err = validID(name,"class")).empty())
            throw Except("invalid id: relation.name : " + name + "cause:" + err );
        if (usedID.find(name) != usedID.end())
            throw Except("duplicate id: relation.name : " + name);
        usedID[name] = true;
        map<string, bool> usedField;
        usedField.clear();
  
        bool defaults = false;
        
        for (size_t i2 = 0; i2 < r.fields.size(); i2++) {
            Field& f = *r.fields[i2];
            if (!(err = validID(f.name)).empty())
                throw Except("invalid id: relation.field.name : " + name + "." + f.name + "cause:" + err );
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
            Relate& rel = *r.related[i2];
            if (!(err = validID(rel.handle)).empty() && !rel.handle.empty())
                throw Except("invalid id: relation.relate.handle : " + name + "." + rel.handle + "cause:" + err );
            if (usedField.find(rel.handle) != usedField.end())
                throw Except("duplicate id: relation.relate.handle : " + name + "." + rel.handle);
            if (objectName.find(rel.objectName) == objectName.end())
                throw Except("unknown object: relation.relate.name : " + name + "." + rel.objectName);
            if (!rel.handle.empty())
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

static void initSchema(DatabasePtr& db,
                ObjectSequence& objects,
                Relation::sequence& relations) {
    for (size_t i = 0; i < objects.size(); i++) {
        Object& o = *objects[i];
        map<string, Database::DBField::Ptr> fldMap;
        Database::Table::Ptr tbl(new Database::Table);
        tbl->name = o.getTable();
        db->tables.push_back(tbl);
        
        if (!o.parentObject.get()) {
          Database::Sequence::Ptr seq(new Database::Sequence);
          seq->name = o.getSequence();
          seq->table = o.getTable();
          db->sequences.push_back(seq);
        }  else {
          Database::DBField::Ptr id(new Database::DBField); 
          id->name("id");
          id->field->type = A_field_type_integer;
          id->primaryKey = true;
          tbl->fields.push_back(id);
        }

        for (size_t i2 = 0; i2 < o.fields.size(); i2++) {
            Field::Ptr f = o.fields[i2];
            Database::DBField::Ptr fld(new Database::DBField);
            fld->name(f->name);
            fldMap[f->name] = fld;
            fld->field->type = f->type;
            fld->primaryKey = (f->name == "id");
            fld->field->unique =  (f->isUnique()) 
                                 ? A_field_unique_true
                                 : A_field_unique_false;

            fld->field = o.fields[i2];
            tbl->fields.push_back(fld);
            
            if (f->isIndexed()) {
              Database::DBIndex::Ptr idx(new Database::DBIndex);
                idx->name = makeDBName(tbl->name + fld->name() + "idx");
                idx->table = tbl->name;
                idx->fields.push_back(fld);
                db->indices.push_back(idx);
            }
                
        }
        for (size_t i2 = 0; i2 < o.indices.size(); i2++) {
            const Index& idx = *o.indices[i2];

            litesql::Split fldNames;
            Database::DBIndex::Ptr index(new Database::DBIndex);
            for (size_t i3 = 0; i3 < idx.fields.size(); i3++) {
                if (fldMap.find(idx.fields[i3].name) == fldMap.end())
                    throw litesql::Except("Indexfield " + o.name + "." + idx.fields[i3].name + " is invalid.");
                index->fields.push_back(fldMap[idx.fields[i3].name]);
                fldNames.push_back(idx.fields[i3].name);
            }

            index->name = makeDBName(tbl->name + "_" + fldNames.join("_") + "_idx");
            index->table = tbl->name;
            
            string unique = "";
            if (idx.isUnique())
                index->unique = true;
            db->indices.push_back(index);
        }
    }
    for (size_t i = 0; i < relations.size(); i++) {
        Relation& r = *relations[i];
        
        Database::Table::Ptr tbl(new Database::Table);
        tbl->name = r.getTable();
        
        db->tables.push_back(tbl);
        
        Database::DBField::sequence objFields;
        map<string, Database::DBField::Ptr> fldMap;
        for (size_t i2 = 0; i2 < r.related.size(); i2++) {
            const xml::Relate& relate = *r.related[i2];
            AT_field_unique unique = A_field_unique_false;
            if (relate.isUnique())
            {
                unique = A_field_unique_true;
            }
            else if (r.related.size() == 2) { 
                if (i2 == 0 && r.related[1]->hasLimit()) {
                    unique = A_field_unique_true;
                }
                if (i2 == 1 && r.related[0]->hasLimit()) {
                    unique = A_field_unique_true;
                }
            }
            Database::DBField::Ptr fld(new Database::DBField);
            fld->name(relate.fieldName);
            fld->field->type = A_field_type_integer;
            fld->field->unique = unique;
            tbl->fields.push_back(fld);
            objFields.push_back(fld);
            
            Database::DBIndex::Ptr idx( new Database::DBIndex);
            idx->name = makeDBName(tbl->name + fld->name() + "idx");
            idx->table = tbl->name;
            idx->fields.push_back(fld);
            db->indices.push_back(idx);
            
        }
        for (size_t i2 = 0; i2 < r.fields.size(); i2++) {
            Field& f = *r.fields[i2];
            Database::DBField::Ptr fld(new Database::DBField);
            fld->name(f.name);
            fldMap[f.name] = fld;
            fld->field->type = f.type;
            fld->primaryKey = false;
            fld->field->unique = f.isUnique() ? A_field_unique_true : A_field_unique_false;
            fld->field = r.fields[i2];
            tbl->fields.push_back(fld);
            
            if (f.isIndexed()) {
              Database::DBIndex::Ptr idx( new Database::DBIndex);
                idx->name = makeDBName(tbl->name + fld->name() + "idx");
                idx->table = tbl->name;
                idx->fields.push_back(fld);
                db->indices.push_back(idx);
            }
            
        }
    
        if (r.related.size() > 1) {

          Database::DBIndex::Ptr idx(new Database::DBIndex);
            idx->name = makeDBName(tbl->name + "_all_idx");
            idx->table = tbl->name;
            for (size_t i2 = 0; i2 < objFields.size(); i2++)
                idx->fields.push_back(objFields[i2]);
            db->indices.push_back(idx);
        }
        for (size_t i2 = 0; i2 < r.indices.size(); i2++) {
            const Index& idx = *r.indices[i2];

            litesql::Split fldNames(idx.fields.size());
            Database::DBIndex::Ptr index(new Database::DBIndex);
            for (size_t i3 = 0; i3 < idx.fields.size(); i3++) {
//                Database::DBField* fld = new Database::DBField;
                if (fldMap.find(idx.fields[i3].name) == fldMap.end())
                    throw litesql::Except("Indexfield " + r.name + "." + idx.fields[i3].name + " is invalid.");
                index->fields.push_back(fldMap[idx.fields[i3].name]);
                fldNames.push_back(idx.fields[i3].name);
            }

            index->name = makeDBName(tbl->name + "_" + fldNames.join("_") + "_idx");
            string unique = "";
            if (idx.isUnique())
                index->unique = true;
            db->indices.push_back(index);
        }
    }

}
void init(DatabasePtr& db,
          ObjectSequence& objects,
          Relation::sequence& relations) {
    map<string, ObjectPtr> objMap;
    Logger::report("validating XML file\n");
    sanityCheck(db, objects, relations);
    Logger::report("linking XML - objects\n"); 
    // make string -> Object mapping

    for (ObjectSequence::const_iterator object = objects.begin();
      object != objects.end();
      ++object)
    {
      objMap[(*object)->name] = (*object);
    }
    // make Object's class hierarchy mapping (parent and children)

    for (size_t i = 0; i < objects.size(); ++i) 
      if (objMap.find(objects[i]->inherits) != objMap.end()) {
            objects[i]->parentObject = objMap[objects[i]->inherits];
            objects[i]->parentObject->children.push_back(objects[i]);
      }
    //for (size_t i = 0; i < objects.size(); i++) 
    //    if (objects[i]->parentObject)
    //        objects[i]->parentObject->children.push_back(objects[i]);

    // sort objects of relations alphabetically (ascii)

    for (Relation::sequence::const_iterator it_rel=relations.begin();it_rel!=relations.end();++it_rel)
    {
        sort((*it_rel)->related.begin(), (*it_rel)->related.end(),Relate::CompareByObjectName());
    }
          
    for (Relation::sequence::const_iterator it_rel=relations.begin();it_rel!=relations.end();++it_rel)
    {
        Relation::Ptr rel = *it_rel;
        bool same = rel->sameTypes() > 1;
        
        for (size_t i2 = 0; i2 < rel->related.size(); i2++) {
          Relate::Ptr& relate = rel->related[i2];
            ObjectPtr& obj = objMap[relate->objectName];
            string num;
            if (same)
                num = toString(i2 + 1);
            relate->fieldTypeName = relate->objectName + num;
            relate->fieldName = relate->objectName + toString(i2 + 1);
            if (obj->relations.find(rel) == obj->relations.end())
              obj->relations[rel] = Relate::sequence();

            // make Object -> Relation mapping

            obj->relations[rel].push_back(relate);
            if (!relate->handle.empty()) {
                
                // make Object's relation handles

              RelationHandle::Ptr handle (new RelationHandle(relate->handle, rel,relate,obj));
                for (size_t i3 = 0; i3 < rel->related.size(); i3++) {
                    if (i2 != i3) {
                        ObjectPtr& o = objMap[rel->related[i3]->objectName];
                        // make RelationHandle -> (Object,Relation) mapping
                        handle->destObjects.push_back(make_pair(o,rel->related[i3]));
                    }
                }
                obj->handles.push_back(handle);
            }
        }
    }
 
    initSchema(db, objects, relations);

}
string safe(const char * s) {
    if (s)
        return s;
    return "";
}

string attribute(const string& name, const string& value)
{
  string a;
  a.append(name).append("=").append("\"").append(value).append("\" ");
  return a;
}

string endtag(const string& name)
{
  string a;
  a.append("</").append(name).append(">");
  return a;
}


}

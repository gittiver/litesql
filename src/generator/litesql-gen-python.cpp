#include "litesql/split.hpp"
#include "litesql/types.hpp"
#include "litesql-gen-python.hpp"
#include "litesql-gen-main.hpp"
#include "pythongenerator.hpp"
#include "xmlobjects.hpp"
#include <ctype.h>
#include <cstdio>
#include <set>
#include <map>
using namespace std;
using namespace gen;
using namespace litesql;

static std::string pyBool(bool b) {
    return  b ? "True" : "False";
}
void writeObject(Block& pre, Block& post,
                 const xml::Database& db,
                 const xml::Object& o) {
    Split fieldTypes, fields;
    for (size_t i = 0; i < o.fields.size(); i++) {
        xml::Field* fld = o.fields[i];
        Split values;
        for (size_t i2 = 0; i2 < fld->values.size(); i2++) {
            values.push_back("litesql.EnumValue" + brackets(
                        quote(fld->values[i2].name) + "," +
                        quote(fld->values[i2].value)));
        }
        Split ftype;
        ftype.push_back(quote(fld->name));
        ftype.push_back(quote(fld->name + "_"));
        ftype.push_back(fld->getPythonType());
        ftype.push_back(quote(o.getTable()));
        ftype.push_back(toString(fld->offset));
        ftype.push_back(fld->getQuotedDefaultValue());
        if (fld->values.size())
            ftype.push_back(brackets(values.join(", ")));

        fieldTypes.push_back("litesql.FieldType" + brackets(ftype.join(", ")));
        post(o.name + "." + xml::capitalize(fld->name) + " = "
                + o.name + ".fieldTypes" + sqbrackets(toString(i)));
        fields.push_back(quote(fld->name) + " : litesql.Field" + brackets(
                    "fieldTypes" + sqbrackets(toString(i))));

    }
    Split handles;
    for (size_t i = 0; i < o.handles.size(); i++) {
        xml::RelationHandle* handle = o.handles[i];
        xml::Relate* relate = handle->relate;
        Split data;
        Split ftype;

        ftype.push_back(quote(relate->fieldTypeName));
        ftype.push_back(quote(relate->fieldName));
        ftype.push_back("int");
        ftype.push_back(quote(handle->relation->getTable()));
        ftype.push_back(toString(relate->paramPos));
        data.push_back(handle->relation->getName());
        data.push_back("litesql.FieldType" + brackets(ftype.join(", ")));
        handles.push_back(quote(handle->name)
                + " : litesql.RelationHandleType" + brackets(data.join(", ")));
    }
    Split methods;
    for (size_t i = 0; i < o.methods.size(); i++) {
        xml::Method* mtd = o.methods[i];
        methods.push_back(quote(mtd->name) + " : litesql.Method()");
        post(o.name + "." + xml::capitalize(mtd->name) + " = " + o.name
                + ".methods" + sqbrackets(quote(mtd->name)));
        if (!db.include.empty())
            post(o.name + "." + xml::capitalize(mtd->name) + ".bind" + brackets(
                 db.include + "." + o.name + "." + mtd->name));
    }
    Split relations;
    for (map<xml::Relation*, vector<xml::Relate*> >::const_iterator i =
             o.relations.begin(); i != o.relations.end(); i++) {
        const xml::Relation * rel = i->first;
        const vector<xml::Relate *> relates = i->second;
        Split params;
        params.push_back(rel->getName());

        for (size_t i2 = 0; i2 < relates.size(); i2++)
            params.push_back(quote(relates[i2]->fieldName));
        relations.push_back(brackets(params.join(", ")));
    }
    Split children;
    Split childrenMap;
    o.getChildrenNames(children);
    for (size_t i = 0; i < children.size(); i++)
        childrenMap.push_back(quote(children[i]) + " : " + children[i]);
    post(o.name + ".subClasses = " + braces(childrenMap.join(", ")));
    pre("class " + o.name + brackets(o.getInherits("python")) + ":");
    pre++;
//    pre("className = " + quote(o.name))
    pre

       ("table = " + quote(o.getTable()))
       ("fieldTypes = " + sqbrackets(fieldTypes.join(",\n" + string(" ") * 14)))
       ("handles = "
            + braces(handles.join(",\n" + string(" ") * 11)))

//       ("fields = " + braces(fields.join(",\n" + string(" ") * 10)))
       ("relations = " + sqbrackets(relations.join(", ")))
       ("methods = " + braces(methods.join(",\n" + string(" ") * 11)))
       --;


}
void writeRelation(Block& pre, Block& post,
                   xml::Database& database,
		           xml::Relation& r) {

    Split objects, attrs, objectFields, fields;
    set<string> objSet;

    for (size_t i = 0; i < r.related.size(); i++) {
        xml::Relate* relate = r.related[i];
        objects.push_back(relate->objectName);

        Split ftype;

        ftype.push_back(quote(relate->fieldTypeName));
        ftype.push_back(quote(relate->fieldName));
        ftype.push_back("int");
        ftype.push_back(quote(r.getTable()));
        ftype.push_back(toString(relate->paramPos));

        if (objSet.find(relate->objectName) == objSet.end())
            objectFields.push_back(quote(relate->objectName)
                    + " : litesql.FieldType"
                                   + brackets(ftype.join(", ")));
        fields.push_back("litesql.FieldType" + brackets(ftype.join(", ")));
        post(r.getName() + "." +
                xml::capitalize(relate->fieldTypeName)
                + " = " + r.getName() + ".fields" + sqbrackets(toString(i)));

        objSet.insert(relate->objectName);

    }
    for (size_t i = 0; i < r.fields.size(); i++) {
        xml::Field* fld = r.fields[i];
        Split ftype;
        ftype.push_back(quote(fld->name));
        ftype.push_back(quote(fld->name + "_"));
        ftype.push_back(fld->getPythonType());
        ftype.push_back(quote(r.getTable()));
        ftype.push_back(toString(r.related.size() + i));
        fields.push_back("litesql.FieldType" + brackets(ftype.join(", ")));
        attrs.push_back("litesql.FieldType" + brackets(ftype.join(", ")));
        post(r.getName() + "." + xml::capitalize(fld->name) + " = "
                + r.getName() + ".attrs"
                                + sqbrackets(toString(i)));


    }

    pre("class " + r.getName() + brackets("litesql.Relation") + ":")
      ++
      ("table = " + quote(r.getTable()))
      ("# objects = " + sqbrackets(objects.join(", ")))
      ("attrs = " + sqbrackets(attrs.join(",\n" + string(" ") * 9)))
      ("fields = " + sqbrackets(fields.join(", " + string(" ") * 10)))
      ("objectFields = " + braces(objectFields.join(",\n" + string(" ")* 16)))
      ("unidir = " + pyBool(r.isUnidir()))
     --;
    post(r.getName() + ".objects = " + sqbrackets(objects.join(", ")));
}



void writeDatabase(Block& pre, Block& post,
                   xml::Database& db,
                   vector<xml::Object*>& objects,
                   vector<xml::Relation*>& relations) {
    Split basicSchema, sequenceSchema;
    Split rec;
    Split firstSchema;
    rec.push_back(quote("schema_"));
    rec.push_back(quote("table"));
    rec.push_back(quote("CREATE TABLE schema_ (name_ TEXT, type_ TEXT, sql_ TEXT)"));
    firstSchema.push_back(brackets(rec.join(", ")));

    for (size_t i = 0; i < db.sequences.size(); i++) {
        Split rec;
        rec.push_back(quote(db.sequences[i]->name));
        rec.push_back(quote("sequence"));
        rec.push_back(quote(db.sequences[i]->getSQL()));
        sequenceSchema.push_back(brackets(rec.join(", ")));
    }
    for (size_t i = 0; i < db.tables.size(); i++) {
        Split rec;
        rec.push_back(quote(db.tables[i]->name));
        rec.push_back(quote("table"));
        rec.push_back(quote(db.tables[i]->getSQL("\" + self.backend.getRowIDType() + \"")));
        basicSchema.push_back(brackets(rec.join(", ")));

    }
    for (size_t i = 0; i < db.indices.size(); i++) {
        Split rec;
        rec.push_back(quote(db.indices[i]->name));
        rec.push_back(quote("index"));
        rec.push_back(quote(db.indices[i]->getSQL()));
        basicSchema.push_back(brackets(rec.join(", ")));
    }
    report(toString(db.tables.size()) + " tables\n");
    report(toString(db.sequences.size()) + " sequences\n");
    report(toString(db.indices.size()) + " indices\n");

    pre("class " + db.name + brackets("litesql.Database") + ":")
       ++
       ("def getSchema(self):")
           ++
           ("schema = " + sqbrackets(firstSchema.join(",\n"+ string(" ") * 7)))
           ("if self.backend.supportsSequences():")
           ("    schema += " + sqbrackets(sequenceSchema.join(",\n"
                                                       + string(" ") * 13)))
           ("schema += "
               + sqbrackets(basicSchema.join(",\n" + string(" ") * 8)))
           ("return schema")
           --
       --;
}
void writePython(xml::Database& db,
                 vector<xml::Object*>& objects,
                 vector<xml::Relation*>& relations) {
    FILE *file;

    Block pre, post;
    string fName = toLower(db.name) + ".py";
    file = fopen(fName.c_str(), "w");
    if (!file) {
        string msg = "could not open file : " + fName;
        perror(msg.c_str());
        return;
    }
    fprintf(file, "import litesql\n");
    if (!db.include.empty())
        fprintf(file, "import %s\n", db.include.c_str());

    report("writing relations\n");
    Split relationNames;
    for (size_t i = 0; i < relations.size(); i++) {
        xml::Relation & o = *relations[i];
        relationNames.push_back(o.getName());
        writeRelation(pre, post, db, o);
    }

    report("writing persistent objects\n");

    Split objectNames;
    for (size_t i = 0; i < objects.size(); i++) {
        xml::Object & o = *objects[i];
        objectNames.push_back(o.name);
        writeObject(pre, post, db, o);
    }
    post("objects = " + sqbrackets(objectNames.join(", ")))
        ("relations = " + sqbrackets(relationNames.join(", ")));

    report("writing database class\n");
    writeDatabase(pre, post, db, objects, relations);
    pre.write(file);
    post.write(file);

    fclose(file);
}


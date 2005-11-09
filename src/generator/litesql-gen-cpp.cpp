#include "litesql/split.hpp"
#include "litesql/types.hpp"
#include "litesql-gen-cpp.hpp"
#include "cppgenerator.hpp"
#include "xmlobjects.hpp"
#include "md5.hpp"
#include <ctype.h>
#include <stdexcept>
#include <map>
using namespace std;
using namespace gen;
using namespace litesql;


string capitalize(string s) {
    if (s.size() == 0)
        return s;
    char buf[2] = {toupper(s[0]), 0};
    return string(buf) + s.substr(1, s.size());
}
string decapitalize(string s) {
    if (s.size() == 0)
        return s;
    char buf[2] = {tolower(s[0]), 0};
    return string(buf) + s.substr(1, s.size());
}
string quote(string s) {
    return "\"" + s + "\"";
}
string brackets(string s) {
    return "(" + s + ")";
}
string makeTableName(string s) {
    if (s.size() > 31)
        return "_" + md5HexDigest(s);
    return s;
}

void writeStaticObjData(Class& cl, const xml::Object& o) {
    Variable type__("type__", "const std::string", quote(o.name));
    type__.static_();
    cl.variable(type__);

    Variable table__("table__", "const std::string", 
                     quote(makeTableName(o.getTable())));
    table__.static_();
    cl.variable(table__);

    if (!o.parentObject) {
        Variable sequence__("sequence__", "const std::string", 
                            quote(makeTableName(o.name + "_seq")));
        sequence__.static_();
        cl.variable(sequence__);
    }
}
void writeObjFields(Class & cl, const xml::Object & o) {
    for (size_t i = 0; i < o.fields.size(); i++) {
        const xml::Field& fld = o.fields[i];
        string data = quote(fld.name + "_") + "," +
                      quote(fld.getSQLType()) + "," +
                      "table__";
        Variable ftype(fld.name + "_", "const litesql::FieldType", data);
        ftype.static_();
        // TOOD: FieldTypeen enum-valuet mukaan
        Variable field(fld.name, "litesql::Field<" + fld.getCPPType() + ">");
        cl.variable(ftype);
        cl.variable(field);
        if (fld.values.size() > 0) {
            Class valueHolder(capitalize(fld.name));
            for (size_t v = 0; v < fld.values.size(); v++) {
                const xml::Value& value = fld.values[v];
                string v;
                if (fld.getCPPType() == "std::string")
                    v = quote(value.value);
                else
                    v = value.value;
                Variable val(value.name, "const " + fld.getCPPType(), v);

                val.static_();
                valueHolder.variable(val);
            }
            cl.class_(valueHolder);
        }
    }
}
void writeObjConstructors(Class& cl, const xml::Object& o) {
    Method defaults("defaults", "void");
    defaults.protected_();
    bool hasDefaults = false;
    for (size_t i = 0; i < o.fields.size(); i++) {
        const xml::Field& f = o.fields[i];
        if ((f.default_.size() > 0) || (f.getCPPType() != "std::string")){
            defaults.body(f.name + " = " + f.getQuotedDefaultValue() + ";");
            hasDefaults = true;
        } 
    }
    
    Method cons1(o.name); // Object(const Database &)
    string consParams = o.inherits + "(db)";
    if (o.fields.size() > 0) {
        Split fieldInst;
        for (size_t i = 0; i < o.fields.size(); i++) {
            const xml::Field& f = o.fields[i];
            fieldInst.push_back(f.name + brackets(f.name + "_"));
        }
        consParams += ", " + fieldInst.join(", ");
    }

    cons1.param(Variable("db", "const litesql::Database&"))
        .constructor(consParams);
    if (hasDefaults)
        cons1.body("defaults();");
    
    Method cons2(o.name); // Object(const Database &, const Record& row
    cons2.param(Variable("db", "const litesql::Database&"))
        .param(Variable("rec", "const litesql::Record&"))
        .constructor(consParams);
    if (hasDefaults) 
        cons2.body("defaults();");
    if (o.fields.size() > 0) {
        cons2.body("switch(rec.size()) {");
        int last = o.getLastFieldOffset();
        for(int i = o.fields.size() - 1; i >= 0; i--) {
            int p = last - o.fields.size() + i;
            cons2.body("case " + toString(p+1) + ": " 
                       + o.fields[i].name + " = rec[" + toString(p) + "];");

        }
        cons2.body("}");
    }
    
    Method cons3(o.name); // Object(const Object& obj);
    string consParams3 = o.inherits + "(obj)";    
    if (o.fields.size() > 0) {
        Split fieldCopy;
        for (size_t i = 0; i < o.fields.size(); i++) {
            const xml::Field& f = o.fields[i];
            fieldCopy.push_back(f.name + brackets("obj." + f.name));
        }
        consParams3 += ", " + fieldCopy.join(", ");
    }
    cons3.param(Variable("obj", "const " + o.name + "&"))
        .constructor(consParams3);
    if (hasDefaults)
        cl.method(defaults);
    
    Method assign("operator=", "const " + o.name + "&");
    assign.param(Variable("obj", "const " + o.name + "&"));
    if (o.fields.size() > 0) {
        assign.body("if (this != &obj) {");
        for (size_t i = 0; i < o.fields.size(); i++) {
            const xml::Field& f = o.fields[i];
            assign.body("    " + f.name + " = obj." + f.name + ";");
        }
        assign.body("}");
    }
    assign.body(o.inherits + "::operator=(obj);");
    assign.body("return *this;");
    

    cl.method(cons1).method(cons2).method(cons3).method(assign);
    
}
void writeObjRelationHandles(Class& cl, xml::Object& o) {
    for (size_t i = 0; i < o.handles.size(); i++) {
        xml::RelationHandle& handle = o.handles[i];
        xml::Relation* rel = handle.relation;

        string className = capitalize(handle.name) + "Handle";
        Class hcl(className,
                  "litesql::RelationHandle<" + o.name + ">");
        Method cons(className);
        cons.param(Variable("owner", "const " + o.name + "&")).
            constructor("litesql::RelationHandle<" + o.name + ">(owner)");
        
        Method link("link", "void");
        Method unlink("unlink", "void");
        Split params;
        params.push_back("owner->getDatabase()");
        params.resize(1 + rel->related.size());
        params[1 + handle.relate->paramPos] = "*owner";

        for (size_t i2 = 0; i2 < handle.destObjects.size(); i2++) {
            xml::Object* dest = handle.destObjects[i2].first;
            xml::Relate * relate = handle.destObjects[i2].second;
            Variable var("o" + toString(i2), "const " + dest->name + "&");
            link.param(var);
            unlink.param(var);
            params[1 + relate->paramPos] = "o" + toString(i2);
        }


        for (size_t i2 = 0; i2 < rel->fields.size(); i2++) {
            xml::Field& field = rel->fields[i2];
            // FIXME: default-arvoiset parametrit viimeiseksi

            link.param(Variable(field.name, field.getCPPType(), 
                                field.getQuotedDefaultValue()));
            unlink.param(Variable(field.name, field.getCPPType()));
            params.push_back(field.name);
        }
        link.body(rel->getName() + "::link(" + params.join(", ") + ");");
        unlink.body(rel->getName() + "::unlink(" + params.join(", ") + ");");
        Variable exprParam("expr", "const litesql::Expr&", "litesql::Expr()");
        Variable srcExprParam("srcExpr", "const litesql::Expr&", "litesql::Expr()");
        Method del("del", "void");
        params.clear();
        params.push_back("owner->getDatabase()");
        params.push_back("expr");
        del.param(exprParam).body(rel->getName() + "::del(" + params.join(", ") + ");");


        hcl.method(cons).method(link).method(unlink).method(del);
        string extraExpr;
        if (!rel->fields.empty())
            extraExpr = " && srcExpr";
        if (handle.destObjects.size() == 1) {
            xml::Object* dest = handle.destObjects[0].first;
            xml::Relate* relate = handle.destObjects[0].second;
            Method get("get", "litesql::DataSource<" + dest->name + ">");

            if (!rel->fields.empty()) 
                get.param(srcExprParam);
            
            params.clear();
            params.push_back("owner->getDatabase()");
            params.push_back("(" + rel->getName() + "::" + handle.relate->fieldTypeName + " == owner->id)"
                             + extraExpr);
            
            params.push_back("expr");
            get.param(exprParam)
                .body("return " + rel->getName() + "::" + relate->getMethodName
                      + brackets(params.join(", ")) + ";");
            
            hcl.method(get);
        } else {
            if (rel->sameTypes() <= 2) {
                Method getTpl("get", "litesql::DataSource<T>");
                getTpl.template_("class T").defineOnly()
                    .param(exprParam);
                if (!rel->fields.empty()) 
                    getTpl.param(srcExprParam);
                hcl.method(getTpl);                
                for (size_t i2 = 0; i2 < handle.destObjects.size(); i2++) {
                    xml::Object* dest = handle.destObjects[i2].first;
                    xml::Relate* relate = handle.destObjects[i2].second;
                    Method get("get", 
                               "litesql::DataSource<" + dest->name + ">");
                    get.templateSpec("").param(exprParam);
                    if (!rel->fields.empty()) 
                        get.param(srcExprParam);
                    params.clear();
                    params.push_back("owner->getDatabase()");
                    params.push_back("(" + rel->getName() + "::" + handle.relate->fieldTypeName + " == owner->id)"
                                     + extraExpr);
                    params.push_back("expr");
                    get.body("return " + rel->getName() + "::" + relate->getMethodName
                              + brackets(params.join(", ")) + ";");
                    hcl.method(get); 
                }    
            } else {
                for (size_t i2 = 0; i2 < handle.destObjects.size(); i2++) {
                    xml::Object* dest = handle.destObjects[i2].first;
                    xml::Relate* relate = handle.destObjects[i2].second;
                    string num = toString(i2 + 1);
                    Method get("get" + dest->name + num, 
                               "litesql::DataSource<" + dest->name + ">");
                    get.param(exprParam);
                    if (!rel->fields.empty()) 
                        get.param(srcExprParam);
                    params.clear();
                    params.push_back("owner->getDatabase()");
                    params.push_back("(" + rel->getName() + "::" + handle.relate->fieldTypeName + " == owner->id)"
                                     + extraExpr);
                    params.push_back("expr");
                    get.body("return " + rel->getName() + "::" + relate->getMethodName
                              + brackets(params.join(", ")) + ";");
                    hcl.method(get);
                }
            }
        }
        Method getRows("getRows", "litesql::DataSource<" 
                       + rel->getName() + "::Row>");
        getRows.param(exprParam)
            .body("return " + rel->getName()
                  + "::getRows(owner->getDatabase(), "
                  "expr && (" + rel->getName() + "::" 
                  + handle.relate->fieldTypeName +
                  " == owner->id));");
        hcl.method(getRows);
        cl.class_(hcl);
        Method hdlMethod(handle.name, o.name + "::" + className);
        hdlMethod.body("return " + o.name + "::" +  className + "(*this);");
        cl.method(hdlMethod);
    }
}
void writeObjBaseMethods(Class& cl, const xml::Object& o) {
    Method insert("insert", "std::string");
    insert.protected_()
        .param(Variable("tables", "litesql::Record&"))
        .param(Variable("fieldRecs", "litesql::Records&"))
        .param(Variable("valueRecs", "litesql::Records&"))
        .body("tables.push_back(table__);")
        .body("litesql::Record fields;")
        .body("litesql::Record values;");

    if (o.parentObject)
        insert.body("fields.push_back(\"id_\");")
            .body("values.push_back(id);");

    for (size_t i = 0; i < o.fields.size(); i++) {
        const xml::Field& f = o.fields[i];
        insert.body("fields.push_back(" + f.name + ".name()" + ");");
        insert.body("values.push_back(" + f.name + ");");
        insert.body(f.name + ".setModified(false);");
    }
    
    insert.body("fieldRecs.push_back(fields);")
        .body("valueRecs.push_back(values);");
    if (o.parentObject) {
        insert.body("return " + o.inherits 
                    + "::insert(tables, fieldRecs, valueRecs);");
    } else
        insert.body("return " + o.inherits
                    + "::insert(tables, fieldRecs, valueRecs, " 
                    + "sequence__);");


    Method create("create", "void");
    create.protected_();
    create.body("litesql::Record tables;")
        .body("litesql::Records fieldRecs;")
        .body("litesql::Records valueRecs;")
        .body("type = type__;")
        .body("std::string newID = insert(tables, fieldRecs, valueRecs);")
        .body("if (id == 0)")
        .body("    id = newID;");


    Method addUpdates("addUpdates", "void");
    addUpdates.protected_().virtual_()
        .param(Variable("updates", "Updates&"))
        .body("prepareUpdate(updates, table__);");
    for (size_t i = 0; i < o.fields.size(); i++) {
        const xml::Field& f = o.fields[i];
        addUpdates.body("updateField(updates, table__, " + f.name + ");");
    }
    if (o.parentObject) 
        addUpdates.body(o.inherits + "::addUpdates(updates);");
    Method addIDUpdates("addIDUpdates", "void");    
    addIDUpdates.protected_().virtual_()
        .param(Variable("updates", "Updates&"));
    if (o.parentObject) {
        addIDUpdates
            .body("prepareUpdate(updates, table__);")
            .body("updateField(updates, table__, id);");
        if (o.parentObject->parentObject)
            addIDUpdates.body(o.inherits + "::addIDUpdates(updates);");
    }
    
    Method getFieldTypes("getFieldTypes", "void");
    getFieldTypes.static_().
        param(Variable("ftypes", "std::vector<litesql::FieldType>&"));
    if (o.parentObject) 
        getFieldTypes.body(o.parentObject->name + "::getFieldTypes(ftypes);");
    for (size_t i = 0; i < o.fields.size(); i++)
        getFieldTypes.body("ftypes.push_back(" + o.fields[i].name + "_);");
    
    Method update("update", "void");
    update.virtual_()
        .body("if (!inDatabase) {")
        .body("    create();")
        .body("    return;")
        .body("}")
        .body("Updates updates;")
        .body("addUpdates(updates);")
        .body("if (id != oldKey) {")
        .body("    if (!typeIsCorrect()) ")
        .body("        upcastCopy()->addIDUpdates(updates);");
    if (o.parentObject) 
        update
            .body("    else")
            .body("        addIDUpdates(updates);");
    
    update
        .body("}")
        .body("litesql::Persistent::update(updates);")
        .body("oldKey = id;");

    Method delRecord("delRecord", "void");
    delRecord.protected_().virtual_()
        .body("deleteFromTable(table__, id);");
    if (o.parentObject)
        delRecord.body(o.inherits + "::delRecord();");
    Method delRelations("delRelations", "void");
    delRelations.protected_().virtual_();
    for (map<xml::Relation*, vector<xml::Relate*> >::const_iterator i = 
             o.relations.begin(); i != o.relations.end(); i++) {
        const xml::Relation * rel = i->first;
        const vector<xml::Relate *> relates = i->second;
        Split params;
        for (size_t i2 = 0; i2 < relates.size(); i2++)
            params.push_back("(" + rel->getName() + "::" + relates[i2]->fieldTypeName
                             + " == id)");
        delRelations.body(rel->getName() + "::del(*db, " + params.join(" || ") + ");");
    }
        
    

    Method del("del", "void");
    del.virtual_()
        .body("if (typeIsCorrect() == false) {")
        .body("    std::auto_ptr<" + o.name + "> p(upcastCopy());")
        .body("    p->delRelations();")
        .body("    p->onDelete();")
        .body("    p->delRecord();")
        .body("} else {")
        .body("    onDelete();")
        .body("    delRecord();")
        .body("}")
        .body("inDatabase = false;");


    Method typeIsCorrect("typeIsCorrect", "bool");    
    typeIsCorrect.body("return type == type__;").virtual_();
    
    Method upcast("upcast", "std::auto_ptr<" + o.name + ">");
    Method upcastCopy("upcastCopy", "std::auto_ptr<" + o.name + ">");
    Split names;
    o.getChildrenNames(names);
    upcastCopy.body(o.name + "* np = NULL;");
    for (size_t i = 0; i < names.size(); i++) {
        upcast.body("if (type == " + names[i] + "::type__)")
            .body("    return auto_ptr<" + o.name + ">(new " + names[i] 
                  + "(select<" + names[i] 
                  + ">(*db, id_ == id).one()));");
        upcastCopy.body("if (type == " + quote(names[i]) + ")")
            .body("    np = new " + names[i] + "(*db);");
    }
    for (size_t i = 0; i < o.fields.size(); i++) {
        upcastCopy.body("np->" + o.fields[i].name + " = " 
                        + o.fields[i].name + ";");
    }
    upcastCopy
        .body("np->inDatabase = inDatabase;")
        .body("if (!np)")
        .body("    np = new " + o.name + "(*this);")
        .body("return auto_ptr<" + o.name + ">(np);");
    upcast.body("return auto_ptr<" + o.name 
                + ">(new " + o.name + "(*this));");

    for (size_t i = 0; i < o.methods.size(); i++) {
        const xml::Method& m = o.methods[i];
        Method mtd(m.name, m.returnType);
        for (size_t i2 = 0; i2 < m.params.size(); i2++) {
            const xml::Param& p = m.params[i2];
            mtd.param(Variable(p.name, p.type));
        
        }
        mtd.defineOnly().virtual_();
        cl.method(mtd);
    }
    
    cl.method(insert).method(create).method(addUpdates)
        .method(addIDUpdates).method(getFieldTypes).method(delRecord).method(delRelations)
         .method(update).method(del)
        .method(typeIsCorrect).method(upcast).method(upcastCopy);

}

void writeStaticRelData(Class& cl, xml::Relation& r) {
    Variable table("table", "const std::string", 
                   quote(makeTableName(r.getTable())));
    table.static_();
    cl.variable(table);
    
    bool same = r.sameTypes() > 1;
    for (size_t i2 = 0; i2 < r.related.size(); i2++) {
        string num;
        if (same)
            num = toString(i2 + 1);        
        r.related[i2].fieldTypeName = decapitalize(r.related[i2].objectName) 
            + num;
        Variable ftype(r.related[i2].fieldTypeName,
                       "const litesql::FieldType",
                       quote(r.related[i2].objectName 
                             + toString(i2 + 1))
                       + "," + quote("INTEGER") + "," + "table");
        ftype.static_();
        cl.variable(ftype);
    }
    for (size_t i2 = 0; i2 < r.fields.size(); i2++) {
        xml::Field& fld = r.fields[i2];
        string data = quote(fld.name + "_") + "," +
                      quote(fld.getSQLType()) + "," +
                      "table";
        Variable ftype(fld.name + "_", "const litesql::FieldType", data);
        ftype.static_();
        cl.variable(ftype);
    }
    Class rowcl("Row");
    Method rowcons("Row");
    rowcons.param(Variable("db", "const litesql::Database&"))
        .param(Variable("rec", "const litesql::Record&", "litesql::Record()"));
//        .constructor("litesql::Record(db, rec)");
    Split consParams;
    int fieldNum = r.related.size() + r.fields.size();
    rowcons.body("switch(rec.size()) {");
    for (int i = r.fields.size()-1; i >= 0; i--) {
        xml::Field& fld = r.fields[i];
        Variable fldvar(fld.name, "litesql::Field<" + fld.getCPPType() + ">");
        rowcl.variable(fldvar);        

        rowcons.body("case " + toString(fieldNum) + ":")
            .body("    " + fld.name + " = rec[" + toString(fieldNum-1) + "];");
        consParams.push_back(fld.name +
                             "(" + r.getName() + "::" + fld.name +"_)");
        fieldNum--;
        
    }
    for (int i = r.related.size()-1; i >= 0; i--) {
        xml::Relate& rel = r.related[i];
        Variable fld(rel.fieldTypeName, "litesql::Field<int>");
        rowcl.variable(fld);
        rowcons.body("case " + toString(fieldNum) + ":")
            .body("    " + rel.fieldTypeName + " = rec[" 
                  + toString(fieldNum-1) + "];"); 

        consParams.push_back(rel.fieldTypeName 
                             + "(" + r.getName() 
                             + "::" + rel.fieldTypeName +")");
        fieldNum--;
    }
    rowcons.body("}");
    rowcons.constructor(consParams.join(", "));
    rowcl.method(rowcons);    
    cl.class_(rowcl);

}
void writeRelMethods(Class& cl, xml::Relation& r) {
    Variable dbparam("db", "const litesql::Database&");
    Variable destExpr("expr", "const litesql::Expr&", 
                       "litesql::Expr()");
    Variable srcExpr("srcExpr", "const litesql::Expr&", 
                       "litesql::Expr()");
    Method link("link", "void");
    Method unlink("unlink", "void");
    Method del("del", "void");
    Method getRows("getRows", 
                   "litesql::DataSource<"+r.getName()+"::Row>");
    
    link.static_().param(dbparam);
    link.body("Record values;")
        .body("Split fields;");
    for (size_t i = 0; i < r.related.size(); i++) {
        xml::Relate& rel = r.related[i];
        link.body("fields.push_back(" + rel.fieldTypeName + ".name());");
        link.body("values.push_back(o" + toString(i) + ".id);");
        rel.paramPos = i;
    }
    for (size_t i = 0; i < r.fields.size(); i++) {
        xml::Field& fld = r.fields[i];
        
        link.body("fields.push_back(" + fld.name + "_.name());");
        if (fld.getCPPType() != "std::string")
            link.body("values.push_back(toString(" + fld.name + "));");
        else
            link.body("values.push_back(" + fld.name + ");");
    }
    link.body("db.insert(table, values, fields);");
    if (r.isUnidir()==false && r.related.size() == 2 && r.sameTypes() == 2) {
        link.body("fields.clear();")
            .body("values.clear();");
        for (size_t i = 0; i < r.related.size(); i++) {
            xml::Relate& rel = r.related[i];
            link.body("fields.push_back(" + rel.fieldTypeName + ".name());");
            link.body("values.push_back(o" + toString(1-i) + ".id);");
        }
        for (size_t i = 0; i < r.fields.size(); i++) {
            xml::Field& fld = r.fields[i];
        
            link.body("fields.push_back(" + fld.name + "_.name());");
            if (fld.getCPPType() != "std::string")
                link.body("values.push_back(toString(" + fld.name + "));");
            else
                link.body("values.push_back(" + fld.name + ");");
        }
        link.body("db.insert(table, values, fields);");
    }

    unlink.static_().param(dbparam);
    Split unlinks;
    for (size_t i = 0; i < r.related.size(); i++) {
        xml::Relate& rel = r.related[i];
        unlinks.push_back(rel.fieldTypeName + " == o" 
                          + toString(i) + ".id");
    }
    for (size_t i = 0; i < r.fields.size(); i++) {
        xml::Field& fld = r.fields[i];
        unlinks.push_back("(" + fld.name + "_ == " + fld.name + ")");
    }
    
    unlink.body("db.delete_(table, (" + unlinks.join(" && ") + "));");
    if (r.isUnidir()==false && r.related.size() == 2 && r.sameTypes() == 2) {
        unlinks.clear();
        for (size_t i = 0; i < r.related.size(); i++) {
            xml::Relate& rel = r.related[i];
            unlinks.push_back(rel.fieldTypeName + " == o" 
                              + toString(1-i) + ".id");
        }
        for (size_t i = 0; i < r.fields.size(); i++) {
            xml::Field& fld = r.fields[i];
            unlinks.push_back("(" + fld.name + "_ == " + fld.name + ")");
        }
        unlink.body("db.delete_(table, (" + unlinks.join(" && ") + "));");
    }

    del.static_().param(dbparam).param(destExpr); 
    del.body("db.delete_(table, expr);");
    getRows.static_().param(dbparam).param(destExpr)
        .body("SelectQuery sel;");

    for (size_t i = 0; i < r.related.size(); i++) {
        xml::Relate& rel = r.related[i];        
        getRows.body("sel.result(" + rel.fieldTypeName + ".fullName());");
    }
    for (size_t i = 0; i < r.fields.size(); i++) {
        xml::Field& fld = r.fields[i];
        getRows.body("sel.result(" + fld.name + "_.fullName());");
    }
    getRows.body("sel.source(table);")
        .body("sel.where(expr);")
        .body("return DataSource<" + r.getName() + "::Row>(db, sel);");

    for (size_t i2 = 0; i2 < r.related.size(); i2++) {
        xml::Relate& rel = r.related[i2];
        Variable obj("o" + toString(i2), "const " + rel.objectName + "&");
        link.param(obj);
        unlink.param(obj);
    }
    for (size_t i2 = 0; i2 < r.fields.size(); i2++) {
        xml::Field& fld = r.fields[i2];
        link.param(Variable(fld.name, fld.getCPPType(), 
                            fld.getQuotedDefaultValue()));
        unlink.param(Variable(fld.name,  fld.getCPPType()));
    }
    cl.method(link).method(unlink).method(del).method(getRows);
    if (r.sameTypes() == 1) {
        Method getTpl("get", "litesql::DataSource<T>");
        getTpl.static_().template_("class T").defineOnly()
            .param(dbparam).param(srcExpr).param(destExpr);
        cl.method(getTpl);
        for (size_t i2 = 0; i2 < r.related.size(); i2++) {
            xml::Relate& rel = r.related[i2];
            Method get("get", "litesql::DataSource<" 
                       + rel.objectName + ">");
            rel.getMethodName = "get<" + rel.objectName + ">";
            get.static_().templateSpec("")
                .param(dbparam).param(srcExpr).param(destExpr)
                .body("SelectQuery sel;")
                .body("sel.source(table);")
                .body("sel.result(" + rel.fieldTypeName + ".fullName());")
                .body("sel.where(srcExpr);")
                .body("return DataSource<" + rel.objectName 
                      + ">(db, "+rel.objectName+"::id_.in(sel) && expr);");
            cl.method(get);
        }
    } else {
        map<string, int> counter;
        for (size_t i2 = 0; i2 < r.related.size(); i2++) {
            string num;
            xml::Relate& rel = r.related[i2];
            if (r.countTypes(rel.objectName) > 1) {
                if (counter.find(rel.objectName) == counter.end())
                    counter[rel.objectName] = 0;
                counter[rel.objectName]++;
                num = toString(counter[rel.objectName]);        
            }
            rel.getMethodName = "get" + rel.objectName + num;
            Method get(rel.getMethodName, 
                       "litesql::DataSource<" 
                        + rel.objectName + ">");

            get.static_()
                .param(dbparam).param(srcExpr).param(destExpr)
                .body("SelectQuery sel;")
                .body("sel.source(table);")
                .body("sel.result(" + rel.fieldTypeName + ".fullName());")
                .body("sel.where(srcExpr);")
                .body("return DataSource<" + rel.objectName 
                      + ">(db, "+rel.objectName+"::id_.in(sel) && expr);");
            cl.method(get);
        }
    }


}
void makeRelationHandles(map<string, xml::Object*>& objMap,
                         vector<xml::Object>& objects,
                         vector<xml::Relation>& relations) {
    for (size_t i = 0; i < relations.size(); i++) {
        xml::Relation& rel = relations[i];
        for (size_t i2 = 0; i2 < rel.related.size(); i2++) {
            xml::Relate& relate = rel.related[i2];
            if (objMap.find(relate.objectName) 
                == objMap.end()) 
                throw Except("undefined object : "                                      + relate.objectName);
        }
        for (size_t i2 = 0; i2 < rel.related.size(); i2++) {
            xml::Relate& relate = rel.related[i2];
            xml::Object* obj = objMap[relate.objectName];
            if (obj->relations.find(&rel) == obj->relations.end())
                obj->relations[&rel] = vector<xml::Relate*>();
            obj->relations[&rel].push_back(&relate);
            if (!relate.handle.empty()) {
                xml::RelationHandle handle(relate.handle, &rel,
                                           &relate, obj);
                for (size_t i3 = 0; i3 < rel.related.size(); i3++) {
                    if (i2 != i3) {
                        xml::Object* o = objMap[rel.related[i3].objectName];
                        handle.destObjects.push_back(make_pair(o,&rel.related[i3]));
                    }
                }
                obj->handles.push_back(handle);
            }
        }
    }
}
Records getSchema(const vector<xml::Object>& objects,
                  const vector<xml::Relation>& relations) {
    Records recs;
    Record rec;
    rec.push_back(quote("schema"));
    rec.push_back(quote("table"));
    rec.push_back(quote("CREATE TABLE schema (name TEXT, type TEXT, sql TEXT);"));
    recs.push_back(rec);
    for (size_t i = 0; i < objects.size(); i++) {
        const xml::Object& obj = objects[i];
        Split fields;

        fields.push_back(obj.name + "::id_.name() + \" \" + backend->getRowIDType()");
        for (size_t i2 = obj.parentObject ? 0 : 1; 
             i2 < obj.fields.size(); i2++) {
            fields.push_back(obj.name + "::" + obj.fields[i2].name + "_.name()" 
                             + " + \" \" + " + obj.name + "::" + obj.fields[i2].name + "_.type()");
        }
        rec.clear();
        rec.push_back(obj.name + "::table__");
        rec.push_back(quote("table"));
        rec.push_back(quote("CREATE TABLE ") + " + " + obj.name + "::table__"
                      + " + \" (\" + " + fields.join(" + \",\" + ") + " + \")\"");
        recs.push_back(rec);
        if (!obj.parentObject) {
            rec.clear();
            rec.push_back(obj.name + "::sequence__");
            rec.push_back(quote("sequence"));
            rec.push_back(quote("CREATE SEQUENCE \" + " 
                                + obj.name + "::sequence__ + \"" 
                                + " START 1 INCREMENT 1"));
            recs.push_back(rec);
        }
    }
        
    for (size_t i = 0; i < relations.size(); i++) { 
        const xml::Relation& rel = relations[i];
        Split fields;
        for (size_t i2 = 0; i2 < rel.related.size(); i2++) {
            const xml::Relate& relate = rel.related[i2];
            string extra;
            if (relate.isUnique())
                    extra = " + " + quote(" UNIQUE");
            else if (rel.related.size() == 2) { 
                if (i2 == 0 && rel.related[1].hasLimit())
                    extra = " + " + quote(" UNIQUE");
                if (i2 == 1 && rel.related[0].hasLimit())
                    extra = " + " + quote(" UNIQUE");
            }
            
                
            fields.push_back(rel.getName() + "::" 
                             + relate.fieldTypeName + ".name() + \" \" + "
                                   + rel.getName() + "::" + relate.fieldTypeName + ".type()" 
                             + extra);

        }
        for (size_t i2 = 0; i2 < rel.fields.size(); i2++) {
            const xml::Field& fld = rel.fields[i2];
            fields.push_back(rel.getName() + "::" + fld.name 
                             + "_.name() + \" \" + " + 
                             rel.getName() + "::" + fld.name + "_.type()");
        }
        rec.clear();
        rec.push_back(rel.getName() + "::table");
        rec.push_back(quote("table"));
        rec.push_back(quote("CREATE TABLE ") + " + " + rel.getName() + "::table"
                      + " + \" (\" + " 
                      + fields.join("+ \",\" + ") + " + \")\"");
        recs.push_back(rec);
    }
    return recs;
}
void writeCPPClasses(FILE* hpp, FILE* cpp,
                       const string& dbName,
                       vector<xml::Object>& objects,
                       vector<xml::Relation>& relations) {
    map<string, xml::Object*> objMap;
    
    for (size_t i = 0; i < objects.size(); i++)
        objMap[objects[i].name] = &objects[i];
    for (size_t i = 0; i < objects.size(); i++) 
        if (objMap.find(objects[i].inherits) != objMap.end())
            objects[i].parentObject = objMap[objects[i].inherits];
    for (size_t i = 0; i < objects.size(); i++) 
        if (objects[i].parentObject)
            objects[i].parentObject->children.push_back(&objects[i]);
    
        
    makeRelationHandles(objMap, objects, relations);

    for (size_t i = 0; i < objects.size(); i++) 
        fprintf(hpp, "class %s;\n", objects[i].name.c_str());
    for (size_t i = 0; i < relations.size(); i++) {
        xml::Relation & o = relations[i];
        sort(o.related.begin(), o.related.end());
        Class cl(o.getName());
        writeStaticRelData(cl, o);
        writeRelMethods(cl, o);
        cl.write(hpp, cpp);
    }
    for (size_t i = 0; i < objects.size(); i++) {
        xml::Object & o = objects[i];
        Class cl(o.name, o.inherits);
        writeStaticObjData(cl, o);
        writeObjFields(cl, o);       
        writeObjConstructors(cl, o);
        writeObjRelationHandles(cl, o);
        writeObjBaseMethods(cl, o);
        cl.write(hpp, cpp);
        gen::Method strMtd("operator<<", "std::ostream &");
        strMtd.param(Variable("os", "std::ostream&"))
            .param(Variable("o", o.name))
            .body("os << \"-------------------------------------\""
                    "<< std::endl;");

        for (size_t i2 = 0; i2 < o.fields.size(); i2++) {
            xml::Field& fld = o.fields[i2];
            strMtd.body("os << o." + fld.name + ".name() << \" = \" << o." 
                        + fld.name + " << std::endl;");
        }
        strMtd.body("os << \"-------------------------------------\""
                    "<< std::endl;")
              .body("return os;");
        strMtd.write(hpp, cpp, "", 0);
    }

    gen::Class db(dbName, "litesql::Database");    
    gen::Method cons(dbName);
    
    cons.param(Variable("backendType", "std::string"))
        .param(Variable("connInfo", "std::string"))
        .constructor("litesql::Database(backendType, connInfo)");
    db.method(cons);
    gen::Method getSchemaMtd("getSchema", "std::vector<litesql::Database::SchemaItem>");
    getSchemaMtd.virtual_().protected_().const_()
        .body("vector<Database::SchemaItem> res;");
    Records schema = getSchema(objects, relations);
    for (Records::iterator i = schema.begin(); i != schema.end(); i++) {
        if ((*i)[1] == quote("sequence"))
            getSchemaMtd.body("if (backend->supportsSequences())");
        getSchemaMtd.body("res.push_back(Database::SchemaItem(" 
                       + (*i)[0] + "," 
                       + (*i)[1] + ","
                       + (*i)[2] + "));");
    }
    
    getSchemaMtd.body("return res;");
    db.method(getSchemaMtd);
    db.write(hpp, cpp);
}


#include "litesql/split.hpp"
#include "litesql/types.hpp"
#include "litesql-gen-cpp.hpp"
#include "cppgenerator.hpp"
#include "common.hpp"
#include <ctype.h>
#include <cstdio>
#include <map>

using namespace std;
using namespace gen;
using namespace litesql;

static string brackets2(string s) {
    return "<" + s + ">";
}

static string getInherits(xml::Object* o) {
    if (o->inherits == "")
        return "litesql::Persistent";
    return o->inherits;
}

static string className(xml::Type* t) {
    
    return xml::capitalize(t->name) + "Type";
}

class CPPGenerator {

    xml::Database& db;
    Args& args;
    bool multiFiles;

    string unitName;
    string headerFileName;
    string headerText;
    FileStream cpp, hpp;


private:

    
    CPPGenerator& unit(const std::string& s) {
        unitName = toLower(s);
        headerFileName = unitName + "." + args["hppext"];
        cpp.file(unitName + "." + args["cppext"]);
        hpp.file(headerFileName);
        return *this;
    }

    void startUnit(const Split& includes = Split()) {
        string defName = "_" + unitName + "_" + args["hppext"] + "_";
        hpp << "#ifndef " << defName << "\n";
        hpp << "#define " << defName << "\n";
        hpp << "\n";
        if (!headerText.empty()) {
            hpp << headerText;
            hpp << "\n";
        }
        Split inc = includes;
        inc.push_back("litesql.hpp");

        for (size_t i = 0; i < inc.size(); i++)
            hpp << "#include " << quote(inc[i]) << "\n";

        cpp << "#include " << quote(headerFileName) << "\n";


        string ns = "namespace " + db.nspace + " {\n";
        hpp << "\n" << ns;
        cpp << "\n" << ns;

    }

    void endUnit() {
        hpp << "}\n";
        hpp << "#endif\n";

        cpp << "}\n";
    }

    void writeType(xml::Type* type) {
        if (type->internal)
            return;
        
        Class c(className(type), "litesql::FieldType");

        c.write(hpp, cpp);
    }

    void writeObject(xml::Object* obj) {
        Class c(obj->name, getInherits(obj));

        c.write(hpp, cpp);
    }

    void writeRelation(xml::Relation* rel) {
        if (rel->abstract)
            return;

        Class c(rel->name, "litesql::Relation");

        c.write(hpp, cpp);
    }

    template <class T>
    void writeMany(vector<T*>& items, 
                   const string& ext,
                   void (CPPGenerator::* func)(T* p)) {

        for (size_t i = 0; i < items.size(); i++) {
            T* t = items[i];

            if (multiFiles) 
                unit(t->name + ext).startUnit();

            (this->*func)(t);

            if (multiFiles)
                endUnit();    
        }
    }

    template <class T>
    void addIncludes(Split& includes, vector<T*>& items, const string& ext) {
        for (size_t i = 0; i < items.size(); i++)
            includes.push_back(toLower(items[i]->name) 
                               + ext + "." + args["hppext"]);
    }

public:

    CPPGenerator(xml::Database& db_, Args& args_) 
        : db(db_), args(args_), multiFiles(args.has("multifiles")) {
            if (args.has("prepend-file"))
                headerText = readFile(args["prepend-file"]);
        }

    void run() {
        Split includes;

        if (multiFiles) {
            addIncludes(includes, db.types, "type");
            addIncludes(includes, db.objects, "objects");
            addIncludes(includes, db.relations, "relation");
        }

        unit(db.name).startUnit(includes);
       
        writeMany(db.types, "type", &CPPGenerator::writeType);

        writeMany(db.objects, "object", &CPPGenerator::writeObject);

        writeMany(db.relations, "relation", &CPPGenerator::writeRelation);

        unit(db.name);
        endUnit();
        cpp.sync();
        hpp.sync();
    }
};


void writeCPPClasses(xml::Database& db, Args& args) {
    CPPGenerator g(db, args);
    g.run();

}


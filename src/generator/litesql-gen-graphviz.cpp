#include "litesql-gen-graphviz.hpp"
using namespace std;
using namespace xml;
void writeInheritance(FILE* f, 
                      vector<Object>& objects) {
    for (size_t i = 0; i < objects.size(); i++) {
        Object& o = objects[i];
        fprintf(f, "    \"%s\"", o.name.c_str());
        if (o.parentObject)
            fprintf(f, " -> \"%s\"", o.inherits.c_str());
        fprintf(f, ";\n");
    }
}

void writeRelations(FILE* f,
                    vector<Object>& objects, 
                    vector<Relation>& relations) {
    for (size_t i = 0; i < relations.size(); i++) {
        Relation& r = relations[i];
        string name = replace(r.getName(), "Relation", "");

        for (size_t i2 = 0; i2 < r.related.size(); i2++) {
            Relate& rel = r.related[i2];
            string extra;
            if (rel.handle.size() > 0)
                extra = " [label=\"" + rel.handle + "\"]";
            fprintf(f, "    \"%s\" -> \"%s\"%s;\n", 
                    rel.objectName.c_str(), name.c_str(),
                    extra.c_str());
        }
    }
}

void writeGraphviz(Database& db,
                   vector<Object>& objects,
                   vector<Relation>& relations) {
   string fname = toLower(db.name + ".dot"); 
   FILE* f = fopen(fname.c_str(), "w");
   if (!f) {
        string msg = "could not open file : " + fname;
        perror(msg.c_str());
        return;
   }
   fprintf(f, "digraph database {\n");
   fprintf(f, "  size = \"6,6\";\n");
   fprintf(f, "  node[color=lightblue,style=filled];\n");
   fprintf(f, "  subgraph inheritance {\n");
   fprintf(f, "    edge[style=dashed];\n");
 //  writeInheritance(f, objects);
   fprintf(f, "  }\n");
   fprintf(f, "  subgraph relations {\n");
   fprintf(f, "    edge[dir=none];\n");
   writeRelations(f, objects, relations);
   fprintf(f, "  }\n");
   fprintf(f, "}\n");
   fclose(f);
}



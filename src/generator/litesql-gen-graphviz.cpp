#include "litesql-gen-graphviz.hpp"
using namespace std;
using namespace xml;
void writeInheritance(FILE* f, 
                      vector<Object*>& objects) {
    for (size_t i = 0; i < objects.size(); i++) {
        Object& o = *objects[i];
        fprintf(f, "    \"%s\"", o.name.c_str());
        if (o.parentObject)
            fprintf(f, " -> \"%s\"", o.inherits.c_str());
        fprintf(f, ";\n");
    }
}

void writeRelations(FILE* f,
                    vector<Object*>& objects, 
                    vector<Relation*>& relations) {
    for (size_t i = 0; i < relations.size(); i++) {
        Relation& r = *relations[i];

        for (size_t i2 = 0; i2 < r.related.size(); i2++) {
            Relate& rel = *r.related[i2];
            string extra;
            for (size_t i3 = 0; i3 < r.related.size(); i3++) {
                if (i3 == i2) 
                    continue;

                Relate& destRel = *r.related[i3];
                if (rel.handle.size() > 0) {
                    extra = " [label=\"" + rel.handle + "\"]";
                    fprintf(f, "    \"%s\" -> \"%s\"%s;\n", 
                            rel.objectName.c_str(), destRel.objectName.c_str(),
                            extra.c_str());
                }
            }
        }
    }
}

void writeGraphviz(xml::Database& db, Args& args) {
    vector<xml::Object*>& objects = db.objects;
    vector<xml::Relation*>& relations = db.relations;

    string fname = toLower(db.name + ".dot"); 
    FILE* f = fopen(fname.c_str(), "w");
    if (!f) {
        string msg = "could not open file : " + fname;
        perror(msg.c_str());
        return;
    }
    fprintf(f, "digraph database {\n");
    fprintf(f, "  node[shape=box,color=black];\n");
    fprintf(f, "  subgraph inheritance {\n");
    fprintf(f, "    edge[style=dashed,dir=forward,arrowhead=normal];\n");
    writeInheritance(f, objects);
    fprintf(f, "  }\n");
    fprintf(f, "  subgraph relations {\n");
    fprintf(f, "    edge[dir=forward,arrowhead=vee];\n");
    writeRelations(f, objects, relations);
    fprintf(f, "  }\n");
    fprintf(f, "}\n");
    fclose(f);
}



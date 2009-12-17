#include "litesql-gen-graphviz.hpp"
#include <fstream>

using namespace std;
using namespace xml;


bool GraphvizGenerator::generate(std::ostream& os,xml::Object* const object    , size_t indent)
{
  string indents(indent,' ');
  os << indents << '"' << object->name << '"';
  
  if (object->parentObject)
    os << " -> \"" << object->inherits << "\"";
  
  os << ';' << endl;
  return true;
}

bool GraphvizGenerator::generate(std::ostream& os,xml::Relation* const relation,size_t indent)
{
  string indents(indent,' ');
  Relation& r = *relation;
  for (size_t i2 = 0; i2 < r.related.size(); i2++) {
    Relate& rel = *r.related[i2];
    string extra;
    for (size_t i3 = 0; i3 < r.related.size(); i3++) {
      if (i3 == i2) 
        continue;

      Relate& destRel = *r.related[i3];
      if (rel.handle.size() > 0) {
        extra = " [label=\"" + rel.handle + "\"]";
        os << indents << "\"" << rel.objectName << "\" -> \""<< destRel.objectName <<"\""<< " [label=\"" << rel.handle << "\"]" <<";" <<endl; 
      }
    }
  }
  return true;
}

bool GraphvizGenerator::generateCode(const ObjectModel* model)
{

  string fname = getOutputDirectory();

  if (!fname.empty())
  {
#ifdef WIN32
    fname.append("\\");
#else
    fname.append("/");
#endif // #ifdef _WINDOWS_
  }
  fname.append(toLower(model->db.name + ".dot")); 

  ofstream os(fname.c_str());
  os << "digraph database {" << endl
     << "  node[shape=box,color=black];" << endl
     << "  subgraph inheritance {" << endl
     << "    edge[style=dashed,dir=forward,arrowhead=normal];" << endl;
    
  CodeGenerator::generate(os,model->objects,4);

  os << "  }" << endl
    << "  subgraph relations {" << endl
    << "    edge[dir=forward,arrowhead=vee];" << endl;
  CodeGenerator::generate(os,model->relations,4);
  os << "  }" << endl
    << "}" << endl;

  os.close();
  return true;
}



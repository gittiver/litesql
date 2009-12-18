#include "litesql-gen-ruby-activerecord.hpp"
#include <fstream>

using namespace std;
using namespace xml;


bool RubyActiveRecordGenerator::generate(xml::Object* const object)
{
  string fname = getOutputFilename(toLower(object->name + ".rb"));

  ofstream os(fname.c_str());
  
  
  string baseClass = object->parentObject ? object->inherits : "ActiveRecord::Base"; 
  os << "class " << object->name << " < " << baseClass << endl;
  
  for (vector<RelationHandle*>::const_iterator it = object->handles.begin(); it!= object->handles.end(); it++) {
    os << (*it)->object << ":"  << (*it)->name << endl;
  }
  
  os << "end" << endl;
  os.close();
  return true;
}

bool RubyActiveRecordGenerator::generateCode(const ObjectModel* model)
{
  CodeGenerator::generate(model->objects);
  return true;
}



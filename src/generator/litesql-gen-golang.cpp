#include "litesql-gen-golang.hpp"
#include <fstream>

using namespace std;
using namespace xml;

const char* GolangGenerator::NAME = "golang";
const char* GolangModuleGenerator::NAME = "golang-modulegenerator";

static string toGolangType(AT_field_type field_type)
{
  switch(field_type)
  {
    case A_field_type_integer:
      return "int";
    case A_field_type_float:
      return "float";
    case A_field_type_double:
      return "double";
    case A_field_type_boolean:
      return "bool";
    case A_field_type_date:
      return "date";
    case A_field_type_time:
      return "timestamp";
    case A_field_type_datetime:
      return "datetime";
    case A_field_type_blob:
      return "blob";
    case A_field_type_string:
      return "string";
    default:
      return "unknown";
  }
}

GolangGenerator::GolangGenerator(): CompositeGenerator(GolangGenerator::NAME)
{
  add(new GolangModuleGenerator());
}

bool GolangModuleGenerator::generate(const xml::ObjectPtr& object)
{
  string fname = getOutputFilename(toLower(object->name + ".go"));

  ofstream os(fname.c_str());
  os  << "import (" << endl
  << "\"github.com/jinzhu/gorm\"" << endl
  << ")" << endl
  << endl;

  os  << "struct " << object->name  << "{" << endl
  << "  gorm.Model" << endl;

  for (Field::sequence::const_iterator it = object->fields.begin(); it!= object->fields.end(); it++)
  {
    // TODO we want to generate for gorm, so we have to add tag conforming to gorm syntax
    os << "  " << (*it)->name << " " << toGolangType((*it)->type);
    if ((*it)->isIndexed()||(*it)->isUnique()) {
      os << " `gorm:\"";
      bool semicolon = (*it)->isIndexed();
      if ((*it)->isIndexed()) {
        os << "index";
      }
      if ((*it)->isUnique()) {
        if (semicolon){
          os << ";";
        }
        os << "unique";
      }
      os << "\"`";

    }

    os << endl;
  }

  for (RelationHandle::sequence::const_iterator it = object->handles.begin(); it!= object->handles.end(); it++)
  {

    os  << "  " << (*it)->name
    << ((*it)->relate->hasLimit() ? "" : " []");

    //       if (!(*it)->name.empty())
    //           os << ", :through => :" << (*it)->name;


    os  << endl;

  }

  os << "}" << endl;
  os.close();
  return true;
}

bool GolangModuleGenerator::generateCode(const ObjectModel* model)
{
  CodeGenerator::generate(model->objects);
  return true;
}

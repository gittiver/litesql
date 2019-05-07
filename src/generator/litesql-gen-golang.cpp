#include "litesql-gen-golang.hpp"
#include <fstream>

using namespace std;
using namespace xml;

const char* GolangGenerator::NAME = "golang";
const char* GolangModuleGenerator::NAME = "golang-modulegenerator";
const char *keywords[]  = {
  "type"
  ,
};

static bool is_keyword(const std::string& s) {
  bool result = false;
  for (auto word: keywords){
    if (s==word) {
      result = true;
      break;
    }
  }
  return result;
}


static string toGolangType(AT_field_type field_type)
{
  switch(field_type)
  {
    case A_field_type_integer:
      return "int";
    case A_field_type_float:
      return "float32";
    case A_field_type_double:
      return "float64";
    case A_field_type_boolean:
      return "bool";
    case A_field_type_date:
      return "date";
    case A_field_type_time:
      return "timestamp";
    case A_field_type_datetime:
      return "datetime";
    case A_field_type_blob:
      return "bytes.buffer";
    case A_field_type_string:
      return "string";
    default:
      return "unknown";
  }
}

GolangGenerator::GolangGenerator()
  : CompositeGenerator(GolangGenerator::NAME)
{
  add(new GolangModuleGenerator());
}

GolangModuleGenerator::GolangModuleGenerator()
  : CodeGenerator(NAME)
{}

bool GolangModuleGenerator::generateCode(const ObjectModel* model)
{
  string nspace = toLower(model->db->nspace);
  string package = toLower(model->db->name);
  ofstream os(package+".go");

  os  << "package " << package << endl
      << endl;

  os  << "import ("   << endl
      << "\"bytes\"" << endl
      << "\"github.com/jinzhu/gorm\"" << endl
      << ")" << endl
      << endl;

  CodeGenerator::generate(model->objects,os,0);
  os.close();
  return true;
}

bool GolangModuleGenerator::generate(const xml::ObjectPtr& object,
                                     std::ostream& os,
                                     size_t UNUSED_ARG(indent))
{

  os   << "type " << object->name  << " struct {" << endl
       << "  gorm.Model" << endl;

  for (Field::sequence::const_iterator it = object->fields.begin(); it!= object->fields.end(); it++)
  {
    bool name_is_keyword = is_keyword((*it)->name);
    vector<string> gormtags;
    if (name_is_keyword) {
      os << "  " << (*it)->name << "_" << " " << toGolangType((*it)->type);
      gormtags.push_back("column:"+(*it)->name);
    }
    else {
      // TODO we want to generate for gorm, so we have to add tag conforming to gorm syntax
      os << "  " << (*it)->name << " " << toGolangType((*it)->type);
    }

    if ((*it)->name == "id") {
      gormtags.push_back("primary_key");
    }
    else {
      if ((*it)->isIndexed()) {
        gormtags.push_back("index");
      }
      if ((*it)->isUnique()) {
        gormtags.push_back("unique");
      }
    }

    if (!gormtags.empty()) {
      os << " `gorm:\"" << Split::join(gormtags,";") << "\"`";
    }

    os << endl;
  }

  for (auto handle : object->handles)
  {
    os  << "  " << handle->name  << " "
        << (handle->relate->hasLimitOne() ? " []":"")
        << handle->destObjects[0].first->name
        << endl;
  }

  os << "}" << endl;
  return true;
}

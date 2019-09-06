#include "litesql-gen-golang.hpp"
#include <fstream>
#include <set>

using namespace std;
using namespace xml;

const char* GolangGenerator::NAME = "golang";
const char* GolangModuleGenerator::NAME = "golang-modulegenerator";
const char *keywords[]  = {
  "type"
  ,
};
const char *gorm_model_keywords[]  = {
  "created_at"
  , "edited_at"
  , "id"

};

static bool is_keyword(const std::string& s) {
  bool result = false;
  for (auto word: keywords) {
    if (s==word) {
      result = true;
      break;
    }
  }
  return result;
}

static bool has_gorm_model(const xml::ObjectPtr& object) {
  bool result[] = {false,false,false};
  for(auto field: object->fields) {
    if (!result[0])
      result[0] = field->name == gorm_model_keywords[0];
    if (!result[1])
      result[1] = field->name == gorm_model_keywords[1];
    if (!result[2])
      result[2] = field->name == gorm_model_keywords[2];
    if (result[0] && result[1] && result[2])
      break;
  }
  return result[0] && result[1] && result[2];
}

static bool has_gorm_model(const ObjectSequence& objects) {
  for (auto o: objects)
  {
    if (has_gorm_model(o))
      return true;
  }
  return false;
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
      return "time.Time";
    case A_field_type_datetime:
      return "time.Time";
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

std::set<AT_field_type> get_types(const ObjectModel* model)
{
  std::set<AT_field_type> result;
  for (auto object: model->objects)
  {
    for(auto field: object->fields)
    {
      result.insert(field->type);
    }
  }
  return result;
}
bool GolangModuleGenerator::generateCode(const ObjectModel* model)
{
  string nspace = toLower(model->db->nspace);
  string package = toLower(model->db->name);
  ofstream os(package+".go");

  os  << "package " << package << endl
      << endl;

  std::set<AT_field_type> types = get_types(model);

  os  << "import ("   << endl;

  if (   (types.end() != types.find(AT_field_type::A_field_type_time))
      || (types.end() != types.find(AT_field_type::A_field_type_date))
      || (types.end() != types.find(AT_field_type::A_field_type_datetime)))
  {
      os << "\"time\"" << endl;
  }
      //      << "\"bytes\"" << endl
  if (has_gorm_model(model->objects)) {
    os  << "\"github.com/jinzhu/gorm\"" << endl;
  }
  os  << ")" << endl
      << endl;

  CodeGenerator::generate(model->objects,os,0);
  os.close();
  return true;
}

bool isGormModelColumn(const counted_ptr<Field>& f) {
  return
     f->name == gorm_model_keywords[0]
  || f->name == gorm_model_keywords[1]
  || f->name == gorm_model_keywords[2];
}

bool GolangModuleGenerator::generate(const xml::ObjectPtr& object,
                                     std::ostream& os,
                                     size_t UNUSED_ARG(indent))
{

  os   << "type " << object->name  << " struct {" << endl;

  bool found_gorm_model = has_gorm_model(object);

  if (found_gorm_model) {
    // we found at least one field which is part of GormObjectModel
    // so we include it first here
    os << "  gorm.Model" << endl;
  };

  for (Field::sequence::const_iterator it = object->fields.begin(); it!= object->fields.end(); it++)
  {
    bool name_is_keyword = is_keyword((*it)->name);
    vector<string> gormtags;
    if (found_gorm_model && isGormModelColumn(*it)) {
      // already included with Gorm Model
    } else if (name_is_keyword) {
      os << "  " << capitalize((*it)->name) << "_" << " " << toGolangType((*it)->type);
      gormtags.push_back("column:"+(*it)->name);
    } else {
      // TODO we want to generate for gorm, so we have to add tag conforming to gorm syntax
      os << "  " << capitalize((*it)->name) << " " << toGolangType((*it)->type);
    }

    if (found_gorm_model && (*it)->name == "id") {
      // do nothing as it is already handled in model
      //gormtags.push_back("primary_key");
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
      os << " `gorm:\"" << join(gormtags,";") << "\"`";
    }

    os << endl;
  }

  for (auto handle : object->handles)
  {
    os  << "  " << capitalize(handle->name)  << " "
        << (handle->relate->hasLimitOne() ? " []":"")
        << handle->destObjects[0].first->name
        << endl;
  }

  os << "}" << endl;
  return true;
}

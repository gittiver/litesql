// include LiteSQL's header file and generated header file
#include "xmlparser.hpp"
#include "objectmodel.hpp"
#include <string.h>
#include "logger.hpp"

using namespace std;
using namespace xml;

#define xmlStrcasecmp(s1,s2)  ((s1==NULL) ? (s2!=NULL) : strcmp(s1,s2))
#define xmlStrEqual(s1,s2)   (!strcmp(s1,s2))

const char* litesql::toString(AT_field_type t)
{
  switch (t) 
  {
  case A_field_type_boolean:
    return "boolean";
  case A_field_type_integer:
    return "integer";
  case A_field_type_string:
    return "string";
  case A_field_type_float: 
    return "float";
  case A_field_type_double: 
    return "double";
  case A_field_type_time:
    return "time";
  case A_field_type_date:
    return "date";
  case A_field_type_datetime:
    return "datetime";
  case A_field_type_blob: 
    return "blob";

  default: 
    return "unknown";   
  }
}

AT_field_type litesql::field_type(const char* value)
{
  AT_field_type t;

  if (!xmlStrcasecmp(value,(XML_Char*)"boolean"))
  {
    t = A_field_type_boolean;
  }
  else if (!xmlStrcasecmp(value,(XML_Char*)"integer"))
  {
    t = A_field_type_integer;
  }
  else if (!xmlStrcasecmp(value,(XML_Char*)"string"))
  {
    t = A_field_type_string;
  }
  else if (!xmlStrcasecmp(value,(XML_Char*)"float"))
  {
    t = A_field_type_float;
  }
  else if (!xmlStrcasecmp(value,(XML_Char*)"double"))
  {
    t = A_field_type_double;
  }
  else if (!xmlStrcasecmp(value,(XML_Char*)"time"))
  {
    t = A_field_type_time;
  }
  else if (!xmlStrcasecmp(value,(XML_Char*)"date"))
  {
    t = A_field_type_date;
  }
  else if (!xmlStrcasecmp(value,(XML_Char*)"datetime"))
  {
    t = A_field_type_datetime;
  }
  else if (!xmlStrcasecmp(value,(XML_Char*)"blob"))
  {
    t = A_field_type_blob;
  }
  else
  {
    t = AU_field_type;   
  }
  return t;
}

static const char* toString(AT_relate_limit t)
{
  switch (t) 
  {
  case A_relate_limit_one:
    return "one";
  case A_relate_limit_many:
    return "many";
  case AU_relate_limit:
  default:
    return "unknown";   
  }
}


static AT_field_unique field_unique(const XML_Char* value)
{
  AT_field_unique t;
  if (!xmlStrcasecmp(value,(XML_Char*)"true"))
  {
    t = A_field_unique_true;
  }
  else if (!xmlStrcasecmp(value,(XML_Char*)"false"))
  {
    t = A_field_unique_false;
  }
  else
  {
    t = AU_field_unique;
  }
  return t;
}

static AT_index_unique index_unique(const XML_Char* value)
{
  AT_index_unique t;
  if (!xmlStrcasecmp(value,(XML_Char*)"true"))
  {
    t = A_index_unique_true;
  }
  else if (!xmlStrcasecmp(value,(XML_Char*)"false"))
  {
    t = A_index_unique_false;
  }
  else
  {
    t = AU_index_unique;
  }
  return t;
}


static AT_field_indexed field_indexed(const XML_Char* value)
{
  AT_field_indexed t;
  if (!xmlStrcasecmp(value,(XML_Char*)"true"))
  {
    t = A_field_indexed_true;
  }
  else if (!xmlStrcasecmp(value,(XML_Char*)"false"))
  {
    t = A_field_indexed_false;
  }
  else
  {
    t = AU_field_indexed;
  }
  return t;
}

static AT_relation_unidir relation_unidir(const XML_Char* value)
{
  AT_relation_unidir t;
  if (!xmlStrcasecmp(value,(XML_Char*)"true"))
  {
    t = A_relation_unidir_true;
  }
  else if (!xmlStrcasecmp(value,(XML_Char*)"false"))
  {
    t = A_relation_unidir_false;
  }
  else
  {
    t = AU_relation_unidir;
  }
  return t;
}

static AT_relate_unique relate_unique(const XML_Char* value)
{
  AT_relate_unique t;
  if (!xmlStrcasecmp(value,(XML_Char*)"true"))
  {
    t = A_relate_unique_true;
  }
  else if (!xmlStrcasecmp(value,(XML_Char*)"false"))
  {
    t = A_relate_unique_false;
  }
  else
  {
    t = AU_relate_unique;
  }
  return t;
}

static AT_relate_limit relate_limit(const XML_Char* value)
{
  AT_relate_limit t;
  if (!xmlStrcasecmp(value,(XML_Char*)"one"))
  {
    t = A_relate_limit_one;
  }
  else if (!xmlStrcasecmp(value,(XML_Char*)"many"))
  {
    t = A_relate_limit_many;
  }
  else
  {
    t = A_relate_limit_many;
  }
  return t;

}

namespace xml {
  class LitesqlParser : public XmlParser {
  public:
    LitesqlParser(ObjectModel* model)
      : m_pObjectModel(model),
      m_parseState(ROOT) {};

  protected:
    void onStartElement(const XML_Char *fullname,
      const XML_Char **atts);

    void onEndElement(const XML_Char *fullname);
    /** ROOT->DATABASE; 
    *
    *    DATABASE->OBJECT;
    *      OBJECT->FIELD;
    *      OBJECT->METHOD;
    *      FIELD->OBJECT;
    *      METHOD->OBJECT;
    *
    *    DATABASE->RELATION;
    *    RELATION->DATABASE;
    *
    *  DATABASE->ROOT;
    * ERROR;
    */
    enum ParseState { ROOT,
      DATABASE,
      OBJECT,
      FIELD,
      METHOD,
      PARAM,
      RELATION,
      INDEX,
      INDEXFIELD,
      INCLUDE,
      UNKNOWN,
      ERROR
    };

  private:
    ObjectModel* m_pObjectModel;

    Object * obj;
    Relation * rel;
    Field * fld;
    Field * rel_fld;
    Method * mtd;
    Index::counted_ptr idx;
    IndexField* idxField;

    ParseState m_parseState;
    vector<ParseState> history;
  };

}

void LitesqlParser::onStartElement(const XML_Char *fullname,
                                   const XML_Char **atts)
{
  //   Logger::report("starting " <<fullname );
  history.push_back(m_parseState);

  if (xmlStrEqual(fullname,(XML_Char*) Database::TAG ))
  {
    if (m_parseState!=ROOT)
    {
      m_parseState = ERROR;
    }
    else
    {
      m_parseState = DATABASE;
      
      DatabasePtr pDb(new xml::Database);
      
      pDb->name = safe((char*)xmlGetAttrValue(atts,"name"));
      pDb->include = safe((char*)xmlGetAttrValue(atts,"include"));
      pDb->nspace = safe((char*)xmlGetAttrValue(atts,"namespace"));
      
      m_pObjectModel->db = pDb;
      Logger::report("database = " , m_pObjectModel->db->name);
    }
  } 
  else if (xmlStrEqual(fullname,(XML_Char*)Object::TAG))
  {
    if (m_parseState!=DATABASE)
    {
      m_parseState = ERROR;
    }
    else
    {
      ObjectPtr pObj(obj = new Object(    (char*)xmlGetAttrValue(atts,"name"), 
        safe((char*)xmlGetAttrValue(atts,"inherits")))); 
      m_pObjectModel->objects.push_back(pObj);
      Logger::report("object = ",obj->name);
      m_parseState = OBJECT; 

    }
  } 
  else if (xmlStrEqual(fullname,(XML_Char*)Field::TAG))
  {
    Field* pNewField = new Field(   (char*)xmlGetAttrValue(atts,"name"), 
      field_type(xmlGetAttrValue(atts,"type")),
      safe(  (char*)xmlGetAttrValue(atts,"default")),
      field_indexed(xmlGetAttrValue(atts,"indexed")),
      field_unique(xmlGetAttrValue(atts,"unique"))
      );

    switch(m_parseState)
    {
    case OBJECT:
      if (!obj) {
        Logger::error("parsing field inside object, but currentObject == NULL ");
      }
      else {
        Logger::report("field = ",obj->name);
        Field::counted_ptr field(fld = pNewField);
        obj->fields.push_back(field);
      };
      m_parseState = FIELD;
      break;

    case RELATION:
      if (!rel) {
        Logger::error("parsing field inside relation, but currentRelation == NULL ");
      }
      else
      {
        Field::counted_ptr field(rel_fld = pNewField);
        rel->fields.push_back(field);
        Logger::report("field = ",rel_fld->name );
      }
      m_parseState = FIELD;
      break;

    default:
      delete pNewField;
      m_parseState = ERROR;
    }
  }
  else if (xmlStrEqual(fullname,(XML_Char*)Index::TAG))
  {
    Index::counted_ptr ptrIndex(new Index(index_unique(xmlGetAttrValue(atts,"unique"))));
      
    switch (m_parseState)
    {
    case OBJECT:
      idx = ptrIndex;
      obj->indices.push_back(ptrIndex);
      m_parseState = INDEX;
      break;

    case RELATION:
      idx = ptrIndex;
      rel->indices.push_back(ptrIndex);
      m_parseState = INDEX;
      break;

    default:
      m_parseState = ERROR;
    }
  }
  else if (xmlStrEqual(fullname,(XML_Char*)IndexField::TAG))
  {
    if (m_parseState!=INDEX)
    {
      m_parseState = ERROR;
    }
    else
    {
      IndexField idxField((char*)xmlGetAttrValue(atts,"name"));
      idx->fields.push_back(idxField);
    }    

    m_parseState = INDEXFIELD;
  }
  else if (xmlStrEqual(fullname,(XML_Char*)Value::TAG))
  {
    if (m_parseState!=FIELD)
    {
      m_parseState = ERROR;
    }
    else
    {
      if (fld) 
      {
        fld->value(Value((char*)xmlGetAttrValue(atts,"name"), (char*)xmlGetAttrValue(atts,"value")));
      }
      Logger::report("value = " );
    }

  } 
  else if (xmlStrEqual(fullname,(XML_Char*)Method::TAG))
  {
    if (m_parseState!=OBJECT)
    {
      m_parseState = ERROR;
    }
    else
    {
      Method::counted_ptr m(mtd = new Method( safe((char*)xmlGetAttrValue(atts,"name")), 
        safe((char*)xmlGetAttrValue(atts,"returntype")) 
        ));
      obj->methods.push_back(m);
      m_parseState= METHOD;
      Logger::report("method = ",mtd->name );
    }

  }
  else if (xmlStrEqual(fullname,(XML_Char*)Param::TAG))
  {
    if (m_parseState!=METHOD)
    {
      m_parseState = ERROR;
    }
    else
    {
      char* pszName = (char*)xmlGetAttrValue(atts,"name");	

      mtd->param(Param(pszName,strdup((char*)xmlGetAttrValue(atts,"type"))));
      m_parseState= PARAM;
      Logger::report("param = ",pszName);
    }
  }
  else if (xmlStrEqual(fullname,(XML_Char*)Relation::TAG))
  {
    if (m_parseState!=DATABASE)
    {
      m_parseState = ERROR;
    }
    else
    {
      Relation::counted_ptr ptrRelation(rel = new Relation(  safe((char*)xmlGetAttrValue(atts,"id")), 
                                                   safe((char*)xmlGetAttrValue(atts,"name")),
                                                   relation_unidir(xmlGetAttrValue(atts,"unidir")))); 
      m_pObjectModel->relations.push_back(ptrRelation);

      Logger::report( "relation = " + ptrRelation->getName());

      m_parseState = RELATION;
    }
  } 
  else if (xmlStrEqual(fullname,(XML_Char*)Relate::TAG))
  {
    if (m_parseState!=RELATION)
    {
      m_parseState = ERROR;
    }
    else
    {
      Relate::counted_ptr ptrRel(
        new Relate( safe((char*)xmlGetAttrValue(atts,"object")), 
                    relate_limit(xmlGetAttrValue(atts,"limit")), 
                    relate_unique(xmlGetAttrValue(atts,"unique")), 
                    safe((char*)xmlGetAttrValue(atts,"handle"))
                  ) 
      );

      rel->related.push_back(ptrRel);
      Logger::report("relate = " );
    }
  } 
  else if (xmlStrEqual(fullname,(XML_Char*)"include"))
  {
    string filename((char*)xmlGetAttrValue(atts,"name"));
    if (m_parseState!=DATABASE)
    {
      m_parseState = ERROR;
    }
    else
    {
      Logger::report("include \"" + filename + '"' );
      ObjectModel includedModel;
      if (!includedModel.loadFromFile(filename)) 
      {
        Logger::report("error on parsing included file \"" + filename + '"' );
      }
      m_pObjectModel->objects.insert(m_pObjectModel->objects.end(),includedModel.objects.begin(),includedModel.objects.end());
      m_pObjectModel->relations.insert(m_pObjectModel->relations.end(),includedModel.relations.begin(),includedModel.relations.end());
      m_parseState = INCLUDE;
    }
  } 
  else
  {
    m_parseState = UNKNOWN;
    Logger::error("unknown = ",fullname );
  } 
}

void LitesqlParser::onEndElement(const XML_Char *fullname)
{
  //  Logger::report("ending ",fullname); 
  if (xmlStrEqual(fullname,(XML_Char*)Database::TAG))
  {
    if (m_parseState!=DATABASE)
    {
      m_parseState = ERROR;
    }
    else
    {
      Logger::report("end ",Database::TAG );
      m_parseState = ROOT;
    }
  } 
  else if (xmlStrEqual(fullname,(XML_Char*)Object::TAG))
  {
    if (m_parseState!=OBJECT)
    {
      m_parseState = ERROR;
    }
    else
    {
      Logger::report("end ", Object::TAG );
      obj = NULL;
      fld= NULL;
      rel = NULL;
      rel_fld = NULL;
      m_parseState = DATABASE;
    }
  } 
  else if (xmlStrEqual(fullname,xml::Field::TAG))
  {
    if (m_parseState!=FIELD)
    {
      m_parseState = ERROR;
    }
    else
    {
      Logger::report("end ",xml::Field::TAG );
      fld = NULL;
      m_parseState = history.back();
    }
  } 
  else if (xmlStrEqual(fullname,(XML_Char*)Index::TAG))
  {
    if (m_parseState!=INDEX)
    {
      m_parseState = ERROR;
    }
    else
    {
      Logger::report("end ", Index::TAG );
      idx = Index::counted_ptr(NULL);
      m_parseState = history.back();
    }
  }
  else if (xmlStrEqual(fullname,(XML_Char*)IndexField::TAG))
  {
    if (m_parseState!=INDEXFIELD)
    {
      m_parseState = ERROR;
    }
    else
    {
      Logger::report("end " ,IndexField::TAG);
      m_parseState = history.back();
    }
  }
  else if (xmlStrEqual(fullname,(XML_Char*)Value::TAG))
  {
    if (m_parseState!=FIELD)
    {
      m_parseState = ERROR;
    }
    else
    {
      Logger::report("end ", Value::TAG );
    }

  } 
  else if (xmlStrEqual(fullname,(XML_Char*)Method::TAG))
  {
    if (m_parseState!=METHOD)
    {
      m_parseState = ERROR;
    }
    else
    {
      m_parseState = OBJECT;
      Logger::report("end ", Method::TAG );
    }

  } 
  else if (xmlStrEqual(fullname,(XML_Char*)Param::TAG))
  {
    if (m_parseState!= PARAM)
    {
      m_parseState = ERROR;
    }
    else
    {
      m_parseState = METHOD;
      Logger::report("end ",Param::TAG);
    }
  } 
  else if (xmlStrEqual(fullname,(XML_Char*)Relation::TAG))
  {
    if (m_parseState!=RELATION)
    {
      m_parseState = ERROR;
    }
    else
    {
      Logger::report("end ",Param::TAG );
      rel = NULL;
      rel_fld = NULL;
      m_parseState = DATABASE;
    }
  } 
  else if (xmlStrEqual(fullname,(XML_Char*)Relate::TAG))
  {
    if (m_parseState!=RELATION)
    {
      m_parseState = ERROR;
    }
    else
    {
      rel_fld = NULL;
      Logger::report("end Relate::TAG" );
    }
  } 
  else if (xmlStrEqual(fullname,(XML_Char*)"include"))
  {
    if (m_parseState!=INCLUDE)
    {
      m_parseState = ERROR;
    }
    else
    {
      Logger::report("end ","include" );
      m_parseState = DATABASE;
    }
  } 
  else 
  {
    m_parseState = history.back();
    Logger::error( "end unknown " );
  }

  history.pop_back();
}

ObjectModel::ObjectModel()
: db(NULL)
{}

ObjectModel::~ObjectModel()
{}

bool ObjectModel::loadFromFile(const std::string& filename)
{
  LitesqlParser parser(this);

  bool successfulParsed = parser.parseFile(filename);
  if (successfulParsed)
  {
    try {
      xml::init(db,objects,relations);
    } catch (Except e) {
      Logger::error(e);
      successfulParsed = false;
    }
  }
  return successfulParsed;
}

bool ObjectModel::remove(Field::counted_ptr& field)
{
  if (field.get()!=NULL)
  {  
    Field::sequence::iterator found;
    for ( ObjectSequence::iterator it=objects.begin();
      it !=objects.end();
      it++)
    {
      found = find((*it)->fields.begin(),(*it)->fields.end(),field);
      if (found!=(*it)->fields.end()) 
      {
        (*it)->fields.erase(found);
        return true;
      }
    }
  }
  return false;
}

bool ObjectModel::remove(Relate::counted_ptr& relate)
{
  if (relate.get()!=NULL)
  {  
    Relate::sequence::iterator found;
    for ( Relation::sequence::iterator it= relations.begin();
      it !=relations.end();
      it++)
    {
      found = find((*it)->related.begin(),(*it)->related.end(),relate);
      if (found!=(*it)->related.end()) 
      {
        (*it)->related.erase(found);
        return true;
      }
    }
  }
  return false;
}

bool ObjectModel::remove(xml::Method::counted_ptr& method)
{
  if (method.get()!=NULL)
  {  
    Method::sequence::iterator found;
    for ( xml::ObjectSequence::iterator it=objects.begin();
      it !=objects.end();
      it++)
    {
      found = find((*it)->methods.begin(),(*it)->methods.end(),method);
      if (found!=(*it)->methods.end()) 
      {
        (*it)->methods.erase(found);
        return true;
      }
    }
  }
  return false;
}

bool ObjectModel::remove(xml::ObjectPtr& object)
{
  if (object.get()!=NULL)
  {  
    ObjectSequence::iterator found = find(objects.begin(),objects.end(),object);
    if (found!=objects.end()) 
    {
      objects.erase(found);
      return true;
    }
  }
  return false;
}

bool ObjectModel::remove(xml::Relation::counted_ptr& relation)
{
  if (relation.get()!=NULL)
  {  
    xml::Relation::sequence::iterator found = find(relations.begin(),relations.end(),relation);
    if (found!=relations.end()) 
    {
      relations.erase(found);
      return true;
    }
  }
  return false;
}
    
// include LiteSQL's header file and generated header file
#include <iostream>
#include "xmlparser.hpp"
#include "objectmodel.hpp"

using namespace std;
using namespace xml;

AT_field_type field_type(const xmlChar* value)
{
   AT_field_type t;

   if (!xmlStrcasecmp(value,(xmlChar*)"boolean"))
   {
      t = A_field_type_boolean;
   }
   else if (!xmlStrcasecmp(value,(xmlChar*)"integer"))
   {
      t = A_field_type_integer;
   }
   else if (!xmlStrcasecmp(value,(xmlChar*)"string"))
   {
      t = A_field_type_string;
   }
   else if (!xmlStrcasecmp(value,(xmlChar*)"float"))
   {
      t = A_field_type_float;
   }
   else if (!xmlStrcasecmp(value,(xmlChar*)"time"))
   {
      t = A_field_type_time;
   }
   else if (!xmlStrcasecmp(value,(xmlChar*)"date"))
   {
      t = A_field_type_date;
   }
   else if (!xmlStrcasecmp(value,(xmlChar*)"datetime"))
   {
      t = A_field_type_datetime;
   }
   else
   {
      t = AU_field_type;   
   }
   return t;
}

AT_field_unique field_unique(const xmlChar* value)
{
   AT_field_unique t;
   if (!xmlStrcasecmp(value,(xmlChar*)"true"))
   {
      t = A_field_unique_true;
   }
   else if (!xmlStrcasecmp(value,(xmlChar*)"false"))
   {
      t = A_field_unique_false;
   }
   else
   {
      t = AU_field_unique;
   }
   return t;
}

AT_field_indexed field_indexed(const xmlChar* value)
{
   AT_field_indexed t;
   if (!xmlStrcasecmp(value,(xmlChar*)"true"))
   {
      t = A_field_indexed_true;
   }
   else if (!xmlStrcasecmp(value,(xmlChar*)"false"))
   {
      t = A_field_indexed_false;
   }
   else
   {
      t = AU_field_indexed;
   }
   return t;
}

AT_relation_unidir relation_unidir(const xmlChar* value)
{
   AT_relation_unidir t;
   if (!xmlStrcasecmp(value,(xmlChar*)"true"))
   {
      t = A_relation_unidir_true;
   }
   else if (!xmlStrcasecmp(value,(xmlChar*)"false"))
   {
      t = A_relation_unidir_false;
   }
   else
   {
      t = AU_relation_unidir;
   }
   return t;
}

class MyParser : public XmlParser {
   public:
      MyParser(ObjectModel* model)
         :  m_parseState(ROOT),
            m_pObjectModel(model){};

   protected:
      void onStartElement(const xmlChar *fullname,
						 const xmlChar **atts);

      void onEndElement(const xmlChar *fullname);
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
                            RELATION,
                            INCLUDE,
                          UNKNOWN,
                          ERROR
      };

   private:
      ObjectModel* m_pObjectModel;

      Object * obj;
      Relation * rel;
      Field * fld;
      Method * mtd;
      Index * idx;

      ParseState m_parseState;
      vector<ParseState> history;
};

void MyParser::onStartElement(const xmlChar *fullname,
						 const xmlChar **atts)
{
//   cout << "starting " <<fullname << endl;
   history.push_back(m_parseState);
   
   if (xmlStrEqual(fullname,(xmlChar*)"database"))
   {
      if (m_parseState!=ROOT)
      {
         m_parseState = ERROR;
      }
      else
      {
         m_parseState = DATABASE;
         m_pObjectModel->db.name = safe((char*)xmlGetAttrValue(atts,"name"));
         m_pObjectModel->db.include = safe((char*)xmlGetAttrValue(atts,"include"));
         m_pObjectModel->db.nspace = safe((char*)xmlGetAttrValue(atts,"namespace"));
         cout << "database = " << m_pObjectModel->db.name << endl;
      }
   } 
   else if (xmlStrEqual(fullname,(xmlChar*)"object"))
   {
      if (m_parseState!=DATABASE)
      {
         m_parseState = ERROR;
      }
      else
      {
         m_pObjectModel->objects.push_back(obj = new Object(    (char*)xmlGetAttrValue(atts,"name"), 
                                             safe((char*)xmlGetAttrValue(atts,"inherits"))));
         cout << "object = " << obj->name << endl;
         m_parseState = OBJECT;
      }
   } 
   else if (xmlStrEqual(fullname,(xmlChar*)"field"))
   {
      switch(m_parseState)
      {
         case OBJECT:
          cout << "field = " << endl;
          if (obj) {
             obj->fields.push_back(fld =new Field( (char*)xmlGetAttrValue(atts,"name"), 
                                                   field_type(xmlGetAttrValue(atts,"type")),
                                                   safe(  (char*)xmlGetAttrValue(atts,"default")),
                                                   field_indexed(xmlGetAttrValue(atts,"indexed")),
                                                   field_unique(xmlGetAttrValue(atts,"unique"))
                                                  )
                                    );
          } 
          m_parseState = FIELD;
          break;

         case RELATION:
            if (rel) {
               rel->fields.push_back(fld =new Field(                 (char*)xmlGetAttrValue(atts,"name"), 
                                                         field_type( xmlGetAttrValue(atts,"type") ),
                                                               safe( (char*)xmlGetAttrValue(atts,"default") ),
                                                      field_indexed( xmlGetAttrValue(atts,"indexed") ),
                                                       field_unique( xmlGetAttrValue(atts,"unique") )
                             )
                                     );
            }
            m_parseState = RELATION;
            break;

         default:
            m_parseState = ERROR;
      }
   } 
   else if (xmlStrEqual(fullname,(xmlChar*)"value"))
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
         cout << "value = " << endl;
      }
   
   } 
   else if (xmlStrEqual(fullname,(xmlChar*)"method"))
   {
   
      if (m_parseState!=OBJECT)
      {
         m_parseState = ERROR;
      }
      else
      {
         cout << "method = " << endl;
      }
   
   } 
   else if (xmlStrEqual(fullname,(xmlChar*)"relation"))
   {
      if (m_parseState!=DATABASE)
      {
         m_parseState = ERROR;
      }
      else
      {
         cout << "relation = " << endl;
         m_pObjectModel->relations.push_back(rel = new Relation(  safe((char*)xmlGetAttrValue(atts,"id")), 
                                                                  safe((char*)xmlGetAttrValue(atts,"name")),
                                                                  relation_unidir(xmlGetAttrValue(atts,"unidir"))));
         m_parseState = RELATION;
      }
   } 
   else if (xmlStrEqual(fullname,(xmlChar*)"relate"))
   {
      if (m_parseState!=RELATION)
      {
         m_parseState = ERROR;
      }
      else
      {
         cout << "relate = " << endl;
      }
   } 
   else if (xmlStrEqual(fullname,(xmlChar*)"include"))
   {
      if (m_parseState!=DATABASE)
      {
         m_parseState = ERROR;
      }
      else
      {
         cout << "include = " << endl;
         m_parseState = INCLUDE;
      }
   } 
   else
   {
      m_parseState = UNKNOWN;
      cerr << "unknown = " << endl;
   } 
}

void MyParser::onEndElement(const xmlChar *fullname)
{
   cout << "ending " <<fullname << endl; 
   if (xmlStrEqual(fullname,(xmlChar*)"database"))
   {
      if (m_parseState!=DATABASE)
      {
         m_parseState = ERROR;
      }
      else
      {
         cout << "end database " << endl;
         m_parseState = ROOT;
      }
   } 
   else if (xmlStrEqual(fullname,(xmlChar*)"object"))
   {
      if (m_parseState!=OBJECT)
      {
         m_parseState = ERROR;
      }
      else
      {
         cout << "end object " << endl;
         obj = NULL;
         m_parseState = DATABASE;
      }
   } 
   else if (xmlStrEqual(fullname,(xmlChar*)"field"))
   {
      if (m_parseState!=FIELD)
      {
         m_parseState = ERROR;
      }
      else
      {
         cout << "end field" << endl;
         fld = NULL;
         m_parseState = history.back();
      }
   } 
   else if (xmlStrEqual(fullname,(xmlChar*)"value"))
   {
      if (m_parseState!=FIELD)
      {
         m_parseState = ERROR;
      }
      else
      {
         cout << "end value" << endl;
      }
   
   } 
   else if (xmlStrEqual(fullname,(xmlChar*)"method"))
   {
      if (m_parseState!=OBJECT)
      {
         m_parseState = ERROR;
      }
      else
      {
         cout << "end method" << endl;
      }
   
   } 
   else if (xmlStrEqual(fullname,(xmlChar*)"relation"))
   {
      if (m_parseState!=RELATION)
      {
         m_parseState = ERROR;
      }
      else
      {
         cout << "end relation " << endl;
         rel = NULL;
         m_parseState = DATABASE;
      }
   } 
   else if (xmlStrEqual(fullname,(xmlChar*)"relate"))
   {
      if (m_parseState!=RELATION)
      {
         m_parseState = ERROR;
      }
      else
      {
         cout << "relate = " << endl;
      }
   } 
   else if (xmlStrEqual(fullname,(xmlChar*)"include"))
   {
      if (m_parseState!=INCLUDE)
      {
         m_parseState = ERROR;
      }
      else
      {
         cout << "end include " << endl;
         m_parseState = DATABASE;
      }
   } 
   else 
   {
      m_parseState = history.back();
      cerr << "end unknown " << endl;
   }

   history.pop_back();
}

ObjectModel::~ObjectModel()
{
}

bool ObjectModel::loadFromFile(const std::string& filename)
{
   MyParser parser(this);
   return parser.parseFile(filename);
}

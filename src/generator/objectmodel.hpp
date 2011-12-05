#ifndef objectmodel_hpp
#define objectmodel_hpp

#include "xmlobjects.hpp"

namespace litesql {

  /** the litesql object model */ 
  class ObjectModel {
  public:
    typedef counted_ptr<ObjectModel> Ptr;


    ObjectModel();
    virtual ~ObjectModel();
    /** load model from xml-file (uses a  specialized XmlParser) */
    bool loadFromFile(const std::string& filename);

    bool remove(xml::Field::Ptr& field);
    bool remove(xml::Method::Ptr& method);
    
    bool remove(xml::ObjectPtr& object);

    bool remove(xml::Relation::Ptr& relation);
    bool remove(xml::Relate::Ptr& relate);

    xml::Relation::sequence relations;
    xml::ObjectSequence objects;

    xml::DatabasePtr db;   
  };

  AT_field_type field_type(const char* value);

}
#endif //#ifndef objectmodel_hpp


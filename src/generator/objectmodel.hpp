#ifndef objectmodel_hpp
#define objectmodel_hpp

#include "xmlobjects.hpp"

namespace litesql {

  /** the litesql object model */ 
  class ObjectModel {
  public:
    typedef counted_ptr<ObjectModel> counted_ptr;


    ObjectModel();
    virtual ~ObjectModel();
    /** load model from xml-file (uses a  specialized XmlParser) */
    bool loadFromFile(const std::string& filename);

    bool remove(xml::Field::counted_ptr& field);
    bool remove(xml::Method::counted_ptr& method);
    
    bool remove(xml::ObjectPtr& object);

    bool remove(xml::Relation::counted_ptr& relation);
    bool remove(xml::Relate::counted_ptr& relate);

    xml::Relation::sequence relations;
    xml::ObjectSequence objects;

    xml::DatabasePtr db;   
  };

  const char* toString(AT_field_type t);
  AT_field_type field_type(const char* value);

}
#endif //#ifndef objectmodel_hpp


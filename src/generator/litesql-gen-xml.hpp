#ifndef xmlgenerator_hpp
#define xmlgenerator_hpp

#include <ostream>
#include "generator.hpp"

namespace litesql {
  class XmlGenerator : public CodeGenerator {
  public:
    XmlGenerator(): CodeGenerator("xml") { };
    virtual void setOutputFilename(const std::string& filename);
    bool generateCode(const ObjectModel* model);
    
  protected:
    bool generate(std::ostream& os,xml::Object *const object    , size_t indent=2);
    bool generate(std::ostream& os,xml::Relation* const relation,size_t indent=4);

  private:
    bool generateDatabase(std::ostream& os,const ObjectModel* model);
    std::string m_outputFilename;
  };
}

#endif //#ifndef xmlgenerator_hpp


#ifndef wxformbuilder_generator_hpp
#define wxformbuilder_generator_hpp

#include <ostream>
#include "generator.hpp"

namespace litesql {
  class wxFormBuilderGenerator : public CodeGenerator {
  public:
      static const char* NAME;

    wxFormBuilderGenerator();
    virtual void setOutputFilename(const std::string& filename);
    bool generateCode(const ObjectModel* model);
    
  protected:
    bool generate(const xml::ObjectPtr& object     , std::ostream& os , size_t indent=2);
    bool generate(const xml::Relation::Ptr& relation , std::ostream& os , size_t indent=4);
    
  private:
    bool generateDatabase(std::ostream& os,const ObjectModel* model);
    std::string m_outputFilename;
  };
}

#endif //#ifndef wxformbuilder_generator_hpp


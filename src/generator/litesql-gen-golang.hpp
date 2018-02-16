#ifndef litesql_gen_golang_hpp
#define litesql_gen_golang_hpp

#include "generator.hpp"

namespace litesql {

  class GolangGenerator : public CompositeGenerator {
  public:
     static const char* NAME;
    GolangGenerator();
  };

  class GolangModuleGenerator : public CodeGenerator {
  public:
    static const char* NAME;
    GolangModuleGenerator();
    bool generateCode(const ObjectModel* model) override;
  protected:
    virtual bool generate(const xml::ObjectPtr& object,
                          std::ostream& os,
                          size_t indent=2) override;
  };
}

#endif // #ifndef litesql_gen_golang_hpp



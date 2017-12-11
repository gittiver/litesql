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
    GolangModuleGenerator() : CodeGenerator(NAME) { };
    bool generateCode(const ObjectModel* model);
  protected:
    bool generate(const xml::ObjectPtr& object);
  };
}

#endif // #ifndef litesql_gen_golang_hpp



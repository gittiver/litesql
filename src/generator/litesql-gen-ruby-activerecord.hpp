#ifndef litesql_gen_ruby_activerecord_hpp
#define litesql_gen_ruby_activerecord_hpp

#include "generator.hpp"

namespace litesql {

  class RubyActiveRecordGenerator : public CompositeGenerator {
  public:
     static const char* NAME;
    RubyActiveRecordGenerator();
  };

  class ActiveRecordClassGenerator : public CodeGenerator {
  public:
     static const char* NAME;
    ActiveRecordClassGenerator(): CodeGenerator(NAME) { };
    bool generateCode(const ObjectModel* model);
  protected:   
    bool generate(const xml::ObjectPtr& object);
  };

  class RubyMigrationsGenerator : public CodeGenerator {
  public:
     static const char* NAME;
    RubyMigrationsGenerator(): CodeGenerator(NAME) { };
    bool generateCode(const ObjectModel* model);
  };

}

#endif

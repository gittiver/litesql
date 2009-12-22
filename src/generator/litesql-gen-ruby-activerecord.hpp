#ifndef litesql_gen_ruby_activerecord_hpp
#define litesql_gen_ruby_activerecord_hpp

#include "generator.hpp"

namespace litesql {
  
  class RubyActiveRecordGenerator : public CodeGenerator {
  public:
    RubyActiveRecordGenerator(): CodeGenerator("ruby-activerecord") { };
    bool generateCode(const ObjectModel* model);
  protected:   
    bool generate(xml::Object* const object);
  };
  
}

#endif

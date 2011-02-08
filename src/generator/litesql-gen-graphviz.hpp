#ifndef _litesql_gen_graphviz_hpp
#define _litesql_gen_graphviz_hpp

#include "generator.hpp"

namespace litesql {
  
  class GraphvizGenerator : public CodeGenerator {
  public:
    static const char* NAME;

    GraphvizGenerator(): CodeGenerator(NAME) { };
    bool generateCode(const ObjectModel* model);
  protected:   
    bool generate(const xml::ObjectPtr& object     , std::ostream& os , size_t indent);
    bool generate(const xml::Relation::Ptr& relation , std::ostream& os , size_t indent);
  };
}

#endif

#ifndef _litesql_gen_graphviz_hpp
#define _litesql_gen_graphviz_hpp
#include "generator.hpp"

class GraphvizGenerator : public CodeGenerator {
  public:
    GraphvizGenerator(): CodeGenerator("graphviz") { };
    bool generateCode(const ObjectModel* model);
protected:   
    bool generate(std::ostream& os,xml::Object* const object    , size_t indent);
    bool generate(std::ostream& os,xml::Relation* const relation,size_t indent);
};

#endif

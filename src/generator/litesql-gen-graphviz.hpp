#ifndef _litesql_gen_graphviz_hpp
#define _litesql_gen_graphviz_hpp
#include "generator.hpp"

class GraphvizGenerator : public CodeGenerator {
  public:
    GraphvizGenerator(): CodeGenerator("graphviz") { };
    bool generateCode(const ObjectModel* model);
};

#endif

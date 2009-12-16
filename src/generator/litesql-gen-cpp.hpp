#ifndef litesql_gen_cpp_hpp
#define litesql_gen_cpp_hpp

#include "generator.hpp"

class CppGenerator : public CodeGenerator {
  public:
    CppGenerator(): CodeGenerator("c++") { };
    bool generateCode(const ObjectModel* model);
};

#endif

#include "generator.hpp"
#include "objectmodel.hpp"
#include <fstream>

using namespace std;
using namespace litesql;

bool CppGenerator::generateCode(const ObjectModel* model)
{
  return  generateDeclarations(model) && generateImplementation(model);
}

bool CppGenerator::generateDeclarations(const ObjectModel* model)
{
  return true;
}

bool CppGenerator::generateImplementation(const ObjectModel* model)
{
  return true;
}
#include "generator.hpp"
#include "objectmodel.hpp"
#include <fstream>

using namespace std;
using namespace litesql;

CodeGenerator::~CodeGenerator()
{}

void CodeGenerator::setOutputDirectory(const string& directory) 
{
  m_directory = directory;
}
const string& CodeGenerator::getOutputDirectory() const 
{
  return m_directory;
}

const char* CodeGenerator::getTarget() const
{return m_target;}

bool CodeGenerator::generate(ostream& os,const std::vector<xml::Object* >& objects,size_t indent)
{
  for (std::vector<xml::Object* >::const_iterator it = objects.begin();
    it != objects.end();
    it++)
  {
      generate(os,*it,indent);
  }
  return true;
}

bool CodeGenerator::generate(ostream& os,const std::vector<xml::Relation* >& relations,size_t indent)
{
  for (std::vector<xml::Relation* >::const_iterator it = relations.begin();
    it != relations.end();
    it++)
  {
      generate(os,*it,indent);
  }
  return true;
}


void CompositeGenerator::add(CodeGenerator* g)
{
  g->setOutputDirectory(getOutputDirectory());
  generators.push_back(g);

}

void CompositeGenerator::setOutputDirectory(const string& directory) 
{
  CodeGenerator::setOutputDirectory(directory);
  for(vector<CodeGenerator*>::iterator it = generators.begin(); it != generators.end();it++)
  {
    (*it)->setOutputDirectory(directory);
  }
}

const string& CompositeGenerator::getOutputDirectory() const
{
  return CodeGenerator::getOutputDirectory();
}

bool CompositeGenerator::generateCode(const ObjectModel* model)
{
  bool success=true;

  for(vector<CodeGenerator*>::iterator it = generators.begin(); it != generators.end();it++)
  {
    success &= (*it)->generateCode(model);
  }
  return success;
}

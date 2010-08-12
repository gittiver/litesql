#include "generator.hpp"
#include "objectmodel.hpp"
#include "litesql-gen-cpp.hpp"
#include "litesql-gen-ruby-activerecord.hpp"
#include "litesql-gen-graphviz.hpp"


using namespace std;
using namespace litesql;

CodeGenerator::FactoryMap::FactoryMap()
{
  registerFactory(new CodeGenerator::Factory<CppGenerator>(CppGenerator::NAME));
  
  registerFactory(new CodeGenerator::Factory<RubyActiveRecordGenerator>(RubyActiveRecordGenerator::NAME));
  registerFactory(new CodeGenerator::Factory<ActiveRecordClassGenerator>(ActiveRecordClassGenerator::NAME));
  registerFactory(new CodeGenerator::Factory<RubyMigrationsGenerator>(RubyMigrationsGenerator::NAME));

  registerFactory(new CodeGenerator::Factory<GraphvizGenerator>(GraphvizGenerator::NAME));

}

CodeGenerator::FactoryMap::~FactoryMap()
{
  for (iterator it  = begin();
                it != end();
                it++ )
  {
    delete it->second;
  }
}

CodeGenerator::FactoryMap& CodeGenerator::getFactoryMap()
{
  static FactoryMap instance;
  return instance;
}

bool CodeGenerator::registerFactory(AbstractFactory* pFactory)
{
  if (!pFactory)
  {
    return false;
  }
  else
  {
    getFactoryMap()[pFactory->getName()] = pFactory;  
    return true;
  }
}

CodeGenerator* CodeGenerator::create(const char* target)
{
  FactoryMap::iterator it = getFactoryMap().find(target);
  if (it != getFactoryMap().end() &&  it->second!=NULL)
  {
    return it->second->create();
  }
  else
  {
    return NULL;
  }
}


CodeGenerator::CodeGenerator()
{}
  
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

std::string CodeGenerator::getOutputFilename(const std::string& name) const
{
  string fname = getOutputDirectory();

  if (!fname.empty())
  {
#ifdef WIN32
    fname.append("\\");
#else
    fname.append("/");
#endif // #ifdef _WINDOWS_
  }
  fname.append(name); 
  return fname;
}
    
const char* CodeGenerator::getTarget() const
{return m_target;}

bool CodeGenerator::generate(const std::vector<xml::Object* >& objects)
{
  for (std::vector<xml::Object* >::const_iterator it = objects.begin();
    it != objects.end();
    it++)
  {
      generate(*it);
  }
  return true;
}

bool CodeGenerator::generate(const std::vector<xml::Relation* >& relations)
{
  for (std::vector<xml::Relation* >::const_iterator it = relations.begin();
    it != relations.end();
    it++)
  {
      generate(*it);
  }
  return true;
}

bool CodeGenerator::generate(const std::vector<xml::Object* >& objects,ostream& os,size_t indent)
{
  for (std::vector<xml::Object* >::const_iterator it = objects.begin();
    it != objects.end();
    it++)
  {
      generate(*it,os,indent);
  }
  return true;
}

bool CodeGenerator::generate(const std::vector<xml::Relation* >& relations,ostream& os,size_t indent)
{
  for (std::vector<xml::Relation* >::const_iterator it = relations.begin();
    it != relations.end();
    it++)
  {
      generate(*it,os,indent);
  }
  return true;
}


void CompositeGenerator::add(CodeGenerator* g)
{
  g->setParentGenerator(this);
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

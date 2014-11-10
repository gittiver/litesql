#include <stdio.h>

#include "generator.hpp"
#include "litesql-gen-cpp.hpp"
#include "litesql-gen-ruby-activerecord.hpp"
#include "litesql-gen-graphviz.hpp"
#include "litesql-gen-wxformbuilder.hpp"
#include "litesql-gen-protobuf.hpp"

using namespace std;
using namespace litesql;
using namespace xml;

CodeGenerator::FactoryMap::FactoryMap()
{
  registerFactory(new Factory<CppGenerator>(CppGenerator::NAME));
  registerFactory(new Factory<RubyActiveRecordGenerator>(RubyActiveRecordGenerator::NAME));
  registerFactory(new Factory<ActiveRecordClassGenerator>(ActiveRecordClassGenerator::NAME));
  registerFactory(new Factory<RubyMigrationsGenerator>(RubyMigrationsGenerator::NAME));
  registerFactory(new Factory<GraphvizGenerator>(GraphvizGenerator::NAME));
  registerFactory(new Factory<wxFormBuilderGenerator>(wxFormBuilderGenerator::NAME));
  registerFactory(new Factory<ProtoBufClassGenerator>(ProtoBufClassGenerator::NAME));
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

bool CodeGenerator::FactoryMap::registerFactory(AbstractFactory* pFactory)
{
  if (!pFactory)
  {
    return false;
  }
  else
  {
    pair<const char*,AbstractFactory*> value(pFactory->getName(),pFactory);
    insert(value);
    return true;
  }
}

bool CodeGenerator::registerFactory(AbstractFactory* pFactory)
{ return getFactoryMap().registerFactory(pFactory); }

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

bool CodeGenerator::generate(const xml::ObjectSequence& objects)
{
  for (xml::ObjectSequence::const_iterator it = objects.begin();
    it != objects.end();
    it++)
  {
      generate(*it);
  }
  return true;
}

bool CodeGenerator::generate(const Relation::sequence& relations)
{
  for (Relation::sequence::const_iterator it = relations.begin();
    it != relations.end();
    it++)
  {
      generate(*it);
  }
  return true;
}

bool CodeGenerator::generate(const ObjectSequence& objects,ostream& os,size_t indent)
{
  for (ObjectSequence::const_iterator it = objects.begin();
    it != objects.end();
    it++)
  {
      generate(*it,os,indent);
  }
  return true;
}

bool CodeGenerator::generate(const Relation::sequence& relations,ostream& os,size_t indent)
{
  for (Relation::sequence::const_iterator it = relations.begin();
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

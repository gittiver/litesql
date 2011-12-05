#ifndef generator_hpp
#define generator_hpp

#include <vector>
#include <map>
#include <ostream>
#include <memory>

#include "objectmodel.hpp"


namespace litesql {

  class CodeGenerator {

  public:
    enum generation_mode_t { REFRESH=0,OVERWRITE };

    CodeGenerator();
    virtual ~CodeGenerator();

    virtual void setOutputDirectory(const std::string& directory);
    virtual const std::string& getOutputDirectory() const;

    void setGenerationMode(generation_mode_t mode) { m_generationMode = mode; };
    generation_mode_t getGenerationMode() const { return m_generationMode; };

    std::string getOutputFilename(const std::string& name) const;

    virtual const char* getTarget() const;
    virtual bool generateCode(const ObjectModel* model)=0;

    CodeGenerator* const getParentGenerator() const { return m_pParentGenerator; };
    void setParentGenerator(CodeGenerator* parent)  { m_pParentGenerator=parent; };

    bool generate(const xml::ObjectSequence& objects);
    bool generate(const xml::Relation::sequence& relations);
    virtual bool generate(const xml::ObjectPtr& object) {return true;};
    virtual bool generate(const xml::Relation::Ptr& relation){return true;};

    bool generate(const xml::ObjectSequence& objects,
		  std::ostream& os,
		  size_t indent=2);
    bool generate(const xml::Relation::sequence& relations,
		  std::ostream& os,
		  size_t indent=2);
    
    virtual bool generate(const xml::ObjectPtr& object, 
			  std::ostream& os, 
			  size_t indent=2) 
    {return true;};
    
    virtual bool generate(const xml::Relation::Ptr& relation, 
			  std::ostream& os, 
			  size_t indent=4)
    {return true;};

    static CodeGenerator* create(const char* target);

  protected:
    CodeGenerator(const char* target)
      : m_target(target),m_generationMode(REFRESH) {};

  protected:
    class AbstractFactory {
    public: 
      AbstractFactory(const char* pszName) : name(pszName) {};
      virtual ~AbstractFactory() {};
      const char* getName() { return name;};
      virtual CodeGenerator* create() =0;

    private:
      const char* name;
    };

  public:

    class FactoryMap : public std::map< std::string,AbstractFactory* > 
    {
    public:
      FactoryMap();
      ~FactoryMap();
      bool registerFactory(AbstractFactory* f);
    };

    static FactoryMap& getFactoryMap();
    static bool registerFactory(AbstractFactory* f);

    template <class T > class Factory : public AbstractFactory {
    public:
      Factory(const char* pszName)
        : AbstractFactory(pszName)  
      {};

      CodeGenerator* create(void)  {return new T(); };
    };

  private:
    const char* m_target;
    generation_mode_t m_generationMode;
    std::string m_directory;
    CodeGenerator* m_pParentGenerator;
  };

  class CompositeGenerator : public CodeGenerator {
  public:
    CompositeGenerator(): CodeGenerator("composite") { };
    CompositeGenerator(const char* target) : CodeGenerator(target) {};

    void setOutputDirectory(const std::string& directory);
    const std::string& getOutputDirectory() const;
    void add(CodeGenerator* g);
    bool generateCode(const ObjectModel* model);
  private:
    std::vector<CodeGenerator*> generators;
  };
}

#endif //#ifndef generator_hpp


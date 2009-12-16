#ifndef generator_hpp
#define generator_hpp

#include <vector>
#include <ostream>

#include "objectmodel.hpp"


namespace litesql {
  class CodeGenerator {

  public:
    virtual ~CodeGenerator();

    virtual void setOutputDirectory(const std::string& directory);
    virtual const std::string& getOutputDirectory() const;

    virtual const char* getTarget() const;
    virtual bool generateCode(const ObjectModel* model) = 0;

    //virtual void generate(std::ostream& os,const ObjectModel* model,size_t indent=0);

    //virtual void generate(std::ostream& os,xml::Object object    , size_t indent=2){};
    //virtual void generate(std::ostream& os,xml::Field* field     , size_t indent=4){};
    //virtual void generate(std::ostream& os,xml::Method* pMethod  , size_t indent=4){};

    //virtual void generate(std::ostream& os,xml::Relation* relation,size_t indent=4){};
    
    static CodeGenerator* create(const char* target);

  protected:
    CodeGenerator(const char* target)
      : m_target(target) {};

  private:
    const char* m_target;
    std::string m_directory;
  };

  class XmlGenerator : public CodeGenerator {
  public:
    XmlGenerator(): CodeGenerator("xml") { };
    virtual void setOutputFilename(const std::string& filename);
    bool generateCode(const ObjectModel* model);
  private:
    std::string m_outputFilename;
  };

  class CppGenerator : public CodeGenerator {
    CppGenerator(): CodeGenerator("c++") { };
  public:
    bool generateCode(const ObjectModel* model);
    bool generateDeclarations(const ObjectModel* model);
    bool generateImplementation(const ObjectModel* model);

  };

  class CompositeGenerator : public CodeGenerator {
    CompositeGenerator(): CodeGenerator("composite") { };
    public:
      void setOutputDirectory(const std::string& directory);
      const std::string& getOutputDirectory() const;
      void add(CodeGenerator* g);
      bool generateCode(const ObjectModel* model);
    private:
      std::vector<CodeGenerator*> generators;
  };

}

#endif //#ifndef generator_hpp


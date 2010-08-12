#include <wx/msgdlg.h>

#include "litesql.hpp"
#include "generator.hpp"
#include "litesql-gen-cpp.hpp"
#include "litesql-gen-graphviz.hpp"
#include "litesql-gen-ruby-activerecord.hpp"

#include "LitesqlGeneratePanel.h"

using namespace std;
using namespace litesql;

LitesqlGeneratePanel::LitesqlGeneratePanel( wxWindow* parent,litesql::ObjectModel* pModel )
:
GeneratePanel( parent ),
m_pModel(pModel)
{
  for (CodeGenerator::FactoryMap::iterator it  = CodeGenerator::getFactoryMap().begin();
                                           it != CodeGenerator::getFactoryMap().end();
                                           it++ )
  {
    m_checkListGenerators->Append(wxString::FromUTF8(it->first.c_str()));
  }
}


void LitesqlGeneratePanel::OnRunClick( wxCommandEvent& event )
{
  m_gaugeRunProgress->SetLabel(_("Running"));

struct options_t {
  string output_dir;
  string output_sources;
  string output_includes;
  bool refresh;
  vector<string> targets;
};

  options_t options = {"","","",true};
  options.targets.push_back("c++");
  options.output_includes = options.output_sources 
                          = options.output_dir 
                          = m_dirPickerOutputDir->GetPath().ToUTF8();

  CompositeGenerator generator;
    
  generator.setOutputDirectory(options.output_dir);
  
  for (int index=0; index < m_checkListGenerators->GetCount();index++)
  {
    if (m_checkListGenerators->IsChecked(index))
    {
      string key(m_checkListGenerators->GetString(index).ToUTF8()); 
      CodeGenerator::create(key.c_str());
      CodeGenerator::FactoryMap::iterator it = CodeGenerator::getFactoryMap().find(key);
      if (it != CodeGenerator::getFactoryMap().end() &&  it->second!=NULL)
      {
        generator.add(it->second->create());
      }

    }
  }

  wxString s (generator.generateCode(  m_pModel) ? _("Success") : _("Fail") );
  wxMessageBox(s);
}


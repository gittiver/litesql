// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#if !wxUSE_DOC_VIEW_ARCHITECTURE
#error You must set wxUSE_DOC_VIEW_ARCHITECTURE to 1 in setup.h!
#endif

#include "VisualLitesqlApp.h"
#include "GenerateView.h"
#include "LitesqlDocument.h"

#include "ui.h"
#include "generator.hpp"
#include "litesql-gen-cpp.hpp"
#include "litesql-gen-graphviz.hpp"
#include "litesql-gen-ruby-activerecord.hpp"


IMPLEMENT_DYNAMIC_CLASS(GenerateView, wxView)

BEGIN_EVENT_TABLE(GenerateView, wxView)
END_EVENT_TABLE()

/** Implementing GeneratePanel */
class LitesqlGeneratePanel : public ui::GeneratePanel
{
public:
	/** Constructor */
  LitesqlGeneratePanel( wxWindow* parent,litesql::ObjectModel::Ptr& pModel);

protected:
  virtual void OnRunClick( wxCommandEvent& event );
private:
  litesql::ObjectModel::Ptr m_pModel;
};

using namespace std;
using namespace litesql;

LitesqlGeneratePanel::LitesqlGeneratePanel( wxWindow* parent,litesql::ObjectModel::Ptr& pModel )
:
GeneratePanel( parent ),
m_pModel(pModel)
{
  CodeGenerator::FactoryMap& map = CodeGenerator::getFactoryMap();
  for (CodeGenerator::FactoryMap::iterator it  =map.begin();
                                           it != map.end();
                                           it++ )
  {
    m_checkListGenerators->Append(wxString::FromUTF8(it->first.c_str()));
  }
}


void LitesqlGeneratePanel::OnRunClick( wxCommandEvent& WXUNUSED(event) )
{
  m_gaugeRunProgress->SetLabel(_("Running"));

struct options_t {
  string output_dir;
  string output_includes;
  bool refresh;
  vector<string> targets;
};

  options_t options = {"","",true};
  options.targets.push_back("c++");
  options.output_includes = options.output_dir 
                          = m_dirPickerOutputDir->GetPath().ToUTF8();

  CompositeGenerator generator;
    
  generator.setOutputDirectory(options.output_dir);
  
  for (size_t index=0; index < m_checkListGenerators->GetCount();index++)
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

  wxString s (generator.generateCode(  m_pModel.get()) ? _("Success") : _("Fail") );
  wxMessageBox(s);
}



GenerateView::GenerateView()
{ frame = (wxMDIChildFrame *) NULL; }

GenerateView::~GenerateView()
{}

// What to do when a view is created. Creates actual
// windows for displaying the view.
bool GenerateView::OnCreate(wxDocument *doc, long WXUNUSED(flags) )
{
    frame = wxGetApp().CreateChildFrame(doc,this);
    frame->SetTitle(_T("GenerateView"));

#ifdef __X__
    // X seems to require a forced resize
    int x, y;
    frame->GetSize(&x, &y);
    frame->SetSize(wxDefaultCoord, wxDefaultCoord, x, y);
#endif
    m_pGeneratePanel = new LitesqlGeneratePanel(frame,((LitesqlDocument*)doc)->GetModel());

    frame->Layout();
    frame->Show(true);
    Activate(true);

    return true;
}

void GenerateView::OnUpdate(wxView * WXUNUSED(sender), wxObject * WXUNUSED(hint) ) 
{
}
    
// Sneakily gets used for default print/preview
// as well as drawing on the screen.
void GenerateView::OnDraw(wxDC *dc)
{
  dc->SetFont(*wxNORMAL_FONT);
  dc->SetPen(*wxBLACK_PEN);
}

// Clean up windows used for displaying the view.
bool GenerateView::OnClose(bool deleteWindow)
{
  if (!GetDocument()->Close())
    return false;
    
  wxString s(wxTheApp->GetAppName());
  if (frame)
    frame->SetTitle(s);

  SetFrame((wxFrame*)NULL);

  Activate(false);

  if (deleteWindow)
  {
    delete frame;
    return true;
  }
  return true;
}

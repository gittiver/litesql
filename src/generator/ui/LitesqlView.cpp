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

#include <wx/treebook.h>

#include "VisualLitesqlApp.h"
#include "LitesqlView.h"
#include "GenerateView.h"

#include "LitesqlDocument.h"

#include "LitesqlObjectPanel.h"
#include "LitesqlFieldPanel.h"
#include "LitesqlRelationPanel.h"
#include "LitesqlDatabasePanel.h"

#include "objectmodel.hpp"


IMPLEMENT_DYNAMIC_CLASS(LitesqlView, wxView)

BEGIN_EVENT_TABLE(LitesqlView, wxView)
EVT_SIZE(OnSize) 
EVT_MENU(VisualLitesqlApp::ID_GENERATE, LitesqlView::OnGenerate)
END_EVENT_TABLE()

using namespace xml;

LitesqlView::LitesqlView() 
{ frame = (wxMDIChildFrame *) NULL;
}

LitesqlView::~LitesqlView()
{}

void FillTree (litesql::ObjectModel* pModel,wxTreebook* pTree) 
{
  int pagePos = 0;

  wxString dbName(pModel->db.name.c_str(),wxConvUTF8);
  pTree->DeleteAllPages();

  pTree->AddPage(new ui::LitesqlDatabasePanel(pTree,&pModel->db),_("database"));
  pagePos++;
  for( vector<Object*>::iterator it = pModel->objects.begin(); it != pModel->objects.end();it++)
  {
    wxString name((*it)->name.c_str(),wxConvUTF8);
    pTree->AddSubPage(new LitesqlObjectPanel(pTree,*it),name +_("(Object)"));

    int subPagePos=pagePos++;

    for (vector<Field*>::iterator field = (*it)->fields.begin();
      field != (*it)->fields.end();
      field++)
    {
      wxString fname((*field)->name.c_str(),wxConvUTF8);
      pTree->InsertSubPage(subPagePos,new LitesqlFieldPanel(pTree, *field),fname +_("(Field)"));
      pagePos++;
    }
  }

  for (vector<Relation*>::iterator relation = pModel->relations.begin(); relation!= pModel->relations.end();relation++)
  {
    wxString name((*relation)->name.c_str(),wxConvUTF8);
    pTree->AddSubPage(new LitesqlRelationPanel(pTree,*relation),name +_("(relation)"));
    pagePos++;
  }

  pTree->Layout();
}

// What to do when a view is created. Creates actual
// windows for displaying the view.
bool LitesqlView::OnCreate(wxDocument *doc, long WXUNUSED(flags) )
{
  frame = wxGetApp().CreateChildFrame(doc, this, true);
  frame->SetTitle(_T("LitesqlView"));

#ifdef __X__
  // X seems to require a forced resize
  int x, y;
  frame->GetSize(&x, &y);
  frame->SetSize(wxDefaultCoord, wxDefaultCoord, x, y);
#endif
  
  m_treebook = new wxTreebook(frame,-1);
  wxSize s = m_treebook->GetTreeCtrl()->GetSize();
  s.SetWidth(s.GetWidth()*2);
  m_treebook->GetTreeCtrl()->SetSize(s);
  frame->Show(true);
  Activate(true);

  return true;
}

void LitesqlView::OnSize(wxSizeEvent& event)
{
  if (frame) {
    m_treebook->SetSize(frame->GetClientSize());
    
    m_treebook->Layout();
    frame->Layout();
  }

}

void LitesqlView::OnUpdate(wxView *sender, wxObject *hint) 
{
  FillTree(((LitesqlDocument*) GetDocument())->GetModel() ,m_treebook);
}

// Sneakily gets used for default print/preview
// as well as drawing on the screen.
void LitesqlView::OnDraw(wxDC *dc)
{
  dc->SetFont(*wxNORMAL_FONT);
  dc->SetPen(*wxBLACK_PEN);
}

// Clean up windows used for displaying the view.
bool LitesqlView::OnClose(bool deleteWindow)
{
  if (!GetDocument()->Close())
    return false;
  wxApp::GetInstance();

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

void LitesqlView::OnCut(wxCommandEvent& WXUNUSED(event) )
{
  int y = 2;
  int s=y;
}

void LitesqlView::OnGenerate(wxCommandEvent& WXUNUSED(event) )
{

  wxView* v ; //= new GenerateView();
  v = (wxView*)GenerateView::wxCreateObject();
  GetDocument()->AddView(v);
  //wxGetApp().CreateChildFrame(GetDocument(), v, true);
}

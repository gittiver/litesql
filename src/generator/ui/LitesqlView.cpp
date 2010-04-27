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

#include <wx/treelistctrl.h>

#include "VisualLitesqlApp.h"
#include "LitesqlView.h"
#include "LitesqlDocument.h"

#include "objectmodel.hpp"


IMPLEMENT_DYNAMIC_CLASS(LitesqlView, wxView)

BEGIN_EVENT_TABLE(LitesqlView, wxView)
// 
END_EVENT_TABLE()

using namespace xml;

LitesqlView::LitesqlView() 
{ /* canvas = (MyCanvas *) NULL;*/ frame = (wxMDIChildFrame *) NULL; }

LitesqlView::~LitesqlView()
{
}

void FillTree (litesql::ObjectModel* pModel,wxTreeListCtrl* pTreeList) 
{
  wxString dbName(pModel->db.name.c_str(),wxConvUTF8);
  
  static wxTreeItemId root = pTreeList->AddRoot (dbName/*_T("Root")*/);
//  pTreeList->SetItemText (root, 1, wxString::Format (_T("Root, text #%d"), 0));
//  pTreeList->SetItemText (root, 2, wxString::Format (_T("Root, text #%d"), 0));
  
  int n = 0;
  wxTreeItemId parent;
  wxTreeItemId item;
  for( vector<Object*>::iterator it = pModel->objects.begin(); it != pModel->objects.end();it++)
  {
    wxString name((*it)->name.c_str(),wxConvUTF8);
    wxString inherits((*it)->inherits.c_str(),wxConvUTF8);
    
    item = pTreeList->AppendItem (root, name +_("(Object)"), ++n);
    pTreeList->SetItemText (item, 1, inherits);
//    pTreeList->SetItemText (item, 2, (*it)->parentObjectwxString::Format (_T("Item #%d, text #%d"), n, ++m));
    parent = item;
  }

  
  for( vector<Relation*>::iterator it = pModel->relations.begin(); it != pModel->relations.end();it++)
  {
    wxString name((*it)->name.c_str(),wxConvUTF8);
    item = pTreeList->AppendItem (root, name +_("(Relation)"), ++n);
    parent = item;
  }
  
  /*
   int n = 0;
    int m = 0;
    // initialize tree
    pTreeList->SetItemText (root, 1, wxString::Format (_T("Root, text #%d"), 0));
    pTreeList->SetItemText (root, 2, wxString::Format (_T("Root, text #%d"), 0));
    wxTreeItemId parent;
    wxTreeItemId item;
    item = pTreeList->AppendItem (root, wxString::Format (_T("Item #%d"), ++n));
    pTreeList->SetItemText (item, 1, wxString::Format (_T("Item #%d, text #%d"), n, ++m));
    pTreeList->SetItemText (item, 2, wxString::Format (_T("Item #%d, text #%d"), n, ++m));
    parent = item;
    item = pTreeList->AppendItem (parent, wxString::Format (_T("Item #%d"), ++n), 4);
    pTreeList->SetItemText (item, 1, wxString::Format (_T("Item #%d, text #%d"), n, ++m));
    pTreeList->SetItemText (item, 2, wxString::Format (_T("Item #%d, text #%d"), n, ++m));
    item = pTreeList->AppendItem (parent, wxString::Format (_T("Item #%d"), ++n), 4);
    pTreeList->SetItemText (item, 1, wxString::Format (_T("Item #%d, text #%d"), n, ++m));
    pTreeList->SetItemText (item, 2, wxString::Format (_T("Item #%d, text #%d"), n, ++m));
    item = pTreeList->AppendItem (parent, wxString::Format (_T("Item #%d"), ++n), 4);
    pTreeList->SetItemText (item, 1, wxString::Format (_T("Item #%d, text #%d"), n, ++m));
    pTreeList->SetItemText (item, 2, wxString::Format (_T("Item #%d, text #%d"), n, ++m));
    item = pTreeList->AppendItem (parent, wxString::Format (_T("Item #%d"), ++n), 4);
    pTreeList->SetItemText (item, 1, wxString::Format (_T("Item #%d, text #%d"), n, ++m));
    pTreeList->SetItemText (item, 2, wxString::Format (_T("Item #%d, text #%d"), n, ++m));
    item = pTreeList->AppendItem (root, wxString::Format (_T("Item #%d"), ++n));
    pTreeList->SetItemText (item, 1, wxString::Format (_T("Item #%d, text #%d"), n, ++m));
    pTreeList->SetItemText (item, 2, wxString::Format (_T("Item #%d, text #%d"), n, ++m));
    parent = item;
    item = pTreeList->AppendItem (parent, wxString::Format (_T("Item #%d"), ++n), 4);
    pTreeList->SetItemText (item, 1, wxString::Format (_T("Item #%d, text #%d"), n, ++m));
    pTreeList->SetItemText (item, 2, wxString::Format (_T("Item #%d, text #%d"), n, ++m));
    item = pTreeList->AppendItem (parent, wxString::Format (_T("Item #%d"), ++n), 4);
    pTreeList->SetItemText (item, 1, wxString::Format (_T("Item #%d, text #%d"), n, ++m));
    pTreeList->SetItemText (item, 2, wxString::Format (_T("Item #%d, text #%d"), n, ++m));
    item = pTreeList->AppendItem (parent, wxString::Format (_T("Item #%d"), ++n), 4);
    pTreeList->SetItemText (item, 1, wxString::Format (_T("Item #%d, text #%d"), n, ++m));
    pTreeList->SetItemText (item, 2, wxString::Format (_T("Item #%d, text #%d"), n, ++m));
    item = pTreeList->AppendItem (parent, wxString::Format (_T("Item #%d"), ++n), 4);
    pTreeList->SetItemText (item, 1, wxString::Format (_T("Item #%d, text #%d"), n, ++m));
    pTreeList->SetItemText (item, 2, wxString::Format (_T("Item #%d, text #%d"), n, ++m));
    item = pTreeList->AppendItem (root, wxString::Format (_T("Item #%d"), ++n));
    pTreeList->SetItemText (item, 1, wxString::Format (_T("Item #%d, text #%d"), n, ++m));
    pTreeList->SetItemText (item, 2, wxString::Format (_T("Item #%d, text #%d"), n, ++m));
    parent = item;
    item = pTreeList->AppendItem (parent, wxString::Format (_T("Item #%d"), ++n), 4);
    pTreeList->SetItemText (item, 1, wxString::Format (_T("Item #%d, text #%d"), n, ++m));
    pTreeList->SetItemText (item, 2, wxString::Format (_T("Item #%d, text #%d"), n, ++m));
    item = pTreeList->AppendItem (parent, wxString::Format (_T("Item #%d"), ++n), 4);
    pTreeList->SetItemText (item, 1, wxString::Format (_T("Item #%d, text #%d"), n, ++m));
    pTreeList->SetItemText (item, 2, wxString::Format (_T("Item #%d, text #%d"), n, ++m));
    item = pTreeList->AppendItem (parent, wxString::Format (_T("Item #%d"), ++n), 4);
    pTreeList->SetItemText (item, 1, wxString::Format (_T("Item #%d, text #%d"), n, ++m));
    pTreeList->SetItemText (item, 2, wxString::Format (_T("Item #%d, text #%d"), n, ++m));
    item = pTreeList->AppendItem (parent, wxString::Format (_T("Item #%d"), ++n), 4);
    pTreeList->SetItemText (item, 1, wxString::Format (_T("Item #%d, text #%d"), n, ++m));
    pTreeList->SetItemText (item, 2, wxString::Format (_T("Item #%d, text #%d"), n, ++m));
*/
  pTreeList->ExpandAll (root);
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
    m_treelist = new wxTreeListCtrl(frame);

    m_treelist->SetBackgroundColour(wxColour(240,240,192));
    int k = 250;
    m_treelist->AddColumn (_T("Main"), k - 32, wxALIGN_LEFT);
    m_treelist->SetColumnEditable (0, true);
    m_treelist->AddColumn (_T("Second"), k, wxALIGN_LEFT);
    m_treelist->SetColumnEditable (1, true);
    m_treelist->SetColumnAlignment (1, wxALIGN_LEFT);
    m_treelist->AddColumn (_T("Third"), k, wxALIGN_CENTER);
    m_treelist->SetColumnEditable (2, true);
    m_treelist->SetColumnAlignment (2, wxALIGN_CENTER);
    
//    FillTree(((LitesqlDocument*) GetDocument())->GetModel() ,m_treelist);
    frame->Show(true);
    Activate(true);

    return true;
}

void LitesqlView::OnUpdate(wxView *sender, wxObject *hint) 
{
  FillTree(((LitesqlDocument*) GetDocument())->GetModel() ,m_treelist);
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
}

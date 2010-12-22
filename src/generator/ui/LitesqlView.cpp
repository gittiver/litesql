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

#include "ui.h"
#include "VisualLitesqlApp.h"
#include "LitesqlView.h"
#include "GenerateView.h"

#include "LitesqlDocument.h"

#include "LitesqlObjectPanel.h"
#include "LitesqlFieldPanel.h"
#include "LitesqlMethodPanel.h"
#include "LitesqlRelationPanel.h"
#include "LitesqlDatabasePanel.h"

#include "objectmodel.hpp"

IMPLEMENT_DYNAMIC_CLASS(LitesqlView, wxView)

BEGIN_EVENT_TABLE(LitesqlView, wxView)
  EVT_MENU(VisualLitesqlApp::ID_GENERATE, LitesqlView::OnGenerate)
  
  EVT_MENU(VisualLitesqlApp::ID_ADD_OBJECT,     LitesqlView::OnAddObject)
  EVT_MENU(VisualLitesqlApp::ID_REMOVE_OBJECT,  LitesqlView::OnRemoveObject)

  EVT_MENU(VisualLitesqlApp::ID_ADD_FIELD,     LitesqlView::OnAddField)
  EVT_MENU(VisualLitesqlApp::ID_REMOVE_FIELD,  LitesqlView::OnRemoveField)

  EVT_MENU(VisualLitesqlApp::ID_ADD_METHOD,     LitesqlView::OnAddMethod)
  EVT_MENU(VisualLitesqlApp::ID_REMOVE_METHOD,  LitesqlView::OnRemoveMethod)

  EVT_MENU(VisualLitesqlApp::ID_ADD_RELATED,    LitesqlView::OnAddRelated)
  EVT_MENU(VisualLitesqlApp::ID_REMOVE_RELATED, LitesqlView::OnRemoveRelated)

  EVT_MENU(VisualLitesqlApp::ID_ADD_RELATION,   LitesqlView::OnAddRelation)
  EVT_MENU(VisualLitesqlApp::ID_REMOVE_RELATION,LitesqlView::OnRemoveRelation)

  EVT_CONTEXT_MENU(LitesqlView::OnContextMenu)
  
END_EVENT_TABLE()

using namespace xml;
using namespace litesql;
using namespace ui;

class LitesqlModelTreePanel : public ui::ModelTreePanel
{
	public:
	/** Constructor */
  LitesqlModelTreePanel( wxWindow* parent)
  : ModelTreePanel( parent )
  {
  }
  
  wxTreeCtrl* GetTreeCtrl()
  {
	  return m_modelTreeCtrl;
  }
private:	
	void OnTreeDeleteItem( wxTreeEvent& event ){ event.Skip(); }
	void OnTreeItemActivated( wxTreeEvent& event ){ event.Skip(); }
	void OnTreeItemGetTooltip( wxTreeEvent& event ){ event.Skip(); }
	void OnTreeItemMenu( wxTreeEvent& event ){ event.Skip(); }
	void OnTreeSelChanged( wxTreeEvent& event ){ event.Skip(); }
	void OnTreeSelChanging( wxTreeEvent& event ){ event.Skip(); }

};

class wxModelItem : public wxTreeItemData {
public:
	wxModelItem() : wxTreeItemData() {};
	virtual ~wxModelItem() {}; 
	virtual wxString GetLabel() const = 0 ;
	//virtual wxWindow* GetEditPanel() const = 0;
	wxWindow* GetEditPanel() const			{	return NULL;	};
	virtual bool hasChildren() const		{	return false;	};
	virtual wxArrayPtrVoid* GetChildren()	{	return NULL;	};

	static void RefreshTree(wxTreeCtrl* pTree,wxTreeItemId& baseItem,wxModelItem* item);
};

void wxModelItem::RefreshTree(wxTreeCtrl* pTree,wxTreeItemId& baseItem,wxModelItem* item)
{
	wxTreeItemId itemId = pTree->InsertItem(baseItem,item->GetId(), item->GetLabel()); 
	if (item->hasChildren())
	{
		wxArrayPtrVoid* pChildren = item->GetChildren();
		for(wxArrayPtrVoid::iterator it =  pChildren->begin(); 
									 it != pChildren->end();
									 it++)
		{
			RefreshTree(pTree,itemId,(wxModelItem*)(*it));
		}
	}
}

class wxCompositeModelItem : public wxModelItem {
public:
	wxCompositeModelItem(): wxModelItem(),m_childrenInitalized(false) {};
	virtual ~wxCompositeModelItem() 
	{
		for(wxArrayPtrVoid::iterator it =  m_children.begin(); 
									 it != m_children.end();
									 it++)
		{
			delete (*it);
		}
	}; 
	
	wxArrayPtrVoid* GetChildren() 
	{
		if (!m_childrenInitalized)
		{
			InitChildren();
			m_childrenInitalized=true;
		}
		return &m_children;
	}
	
protected:
	virtual void InitChildren()=0;
	wxArrayPtrVoid m_children;

private:
		bool m_childrenInitalized;
};

class wxFieldItem : public wxModelItem 
{
public:
	wxFieldItem(Field* pField) : m_pField(pField) {}
 
	wxString GetLabel() const			{ wxString label(m_pField->name.c_str(),wxConvUTF8);
		return label;	};
private:
	Field* m_pField;

};

class wxMethodItem: public wxModelItem 
{
public:
	wxMethodItem(Method* pMethod) : m_pMethod(pMethod) {}
	wxString GetLabel() const			{ wxString label(m_pMethod->name.c_str(),wxConvUTF8);
		return label;	};
private:
	Method* m_pMethod;
};

class wxLitesqlObject : public wxCompositeModelItem {
	public:
		wxLitesqlObject(Object* pObject)
			:	m_pObject(pObject)	{};
		wxString GetLabel() const			{ wxString label(m_pObject->name.c_str(),wxConvUTF8);
		return label;	};
		wxWindow* GetEditPanel() const      { return NULL;				};
		bool hasChildren() const {	return true; };

		void InitChildren()
		{
			for (vector<Field*>::iterator field = m_pObject->fields.begin();
				field != m_pObject->fields.end();
				field++)
			{
				m_children.push_back(new wxFieldItem(*field));
			}

			for (vector<Method*>::iterator method = m_pObject->methods.begin();
				method != m_pObject->methods.end();
				method++)
			{
				m_children.push_back(new wxMethodItem(*method));
			}
		}
	
private:
		Object* m_pObject;	
};

class wxLitesqlModel : public wxCompositeModelItem {
	public:
		wxLitesqlModel(ObjectModel* pModel)
			:	m_pModel(pModel) { 	};

		wxString GetLabel() const			{ wxString label(m_pModel->db.name.c_str(),wxConvUTF8);
		return label; };
		wxWindow* GetEditPanel() const      { return NULL;				};
		bool hasChildren() const {	return true; };

		void InitChildren()
		{
			for( vector<Object*>::iterator it = m_pModel->objects.begin(); it != m_pModel->objects.end();it++)
			{
				m_children.push_back(new wxLitesqlObject(*it));
	
			}
		}

private:
		ObjectModel* m_pModel;
};

LitesqlView::LitesqlView() 
  : frame((wxMDIChildFrame *) NULL),m_ctxMenu(NULL)
{}

LitesqlView::~LitesqlView()
{
  if (m_ctxMenu)
  {
    delete m_ctxMenu;
  }
}

wxMenu* LitesqlView::GetContextMenu()
{
  if (!m_ctxMenu) 
  {
    m_ctxMenu = new wxMenu;
    m_ctxMenu->Append(VisualLitesqlApp::ID_ADD_OBJECT,_("Add Object ..."));
    m_ctxMenu->Append(VisualLitesqlApp::ID_REMOVE_OBJECT,_("Remove Object ..."));
    m_ctxMenu->AppendSeparator();
    m_ctxMenu->Append(VisualLitesqlApp::ID_ADD_FIELD,_("Add Field..."));
    m_ctxMenu->Append(VisualLitesqlApp::ID_REMOVE_FIELD,_("Remove Field ..."));
    m_ctxMenu->AppendSeparator();
    m_ctxMenu->Append(VisualLitesqlApp::ID_ADD_METHOD,_("Add Method ..."));
    m_ctxMenu->Append(VisualLitesqlApp::ID_REMOVE_METHOD,_("Remove Method..."));
    m_ctxMenu->AppendSeparator();
    m_ctxMenu->Append(VisualLitesqlApp::ID_ADD_RELATED,_("Add Related..."));
    m_ctxMenu->Append(VisualLitesqlApp::ID_REMOVE_RELATED,_("Remove Related ..."));
    m_ctxMenu->AppendSeparator();
    m_ctxMenu->Append(VisualLitesqlApp::ID_ADD_RELATION,_("Add Relation..."));
    m_ctxMenu->Append(VisualLitesqlApp::ID_REMOVE_RELATION,_("Remove Relation ..."));
    m_ctxMenu->AppendSeparator();
    m_ctxMenu->Append(VisualLitesqlApp::ID_GENERATE,_("Generate..."));
  }

  // \TODO change menu on selected item
  return m_ctxMenu;
}

// What to do when a view is created. Creates actual
// windows for displaying the view.
bool LitesqlView::OnCreate(wxDocument *doc, long WXUNUSED(flags) )
{
	frame = wxGetApp().CreateChildFrame(doc, this);
	frame->SetTitle(_T("LitesqlView"));

#ifdef __X__
	// X seems to require a forced resize
	int x, y;
	frame->GetSize(&x, &y);
	frame->SetSize(wxDefaultCoord, wxDefaultCoord, x, y);
#endif

	panel = new LitesqlModelTreePanel(frame);

	panel->GetTreeCtrl()->DeleteAllItems();
	wxTreeItemId rootId = panel->GetTreeCtrl()->AddRoot(_("root"));
	wxModelItem::RefreshTree(panel->GetTreeCtrl(),rootId,new wxLitesqlModel(((LitesqlDocument*) doc)->GetModel()));

	panel->Layout();
	frame->Show(true);
	Activate(true);

	return true;
}

void LitesqlView::OnUpdate(wxView *sender, wxObject *hint) 
{
	panel->GetTreeCtrl()->DeleteAllItems();
	wxTreeItemId rootId = panel->GetTreeCtrl()->AddRoot(_("root"));
	wxModelItem::RefreshTree(panel->GetTreeCtrl(),rootId,
							 new wxLitesqlModel( ((LitesqlDocument*) GetDocument())->GetModel())
							 );

	panel->Layout();
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

void LitesqlView::OnContextMenu(wxContextMenuEvent& event)
{
  if (frame)
  {
    frame->PopupMenu(GetContextMenu(),frame->ScreenToClient(event.GetPosition()));
  }
}


void LitesqlView::OnAddObject(wxCommandEvent& WXUNUSED(event) )
{
//  wxMessageBox(_("OnAddObject"));
  ObjectModel* pModel = ((LitesqlDocument*) GetDocument())->GetModel();
  Object* newObject = new Object("newObject","");
  pModel->objects.push_back(newObject);
  
  GetDocument()->Modify(true);
  GetDocument()->UpdateAllViews(this,NULL);
}

void LitesqlView::OnRemoveObject(wxCommandEvent& WXUNUSED(event) )
{
  wxMessageBox(_("OnRemoveObject"));
  GetDocument()->Modify(true);
  GetDocument()->UpdateAllViews(this,NULL);
}

void LitesqlView::OnAddField(wxCommandEvent& WXUNUSED(event) )
{
/*
wxWindow *pPage = m_treebook->GetCurrentPage();
  if (!pPage) 
  {
  
  }
  else if (pPage->IsKindOf(CLASSINFO(LitesqlObjectPanel)))
  {
    xml::Field* newField = new xml::Field("newField",AU_field_type,"",AU_field_indexed,AU_field_unique);
    ((LitesqlObjectPanel*)pPage)->GetObject()->fields.push_back(newField);
    m_treebook->InsertSubPage(  m_treebook->GetSelection(),
                                new LitesqlFieldPanel(m_treebook, newField),_("newField(Field)"),
                                true);
      GetDocument()->Modify(true);
      GetDocument()->UpdateAllViews(this,NULL);
  }
  else
  {
  
  }
*/
}

void LitesqlView::OnRemoveField(wxCommandEvent& WXUNUSED(event) )
{
/*
wxWindow *pPage = m_treebook->GetCurrentPage();
  if (!pPage) 
  {
  
  }
  else if (pPage->IsKindOf(CLASSINFO(LitesqlFieldPanel)))
  {
    ((LitesqlDocument*) GetDocument())->RemoveField(((LitesqlFieldPanel*)pPage)->GetField());
  }
  else
  {
  
  }
*/

}

void LitesqlView::OnAddMethod(wxCommandEvent& WXUNUSED(event) )
{
	
}

void LitesqlView::OnRemoveMethod(wxCommandEvent& WXUNUSED(event) )
{}

void LitesqlView::OnAddRelated(wxCommandEvent& WXUNUSED(event) )
{}

void LitesqlView::OnRemoveRelated(wxCommandEvent& WXUNUSED(event) )
{}

void LitesqlView::OnAddRelation(wxCommandEvent& WXUNUSED(event) )
{}

void LitesqlView::OnRemoveRelation(wxCommandEvent& WXUNUSED(event) )
{}

void LitesqlView::OnGenerate(wxCommandEvent& WXUNUSED(event) )
{
  wxGetApp().m_pGenerateViewTemplate->CreateView(GetDocument());
}

void LitesqlView::OnPageChanged(wxTreebookEvent& WXUNUSED(event))
{
/*	wxWindow *pPage = m_treebook->GetCurrentPage();
	if (pPage)
	{
		pPage->TransferDataToWindow();
	}
*/
}

void LitesqlView::OnPageChanging(wxTreebookEvent& event)
{
/*
  wxWindow *pPage = m_treebook->GetCurrentPage();
  if (pPage)
  {
    wxString s;
	pPage->TransferDataFromWindow();
	
	if (pPage->IsKindOf(CLASSINFO(LitesqlObjectPanel)))
	{
		s = ((LitesqlObjectPanel*)pPage)->GetObject()->name + _("(Object)"); 
	}
	else if (pPage->IsKindOf(CLASSINFO(LitesqlFieldPanel)))
	{
		s = ((LitesqlFieldPanel*)pPage)->GetPageText(); 
		m_treebook->SetPageText(event.GetOldSelection(),s);
	}
	else if (pPage->IsKindOf(CLASSINFO(LitesqlMethodPanel)))
	{
		s = ((LitesqlMethodPanel*)pPage)->GetPageText(); 
		m_treebook->SetPageText(event.GetOldSelection(),s);
	}
	else if (pPage->IsKindOf(CLASSINFO(LitesqlRelationPanel)))
	{
		s = ((LitesqlRelationPanel*)pPage)->GetPageText(); 
		m_treebook->SetPageText(event.GetOldSelection(),s);
	}
	else if (pPage->IsKindOf(CLASSINFO(LitesqlDatabasePanel)))
	{
		s = ((LitesqlDatabasePanel*)pPage)->GetPageText(); 
		m_treebook->SetPageText(event.GetOldSelection(),s);
	}
	
	m_treebook->SetPageText(event.GetOldSelection(),s);
	
  }
*/
}

#include "LitesqlModelTreePanel.h"

using namespace xml;
using namespace litesql;
using namespace ui;

#include <wx/arrimpl.cpp>

WX_DEFINE_OBJARRAY(wxModelItemArray);

;
wxCompositeModelItem::wxCompositeModelItem(): wxModelItem(),m_childrenInitalized(false) {};

wxCompositeModelItem::~wxCompositeModelItem() 
{
  for(wxModelItemArray::iterator it =  m_children.begin(); 
      it != m_children.end();
      it++)
  {
    delete (*it);
  }
}; 

wxModelItemArray* wxCompositeModelItem::GetChildren() 
{
  if (!m_childrenInitalized)
  {
    InitChildren();
    m_childrenInitalized=true;
  }
  return &m_children;
}

class wxFieldItem : public wxModelItem 
{
public:
	wxFieldItem(Field* pField) : m_pField(pField) {}
  ~wxFieldItem()  { }
  
	wxString GetLabel() const			{ wxString label(m_pField->name.c_str(),wxConvUTF8);
		return label;	};
	
  wxWindow* GetEditPanel(wxWindow *parent) 			
  {	
    LitesqlFieldPanel* pPanel = new LitesqlFieldPanel(parent,m_pField);
    return pPanel;
  };
  
private:
	Field* m_pField;

};

class wxMethodItem: public wxModelItem 
{
public:
	wxMethodItem(Method* pMethod) : m_pMethod(pMethod) {}
	~wxMethodItem()  {}
  
  wxString GetLabel() const			{ wxString label(m_pMethod->name.c_str(),wxConvUTF8);
		return label;	};
  
  wxWindow* GetEditPanel(wxWindow *parent) 			
  {	
    LitesqlMethodPanel* pPanel = new LitesqlMethodPanel(parent,m_pMethod);
    return pPanel;
  };
  
private:
	Method* m_pMethod;
};

class wxLitesqlObject : public wxCompositeModelItem {
	public:
    wxLitesqlObject(Object* pObject,ObjectModel* pModel)
			:	m_pObject(pObject),m_pModel(pModel)	{};
  ~wxLitesqlObject()  { }
  
  wxString GetLabel() const			{ wxString label(m_pObject->name.c_str(),wxConvUTF8);
		return label;	};

  wxWindow* GetEditPanel(wxWindow *parent) 			
  {	
    LitesqlObjectPanel* pPanel = new LitesqlObjectPanel(parent,m_pObject);
    pPanel->setBaseClasses(m_pModel->objects);
    return pPanel;
  };
  
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
  ObjectModel* m_pModel;
};

wxLitesqlModel::wxLitesqlModel(ObjectModel* pModel)
:	m_pModel(pModel) { 	}

wxWindow* wxLitesqlModel::GetEditPanel(wxWindow *parent) 			
{	
  LitesqlDatabasePanel* pPanel = new LitesqlDatabasePanel(parent,&m_pModel->db);
  return pPanel;
}


wxString wxLitesqlModel::GetLabel() const			{ wxString label(m_pModel->db.name.c_str(),wxConvUTF8);
  return label; };

void   wxLitesqlModel::InitChildren()
{
  for( vector<Object*>::iterator it = m_pModel->objects.begin(); it != m_pModel->objects.end();it++)
  {
    m_children.push_back(new wxLitesqlObject(*it,m_pModel));
    
  }
}

// ////////////////////////////////////////////////////////////////
//

/** Constructor */
LitesqlModelTreePanel::LitesqlModelTreePanel( wxWindow* parent)
: ModelTreePanel( parent )
{
  
}


LitesqlModelTreePanel::~LitesqlModelTreePanel()
{
  if (m_pModel)
    delete m_pModel;
}

void LitesqlModelTreePanel::setObjectModel(litesql::ObjectModel* pModel)
{
  m_pModel = new wxLitesqlModel(pModel);
  m_modelTreeCtrl->DeleteAllItems();
  wxTreeItemId rootId = m_modelTreeCtrl->AddRoot(_("root"));
  wxModelItem::RefreshTree(m_modelTreeCtrl,rootId,m_pModel);
}

wxTreeItemId LitesqlModelTreePanel::AddObject(Object* newObject)
{
  wxLitesqlObject* pObject = new wxLitesqlObject(newObject,m_pModel->GetModel());
  m_pModel->GetModel()->objects.push_back(newObject);
  return m_modelTreeCtrl->InsertItem(m_pModel->GetId(), -1, pObject->GetLabel(), -1, -1, pObject);
}

void LitesqlModelTreePanel::OnTreeSelChanged( wxTreeEvent& event )
{
  int pageIndex = GetDetailNotebook()->GetSelection();
  
  if (pageIndex!=-1)
  {
    GetDetailNotebook()->GetPage(pageIndex)->TransferDataFromWindow();
    GetDetailNotebook()->DeletePage(pageIndex);
    GetDetailNotebook()->Layout();
  }
    // update the label after getting the data
  wxModelItem* pOldItem = (wxModelItem*)GetTreeCtrl()->GetItemData(event.GetOldItem());
  if (pOldItem)
  {
    GetTreeCtrl()->SetItemText(event.GetOldItem(), pOldItem->GetLabel());
  }
                               
    // add new page 
  wxModelItem* pItem = (wxModelItem*)GetTreeCtrl()->GetItemData(event.GetItem());
  if (pItem) {
    wxWindow *pPage = pItem->GetEditPanel(GetDetailNotebook());
    if (pPage)
    {
      GetDetailNotebook()->AddPage(pPage,pItem->GetLabel());
      
      GetDetailNotebook()->Layout();
      pPage->Layout();
      pPage->TransferDataToWindow();
    }
  }
}

void LitesqlModelTreePanel::OnTreeSelChanging( wxTreeEvent& event )
{
    //  wxModelItem* pItem = (wxModelItem*)GetTreeCtrl()->GetItemData(event.GetItem());
}

void wxModelItem::RefreshTree(wxTreeCtrl* pTree,wxTreeItemId& baseItem,wxModelItem* item)
{
	wxTreeItemId itemId = pTree->InsertItem(baseItem,item->GetId(), item->GetLabel());
  pTree->SetItemData(itemId, item);
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



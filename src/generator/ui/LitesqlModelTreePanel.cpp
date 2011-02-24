#include "LitesqlModelTreePanel.h"

#include "LitesqlDatabasePanel.h"
#include "LitesqlObjectPanel.h"
#include "LitesqlRelationPanel.h"
#include "LitesqlFieldPanel.h"
#include "LitesqlMethodPanel.h"

using namespace xml;
using namespace litesql;
using namespace ui;

wxCompositeModelItem::wxCompositeModelItem(): wxModelItem(),m_childrenInitalized(false) {};

wxCompositeModelItem::~wxCompositeModelItem() 
{}

wxModelItem::sequence* wxCompositeModelItem::GetChildren() 
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
	wxFieldItem(Field::Ptr pField) : m_pField(pField) {}
  ~wxFieldItem()  { }
  
	wxString GetLabel() { wxString tmp(m_pField->name.c_str(),wxConvUTF8);
		return label=tmp;	};
	
  wxWindow* GetEditPanel(wxWindow *parent) 			
  {	
    LitesqlFieldPanel* pPanel = new LitesqlFieldPanel(parent,m_pField);
    return pPanel;
  };
  
  Field::Ptr& field() { return m_pField; }
private:
	Field::Ptr m_pField;

};

class wxRelateItem;

class wxRelateItem : public wxModelItem 
{
public:
	wxRelateItem(Relate::Ptr& pRelate,ObjectModel::Ptr& pModel) 
    : m_pRelate(pRelate), m_pModel(pModel) {}
  ~wxRelateItem()  { }
  
	wxString GetLabel() { wxString tmp(m_pRelate->objectName.c_str(),wxConvUTF8);
		return label= _("Relate ") + tmp;	};
	
  wxWindow* GetEditPanel(wxWindow *parent) 			
  {	
    LitesqlRelatePanel* pPanel = new LitesqlRelatePanel(parent,m_pRelate);
    pPanel->setBaseClasses(m_pModel->objects);
    return pPanel;
  };
  
  Relate::Ptr& relate() { return m_pRelate; }
private:
	Relate::Ptr m_pRelate;
  ObjectModel::Ptr m_pModel;
};

class wxMethodItem: public wxModelItem 
{
public:
	wxMethodItem(Method::Ptr& pMethod) : m_pMethod(pMethod) {}
	~wxMethodItem()  {}
  
  wxString GetLabel() 		{ wxString tmp(m_pMethod->name.c_str(),wxConvUTF8);
		return label=tmp;	};
  
  wxWindow* GetEditPanel(wxWindow *parent) 			
  {	
    LitesqlMethodPanel* pPanel = new LitesqlMethodPanel(parent,m_pMethod);
    return pPanel;
  };
  Method::Ptr& method() {return m_pMethod; };
private:
	Method::Ptr m_pMethod;
};

class wxLitesqlObject : public wxCompositeModelItem {
	public:
    wxLitesqlObject(ObjectPtr& pObject,ObjectModel::Ptr& pModel)
			:	m_pObject(pObject),m_pModel(pModel)	{};
  ~wxLitesqlObject()  { }
  
  wxString GetLabel() 		{ wxString tmp(m_pObject->name.c_str(),wxConvUTF8);
		return label=tmp;	};

  wxWindow* GetEditPanel(wxWindow *parent) 			
  {	
    LitesqlObjectPanel* pPanel = new LitesqlObjectPanel(parent,m_pObject);
    pPanel->setBaseClasses(m_pModel->objects);
    return pPanel;
  };

  void AddField(wxFieldItem* pField)
  {

    m_pObject->fields.push_back(pField->field());
    m_children.push_back(pField);
  }

  void AddMethod(wxMethodItem* pMethod)
  {
    m_pObject->methods.push_back(pMethod->method());
    m_children.push_back(pMethod);
  }

  bool hasChildren() const {	return true; };

		void InitChildren()
		{
			for (Field::sequence::iterator field = m_pObject->fields.begin();
				field != m_pObject->fields.end();
				field++)
			{
				m_children.push_back(new wxFieldItem(*field));
			}

			for (Method::sequence::iterator method = m_pObject->methods.begin();
				method != m_pObject->methods.end();
				method++)
			{
				m_children.push_back(new wxMethodItem(*method));
			}
		}
	
    ObjectPtr& object() {return m_pObject;}
private:
		ObjectPtr m_pObject;	
    ObjectModel::Ptr m_pModel;
};

class wxLitesqlRelation : public wxCompositeModelItem {
	public:
    wxLitesqlRelation(Relation::Ptr& pRelation,ObjectModel::Ptr& pModel)
			:	m_pRelation(pRelation),m_pModel(pModel)	{};
  ~wxLitesqlRelation()  { }
  
  wxString GetLabel() { wxString tmp(m_pRelation->name.c_str(),wxConvUTF8);
		return label=tmp;	};

  wxWindow* GetEditPanel(wxWindow *parent) 			
  {	
    LitesqlRelationPanel* pPanel = new LitesqlRelationPanel(parent,m_pRelation);
    return pPanel;
  };

  void AddField(wxFieldItem* pField)
  {
    m_pRelation->fields.push_back(pField->field());
    m_children.push_back(pField);
  }

  void AddRelate(wxRelateItem* pRelate)
  {
    m_pRelation->related.push_back(pRelate->relate());
    m_children.push_back(pRelate);
  }

  bool hasChildren() const {	return true; };

		void InitChildren()
		{
			for (Field::sequence::iterator field = m_pRelation->fields.begin();
				field != m_pRelation->fields.end();
				field++)
			{
				m_children.push_back(new wxFieldItem(*field));
			}

      for (Relate::sequence::iterator relate = m_pRelation->related.begin();
				relate != m_pRelation->related.end();
				relate++)
			{
				m_children.push_back(new wxRelateItem(*relate,m_pModel));
			}


		}
    Relation::Ptr& relation() { return m_pRelation; }
private:
  Relation::Ptr m_pRelation;	
  ObjectModel::Ptr m_pModel;
};


wxLitesqlModel::wxLitesqlModel(ObjectModel::Ptr& pModel)
:	m_pModel(pModel) { 	}

wxWindow* wxLitesqlModel::GetEditPanel(wxWindow *parent) 			
{	
  LitesqlDatabasePanel* pPanel = new LitesqlDatabasePanel(parent,m_pModel->db);
  return pPanel;
}


wxString wxLitesqlModel::GetLabel() {
  wxString tmp(m_pModel->db->name.c_str(),wxConvUTF8);

  return label=tmp; };

void   wxLitesqlModel::InitChildren()
{
  for( ObjectSequence::iterator it = m_pModel->objects.begin(); it != m_pModel->objects.end();it++)
  {
    m_children.push_back(new wxLitesqlObject(*it,m_pModel));
    
  }

  for( Relation::sequence::iterator it = m_pModel->relations.begin(); it != m_pModel->relations.end();it++)
  {
    m_children.push_back(new wxLitesqlRelation(*it,m_pModel));
  }
}

// ////////////////////////////////////////////////////////////////
//

/** Constructor */
LitesqlModelTreePanel::LitesqlModelTreePanel( wxWindow* parent)
: ModelTreePanel( parent )
{}


LitesqlModelTreePanel::~LitesqlModelTreePanel()
{}

void LitesqlModelTreePanel::setObjectModel(litesql::ObjectModel::Ptr& pModel)
{
  m_pModel = new wxLitesqlModel(pModel);
  m_modelTreeCtrl->DeleteAllItems();
  wxTreeItemId rootId = m_modelTreeCtrl->AddRoot(_("root"));
  wxModelItem::RefreshTree(m_modelTreeCtrl,rootId,m_pModel);
}

wxTreeItemId LitesqlModelTreePanel::AddObject(ObjectPtr& newObject)
{
  wxLitesqlObject* pObject = new wxLitesqlObject(newObject,m_pModel->GetModel());
  m_pModel->GetModel()->objects.push_back(newObject);
  wxTreeItemId newItem = m_modelTreeCtrl->InsertItem(m_pModel->GetId(), -1, pObject->GetLabel(), -1, -1, pObject);

  m_modelTreeCtrl->SelectItem(newItem);

  return newItem;
}

wxTreeItemId LitesqlModelTreePanel::AddRelation()
{
  xml::Relation::Ptr r(new xml::Relation("","",AU_relation_unidir));
  m_pModel->GetModel()->relations.push_back(r);
  
  wxLitesqlRelation* pRelation = new wxLitesqlRelation(r,m_pModel->GetModel());
  wxTreeItemId newItem = m_modelTreeCtrl->InsertItem(m_pModel->GetId(), -1, pRelation->GetLabel(), -1, -1, pRelation);
  m_modelTreeCtrl->SelectItem(newItem);

  return newItem;
}

wxTreeItemId LitesqlModelTreePanel::AddField()
{
  wxTreeItemId newItem; 
  wxTreeItemId selectedItem = GetTreeCtrl()->GetSelection();
  if (!selectedItem)
  {
//     wxMessageBox("select an object or relation");
  }
  else
  {
    wxLitesqlObject* data = dynamic_cast<wxLitesqlObject*>(GetTreeCtrl()->GetItemData(selectedItem));
    if (data)
    {
      Field::Ptr newField(new xml::Field("newField",AU_field_type,"",AU_field_indexed,AU_field_unique));
      wxFieldItem* field = new wxFieldItem(newField);
      data->AddField(field);
      newItem = m_modelTreeCtrl->InsertItem(selectedItem, -1, field->GetLabel(), -1, -1, field);
      m_modelTreeCtrl->SelectItem(newItem);
    }
    else 
    {
      wxLitesqlRelation* relation = dynamic_cast<wxLitesqlRelation*>(GetTreeCtrl()->GetItemData(selectedItem));
      if (relation)
      {
        Field::Ptr newField(new xml::Field("newField",AU_field_type,"",AU_field_indexed,AU_field_unique));
        wxFieldItem* field = new wxFieldItem(newField);
        relation->AddField(field);
        newItem = m_modelTreeCtrl->InsertItem(selectedItem, -1, field->GetLabel(), -1, -1, field);
        m_modelTreeCtrl->SelectItem(newItem);
      }
    }
  }
  return newItem;
}
wxTreeItemId LitesqlModelTreePanel::AddRelated()
{
  wxTreeItemId newItem; 
  wxTreeItemId selectedItem = GetTreeCtrl()->GetSelection();
  if (!selectedItem)
  {
//     wxMessageBox("select an object or relation");
  }
  else
  {
    wxLitesqlRelation* relation = dynamic_cast<wxLitesqlRelation*>(GetTreeCtrl()->GetItemData(selectedItem));
    if (relation)
    {
      Relate::Ptr newRelate(new xml::Relate( "newRelate", 
                                                      AU_relate_limit,
                                                      AU_relate_unique, 
                                                      "newRelateHandle" )
                                      );
      wxRelateItem* relate = new wxRelateItem(newRelate,m_pModel->GetModel());
      relation->AddRelate(relate);

      newItem = m_modelTreeCtrl->InsertItem(selectedItem, 
        -1, 
        relate->GetLabel(), 
        -1, 
        -1, 
        relate);
      m_modelTreeCtrl->SelectItem(newItem);
    }
  }
  return newItem;
}

wxTreeItemId LitesqlModelTreePanel::AddMethod()
{
  wxTreeItemId newItem; 
  wxTreeItemId selectedItem = GetTreeCtrl()->GetSelection();
  if (selectedItem)
  {
    wxLitesqlObject* data = dynamic_cast<wxLitesqlObject*>(GetTreeCtrl()->GetItemData(selectedItem));
    if (data)
    {
      Method::Ptr ptrMethod(new xml::Method("newMethod", ""));
      wxMethodItem* method = new wxMethodItem(ptrMethod);
      data->AddMethod(method);
      newItem = m_modelTreeCtrl->InsertItem(selectedItem, -1, method->GetLabel(), -1, -1, method);
      m_modelTreeCtrl->SelectItem(newItem);
    }
  }
  return newItem;
}

bool LitesqlModelTreePanel::RemoveObject()
{
  bool success;
  
  wxTreeItemId selectedItem = GetTreeCtrl()->GetSelection();
  if (!selectedItem)
  {
    success = false;
  }
  else
  {
    wxLitesqlObject* data = dynamic_cast<wxLitesqlObject*>(GetTreeCtrl()->GetItemData(selectedItem));
    if (data)
    {
      m_pModel->GetModel()->remove(data->object());
      m_modelTreeCtrl->Delete(selectedItem);
    }
    success = (data!=NULL);
  }
  return success;
}

bool LitesqlModelTreePanel::RemoveField()
{
  bool success;
  
  wxTreeItemId selectedItem = GetTreeCtrl()->GetSelection();
  if (!selectedItem)
  {
    success = false;
  }
  else
  {
    wxTreeItemId selectedItem = GetTreeCtrl()->GetSelection();
    wxFieldItem* data = dynamic_cast<wxFieldItem*>(GetTreeCtrl()->GetItemData(selectedItem));
    if (data)
    {
      m_pModel->GetModel()->remove(data->field());
      m_modelTreeCtrl->Delete(selectedItem);
    }
    success = (data!=NULL);
  }
  return success;
}

bool LitesqlModelTreePanel::RemoveRelated()
{
  bool success;
  
  wxTreeItemId selectedItem = GetTreeCtrl()->GetSelection();
  if (!selectedItem)
  {
    success = false;
  }
  else
  {
    wxTreeItemId selectedItem = GetTreeCtrl()->GetSelection();
    wxRelateItem* data = dynamic_cast<wxRelateItem*>(GetTreeCtrl()->GetItemData(selectedItem));
    if (data)
    {
      m_pModel->GetModel()->remove(data->relate());
      m_modelTreeCtrl->Delete(selectedItem);
    }
    success = (data!=NULL);
  }
  return success;
}

bool LitesqlModelTreePanel::RemoveMethod()
{
  bool success;
  
  wxTreeItemId selectedItem = GetTreeCtrl()->GetSelection();
  if (!selectedItem)
  {
    success = false;
  }
  else
  {
    wxMethodItem* data = dynamic_cast<wxMethodItem*>(GetTreeCtrl()->GetItemData(selectedItem));
    if (data)
    {
      m_pModel->GetModel()->remove(data->method());
      m_modelTreeCtrl->Delete(selectedItem);
    }
    success = (data!=NULL);
  }
  return success;
}

bool LitesqlModelTreePanel::RemoveRelation()
{
  bool success;
  
  wxTreeItemId selectedItem = GetTreeCtrl()->GetSelection();
  if (!selectedItem)
  {
    success = false;
  }
  else
  {
    wxLitesqlRelation* data = dynamic_cast<wxLitesqlRelation*>(GetTreeCtrl()->GetItemData(selectedItem));
    if (data)
    {
      m_pModel->GetModel()->remove(data->relation());
      m_modelTreeCtrl->Delete(selectedItem);
    }
    success = (data!=NULL);
  }
  return success;
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
  if (event.GetOldItem())
  {
    wxModelItem* pOldItem = (wxModelItem*)GetTreeCtrl()->GetItemData(event.GetOldItem());
    if (pOldItem)
    {
      GetTreeCtrl()->SetItemText(event.GetOldItem(), pOldItem->GetLabel());
    }
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

void LitesqlModelTreePanel::OnTreeSelChanging( wxTreeEvent& WXUNUSED(event) )
{
    //  wxModelItem* pItem = (wxModelItem*)GetTreeCtrl()->GetItemData(event.GetItem());
}

void wxModelItem::RefreshTree(wxTreeCtrl* pTree,wxTreeItemId& baseItem,wxModelItem* item)
{
	wxTreeItemId itemId = pTree->InsertItem(baseItem,item->GetId(), item->GetLabel());
  pTree->SetItemData(itemId, item);
	if (item->hasChildren())
	{
    wxModelItem::sequence* pChildren = item->GetChildren();
		for(wxModelItem::sequence::iterator it =  pChildren->begin(); 
        it != pChildren->end();
        it++)
		{
			RefreshTree(pTree,itemId,(wxModelItem*)(*it));
		}
	}
}



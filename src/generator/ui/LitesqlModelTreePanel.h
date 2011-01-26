#ifndef LitesqlModelTreePanel_h
#define LitesqlModelTreePanel_h


#include "ui.h"

#include "objectmodel.hpp"

class wxModelItem;


class wxModelItem : public wxTreeItemData {
public:
  typedef std::vector<wxModelItem*> sequence;
	wxModelItem() : wxTreeItemData() { SetId(this);};
	virtual ~wxModelItem() {}; 
  virtual wxString GetLabel() {return label;};
	virtual wxWindow* GetEditPanel(wxWindow *parent) {return NULL; };
	virtual bool hasChildren() const      {	return false;	};
	virtual sequence* GetChildren()	{	return NULL;	};

	static void RefreshTree(wxTreeCtrl* pTree,wxTreeItemId& baseItem,wxModelItem* item);
protected:
  wxString label;
};




class wxCompositeModelItem : public wxModelItem {
public:
	wxCompositeModelItem(); 
  virtual ~wxCompositeModelItem(); 

	
	wxModelItem::sequence* GetChildren();
	
protected:
	virtual void InitChildren()=0;
	wxModelItem::sequence m_children;
  
private:
  bool m_childrenInitalized;
};

class wxLitesqlModel : public wxCompositeModelItem {
public:
  wxLitesqlModel(litesql::ObjectModel::counted_ptr& pModel);
  wxString GetLabel();
  wxWindow* GetEditPanel(wxWindow *parent);
  bool hasChildren() const {	return true; };
  litesql::ObjectModel::counted_ptr& GetModel() {return m_pModel; };
  
protected:
  void InitChildren();
  
private:
  litesql::ObjectModel::counted_ptr m_pModel;
};

class LitesqlModelTreePanel : public ui::ModelTreePanel
{
public:
	/** Constructor */
  LitesqlModelTreePanel( wxWindow* parent);
  virtual ~LitesqlModelTreePanel();
  
  wxTreeCtrl* GetTreeCtrl()
  {
	  return m_modelTreeCtrl;
  }
  
  wxNotebook* GetDetailNotebook()
  {
    return m_detailNotebook;
  }

  void setObjectModel(litesql::ObjectModel::counted_ptr& pModel);

  wxTreeItemId AddObject(xml::ObjectPtr& newObject);
  wxTreeItemId AddField();
  wxTreeItemId AddMethod();
  wxTreeItemId AddRelation();
  wxTreeItemId AddRelated();

  bool RemoveObject();
  bool RemoveField();
  bool RemoveMethod();
  bool RemoveRelation();
  bool RemoveRelated();

  
private:
  wxLitesqlModel* m_pModel;
  
  
	void OnTreeDeleteItem( wxTreeEvent& event ){ event.Skip(); }
	void OnTreeItemActivated( wxTreeEvent& event ){ event.Skip(); }
	void OnTreeItemGetTooltip( wxTreeEvent& event ){ event.Skip(); }
	void OnTreeSelChanged( wxTreeEvent& event );
	void OnTreeSelChanging( wxTreeEvent& event );  
};

#endif // #ifndef LitesqlModelTreePanel_h


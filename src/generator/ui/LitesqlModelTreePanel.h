#ifndef LitesqlModelTreePanel_h
#define LitesqlModelTreePanel_h


#include "ui.h"

#include "objectmodel.hpp"

class wxModelItem;


class wxModelItem : public wxTreeItemData {
public:
  typedef std::vector<wxModelItem*> sequence;
  wxModelItem(const wxChar* _labelPrefix) 
    : wxTreeItemData(),
      labelPrefix(_labelPrefix) 
  { SetId(this);};
	
  virtual ~wxModelItem() {}; 
  const wxString& GetLabelPrefix() const { return labelPrefix; };
	virtual wxString GetLabel() const = 0;
  virtual int GetItemImage() { return -1; }
  virtual int GetSelectedItemImage() { return GetItemImage(); }

  
  virtual wxString GetItemText() 
  { wxString tmp = GetLabelPrefix();
    tmp += _T(" ");
    tmp+= GetLabel();
    return tmp;
  };
	
	virtual wxWindow* GetEditPanel(wxWindow *parent) {return NULL; };
	virtual bool hasChildren() const      {	return false;	};
	virtual sequence* GetChildren()	{	return NULL;	};

protected:
  wxString labelPrefix;
};




class wxCompositeModelItem : public wxModelItem {
public:
  wxCompositeModelItem(const wxChar* _labelPrefix); 
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
  wxLitesqlModel(litesql::ObjectModel::Ptr& pModel);
  wxString GetLabel() const ;
  wxWindow* GetEditPanel(wxWindow *parent);
  bool hasChildren() const {	return true; };
  litesql::ObjectModel::Ptr& GetModel() {return m_pModel; };
  
protected:
  void InitChildren();
  
private:
  litesql::ObjectModel::Ptr m_pModel;
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

  void setObjectModel(litesql::ObjectModel::Ptr& pModel);

  wxTreeItemId InsertItem(const wxTreeItemId& parent,wxModelItem* pItem);
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


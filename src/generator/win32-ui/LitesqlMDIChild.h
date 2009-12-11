//////////////////////////////////////////////////////
// SimpleMDIChild.h
//  Declaration of the CLitesqlView and CSimpleMDIChild class

#ifndef MDICHILDVIEW_H
#define MDICHILDVIEW_H

#include "text.h"
#include "mdi.h"
#include "treeview.h"
#include "splitter.h"

#include "litesqldocument.hpp"

class CMyTreeView : public CTreeView
{
public:
  CMyTreeView() { };
  virtual ~CMyTreeView() {};

  void OnInitialUpdate();
  void setObjectModel(ObjectModel* pModel);
  
  private:
    HTREEITEM AddItem(HTREEITEM hParent, LPCTSTR szText, int iImage);

    void loadTree();
    ObjectModel* m_pModel;
};

class CFilesTab : public CTab
{
public:
  CFilesTab();
  virtual ~CFilesTab() {};
};

// Declaration of CLitesqlView
class CLitesqlView : public CSplitter
{
public:
	CLitesqlView(LitesqlDocument* pDocument);
  virtual ~CLitesqlView(){};

private:

  LitesqlDocument* m_pDocument;
  
  CMyTreeView mTree;
  CFilesTab mFiles;
};



// Declaration of CSimpleMDIChild
class CSimpleMDIChild : public CMDIChild
{
public:
	CSimpleMDIChild(LPCTSTR pszFilename=NULL);
  virtual ~CSimpleMDIChild();

  LitesqlDocument* getDocument() { return &m_Document; };
protected:
	virtual void OnInitialUpdate();

private:
  CLitesqlView m_View;
  LitesqlDocument m_Document;
};

#endif  //MDICHILDVIEW_H

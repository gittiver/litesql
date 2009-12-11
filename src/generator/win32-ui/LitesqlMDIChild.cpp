//////////////////////////////////////////////
// SimpleMDIChild.cpp
//  Definitions for the CLitesqlView and CSimpleMDIChild classes

#include "LitesqlMDIChild.h"
#include "resource.h"


// CLitesqlView definitions
CLitesqlView::CLitesqlView(LitesqlDocument* pDoc)
{
  m_pDocument = pDoc;
  mTree.setObjectModel(&m_pDocument->getModel());
  SetPanes(mTree,mFiles);
}

// CSimpleMDIChild definitions
CSimpleMDIChild::CSimpleMDIChild(const char* pszFilename)
  : m_Document(pszFilename),m_View(&m_Document)
{

	// Set m_MainView as the view window of the frame
	SetView(m_View);

  
	// Set the menu for this MDI child
	SetChildMenu(_T("MdiMenuView"));
}

CSimpleMDIChild::~CSimpleMDIChild()
{
}

void CSimpleMDIChild::OnInitialUpdate()
{
  std::string title = _T("Litesql Gen Project - ");
  title.append(m_Document.getFilename());
  // Set the window caption
  ::SetWindowText(m_hWnd, title.c_str());
   
    // Set the window icons
  SetIconLarge(IDI_VIEW);
	SetIconSmall(IDI_VIEW);
}

CFilesTab::CFilesTab()
{ 
  AddTabPage(new CViewText,"Cpp"); 
  AddTabPage(new CViewText,"hpp");
};

void CMyTreeView::setObjectModel(ObjectModel* pModel)
{
  m_pModel = pModel;
  loadTree();
}

void CMyTreeView::OnInitialUpdate()
{
DWORD dwStyle = (DWORD)GetWindowLongPtr(GWL_STYLE);
	dwStyle |= TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT;
	SetWindowLongPtr(GWL_STYLE, dwStyle);
loadTree();
}

void CMyTreeView::loadTree()
{
  DeleteAllItems();
  HTREEITEM hItem = AddItem(NULL,"Model",0);
  m_pModel->db;
  for (std::vector<xml::Object* >::const_iterator it = m_pModel->objects.begin();
    it != m_pModel->objects.end();
    it++)
  {
    HTREEITEM hObjectItem = AddItem(hItem,(*it)->name.c_str(),1);
    
    for (vector<xml::Field*>::const_iterator field_it = (*it)->fields.begin();
          field_it != (*it)->fields.end();
          field_it++)
    {
      AddItem(hObjectItem,(*field_it)->name.c_str(),1);
    }

    for (vector<xml::Method*>::const_iterator method_it = (*it)->methods.begin();
          method_it  != (*it)->methods.end();
          method_it++)
    {
      AddItem(hObjectItem,(*method_it)->name.c_str(),1);
    }
  }

  for ( vector<xml::Relation*>::const_iterator it_relation = m_pModel->relations.begin();
    it_relation != m_pModel->relations.end();
    it_relation++)
  {
    HTREEITEM hRelationItem = AddItem(hItem,(*it_relation)->name.c_str(),1);
  
    for(vector<xml::Relate*>::const_iterator it = (*it_relation)->related.begin();
      it != (*it_relation)->related.end();
      it++)
    {
      AddItem(hRelationItem,(*it)->handle.c_str(),1);
    }
    
  }

}

HTREEITEM CMyTreeView::AddItem(HTREEITEM hParent, LPCTSTR szText, int iImage)
{
	TVITEM tvi = {0};
	tvi.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvi.iImage = iImage;
	tvi.iSelectedImage = iImage;
	tvi.pszText = (LPTSTR)szText;

	TVINSERTSTRUCT tvis = {0};
	tvis.hParent = hParent;
	tvis.item = tvi;

	return InsertItem(tvis);
}

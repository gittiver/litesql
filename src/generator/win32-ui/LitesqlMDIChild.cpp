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

void CModelTreeView::setObjectModel(ObjectModel* pModel)
{
  m_pModel = pModel;
  loadTree();
}

void CModelTreeView::OnInitialUpdate()
{
  DWORD dwStyle = (DWORD)GetWindowLongPtr(GWL_STYLE);
  dwStyle |= TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT;
  SetWindowLongPtr(GWL_STYLE, dwStyle);
 
  loadTree();
}

void CModelTreeView::loadTree()
{
  DeleteAllItems();
  HTREEITEM hItem = AddItem(NULL,(LPARAM)m_pModel);
  
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

void CModelTreeView::DoContextMenu(CPoint& ptScreen)
{
  CPoint m = ptScreen;
  //ScreenToClient(m_hWnd, &m);
  //HMENU hPopupMenu = LoadMenu(GetApp()->GetResourceHandle(),MAKEINTRESOURCE(IDM_MODELTREE_VIEW));
  HMENU hPopup = CreatePopupMenu();
  AppendMenu(hPopup,MF_STRING,IDM_ADD_OBJECT,_T("Add Object"));
  AppendMenu(hPopup,MF_STRING,IDM_REMOVE_OBJECT,_T("Remove Object"));

  UINT idCmd = ::TrackPopupMenu(hPopup, TPM_LEFTALIGN | TPM_RETURNCMD | TPM_RIGHTBUTTON ,
							ptScreen.x, ptScreen.y, 0, m_hWnd, NULL);

  switch (idCmd)
  {
  case IDM_ADD_OBJECT:
    m_pModel->objects.push_back(new xml::Object("newObject",""));
    loadTree();
    break;
  case IDM_REMOVE_OBJECT:
    {
      HTREEITEM selected = GetSelection();
      DeleteItem(selected );
    }
    
    break;

  default:
    ;

  }
						//A Treeview control sometimes requires this to end the
						// TrackPopupMenu properly
						::PostMessage(GetParent(), WM_CANCELMODE, 0, 0);
            ::DestroyMenu(hPopup);
}

LRESULT CModelTreeView::OnNotifyReflect(WPARAM /*wParam*/, LPARAM lParam)
{
  LPNMHDR  lpnmh = (LPNMHDR) lParam;

  switch(lpnmh->code)
  {
  case NM_DBLCLK:
    {
      CPoint ptScreen;
      ::GetCursorPos(&ptScreen);
    }
    break;
    
  case NM_RCLICK:
    {
      CPoint ptScreen;
      ::GetCursorPos(&ptScreen);
      DoContextMenu(ptScreen);
    }
    break;
  case TVN_GETDISPINFO:
    {

      LPNMTVDISPINFO lpdi = (LPNMTVDISPINFO)lParam;
      ObjectModel* m = dynamic_cast<ObjectModel*>((ObjectModel*)(lpdi->item.lParam));
      if (m)
      {
      	if(lpdi->item.mask & TVIF_TEXT)
        {
          lpdi->item.pszText = "Model";
        }
      }
      else
      {
      }

      
      //TreeItemData* pItem = (TreeItemData*)lpdi->item.lParam;

      //	//do we need to supply the text?
      //	if(lpdi->item.mask & TVIF_TEXT)
      //	{
      //		SHFILEINFO sfi = {0};

      //		//get the display name of the item
      //		if(pItem->GetFullCpidl().SHGetFileInfo(0, sfi, SHGFI_PIDL | SHGFI_DISPLAYNAME))
      //			::lstrcpyn(lpdi->item.pszText, sfi.szDisplayName, lpdi->item.cchTextMax -1);
      //	}

      //	//do we need to supply the unselected image?
      //	if(lpdi->item.mask & TVIF_IMAGE)
      //	{
      //		SHFILEINFO sfi = {0};

      //		//get the unselected image for this item
      //		if(pItem->GetFullCpidl().SHGetFileInfo(0, sfi, SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_LINKOVERLAY))
      //			lpdi->item.iImage = sfi.iIcon;
      //	}

      //	//do we need to supply the selected image?
      //	if(lpdi->item.mask & TVIF_SELECTEDIMAGE)
      //	{
      //		SHFILEINFO sfi = {0};

      //		//get the selected image for this item
      //		if(pItem->GetFullCpidl().SHGetFileInfo(0, sfi, SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_OPENICON))
      //			lpdi->item.iSelectedImage = sfi.iIcon;
      //	}
    }
    break;

  case TVN_ITEMEXPANDING:
    {
      LPNMTREEVIEW pnmtv = (LPNMTREEVIEW)lParam;

      switch(pnmtv->action)
      {
      case TVE_EXPAND:
        {
          //UINT ExpandedOnce = pnmtv->itemNew.state & TVIS_EXPANDEDONCE;
          //if (!ExpandedOnce)
          //	GetChildItems(pnmtv->itemNew.hItem);
        }
        break;
      }
    }
    break;

  case TVN_SELCHANGED:
    {
      LPNMTREEVIEW pnmtv = (LPNMTREEVIEW)lParam;
      //TreeItemData* pItem = (TreeItemData*)pnmtv->itemNew.lParam;

      //CMyListView* LeftView = GetExplorerApp().GetMainFrame().GetListView();
      //LeftView->DisplayFolder(pItem->GetParentFolder(), pItem->GetFullCpidl(), pItem->GetRelCpidl());
    }
    break;


//  default:
  }

  // return 0L for unhandled notifications
  return 0L;
}

HTREEITEM CModelTreeView::AddItem(HTREEITEM hParent, LPARAM lParam)
{
  TVITEM tvi = {0};
  tvi.mask = TVIF_PARAM|TVIF_CHILDREN|TVIF_TEXT;
  tvi.lParam = lParam;
  tvi.cChildren = I_CHILDRENCALLBACK;
  tvi.pszText = LPSTR_TEXTCALLBACK;

  TVINSERTSTRUCT tvis = {0};
  tvis.hParent = hParent;
  tvis.item = tvi;

  return InsertItem(tvis);
}

HTREEITEM CModelTreeView::AddItem(HTREEITEM hParent, LPCTSTR szText, int iImage)
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

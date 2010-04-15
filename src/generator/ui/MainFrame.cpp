#include "MainFrame.h"

#include "objectmodel.hpp"

IMPLEMENT_CLASS(MainFrame, wxDocMDIParentFrame)
BEGIN_EVENT_TABLE(MainFrame, wxDocMDIParentFrame)
//    EVT_MENU(DOCVIEW_ABOUT, MainFrame::OnAbout)
END_EVENT_TABLE()

MainFrame::MainFrame(wxDocManager *manager, wxFrame *frame, const wxString& title,
    const wxPoint& pos, const wxSize& size, long type):
  wxDocMDIParentFrame(manager, frame, wxID_ANY, title, pos, size, type, _T("myFrame"))
{
  editMenu = (wxMenu *) NULL;
}


//void MainFrame::OnAbout(wxCommandEvent& WXUNUSED(event) )
//{
//    (void)wxMessageBox(_T("DocView Demo\nAuthor: Julian Smart\nUsage: docview.exe"), _T("About DocView"));
//}

// Creates a canvas. Called from view.cpp when a new drawing
// view is created.
//MyCanvas *MainFrame::CreateCanvas(wxView *view, wxMDIChildFrame *parent)
//{
//  int width, height;
//  parent->GetClientSize(&width, &height);
//
//  // Non-retained canvas
//  MyCanvas *canvas = new MyCanvas(view, parent, wxPoint(0, 0), wxSize(width, height), 0);
//  canvas->SetCursor(wxCursor(wxCURSOR_PENCIL));
//
//  // Give it scrollbars
//  canvas->SetScrollbars(20, 20, 50, 50);
//
//  return canvas;
//}

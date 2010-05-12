#include <wx/cmdproc.h>
#include <wx/msgdlg.h>

#include "config.h"

#include "objectmodel.hpp"

#include "MainFrame.h"
#include "VisualLitesqlApp.h"
#include "LitesqlDocument.h"
#include "GenerateView.h"

#include "ui.h"

IMPLEMENT_CLASS(MainFrame, wxDocMDIParentFrame)
BEGIN_EVENT_TABLE(MainFrame, wxDocMDIParentFrame)
    EVT_MENU(VisualLitesqlApp::ID_ABOUT, MainFrame::OnAbout)
END_EVENT_TABLE()

MainFrame::MainFrame(wxDocManager *manager, wxFrame *frame, const wxString& title,
    const wxPoint& pos, const wxSize& size, long type):
  wxDocMDIParentFrame(manager, frame, wxID_ANY, title, pos, size, type, _T("myFrame"))
{
  editMenu = (wxMenu *) NULL;
}

MainFrame::~MainFrame()
{}

void MainFrame::OnAbout(wxCommandEvent& WXUNUSED(event) )
{
  ui::AboutDialog dlg(this);
  dlg.ShowModal();
}


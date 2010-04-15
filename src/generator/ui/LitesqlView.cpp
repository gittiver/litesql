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

#include "VisualLitesqlApp.h"
#include "LitesqlView.h"


IMPLEMENT_DYNAMIC_CLASS(LitesqlView, wxView)

BEGIN_EVENT_TABLE(LitesqlView, wxView)
// 
END_EVENT_TABLE()

// What to do when a view is created. Creates actual
// windows for displaying the view.
bool LitesqlView::OnCreate(wxDocument *doc, long WXUNUSED(flags) )
{
    frame = wxGetApp().CreateChildFrame(doc, this, true);
    frame->SetTitle(_T("LitesqlView"));

#ifdef __X__
    // X seems to require a forced resize
    int x, y;
    frame->GetSize(&x, &y);
    frame->SetSize(wxDefaultCoord, wxDefaultCoord, x, y);
#endif
    frame->Show(true);
    Activate(true);

    return true;
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

void LitesqlView::OnCut(wxCommandEvent& WXUNUSED(event) )
{
}

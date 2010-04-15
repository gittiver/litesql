#ifndef LITESQL_VIEW_H
#define LITESQL_VIEW_H

#include <wx/docview.h>

class LitesqlView: public wxView
{
public:
    wxMDIChildFrame *frame;
//    MyCanvas *canvas;
  
    LitesqlView() { /* canvas = (MyCanvas *) NULL;*/ frame = (wxMDIChildFrame *) NULL; }
    ~LitesqlView() {}


    bool OnCreate(wxDocument *doc, long flags);
    void OnDraw(wxDC *dc);
    bool OnClose(bool deleteWindow = true);

    void OnCut(wxCommandEvent& event);

private:
    DECLARE_DYNAMIC_CLASS(LitesqlView)
    DECLARE_EVENT_TABLE()
};

#endif // #ifndef LITESQL_VIEW_H

#ifndef LITESQL_VIEW_H
#define LITESQL_VIEW_H

#include <wx/docview.h>

class wxTreeListCtrl;

class LitesqlView: public wxView
{
public:
    wxMDIChildFrame *frame;
    wxTreeListCtrl* m_treelist;
//    MyCanvas *canvas;
  
    LitesqlView();
    virtual ~LitesqlView();


    bool OnCreate(wxDocument *doc, long flags);
    void OnUpdate(wxView *sender, wxObject *hint = (wxObject *) NULL);
    
    void OnDraw(wxDC *dc);
    bool OnClose(bool deleteWindow = true);

    void OnCut(wxCommandEvent& event);

private:
    DECLARE_DYNAMIC_CLASS(LitesqlView)
    DECLARE_EVENT_TABLE()
};

#endif // #ifndef LITESQL_VIEW_H

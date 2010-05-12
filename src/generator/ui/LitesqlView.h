#ifndef LITESQL_VIEW_H
#define LITESQL_VIEW_H

#include <wx/docview.h>

class wxTreebook;

class LitesqlView: public wxView
{
public:
    wxMDIChildFrame *frame;
    wxTreebook* m_treebook;
  
    LitesqlView();
    virtual ~LitesqlView();


protected:

    bool OnCreate(wxDocument *doc, long flags);
    void OnUpdate(wxView *sender, wxObject *hint = (wxObject *) NULL);
    
    void OnDraw(wxDC *dc);
    bool OnClose(bool deleteWindow = true);
    void OnSize(wxSizeEvent& event);

    void OnCut(wxCommandEvent& event);
    void OnGenerate(wxCommandEvent& event );

private:
    DECLARE_DYNAMIC_CLASS(LitesqlView)
    DECLARE_EVENT_TABLE()
};

#endif // #ifndef LITESQL_VIEW_H

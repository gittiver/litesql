#ifndef GENERATE_VIEW_H
#define GENERATE_VIEW_H

#include <wx/docview.h>


class GenerateView: public wxView
{
public:
    wxMDIChildFrame *frame;
  
    GenerateView();
    virtual ~GenerateView();


    bool OnCreate(wxDocument *doc, long flags);
    void OnUpdate(wxView *sender, wxObject *hint = (wxObject *) NULL);
    
    void OnDraw(wxDC *dc);
    bool OnClose(bool deleteWindow = true);

    void OnCut(wxCommandEvent& event);
    void OnGenerate(wxCommandEvent& event );

private:
    DECLARE_DYNAMIC_CLASS(GenerateView)
    DECLARE_EVENT_TABLE()
};

#endif // #ifndef GENERATE_VIEW_H

#ifndef VisualLitesqlApp_H
#define VisualLitesqlApp_H

#include <wx/app.h>
#include <wx/docmdi.h>

class MainFrame;

class VisualLitesqlApp : public wxApp
{
public:
    VisualLitesqlApp(void); 
    virtual ~VisualLitesqlApp(void);

    void info(const wxString& message);
    void warn(const wxString& message);
    void error(const wxString& message);

    wxMDIChildFrame *CreateChildFrame(wxDocument *doc, wxView *view, bool isCanvas);

protected:
	bool OnInit(void);
   int OnExit(void);

private:

  protected:
    wxDocManager* m_docManager;
    MainFrame* pMainframe;
};

DECLARE_APP(VisualLitesqlApp)

#endif // #ifndef VisualLitesqlApp_H

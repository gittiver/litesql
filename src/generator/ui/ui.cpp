///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "ui.h"

// Using the construction of a static object to ensure that the help provider is set
class wxFBContextSensitiveHelpSetter
{
public:
wxFBContextSensitiveHelpSetter()
{
wxHelpProvider::Set( new wxHelpControllerHelpProvider );
}
};

static wxFBContextSensitiveHelpSetter s_wxFBSetTheHelpProvider;
///////////////////////////////////////////////////////////////////////////
using namespace ui;

DocumentFrameBase::DocumentFrameBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	m_menu_document = new wxMenuBar( 0 );
	file = new wxMenu();
	wxMenuItem* m_new;
	m_new = new wxMenuItem( file, ID_NEW, wxString( _("New") ) , wxEmptyString, wxITEM_NORMAL );
	file->Append( m_new );
	
	wxMenuItem* open;
	open = new wxMenuItem( file, ID_OPEN, wxString( _("Open") ) , wxEmptyString, wxITEM_NORMAL );
	file->Append( open );
	
	wxMenuItem* save;
	save = new wxMenuItem( file, ID_SAVE, wxString( _("Save") ) , wxEmptyString, wxITEM_NORMAL );
	file->Append( save );
	
	wxMenuItem* saveAs;
	saveAs = new wxMenuItem( file, ID_SAVE_AS_, wxString( _("Save As ...") ) , wxEmptyString, wxITEM_NORMAL );
	file->Append( saveAs );
	
	file->AppendSeparator();
	
	wxMenuItem* exit;
	exit = new wxMenuItem( file, ID_EXIT, wxString( _("Exit") ) , wxEmptyString, wxITEM_NORMAL );
	file->Append( exit );
	
	m_menu_document->Append( file, _("File") );
	
	edit = new wxMenu();
	m_menu_document->Append( edit, _("Edit") );
	
	help = new wxMenu();
	wxMenuItem* about;
	about = new wxMenuItem( help, ID_ABOUT, wxString( _("About") ) , wxEmptyString, wxITEM_NORMAL );
	help->Append( about );
	
	m_menu_document->Append( help, _("Help") );
	
	this->SetMenuBar( m_menu_document );
	
	wxBoxSizer* bDocumentFrameBoxSizer;
	bDocumentFrameBoxSizer = new wxBoxSizer( wxVERTICAL );
	
	m_splitterDocumentFrame = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	m_splitterDocumentFrame->Connect( wxEVT_IDLE, wxIdleEventHandler( DocumentFrameBase::m_splitterDocumentFrameOnIdle ), NULL, this );
	m_leftWindow = new wxScrolledWindow( m_splitterDocumentFrame, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_leftWindow->SetScrollRate( 5, 5 );
	wxBoxSizer* bSizerLeftWindow;
	bSizerLeftWindow = new wxBoxSizer( wxVERTICAL );
	
	m_treeCtrlModel = new wxTreeCtrl( m_leftWindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE );
	bSizerLeftWindow->Add( m_treeCtrlModel, 1, wxALL|wxEXPAND, 5 );
	
	m_leftWindow->SetSizer( bSizerLeftWindow );
	m_leftWindow->Layout();
	bSizerLeftWindow->Fit( m_leftWindow );
	m_rightWindow = new wxScrolledWindow( m_splitterDocumentFrame, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_rightWindow->SetScrollRate( 5, 5 );
	wxBoxSizer* bSizerRightWindow;
	bSizerRightWindow = new wxBoxSizer( wxVERTICAL );
	
	m_notebookViews = new wxNotebook( m_rightWindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	
	bSizerRightWindow->Add( m_notebookViews, 1, wxEXPAND | wxALL, 5 );
	
	m_rightWindow->SetSizer( bSizerRightWindow );
	m_rightWindow->Layout();
	bSizerRightWindow->Fit( m_rightWindow );
	m_splitterDocumentFrame->SplitVertically( m_leftWindow, m_rightWindow, 0 );
	bDocumentFrameBoxSizer->Add( m_splitterDocumentFrame, 1, wxEXPAND, 5 );
	
	this->SetSizer( bDocumentFrameBoxSizer );
	this->Layout();
	
	// Connect Events
	this->Connect( about->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( DocumentFrameBase::OnAbout ) );
}

DocumentFrameBase::~DocumentFrameBase()
{
	// Disconnect Events
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( DocumentFrameBase::OnAbout ) );
}

MainFrame::MainFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	m_menu_document = new wxMenuBar( 0 );
	file = new wxMenu();
	wxMenuItem* m_new;
	m_new = new wxMenuItem( file, ID_NEW, wxString( _("New") ) , wxEmptyString, wxITEM_NORMAL );
	file->Append( m_new );
	
	wxMenuItem* open;
	open = new wxMenuItem( file, ID_OPEN, wxString( _("Open") ) , wxEmptyString, wxITEM_NORMAL );
	file->Append( open );
	
	wxMenuItem* save;
	save = new wxMenuItem( file, ID_SAVE, wxString( _("Save") ) , wxEmptyString, wxITEM_NORMAL );
	file->Append( save );
	
	wxMenuItem* saveAs;
	saveAs = new wxMenuItem( file, ID_SAVE_AS_, wxString( _("Save As ...") ) , wxEmptyString, wxITEM_NORMAL );
	file->Append( saveAs );
	
	file->AppendSeparator();
	
	wxMenuItem* exit;
	exit = new wxMenuItem( file, ID_EXIT, wxString( _("Exit") ) , wxEmptyString, wxITEM_NORMAL );
	file->Append( exit );
	
	m_menu_document->Append( file, _("File") );
	
	edit = new wxMenu();
	m_menu_document->Append( edit, _("Edit") );
	
	help = new wxMenu();
	wxMenuItem* about;
	about = new wxMenuItem( help, ID_ABOUT, wxString( _("About") ) , wxEmptyString, wxITEM_NORMAL );
	help->Append( about );
	
	m_menu_document->Append( help, _("Help") );
	
	this->SetMenuBar( m_menu_document );
	
	
	// Connect Events
	this->Connect( about->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnAbout ) );
}

MainFrame::~MainFrame()
{
	// Disconnect Events
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnAbout ) );
}

AboutDialog::AboutDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
}

AboutDialog::~AboutDialog()
{
}

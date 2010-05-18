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

ObjectPanel::ObjectPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxGridSizer* gSizer1;
	gSizer1 = new wxGridSizer( 2, 2, 0, 0 );
	
	lblName = new wxStaticText( this, wxID_ANY, _("Name"), wxDefaultPosition, wxDefaultSize, 0 );
	lblName->Wrap( -1 );
	gSizer1->Add( lblName, 1, wxALL, 5 );
	
	m_textCtrlName = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrlName->SetMinSize( wxSize( 150,-1 ) );
	
	gSizer1->Add( m_textCtrlName, 1, wxALL, 5 );
	
	lblInherits = new wxStaticText( this, wxID_ANY, _("Inherit From"), wxDefaultPosition, wxDefaultSize, 0 );
	lblInherits->Wrap( -1 );
	gSizer1->Add( lblInherits, 0, wxALL, 5 );
	
	wxArrayString m_choiceInheritsFromChoices;
	m_choiceInheritsFrom = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceInheritsFromChoices, 0 );
	m_choiceInheritsFrom->SetSelection( 0 );
	m_choiceInheritsFrom->SetMinSize( wxSize( 150,-1 ) );
	
	gSizer1->Add( m_choiceInheritsFrom, 1, wxALL|wxEXPAND, 5 );
	
	this->SetSizer( gSizer1 );
	this->Layout();
}

ObjectPanel::~ObjectPanel()
{
}

FieldPanel::FieldPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxGridSizer* gSizer1;
	gSizer1 = new wxGridSizer( 2, 2, 0, 0 );
	
	this->SetSizer( gSizer1 );
	this->Layout();
}

FieldPanel::~FieldPanel()
{
}

RelationPanel::RelationPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxGridSizer* gSizer1;
	gSizer1 = new wxGridSizer( 2, 2, 0, 0 );
	
	m_staticText2 = new wxStaticText( this, wxID_ANY, _("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	gSizer1->Add( m_staticText2, 0, wxALL, 5 );
	
	m_textCtrl2 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1->Add( m_textCtrl2, 0, wxALL, 5 );
	
	this->SetSizer( gSizer1 );
	this->Layout();
}

RelationPanel::~RelationPanel()
{
}

AboutDialog::AboutDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText4 = new wxStaticText( this, wxID_ANY, _("Visual-Litesql"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	m_staticText4->Wrap( -1 );
	bSizer4->Add( m_staticText4, 1, wxALL|wxEXPAND, 5 );
	
	m_staticText5 = new wxStaticText( this, wxID_ANY, _("Version"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	m_staticText5->Wrap( -1 );
	bSizer4->Add( m_staticText5, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_btnClose = new wxButton( this, wxID_OK, _("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_btnClose, 0, wxALL|wxEXPAND, 5 );
	
	this->SetSizer( bSizer4 );
	this->Layout();
}

AboutDialog::~AboutDialog()
{
}

DatabasePanel::DatabasePanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxGridSizer* panelGridSizer;
	panelGridSizer = new wxGridSizer( 2, 2, 0, 0 );
	
	m_staticText7 = new wxStaticText( this, wxID_ANY, _("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	panelGridSizer->Add( m_staticText7, 0, wxALL, 5 );
	
	m_textCtrl3 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	panelGridSizer->Add( m_textCtrl3, 0, wxALL, 5 );
	
	m_staticText8 = new wxStaticText( this, wxID_ANY, _("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	panelGridSizer->Add( m_staticText8, 0, wxALL, 5 );
	
	m_textCtrl4 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	panelGridSizer->Add( m_textCtrl4, 0, wxALL, 5 );
	
	this->SetSizer( panelGridSizer );
	this->Layout();
}

DatabasePanel::~DatabasePanel()
{
}

GeneratePanel::GeneratePanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxGridSizer* panelSizer;
	panelSizer = new wxGridSizer( 4, 2, 0, 0 );
	
	m_staticOutputDir = new wxStaticText( this, wxID_ANY, _("Output Directory"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticOutputDir->Wrap( -1 );
	panelSizer->Add( m_staticOutputDir, 0, wxALL, 5 );
	
	m_dirPickerOutputDir = new wxDirPickerCtrl( this, wxID_ANY, wxEmptyString, _("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE|wxDIRP_DIR_MUST_EXIST );
	panelSizer->Add( m_dirPickerOutputDir, 1, wxALL, 5 );
	
	m_staticGenerators = new wxStaticText( this, wxID_ANY, _("Select generators"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticGenerators->Wrap( -1 );
	panelSizer->Add( m_staticGenerators, 0, wxALL, 5 );
	
	wxArrayString m_checkListGeneratorsChoices;
	m_checkListGenerators = new wxCheckListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_checkListGeneratorsChoices, 0 );
	panelSizer->Add( m_checkListGenerators, 10, wxALL|wxEXPAND, 5 );
	
	m_buttonRun = new wxButton( this, wxID_ANY, _("Run"), wxDefaultPosition, wxDefaultSize, 0 );
	panelSizer->Add( m_buttonRun, 0, wxALL, 5 );
	
	m_gaugeRunProgress = new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxSize( -1,20 ), wxGA_HORIZONTAL );
	m_gaugeRunProgress->SetMinSize( wxSize( 100,20 ) );
	m_gaugeRunProgress->SetMaxSize( wxSize( -1,20 ) );
	
	panelSizer->Add( m_gaugeRunProgress, 1, wxALL|wxFIXED_MINSIZE, 5 );
	
	this->SetSizer( panelSizer );
	this->Layout();
	
	// Connect Events
	m_buttonRun->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GeneratePanel::OnRunClick ), NULL, this );
}

GeneratePanel::~GeneratePanel()
{
	// Disconnect Events
	m_buttonRun->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GeneratePanel::OnRunClick ), NULL, this );
}

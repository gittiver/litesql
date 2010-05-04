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
	gSizer1->Add( lblName, 0, wxALL, 5 );
	
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

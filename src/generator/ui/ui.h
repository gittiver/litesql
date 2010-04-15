///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __ui__
#define __ui__

#include <wx/cshelp.h>
#include <wx/intl.h>

#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/treectrl.h>
#include <wx/sizer.h>
#include <wx/scrolwin.h>
#include <wx/notebook.h>
#include <wx/splitter.h>
#include <wx/frame.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

namespace ui
{
	///////////////////////////////////////////////////////////////////////////////
	/// Class DocumentFrameBase
	///////////////////////////////////////////////////////////////////////////////
	class DocumentFrameBase : public wxFrame 
	{
		private:
		
		protected:
			enum
			{
				ID_NEW = 1000,
				ID_OPEN,
				ID_SAVE,
				ID_SAVE_AS_,
				ID_EXIT,
				ID_ABOUT,
			};
			
			wxMenuBar* m_menu_document;
			wxMenu* file;
			wxMenu* edit;
			wxMenu* help;
			wxSplitterWindow* m_splitterDocumentFrame;
			wxScrolledWindow* m_leftWindow;
			wxTreeCtrl* m_treeCtrlModel;
			wxScrolledWindow* m_rightWindow;
			wxNotebook* m_notebookViews;
			
			// Virtual event handlers, overide them in your derived class
			virtual void OnAbout( wxCommandEvent& event ){ event.Skip(); }
			
		
		public:
			DocumentFrameBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
			~DocumentFrameBase();
			void m_splitterDocumentFrameOnIdle( wxIdleEvent& )
			{
			m_splitterDocumentFrame->SetSashPosition( 0 );
			m_splitterDocumentFrame->Disconnect( wxEVT_IDLE, wxIdleEventHandler( DocumentFrameBase::m_splitterDocumentFrameOnIdle ), NULL, this );
			}
			
		
	};
	
	///////////////////////////////////////////////////////////////////////////////
	/// Class MainFrame
	///////////////////////////////////////////////////////////////////////////////
	class MainFrame : public wxFrame 
	{
		private:
		
		protected:
			enum
			{
				ID_NEW = 1000,
				ID_OPEN,
				ID_SAVE,
				ID_SAVE_AS_,
				ID_EXIT,
				ID_ABOUT,
			};
			
			wxMenuBar* m_menu_document;
			wxMenu* file;
			wxMenu* edit;
			wxMenu* help;
			
			// Virtual event handlers, overide them in your derived class
			virtual void OnAbout( wxCommandEvent& event ){ event.Skip(); }
			
		
		public:
			MainFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
			~MainFrame();
		
	};
	
	///////////////////////////////////////////////////////////////////////////////
	/// Class AboutDialog
	///////////////////////////////////////////////////////////////////////////////
	class AboutDialog : public wxDialog 
	{
		private:
		
		protected:
		
		public:
			AboutDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE );
			~AboutDialog();
		
	};
	
} // namespace ui

#endif //__ui__

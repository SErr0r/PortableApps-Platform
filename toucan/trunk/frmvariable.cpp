/////////////////////////////////////////////////////////////////////////////
// Name:        frmvariable.cpp
// Purpose:     
// Author:      Steven Lamerton
// Modified by: 
// Created:     06/01/2007 15:57:03
// RCS-ID:      
// Copyright:   Copyright (C)  2006 Steven Lamerton
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (Personal Edition), 06/01/2007 15:57:03

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "frmvariable.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "toucan.h"
#include "frmvariable.h"

#include <wx/fileconf.h>
#include <wx/stdpaths.h>

////@begin XPM images
////@end XPM images

/*!
* frmVariable type definition
*/

IMPLEMENT_DYNAMIC_CLASS( frmVariable, wxDialog )

/*!
* frmVariable event table definition
*/

BEGIN_EVENT_TABLE( frmVariable, wxDialog )

////@begin frmVariable event table entries
    EVT_BUTTON( wxID_OK, frmVariable::OnOkClick )

    EVT_BUTTON( wxID_CANCEL, frmVariable::OnCancelClick )

////@end frmVariable event table entries

END_EVENT_TABLE()

/*!
* frmVariable constructors
*/

frmVariable::frmVariable( )
{}

frmVariable::frmVariable( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{	Create(parent, id, caption, pos, size, style);
}/*!
* frmVariable creator
*/

bool frmVariable::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{	////@begin frmVariable member initialisation
	m_List = NULL;
	////@end frmVariable member initialisation

	////\@begin frmVariable creation
	SetExtraStyle(GetExtraStyle()|wxWS_EX_BLOCK_EVENTS);
	wxDialog::Create( parent, id, caption, pos, size, style );

	CreateControls();
	SetIcon(GetIconResource(wxT("Toucan.ico")));
	if (GetSizer())
	{
		GetSizer()->SetSizeHints(this);
	}
	Centre();
	////\@end frmVariable creation
	return true;
}/*!
* Control creation for frmVariable
*/

void frmVariable::CreateControls()
{    
    if(wxFileExists(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("Data") + wxFILE_SEP_PATH + wxT("fr") + wxFILE_SEP_PATH + wxT("toucan.mo")))
	{
		//Load language if one exists from toucan.h
		wxGetApp().SelectLanguage(wxLANGUAGE_FRENCH);
	}
	////@begin frmVariable content construction
    // Generated by DialogBlocks, 25/05/2007 13:38:01 (Personal Edition)

    frmVariable* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxArrayString m_ListStrings;
    m_List = new wxComboBox( itemDialog1, ID_COMBOBOX1, _T(""), wxDefaultPosition, wxSize(150, -1), m_ListStrings, wxCB_DROPDOWN );
    itemBoxSizer3->Add(m_List, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxButton* itemButton6 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer5->Add(itemButton6, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton7 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer5->Add(itemButton7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	////@end frmVariable content construction
	wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""), wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxT("\\Data\\Variables.ini") );
	wxFileConfig::Set( config );
		wxString str;
	long dummy;
	bool bCont = config->GetFirstGroup(str, dummy);
	while ( bCont ) 
	{
		m_List->Append(str);
		bCont = config->GetNextGroup(str, dummy);
	}
	m_List->Append(wxT("drive"));
	m_List->Append(wxT("docs"));
}/*!
* Should we show tooltips?
*/

bool frmVariable::ShowToolTips()
{	return true;
}/*!
* Get bitmap resources
*/

wxBitmap frmVariable::GetBitmapResource( const wxString& name )
{	// Bitmap retrieval
	////@begin frmVariable bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
	////@end frmVariable bitmap retrieval
}/*!
* Get icon resources
*/

wxIcon frmVariable::GetIconResource( const wxString& name )
{	// Icon retrieval
	////\@begin frmVariable icon retrieval
	wxUnusedVar(name);
	if (name == _T("Toucan.ico"))
	{
		wxIcon icon(_T("App\\bitmaps\\Toucan.ico"), wxBITMAP_TYPE_ICO);
		return icon;
	}
	return wxNullIcon;
	////\@end frmVariable icon retrieval
}/*!
* wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
*/

void frmVariable::OnOkClick( wxCommandEvent& event )
{	wxGetApp().SetStrTemp(m_List->GetValue());
	EndModal(wxID_OK);

}

/*!
* wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL
*/

void frmVariable::OnCancelClick( wxCommandEvent& event )
{	EndModal(wxID_CANCEL);

}


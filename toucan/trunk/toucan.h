/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006-2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef _TOUCAN_H_
#define _TOUCAN_H_

#include <wx/wx.h>
#include <wx/process.h>
#include <wx/image.h>
#include <wx/intl.h>
#include <wx/fileconf.h>
#include <wx/html/helpctrl.h>

class frmMain;
class frmProgress;
class Settings;
class ScriptManager;

class Toucan: public wxApp
{    
	public:

	//Start and end
	virtual bool OnInit();
	virtual int OnExit();

	void SetLanguage(wxString langcode);
				
	bool ShouldAbort() { return blAbort; }
	void SetAbort(bool abort) { blAbort = abort; }
	
	wxString GetSettingsPath() { return strSettingsPath; }
	void SetSettingsPath(wxString settingspath) { strSettingsPath = settingspath; }
	
	void SetUsesGUI(bool gui) { blGUI = gui; }
	bool GetUsesGUI() { return blGUI; }
		
	//The two persistant forms
	frmMain* MainWindow;
	frmProgress* ProgressWindow;
	
	//Settings
	Settings* m_Settings;
	
	//Locale
	wxLocale* m_Locale;	
	
	//Script manager
	ScriptManager* m_Script;
	
	//Config
	wxFileConfig* m_Jobs_Config;
	wxFileConfig* m_Rules_Config;
	wxFileConfig* m_Scripts_Config;
	wxFileConfig* m_Variables_Config;
	
	//Help Controller
	wxHtmlHelpController* m_Help;
	
private:
	//Abort the current job
	bool blAbort;
	//The settings path
	wxString strSettingsPath;
	//Are we in GUI mode
	bool blGUI;
};

DECLARE_APP(Toucan)

#endif

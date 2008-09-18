/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007-2008 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include <wx/arrstr.h>
#include <wx/tokenzr.h>
#include <wx/fileconf.h>

#include "script.h"
#include "waitthread.h"
#include "syncdata.h"
#include "sync.h"
#include "backupdata.h"
#include "backupprocess.h"
#include "securedata.h"
#include "secure.h"
#include "variables.h"
#include "basicfunctions.h"
#include "rootdata.h"

void ScriptManager::SetCommand(int i){
	m_Command = i;
}

int ScriptManager::GetCommand(){
	return m_Command;
}

int ScriptManager::GetCount(){
	return m_Script.GetCount();
}

void ScriptManager::SetScript(wxArrayString script){
	m_Script = script;
}

wxArrayString ScriptManager::GetScript(){
	return m_Script;
}

bool ScriptManager::Validate(){
	//First check that the whole script is valid (basic number of parameters check)
	wxString strLine, strTemp;
	bool blParseError = false;
	bool blPassNeeded = false;
	for(unsigned int i = 0; i < m_Script.Count(); i++){
		strLine = m_Script.Item(i); 
		wxStringTokenizer tkz(strLine, wxT("\""), wxTOKEN_STRTOK);
		wxString strToken = tkz.GetNextToken();
		strToken.Trim();
		if(strToken == wxT("Sync") || strToken == wxT("Secure") || strToken == _("Delete") || strToken == _("Execute") || strToken == wxT("Backup")){
			if(tkz.CountTokens() != 1){
				strTemp.Printf(_("Line %d has an incorrect number of parameters"), i+1);
				OutputProgress(strTemp);
				blParseError = true;
			}
		}
		else if(strToken == _("Move") || strToken == _("Copy")){
			if(tkz.CountTokens() != 3){
				strTemp.Printf(_("Line %d has an incorrect number of parameters"), i+1);
				OutputProgress(strTemp);
				blParseError = true;
			}
		}
		else{
			strTemp.Printf(strToken + _(" not recognised on line %d"), i+1);
			OutputProgress(strTemp);
			blParseError = true;
		}
		if(strToken == wxT("Secure")){
			blPassNeeded = true;
		}
		if(strToken == wxT("Backup")){
			wxString strJob = tkz.GetNextToken();
			BackupData data;
			if(data.TransferFromFile()){
				if(data.IsPassword == true){
					blPassNeeded = true;
				}
			}
		}
	}
	if(blParseError){
		return false;
	}
	return true;
}

bool ScriptManager::ParseCommand(int i){	
	wxDateTime now = wxDateTime::Now();
	
	wxString strLine = m_Script.Item(i);
	
	wxStringTokenizer tkz(strLine, wxT("\""), wxTOKEN_STRTOK);
	wxString strToken = tkz.GetNextToken();
	
	strToken.Trim();
	
	RootData* data;
	
	if(strToken == wxT("Sync")){
		data = new SyncData();
		data->SetName(wxT("LastSyncJob"));
	}	
	else if(strToken == wxT("Backup")){
		data = new SyncData();
		data->SetName(wxT("LastBackupJob"));
	}
	
	if(data->NeedsPassword() == true){
		data->SetPassword(InputPassword());
	}	
	
	/*else if(strToken == wxT("Secure")){
		SecureData data;
		if(m_Password != wxEmptyString){
			data.SetPass(m_Password);						
		}
		//Ensure the data is filled
		wxString strJob = tkz.GetNextToken();
		if(!data.TransferFromFile(strJob)){
			CleanUp();
			return false;
		}
		Rules rules;
		wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""), wxGetApp().GetSettingsPath() + wxT("Jobs.ini"));
		if (config->Read(strJob + wxT("/Rules")) != wxEmptyString) {
			rules.TransferFromFile(config->Read(strJob + wxT("/Rules")));
		}
		delete config;
		for(unsigned int i = 0; i < data.GetLocations().GetCount(); i++){
			data.SetLocation(i, Normalise(data.GetLocation(i)));
			data.SetLocation(i, Normalise(data.GetLocation(i)));
		}
		//Call the secure function
		Secure(data, rules, m_ProgressWindow);
	}
	else if(strToken == _("Delete")){
		wxString strSource = tkz.GetNextToken();
		if(wxRemoveFile(strSource)){
			OutputProgress(_("Deleted ") +strSource + wxT("\n"));	
		}
		else{
			OutputProgress(_("Failed to delete ") +strSource + wxT("\n"));				
		}
	}
	else if(strToken == _("Move")){
		wxString strSource = tkz.GetNextToken();
		tkz.GetNextToken();
		wxString strDest = tkz.GetNextToken();
		if(wxCopyFile(strSource, strDest, true)){
			if(wxRemoveFile(strSource)){
				OutputProgress(_("Moved") +strSource + wxT("\n"));	
			}
			else{
				OutputProgress(_("Failed to move ") +strSource + wxT("\n"));
			}
		}
		else{
			OutputProgress(_("Failed to move ") +strSource + wxT("\n"));		
		}
	}
	else if(strToken == _("Copy")){
		wxString strSource = tkz.GetNextToken();
		tkz.GetNextToken();
		wxString strDest = tkz.GetNextToken();
		if(wxCopyFile(strSource, strDest, true)){
			OutputProgress(_("Copied ") +strSource + wxT("\n"));	
		}
		else{
			OutputProgress(_("Failed to copy ") +strSource + wxT("\n"));
		}
	}
	else if(strToken == _("Execute")){
		wxString strExecute = tkz.GetNextToken();
		wxExecute(strExecute, wxEXEC_SYNC|wxEXEC_NODISABLE);
		OutputProgress(_("Executed ") + strExecute + wxT("\n"));
	}*/
	
	
	Rules rules;
	wxFileConfig *config = new wxFileConfig( wxT(""), wxT(""), wxGetApp().GetSettingsPath() + wxT("Jobs.ini"));
	if (config->Read(data->GetName() + wxT("/Rules")) != wxEmptyString) {
		rules.TransferFromFile(config->Read(data->GetName() + wxT("/Rules")));
	}
	delete config;
	
	return true;	
}

bool ScriptManager::Execute(){
	StartUp();
	wxMessageBox(_("Started"));
	if(!Validate()){
		CleanUp();
	}
	wxMessageBox(_("Validated"));
	if(GetCount() != 0){
	wxMessageBox(_("Validated"));
		SetCommand(1);
		ParseCommand(0);
	}
	return true;
}

bool ScriptManager::CleanUp(){
	m_ProgressWindow->m_OK->Enable(true);
	m_ProgressWindow->m_Save->Enable(true);
	m_ProgressWindow->m_Cancel->Enable(false);
	wxDateTime now = wxDateTime::Now();
	OutputProgress(_("Time: ") + now.FormatISOTime() + wxT("\n"));
	OutputProgress(_("Finished"));
	return true;	
}

bool ScriptManager::StartUp(){
	//Set up all of the form related stuff
	m_ProgressWindow = wxGetApp().ProgressWindow;
	m_ProgressWindow->MakeModal();
	m_ProgressWindow->m_Text->Clear();
	//Send all errors to the text control
	wxLogTextCtrl* logTxt = new wxLogTextCtrl(m_ProgressWindow->m_Text);
	delete wxLog::SetActiveTarget(logTxt);
	//Set up the buttons on the progress box
	m_ProgressWindow->m_OK->Enable(false);
	m_ProgressWindow->m_Save->Enable(false);
	m_ProgressWindow->m_Cancel->Enable(true);	
	OutputProgress(_("Starting...\n"));
	wxDateTime now = wxDateTime::Now();
	OutputProgress(_("Time: ") + now.FormatISOTime() + wxT("\n"));
	//Show the window
	if(wxGetApp().blGUI){
		m_ProgressWindow->Refresh();
		m_ProgressWindow->Update();
		m_ProgressWindow->Show();
	}
	return true;
}

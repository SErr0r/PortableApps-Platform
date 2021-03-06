/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2008 - 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include "backupdata.h"
#include "../filecounter.h"
#include "../basicfunctions.h"
#include "../toucan.h"
#include "../variables.h"
#include "../backupprocess.h"
#include "../waitthread.h"
#include "../script.h"
#include <wx/fileconf.h>
#include <wx/stdpaths.h>
#include <wx/dir.h>


bool BackupData::TransferFromFile(){
	wxString strName = GetName();

	bool blError;
	int iTemp;
	wxString strTemp;
	bool blTemp;

	blError = wxGetApp().m_Jobs_Config->Read(strName + wxT("/BackupLocation"), &strTemp);
	if(blError){ SetBackupLocation(strTemp); }	
	blError = wxGetApp().m_Jobs_Config->Read(strName + wxT("/Locations"), &strTemp);
	if(blError){ SetLocations(StringToArrayString(strTemp, wxT("|"))); }	
	blError = wxGetApp().m_Jobs_Config->Read(strName + wxT("/Function"), &strTemp);
	if(blError){ SetFunction(strTemp); }
	blError = wxGetApp().m_Jobs_Config->Read(strName + wxT("/Format"), &strTemp);
	if(blError){ SetFormat(strTemp); }
	blError = wxGetApp().m_Jobs_Config->Read(strName + wxT("/Ratio"), &iTemp);
	if(blError){ SetRatio(iTemp); }
	blError = wxGetApp().m_Jobs_Config->Read(strName + wxT("/IsPass"), &blTemp);
	if(blError){ IsPassword = blTemp; }
	
	if(!blError){
		ErrorBox(_("There was an error reading from the jobs file, \nplease check it is not set as read only or in use."));
		return false;
	}
	return true;
}

bool BackupData::TransferToFile(){
	wxString strName = GetName();
	bool blError = false;

	//Delete any existing jobs with this name to ensure correct settings are retained
	wxGetApp().m_Jobs_Config->DeleteGroup(strName);
	wxGetApp().m_Jobs_Config->Flush();

	//Add the files to be written
	if(!wxGetApp().m_Jobs_Config->Write(strName + wxT("/BackupLocation"),  GetBackupLocation())){
		blError = true;
	}
	if(!wxGetApp().m_Jobs_Config->Write(strName + wxT("/Locations"),  ArrayStringToString(GetLocations(), wxT("|")))){
		blError = true;
	}		
	if(!wxGetApp().m_Jobs_Config->Write(strName + wxT("/Function"), GetFunction())){
		blError = true;
	}
	if(!wxGetApp().m_Jobs_Config->Write(strName + wxT("/Format"), GetFormat())){
		blError = true;
	}
	if(!wxGetApp().m_Jobs_Config->Write(strName + wxT("/Ratio"), GetRatio())){
		blError = true;
	}
	if(!wxGetApp().m_Jobs_Config->Write(strName + wxT("/IsPass"), IsPassword)){
		blError = true;
	}
	
	//Write the files
	wxGetApp().m_Jobs_Config->Flush();
	
	if(blError){
		ErrorBox(_("There was an error saving to the jobs file, \nplease check it is not set as read only or in use."));
		return false;
	}
	return true;
}

bool BackupData::TransferToForm(){
	frmMain *window = wxGetApp().MainWindow;
	if(window == NULL){
		return false;
	}
	window->m_Backup_Location->SetValue(GetBackupLocation());
	window->m_Backup_TreeCtrl->DeleteAllItems();
	window->m_Backup_TreeCtrl->AddRoot(wxT("Hidden root"));
	
	//Remove all of the items in the filepath list
	wxGetApp().MainWindow->m_BackupLocations->Clear();
	//Add the new locations to the treectrl and the list
	for(unsigned int j = 0; j < GetLocations().GetCount(); j++){
		wxGetApp().MainWindow->m_BackupLocations->Add(GetLocations().Item(j));
		window->m_Backup_TreeCtrl->AddNewPath(Normalise(Normalise(GetLocations().Item(j))));
	}
	//Set the rest of the window up
	window->m_Backup_Function->SetStringSelection(GetFunction());
	window->m_Backup_Format->SetStringSelection(GetFormat());
	window->m_Backup_Ratio->SetValue(GetRatio());
	window->m_Backup_IsPass->SetValue(IsPassword);
	return false;
}

/* This function sets all of the fields in SyncData based on the user inputted data in the
Main program window.*/
bool BackupData::TransferFromForm(){
	frmMain *window = wxGetApp().MainWindow;

	SetLocations(*wxGetApp().MainWindow->m_BackupLocations);
	SetBackupLocation(window->m_Backup_Location->GetValue()); 
	SetFunction(window->m_Backup_Function->GetStringSelection()); 
	SetFormat(window->m_Backup_Format->GetStringSelection()) ; 
	SetRatio(window->m_Backup_Ratio->GetValue());
	IsPassword = window->m_Backup_IsPass->GetValue();
	return true;	
}

/*This is a debugging tool only, not for use in release  versions of Toucan */
void BackupData::Output(){
	MessageBox(GetBackupLocation(), wxT("Backup Location"));
	for(unsigned int i = 0; i < GetLocations().GetCount(); i++){
		MessageBox(GetLocations().Item(i), wxT("Location"));
	}
	MessageBox(GetFunction(), wxT("Function"));
	MessageBox(GetFormat(), wxT("Format"));
}

wxString BackupData::CreateCommand(int i){
	
	wxString strCommand, strTempDir;
	//Setting up to use the first item in the array.
	wxString strSecond = GetLocations().Item(i);
	
	//strSolid is only used if 7zip is chosen to allow updating
	wxString strSolid = wxEmptyString;

	//Turn the inputted format into one 7zip will understand
	if (GetFormat() == wxT("Zip")){
		SetFormat(wxT("zip"));
	}
   	else if (GetFormat() == wxT("7-Zip")){
		SetFormat(wxT("7z"));
		//Make sure solid mode is turned off so updating works
		strSolid = wxT(" -ms=off");
	}
	//Ratio stuff, will need to be updated for new sliding scale
	wxString strRatio = wxT(" -mx5");
	if (GetRatio() == 0){
		strRatio = wxT(" -mx0");
	}
	else if (GetRatio() == 1){
		strRatio = wxT(" -mx1");
	}
	else if (GetRatio() == 2){
		strRatio = wxT(" -mx3");
	}
	else if (GetRatio() == 3){
		strRatio = wxT(" -mx5");
	}
	else if (GetRatio() == 4){
		strRatio = wxT(" -mx7");
	}
	else if (GetRatio() == 5){
		strRatio = wxT(" -mx9");
	}
	if(i == 0){
		//Checking to see if there is a password used
		if(GetPassword() != wxEmptyString){
			SetPassword(wxT(" -p") + GetPassword());
			//If 7zip is used then make sure headers are encrypted
			if(GetFormat() == wxT("7z")){
				SetPassword(GetPassword() + wxT(" -mhe=on"));
			}
		}
	}
   
	strTempDir = wxT(" -w\"") + wxPathOnly(GetBackupLocation()) + wxT("\"");

	if(GetFunction() == _("Complete")){
		strCommand = wxT("7za.exe a -t") + GetFormat() + GetPassword() + strRatio + strSolid +  wxT(" \"") + GetBackupLocation() + wxT("\"") +  wxT(" @\"") + wxGetApp().GetSettingsPath() + wxT("Includes.txt") + wxT("\" ") + strTempDir;	
	}
	else if(GetFunction() == _("Update")){
		strCommand = wxT("7za.exe u -t") + GetFormat() + GetPassword() + strRatio + strSolid + wxT(" \"") + GetBackupLocation() + wxT("\"") +  wxT(" @\"") + wxGetApp().GetSettingsPath() + wxT("Includes.txt") + wxT("\" ") + strTempDir; 
	}
	else if(GetFunction() == _("Restore")){
		strCommand =  wxT("7za.exe  x -aoa \"") + GetLocation(i) + wxT("\" -o\"") + GetBackupLocation() + wxT("\" * -r") + GetPassword();	
	}
	//With the Differential type the first use creates a file called base file. On subsequent runs a file is created with a filename based on both the date and time.    
	else if(GetFunction() == _("Differential")){
		if (GetBackupLocation()[GetBackupLocation().length()-1] != wxFILE_SEP_PATH) {
			SetBackupLocation(GetBackupLocation() + wxFILE_SEP_PATH);       
		}
		if(wxFileExists(GetBackupLocation() + wxFILE_SEP_PATH + wxT("BaseFile.") + GetFormat())){
			wxDateTime now = wxGetApp().m_Script->GetTime();
			strCommand = wxT("7za.exe u") + GetPassword() + strRatio + strSolid + wxT(" \"") + GetBackupLocation() + wxT("BaseFile.") + strFormat + wxT("\" -u- -up0q3x2z0!\"") + GetBackupLocation() + now.FormatISODate()+ wxT("-") + now.Format(wxT("%H")) + wxT("-") +  now.Format(wxT("%M")) + wxT(".") + strFormat + wxT("\"") +  wxT(" @\"") + wxGetApp().GetSettingsPath() + wxT("Includes.txt") + wxT("\" ") + strTempDir;
		}
		else{
			strCommand = wxT("7za.exe a -t") + GetFormat() + GetPassword() + strRatio + strSolid +  wxT(" \"") + GetBackupLocation() + wxT("BaseFile.") + strFormat + wxT(" \"") +  wxT(" @\"") + wxGetApp().GetSettingsPath() + wxT("Includes.txt") + wxT("\" ") + strTempDir;
		}
	}
	return strCommand;
}


bool BackupData::CreateList(wxTextFile *file, Rules rules, wxString strPath, int iRootLength){
	if(wxGetApp().ShouldAbort()){
		return true;
	}
	wxGetApp().Yield();
	if(wxDirExists(strPath)){
		//Clean up the path passed
		if (strPath[strPath.length()-1] != wxFILE_SEP_PATH) {
			strPath += wxFILE_SEP_PATH;       
		}
		wxDir dir(strPath);
		wxString strFilename;
		bool blDir = dir.GetFirst(&strFilename);
		//If the path is ok
		if(blDir){
			//Loop through all of the files and folders in the directory
			do {
				//If it is a directory
				if(wxDirExists(strPath + strFilename))
				{
					//If the direcotry doesnt have any subfiles then we can safely add it to the file list
					wxDir* dircheck = new wxDir(strPath + strFilename);
					if(!dircheck->HasFiles() && !dircheck->HasSubDirs()){
						if(!rules.ShouldExclude(strPath + strFilename, true)){
							wxString strCombined = strPath + strFilename;
							strCombined = strCombined.Right(strCombined.Length() - iRootLength);
							file->AddLine(strCombined);
						}
					}
					delete dircheck;
					//Always call the function again to ensure that ALL files and folders are processed
					CreateList(file, rules, strPath + strFilename, iRootLength);
				}
				//If it is a file
				else{
					if(!rules.ShouldExclude(strPath + strFilename, false)){
						wxString strCombined = strPath + strFilename;
						strCombined = strCombined.Right(strCombined.Length() - iRootLength);
						file->AddLine(strCombined);
					}
				}
			}
			while (dir.GetNext(&strFilename) );
		}  
	}
	//We have been passed a file
	else{
		if(!rules.ShouldExclude(strPath, false)){
			strPath = strPath.Right(strPath.Length() - iRootLength);
			file->AddLine(strPath);
		}		
	}
	return true;
}

bool BackupData::Execute(Rules rules){
	//Expand all of the variables
	for(unsigned int i = 0; i < GetLocations().Count(); i++){
		SetLocation(i, Normalise(Normalise(GetLocation(i))));
	}
	SetBackupLocation(Normalise(Normalise(GetBackupLocation())));
	for(unsigned int i = 0; i < GetLocations().Count(); i++){
		wxString path = GetLocation(i);
		bool isDir = false;
		//If we have a directory clean it up
		if(wxDirExists(path)){
			if (path[path.length()-1] != wxFILE_SEP_PATH) {
				path += wxFILE_SEP_PATH;       
			}
			isDir = true;
		}
		//If we are not running a restore job we need to create a list file
		if(GetFunction() != _("Restore")){
			//Set up the include file
			wxTextFile *file = new wxTextFile(wxGetApp().GetSettingsPath() + wxT("Includes.txt"));
			if(wxFileExists(wxGetApp().GetSettingsPath() + wxT("Includes.txt"))){
				file->Open();
				file->Clear();
				file->Write();
			}
			else{
				file->Create();
			}
			wxFileName filename(path);
			int length; 
			//If we have a directory then take of the last dir and take off one for the remaining slash
			if(isDir){
				filename.RemoveLastDir();
				length = filename.GetFullPath().Length();
				path = filename.GetFullPath();
			}
			//For files remove the filename and the remaining slash
			else{
				length = filename.GetPath().Length() - 1;
				path = filename.GetPath();
				if(path.Length() != 3){
					length += 2;
				}
			}
			OutputProgress(_("Creating file list, this may take some time."));
			if(!CreateList(file, rules, GetLocations().Item(i), length)){
				return false;
			}
			file->Write();
			//Set up the progress bar
			EnableGauge(true);
		}
		else{
			EnableGauge(false);
		}
		wxString strCommand = CreateCommand(i);
		wxSetWorkingDirectory(path);
		PipedProcess *process = new PipedProcess(wxGetApp().ProgressWindow);
		long lgPID = wxExecute(strCommand, wxEXEC_ASYNC|wxEXEC_NODISABLE, process);
		process->SetRealPid(lgPID);
		WaitThread *thread = new WaitThread(lgPID, process);
		thread->Create();
		thread->Run();	
		thread->Wait();
		while(process->HasInput())
			;
	}
	wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, ID_SCRIPTFINISH);
	wxPostEvent(wxGetApp().ProgressWindow, event);	
	return true;
}

bool BackupData::NeededFieldsFilled(){
	bool blFilled = true;
	if(arrLocations.Count() == 0){
		blFilled = false;
	}
	if(GetBackupLocation() == wxEmptyString){
		blFilled = false;
	}
	if(GetFunction() == wxEmptyString){
		blFilled = false;
	}
	if(GetFormat() == wxEmptyString){
		blFilled = false;
	}
	return blFilled;
}

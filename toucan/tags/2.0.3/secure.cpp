/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006-2007 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include <wx/file.h>
#include <wx/filefn.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>

#include "secure.h"
#include "toucan.h"
#include "basicfunctions.h"

bool Secure(SecureData data, Rules rules, frmProgress *window){
	wxArrayString arrLocation = data.GetLocations();
	//Iterate through the entries in the array
	for(unsigned int i = 0; i < arrLocation.Count(); i++)
	{
		if(wxGetApp().ShouldAbort()){
			return true;
		}
		//Need to add normalisation to SecureData
		if(arrLocation.Item(i) != wxEmptyString){
			if(wxDirExists(arrLocation.Item(i))){
				CryptDir(arrLocation.Item(i), data, rules, window);
			}
			else if(wxFileExists(arrLocation.Item(i))){
				CryptFile(arrLocation.Item(i), data, rules, window);
			}
		}
	}
	if(wxGetApp().blGUI){
		wxGetApp().MainWindow->m_Secure_TreeCtrl->DeleteAllItems();
		wxGetApp().MainWindow->m_Secure_TreeCtrl->AddRoot(wxT("HiddenRoot"));
		for(unsigned int i = 0; i < wxGetApp().MainWindow->m_SecureLocations->GetCount(); i++){
			wxGetApp().MainWindow->m_Secure_TreeCtrl->AddNewPath(wxGetApp().MainWindow->m_SecureLocations->Item(i));
		}
	}
	wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, ID_SCRIPTFINISH);
	wxPostEvent(wxGetApp().MainWindow, event);	
	return true;
}


/*The main loop for the Secure process. It is called by Secure initially and then either calls itself when it reaches a
folder or CryptFile when it reaches a file.*/
bool CryptDir(wxString strPath, SecureData data, Rules rules, frmProgress* window)
{   
	if(wxGetApp().ShouldAbort()){
		return true;
	}
	wxDir dir(strPath);
	wxString filename;
	bool blDir = dir.GetFirst(&filename);
	if (blDir)
	{
		do {
			if(wxGetApp().ShouldAbort()){
				return true;
			}
			if (wxDirExists(strPath + wxFILE_SEP_PATH + filename) ){
				CryptDir(strPath + wxFILE_SEP_PATH + filename, data, rules, window);
			}
			else{
				CryptFile(strPath + wxFILE_SEP_PATH + filename, data, rules, window);
			}
		}
		while (dir.GetNext(&filename) );
	}   
	return true;
}


bool CryptFile(wxString strFile, SecureData data, Rules rules, frmProgress* window)
{
	if(wxGetApp().ShouldAbort()){
		return true;
	}
	//Check to see it the file should be excluded	
	if(rules.ShouldExclude(strFile, false)){
		return true;
	}
	//Make sure that it is a 'real' file
	wxFileName filename(strFile);
	if(filename.IsOk() == true){
		wxString size = filename.GetHumanReadableSize();
		if(size == wxT("Not available")){
			return false;
		}
	}

	//Ensure that we are not encrypting an already encrypted file or decrypting a non encrypted file
	if(filename.GetExt() == wxT("cpt") && data.GetFunction() == _("Encrypt")){
		return false;
	}
	if(filename.GetExt() != wxT("cpt") && data.GetFunction() == _("Decrypt")){
		return false;
	}	
	
	//A couple of arrays to catch the output and surpress the command window
	wxArrayString arrErrors;
	wxArrayString arrOutput;

	if(data.GetFunction() == _("Encrypt")){
		//Set the file to have normal attributes so it can be encrypted
		#ifdef __WXMSW__   
			int filearrtibs = GetFileAttributes(strFile);
			SetFileAttributes(strFile,FILE_ATTRIBUTE_NORMAL);  
		#endif

		//Create and execute the command
		wxString command = wxT("ccrypt -e -K\"") + data.GetPassword() + wxT("\" \"") + strFile + wxT("\"");
		long lgReturn = wxExecute(command, arrErrors, arrOutput, wxEXEC_SYNC|wxEXEC_NODISABLE);
		
		//Put the old attributed back
		#ifdef __WXMSW__
			SetFileAttributes(strFile,filearrtibs);
		#endif
		
		if(lgReturn == 0){        
			OutputProgress(_("Encrypted ") + strFile);
		}
		else{
			OutputProgress(_("Failed to encrypt ") + strFile);
		}
	}

	else if(data.GetFunction() == _("Decrypt")){
		//Set the file to have normal attributes so it can be decrypted
		#ifdef __WXMSW__
			int filearrtibs = GetFileAttributes(strFile);
			SetFileAttributes(strFile,FILE_ATTRIBUTE_NORMAL); 
		#endif

		//Create and execute the command
		wxString command = wxT("ccrypt -d -K\"") + data.GetPassword() + wxT("\" \"") + strFile + wxT("\" ");
		long lgReturn = wxExecute(command, arrErrors, arrOutput, wxEXEC_SYNC|wxEXEC_NODISABLE);
		

		//Put the old attributed back
		#ifdef __WXMSW__
			SetFileAttributes(strFile,filearrtibs);
		#endif

		if(lgReturn == 0){       
 			OutputProgress(_("Decrypted ") + strFile);
		}
		else{
 			OutputProgress(_("Failed to decrypt ") + strFile);
		}
	}
	return true;
}

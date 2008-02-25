/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2007 Steven Lamerton
// Licence:     GNU GPL 2 (See readme for more info
/////////////////////////////////////////////////////////////////////////////////

#include <wx/filefn.h>
#include <wx/dir.h>
#include <wx/filename.h>

#ifndef _BACKUPFUNC_
#define _BACKUPFUNC_

/*GenerateExclusions creates the necessary text file for the use of exclusions in 7z*/
bool GenerateExclusions(wxString strFrom, wxArrayString arrExclusions, bool blBox)
{

    if (strFrom[strFrom.length()-1] != wxFILE_SEP_PATH) {
            strFrom += wxFILE_SEP_PATH;       
    }

    wxDir dir(strFrom);
    wxString strFilename;
    bool bla = dir.GetFirst(&strFilename);
   
    if (bla){
        do {
            if (wxDirExists(strFrom + strFilename))
            {
                unsigned int i;
                bool blEqual = false;
                for(i = 0; i <arrExclusions.GetCount(); i++)
                {
                    if(strFilename == arrExclusions.Item(i)){
                        blEqual = true;
                    }
                }
                if(blEqual == false){
                    GenerateExclusions(strFrom + strFilename, arrExclusions, blBox);
                }
                else{
                    wxString strPath = wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("temp-exclusions.txt");
                    wxTextFile file;
                    file.Open(strPath);
                    file.AddLine(strFrom + strFilename);
                    file.Write();
					file.Close();
                }
            }
            else{
                wxFileName flName(strFrom + strFilename);
                unsigned int i;
                bool blEqual = false;
                for(i = 0; i <arrExclusions.GetCount(); i++)
                {
                    if(wxT(".") + flName.GetExt() == arrExclusions.Item(i)){
                        blEqual = true;
                    }
                }
                if(blEqual == true)
                {
                    wxString strPath = wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH +  wxT("temp-exclusions.txt");
                    wxTextFile file;
                    file.Open(strPath);
                    file.AddLine(strFrom + strFilename);
                    file.Write();
					file.Close();
                }
            }
        }
        while (dir.GetNext(&strFilename) );
    }  
    return true;
   
}

/*CutStart chops the beginning of the GenerateExclusions file to make
them 7z compatible. Could do with both being integrated into one function*/
bool CutStart(wxString strBasePath, bool blBox)
{

	wxString strPath = wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("temp-exclusions.txt");
    wxTextFile file;
    file.Open(strPath);
    unsigned int i;
    for(i = 0; i < file.GetLineCount(); i++)
    {
		if (strBasePath[strBasePath.length()-1] != wxFILE_SEP_PATH) {
			strBasePath += wxFILE_SEP_PATH;       
		}
		//wxMessageBox(strBasePath);
        wxString strLine = file.GetLine(i);
		//wxMessageBox(strLine);
        wxString strTemp = strLine.Right(strLine.Length() - strBasePath.Length());
		//wxMessageBox(strTemp);
		//wxMessageBox(strTemp);
        file.RemoveLine(i);
        file.InsertLine(strTemp, i);
        file.Write();
    }
    return true;
}

/*Creates the call to 7z, called by Backup*/
wxString CreateCommand(wxString strFirst, wxString strSecond, wxString strType, wxString strFormat, wxString strRatio, wxString strPass)
{
	wxString strSolid = wxEmptyString;
	//Prepare the 7zip command to be excuted
	if (strFormat == wxT("Zip")){
        strFormat = wxT("zip");
    }
    else if (strFormat == wxT("7 Zip")){
        strFormat = wxT("7z");
		strSolid = wxT(" -ms=off");
    }
    
    if (strRatio == _("Normal")){
        strRatio = wxT("5");
    }
    else if (strRatio == _("Maximum")){
        strRatio = wxT("9");
    }
	if(strPass != wxEmptyString){
		strPass = wxT(" -p") + strPass;
        if(strFormat = wxT("7z")){
            strPass += wxT(" -mhe=on");
        }
	}
   
	wxString command;
    if(strType == _("Complete")){
        command = wxT("7za.exe a -t") + strFormat + strPass + wxT(" -mx") + strRatio + wxT(" \"") + strSecond + wxT("\" \"") + strFirst + wxT("\\*\"") + wxT(" -x@\"") + wxPathOnly(wxStandardPaths::Get().GetExecutablePath())+ wxFILE_SEP_PATH + wxT("temp-exclusions.txt") + wxT("\"") + strSolid;    
        //wxMessageBox(command);
    }
	else if(strType == _("Update")){
        command = wxT("7za.exe  u -t") + strFormat + wxT(" \"") + strSecond + wxT("\" \"") + strFirst + wxT("\\*\"")  + wxT(" -x@\"") + wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) +  + wxFILE_SEP_PATH + wxT("temp-exclusions.txt") + wxT("\"")  + strPass+ strSolid;
	}
	else if(strType == _("Restore")){
        command = wxT("7za.exe  x -aoa \"") + strFirst + wxT("\" -o\"") + strSecond + wxT("\" * -r")  + wxT(" -x@\"") + wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH  + wxT("temp-exclusions.txt") + wxT("\"") + strPass + strSolid;
	}
	//With the incremental type the first use creates a file called base file. On subsequent runs a file is created with a filename based on both the date and time.    
	else if(strType == _("Incremental")){
        if(wxFileExists(strSecond + wxFILE_SEP_PATH + wxT("BaseFile.") + strFormat)){
            wxDateTime now = wxDateTime::Now();
            if (strFirst[strFirst.length()-1] != wxFILE_SEP_PATH) {
                strFirst += wxFILE_SEP_PATH;       
            }
             command = wxT("7za.exe u \"") + strSecond + wxFILE_SEP_PATH + wxT("BaseFile.") + strFormat + wxT("\" -u- -up0q3x2z0!\"") + strSecond + wxFILE_SEP_PATH + now.FormatISODate()+ wxT("-") + now.Format(wxT("%H")) + wxT("-") +  now.Format(wxT("%M")) + wxT(".") + strFormat + wxT("\" \"") + strFirst + wxT("*\"") + wxT(" -x@\"") + wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("temp-exclusions.txt") + wxT("\"") + strSolid + strPass + wxT(" -mx") + strRatio;
        }
        else{
        		 command = wxT("7za.exe a -t") + strFormat + wxT(" -mx") + strRatio + wxT(" \"") + strSecond + wxFILE_SEP_PATH + wxT("BaseFile.") + strFormat + wxT("\" \"") + strFirst + wxT("\\*\"")  + wxT(" -x@\"") + wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH  + wxT("temp-exclusions.txt") + wxT("\"") + strSolid + strPass  + wxT(" -mx") + strRatio;    

        }
    }
    //wxMessageBox(command);

    return command;
}

#endif



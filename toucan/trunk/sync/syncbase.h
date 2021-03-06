/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#ifndef H_SYNCBASE
#define H_SYNCBASE

#include <map>
#include <list>
#include <wx/string.h>
#include "syncdata.h"
#include "../rules.h"

class SyncBase{
public:
	SyncBase();
	virtual ~SyncBase();
protected:
	//Store the root paths we have so we can pass them onto the next level of folders if needed
	wxString sourceroot;
	wxString destroot;
	//We need to know what sort of operation we are doing
	SyncData* data;
	Rules rules;
	//Adds all the contents of a folder to a std::list
	std::list<wxString> FolderContentsToList(wxString path);
	//Merges two lists into a map(wxString, location)
	//Definition of short
	//1 = source only
	//2 = dest only
	//3 = source and dest
	std::map<wxString, short> MergeListsToMap(std::list<wxString> sourcelist, std::list<wxString> destlist);
	//Iterates through the map and calles the appropriate copying operations
	bool OperationCaller(std::map<wxString, short> paths);
	bool preview;
private:
	//The functions that do the copying
	virtual bool OnSourceNotDestFile(wxString path) = 0;
	virtual bool OnNotSourceDestFile(wxString path) = 0;
	virtual bool OnSourceAndDestFile(wxString path) = 0;
	virtual bool OnSourceNotDestFolder(wxString path) = 0;
	virtual bool OnNotSourceDestFolder(wxString path) = 0;
	virtual bool OnSourceAndDestFolder(wxString path) = 0;
};

#endif

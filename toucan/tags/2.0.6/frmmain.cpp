/////////////////////////////////////////////////////////////////////////////////
// Author:      Steven Lamerton
// Copyright:   Copyright (C) 2006-2009 Steven Lamerton
// License:     GNU GPL 2 (See readme for more info)
/////////////////////////////////////////////////////////////////////////////////

#include <wx/stdpaths.h>
#include <wx/aboutdlg.h>
#include <wx/fileconf.h>
#include <wx/textctrl.h>
#include <wx/aui/auibook.h>
#include <wx/html/helpctrl.h>
#include <wx/wx.h>

#include "toucan.h"
#include "virtualdirtreectrl.h"
#include "dragndrop.h"

#include "frmmain.h"
#include "frmprogress.h"
#include "frmrestore.h"
#include "frmvariable.h"

#include "script.h"
#include "variables.h"
#include "securedata.h"
#include "syncdata.h"
#include "backupdata.h"
#include "basicfunctions.h"
#include "settings.h"

#include "extendeddirctrl.h"

//frmMain event table
BEGIN_EVENT_TABLE(frmMain, wxFrame)

	//Sync
	EVT_BUTTON(ID_SYNC_OK, frmMain::OnSyncOKClick)
	EVT_BUTTON(ID_SYNC_PREVIEW, frmMain::OnSyncPreviewClick)	
	EVT_BUTTON(ID_SYNC_SOURCE_EXPAND, frmMain::OnSyncSourceExpandClick)
	EVT_BUTTON(ID_SYNC_DEST_EXPAND, frmMain::OnSyncDestExpandClick)
	EVT_BUTTON(ID_SYNC_JOB_ADD, frmMain::OnSyncJobAddClick)
	EVT_BUTTON(ID_SYNC_JOB_REMOVE, frmMain::OnSyncJobRemoveClick)
	EVT_BUTTON(ID_SYNC_JOB_SAVE, frmMain::OnSyncJobSaveClick)
	EVT_COMBOBOX(ID_SYNC_JOB_SELECT, frmMain::OnSyncJobSelectSelected)
	EVT_BUTTON(ID_SYNC_SOURCE_BTN, frmMain::OnSyncSourceBtnClick)
	EVT_BUTTON(ID_SYNC_DEST_BTN, frmMain::OnSyncDestBtnClick)
	EVT_TREE_ITEM_RIGHT_CLICK(ID_SYNC_SOURCE_TREE, frmMain::OnSyncSourceTreeRightClick)
	EVT_TREE_ITEM_RIGHT_CLICK(ID_SYNC_DEST_TREE, frmMain::OnSyncDestTreeRightClick)
	
	//Backup
	EVT_BUTTON(ID_BACKUP_OK, frmMain::OnBackupOKClick)
	EVT_BUTTON(ID_BACKUP_RESTORE, frmMain::OnBackupRestoreClick)
	EVT_BUTTON(ID_BACKUP_PREVIEW, frmMain::OnBackupPreviewClick)
	EVT_BUTTON(ID_BACKUP_LOCATION, frmMain::OnBackupLocationClick)
	EVT_BUTTON(ID_BACKUP_ADD, frmMain::OnBackupAddClick)
	EVT_BUTTON(ID_BACKUP_REMOVE, frmMain::OnBackupRemoveClick)
	EVT_BUTTON(ID_BACKUP_EXPAND, frmMain::OnBackupExpandClick)	
	EVT_BUTTON(ID_BACKUP_JOB_SAVE, frmMain::OnBackupJobSaveClick)
	EVT_BUTTON(ID_BACKUP_JOB_ADD, frmMain::OnBackupJobAddClick)
	EVT_BUTTON(ID_BACKUP_JOB_REMOVE, frmMain::OnBackupJobRemoveClick)
	EVT_BUTTON(ID_BACKUP_ADDVAR, frmMain::OnBackupAddVarClick)
	EVT_COMBOBOX(ID_BACKUP_JOB_SELECT, frmMain::OnBackupJobSelectSelected)
	EVT_COMMAND_SCROLL_CHANGED(ID_BACKUP_RATIO, frmMain::OnBackupRatioChanged)
	EVT_TREE_ITEM_GETTOOLTIP(ID_BACKUP_TREECTRL, frmMain::OnBackupTreeCtrlTooltip)
	EVT_TREE_ITEM_RIGHT_CLICK(ID_BACKUP_TREECTRL, frmMain::OnBackupTreeRightClick)

	//Secure
	EVT_BUTTON(ID_SECURE_OK, frmMain::OnSecureOKClick)
	EVT_BUTTON(ID_SECURE_PREVIEW, frmMain::OnSecurePreviewClick)
	EVT_COMBOBOX(ID_SECURE_JOB_SELECT, frmMain::OnSecureJobSelectSelected)
	EVT_BUTTON(ID_SECURE_JOB_SAVE, frmMain::OnSecureJobSaveClick)
	EVT_BUTTON(ID_SECURE_JOB_ADD, frmMain::OnSecureJobAddClick)
	EVT_BUTTON(ID_SECURE_JOB_REMOVE, frmMain::OnSecureJobRemoveClick)
	EVT_BUTTON(ID_SECURE_ADD, frmMain::OnSecureAddClick)
	EVT_BUTTON(ID_SECURE_REMOVE, frmMain::OnSecureRemoveClick)
	EVT_BUTTON(ID_SECURE_EXPAND, frmMain::OnSecureExpandClick)	
	EVT_BUTTON(ID_SECURE_ADDVAR, frmMain::OnSecureAddVarClick)	
	EVT_TREE_ITEM_GETTOOLTIP(ID_SECURE_TREECTRL, frmMain::OnSecureTreeCtrlTooltip)
	EVT_TREE_ITEM_RIGHT_CLICK(ID_SECURE_TREECTRL, frmMain::OnSecureTreeRightClick)
	
	//Rules
	EVT_COMBOBOX(ID_RULES_COMBO, frmMain::OnRulesComboSelected)
	EVT_BUTTON(ID_RULES_SAVE, frmMain::OnRulesSaveClick)
	EVT_BUTTON(ID_RULES_ADD, frmMain::OnRulesAddClick)
	EVT_BUTTON(ID_RULES_REMOVE, frmMain::OnRulesRemoveClick)
	EVT_BUTTON(ID_RULES_ADD_FILEEXCLUDE, frmMain::OnRulesAddFileexcludeClick)
	EVT_BUTTON(ID_RULES_REMOVE_FILEEXCLUDE, frmMain::OnRulesRemoveFileexcludeClick)
	EVT_BUTTON(ID_RULES_ADD_FOLDEREXCLUDE, frmMain::OnRulesAddFolderexcludeClick)
	EVT_BUTTON(ID_RULES_REMOVE_FOLDEREXCLUDE, frmMain::OnRulesRemoveFolderexcludeClick)
	EVT_BUTTON(ID_RULES_ADD_LOCATIONINCLUDE, frmMain::OnRulesAddLocationincludeClick)
	EVT_BUTTON(ID_RULES_REMOVE_LOCATIONINCLUDE, frmMain::OnRulesRemoveLocationincludeClick)
	
	//Portable Variables
	EVT_BUTTON(ID_VARIABLES_ADD, frmMain::OnVariablesAddClick)
	EVT_BUTTON(ID_VARIABLES_REMOVE, frmMain::OnVariablesRemoveClick )
	EVT_BUTTON(ID_VARIABLES_ADDITEM, frmMain::OnVariablesAddItemClick)
	EVT_BUTTON(ID_VARIABLES_REMOVEITEM, frmMain::OnVariablesRemoveItemClick)
	EVT_COMBOBOX(ID_VARIABLES_NAME, frmMain::OnVariablesNameSelected)
	EVT_LIST_ITEM_ACTIVATED(ID_VARIABLES_LIST, frmMain::OnVariablesListActivated)
	
	//Script
	EVT_BUTTON(ID_SCRIPT_EXECUTE, frmMain::OnScriptExecute)
	EVT_BUTTON(ID_SCRIPT_SAVE, frmMain::OnScriptSaveClick)
	EVT_BUTTON(ID_SCRIPT_ADD, frmMain::OnScriptAddClick)
	EVT_BUTTON (ID_SCRIPT_REMOVE, frmMain::OnScriptRemoveClick)
	EVT_COMBOBOX(ID_SCRIPT_NAME, frmMain::OnScriptSelected)
	
	//Other
	EVT_CLOSE(frmMain::OnCloseWindow)
	EVT_BUTTON(wxID_ABOUT, frmMain::OnAboutClick)
	EVT_AUINOTEBOOK_PAGE_CHANGED(ID_AUINOTEBOOK, frmMain::OnTabChanged)
	
	//Menu
	EVT_MENU(ID_MENU_FILEEXCLUDE_EXTENSION, frmMain::OnMenuFileExcludeExtensionClick)
	EVT_MENU(ID_MENU_FILEEXCLUDE_NAME, frmMain::OnMenuFileExcludeNameClick)
	EVT_MENU(ID_MENU_LOCATIONINCLUDE_EXTENSION, frmMain::OnMenuLocationIncludeExtensionClick)
	EVT_MENU(ID_MENU_LOCATIONINCLUDE_NAME, frmMain::OnMenuLocationIncludeNameClick)
	EVT_MENU(ID_MENU_FOLDEREXCLUDE_NAME, frmMain::OnMenuFolderExcludeNameClick)

	//ScriptManager
	EVT_BUTTON(ID_SCRIPTFINISH, frmMain::OnScriptFinish)
	
END_EVENT_TABLE()

//Constructor
frmMain::frmMain(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style){
	Init();
	wxFrame::Create(parent, id, caption, pos, size, style);
	CreateControls();
}

//Destructor
frmMain::~frmMain(){
	m_auiManager.UnInit();
}

//frmMain initialisation
void frmMain::Init(){
	m_Notebook = NULL;
	m_Sync_Job_Select = NULL;
	m_Sync_Rules = NULL;
	m_Sync_Source_Txt = NULL;
	m_Sync_Source_Tree = NULL;
	m_Sync_Dest_Txt = NULL;
	m_Sync_Dest_Tree = NULL;
	m_Sync_Function = NULL;
	m_Sync_Timestamp = NULL;
	m_Sync_Attributes = NULL;
	m_Sync_Ignore_Readonly = NULL;
	m_Sync_Ignore_DaylightS = NULL;
	m_Backup_Job_Select = NULL;
	m_Backup_Rules = NULL;
	m_Backup_Location = NULL;
	m_Backup_DirCtrl = NULL;
	m_Backup_TreeCtrl = NULL;
	m_Backup_Function = NULL;
	m_Backup_Format = NULL;
	m_Backup_Ratio = NULL;
	m_Backup_Pass = NULL;
	m_Backup_Repass = NULL;
	m_Secure_Rules = NULL;
	m_Secure_Job_Select = NULL;
	m_Secure_DirCtrl = NULL;
	m_Secure_TreeCtrl = NULL;
	m_Secure_Function = NULL;
	m_Secure_Pass = NULL;
	m_Secure_Repass = NULL;
	m_Rules_Name = NULL;
	m_Rules_FileExclude = NULL;
	m_Rules_FolderExclude = NULL;
	m_Rules_LocationInclude = NULL;
	m_Rules_FileDelete = NULL;
	m_Script_Rich = NULL;
	
	menuTree = NULL;
	menuRules = NULL;
	
	m_BackupLocations = new wxArrayString();
	m_SecureLocations = new wxArrayString();
}

//Create controls
void frmMain::CreateControls()
{
	//Create a pointer so that we have something to add the items to	
	frmMain* itemFrame1 = this;
	
	//Set up the correct type of border
	int BORDER = wxWindow::GetThemedBorderStyle();
	
	//Set our min size
	itemFrame1->SetMinSize(wxSize(780, 450));
	
	//Set the font from the settings
	wxFont font;
	font.SetNativeFontInfo(wxGetApp().m_Settings->GetFont());
	itemFrame1->SetFont(font);

	//Set the form up as managed by AUI
	m_auiManager.SetManagedWindow(this);

	//Create the root AUI notebook
	m_Notebook = new wxAuiNotebook( itemFrame1, ID_AUINOTEBOOK, wxDefaultPosition, wxDefaultSize, wxAUI_NB_SCROLL_BUTTONS|wxNO_BORDER );

	//The sync panel
	wxPanel* itemPanel6 = new wxPanel( m_Notebook, ID_PANEL_SYNC, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );
	wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxVERTICAL);
	itemPanel6->SetSizer(itemBoxSizer7);

	wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer7->Add(itemBoxSizer8, 1, wxGROW|wxALL, 0);
	wxBoxSizer* itemBoxSizer9 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer8->Add(itemBoxSizer9, 0, wxGROW|wxALL, 0);

	//Top sizer
	
	wxBoxSizer* itemBoxSizer28 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer8->Add(itemBoxSizer28, 0, wxGROW|wxALL, 5);
	
	wxBoxSizer* itemBoxSizer001 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer28->Add(itemBoxSizer001, 0, wxGROW|wxALL, 0);	

	//Job section
	wxStaticBox* itemStaticBoxSizer10Static = new wxStaticBox(itemPanel6, wxID_ANY, _("Job Name"));
	wxStaticBoxSizer* itemStaticBoxSizer10 = new wxStaticBoxSizer(itemStaticBoxSizer10Static, wxHORIZONTAL);
	itemBoxSizer001->Add(itemStaticBoxSizer10, 0, wxALIGN_TOP|wxALL, 5);
	wxArrayString m_Sync_Job_SelectStrings;
	m_Sync_Job_Select = new wxComboBox( itemPanel6, ID_SYNC_JOB_SELECT, _T(""), wxDefaultPosition, wxDefaultSize, m_Sync_Job_SelectStrings, wxCB_DROPDOWN|wxCB_READONLY);
	itemStaticBoxSizer10->Add(m_Sync_Job_Select, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBitmapButton* itemBitmapButton12 = new wxBitmapButton( itemPanel6, ID_SYNC_JOB_SAVE, itemFrame1->GetBitmapResource(wxT("save.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	itemStaticBoxSizer10->Add(itemBitmapButton12, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBitmapButton* itemBitmapButton13 = new wxBitmapButton( itemPanel6, ID_SYNC_JOB_ADD, itemFrame1->GetBitmapResource(wxT("add.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	itemStaticBoxSizer10->Add(itemBitmapButton13, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBitmapButton* itemBitmapButton14 = new wxBitmapButton( itemPanel6, ID_SYNC_JOB_REMOVE, itemFrame1->GetBitmapResource(wxT("remove.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	itemStaticBoxSizer10->Add(itemBitmapButton14, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	//Rules section
	wxStaticBox* itemStaticBoxSizer15Static = new wxStaticBox(itemPanel6, wxID_ANY, _("Rules"));
	wxStaticBoxSizer* itemStaticBoxSizer15 = new wxStaticBoxSizer(itemStaticBoxSizer15Static, wxHORIZONTAL);
	itemBoxSizer001->Add(itemStaticBoxSizer15, 0, wxALIGN_TOP|wxALL|wxGROW, 5);
	wxArrayString m_Sync_RulesStrings;
	m_Sync_Rules = new wxComboBox( itemPanel6, ID_SYNC_RULES, _T(""), wxDefaultPosition, wxDefaultSize, m_Sync_RulesStrings, wxCB_DROPDOWN|wxCB_READONLY);
	itemStaticBoxSizer15->Add(m_Sync_Rules, 1, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5);

	//Options section
	wxArrayString m_Sync_FunctionStrings;
	m_Sync_FunctionStrings.Add(_("Copy"));
	m_Sync_FunctionStrings.Add(_("Update"));
	m_Sync_FunctionStrings.Add(_("Mirror (Copy)"));
	m_Sync_FunctionStrings.Add(_("Mirror (Update)"));
	m_Sync_FunctionStrings.Add(_("Equalise"));
	m_Sync_Function = new wxRadioBox( itemPanel6, ID_SYNC_FUNCTION, _("Function"), wxDefaultPosition, wxDefaultSize, m_Sync_FunctionStrings, 1, wxRA_SPECIFY_COLS );
	m_Sync_Function->SetSelection(0);
	itemBoxSizer28->Add(m_Sync_Function, 0, wxALIGN_TOP|wxALL, 5);

	wxStaticBox* itemStaticBoxSizer30Static = new wxStaticBox(itemPanel6, wxID_ANY, _("Other"));
	wxStaticBoxSizer* itemStaticBoxSizer30 = new wxStaticBoxSizer(itemStaticBoxSizer30Static, wxVERTICAL);
	itemBoxSizer28->Add(itemStaticBoxSizer30, 0, wxALIGN_TOP|wxALL, 5);
	m_Sync_Timestamp = new wxCheckBox( itemPanel6, ID_SYNC_TIMESTAMP, _("Retain Timestamps"), wxDefaultPosition, wxDefaultSize, 0 );
	m_Sync_Timestamp->SetValue(false);
	itemStaticBoxSizer30->Add(m_Sync_Timestamp, 0, wxALIGN_LEFT|wxALL, 5);

	m_Sync_Attributes = new wxCheckBox( itemPanel6, ID_SYNC_ATTRIB, _("Retain Attributes"), wxDefaultPosition, wxDefaultSize, 0 );
	m_Sync_Attributes->SetValue(false);
	itemStaticBoxSizer30->Add(m_Sync_Attributes, 0, wxALIGN_LEFT|wxALL, 5);

	m_Sync_Ignore_Readonly = new wxCheckBox( itemPanel6, ID_SYNC_IGNORERO, _("Ignore Read-Only"), wxDefaultPosition, wxDefaultSize, 0 );
	m_Sync_Ignore_Readonly->SetValue(false);
	itemStaticBoxSizer30->Add(m_Sync_Ignore_Readonly, 0, wxALIGN_LEFT|wxALL, 5);

	m_Sync_Ignore_DaylightS = new wxCheckBox( itemPanel6, ID_SYNC_IGNOREDS, _("Ignore Daylight Savings"), wxDefaultPosition, wxDefaultSize, 0 );
	m_Sync_Ignore_DaylightS->SetValue(false);
	itemStaticBoxSizer30->Add(m_Sync_Ignore_DaylightS, 0, wxALIGN_LEFT|wxALL, 5);
	
	
	wxBoxSizer* itemBoxSizer002 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer28->Add(itemBoxSizer002, 1, wxGROW|wxALL|wxALIGN_CENTER_VERTICAL, 5);	
	
	wxButton* itemButtonSyncOK = new wxButton( itemPanel6, ID_SYNC_OK, _("OK"));
	itemBoxSizer002->Add(itemButtonSyncOK, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
	wxButton* itemButtonSyncPreview = new wxButton( itemPanel6, ID_SYNC_PREVIEW , _("Preview"));
	itemBoxSizer002->Add(itemButtonSyncPreview, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer17 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer8->Add(itemBoxSizer17, 1, wxGROW|wxALL, 5);
	
	
	wxBoxSizer* itemBoxSizer18 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer17->Add(itemBoxSizer18, 1, wxGROW|wxALL, 5);
	
	wxStaticText* SyncSourceText = new wxStaticText( itemPanel6, wxID_STATIC, _("Source"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer18->Add(SyncSourceText, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);
	
	wxBoxSizer* itemBoxSizer19 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer18->Add(itemBoxSizer19, 0, wxGROW|wxALL, 0);
	
	wxBoxSizer* itemBoxSizer2002 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer18->Add(itemBoxSizer2002, 1, wxGROW|wxALL, 0);
	
	//Source section
	m_Sync_Source_Txt = new wxTextCtrl( itemPanel6, ID_SYNC_SOURCE_TXT, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer19->Add(m_Sync_Source_Txt, 1, wxALIGN_TOP|wxALL, 5);

	wxButton* itemButton21 = new wxButton( itemPanel6, ID_SYNC_SOURCE_BTN, wxT("..."), wxDefaultPosition, wxSize(25, 25), 0 );
	itemBoxSizer19->Add(itemButton21, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_Sync_Source_Tree = new wxVirtualDirTreeCtrl( itemPanel6, ID_SYNC_SOURCE_TREE, wxDefaultPosition, wxDefaultSize, wxTR_HAS_BUTTONS | wxTR_LINES_AT_ROOT | wxTR_HIDE_ROOT | wxTR_SINGLE | BORDER );
	itemBoxSizer2002->Add(m_Sync_Source_Tree, 1, wxGROW|wxALL, 5);
	
	wxBitmapButton* itemBitmapButton006 = new wxBitmapButton( itemPanel6, ID_SYNC_SOURCE_EXPAND, GetBitmapResource(wxT("expandall.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	itemBoxSizer2002->Add(itemBitmapButton006, 0, wxALIGN_CENTER_VERTICAL, 0);	

	wxBoxSizer* itemBoxSizer23 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer17->Add(itemBoxSizer23, 1, wxGROW|wxALL, 5);

	wxStaticText* SyncDestText = new wxStaticText( itemPanel6, wxID_STATIC, _("Destination"), wxDefaultPosition, wxDefaultSize, 0);
	itemBoxSizer23->Add(SyncDestText, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

	wxBoxSizer* itemBoxSizer24 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer23->Add(itemBoxSizer24, 0, wxGROW|wxALL, 0);
	
	wxBoxSizer* itemBoxSizer2001 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer23->Add(itemBoxSizer2001, 1, wxGROW|wxALL, 0);


	//Destination section
	m_Sync_Dest_Txt = new wxTextCtrl( itemPanel6, ID_SYNC_DEST_TXT, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer24->Add(m_Sync_Dest_Txt, 1, wxALIGN_TOP|wxALL, 5);

	wxButton* itemButton26 = new wxButton( itemPanel6, ID_SYNC_DEST_BTN, wxT("..."), wxDefaultPosition, wxSize(25, 25), 0 );
	itemBoxSizer24->Add(itemButton26, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_Sync_Dest_Tree = new wxVirtualDirTreeCtrl( itemPanel6, ID_SYNC_DEST_TREE, wxDefaultPosition, wxDefaultSize, wxTR_HAS_BUTTONS | wxTR_LINES_AT_ROOT | wxTR_HIDE_ROOT | wxTR_SINGLE | BORDER );
	itemBoxSizer2001->Add(m_Sync_Dest_Tree, 1, wxGROW|wxALL, 5);
	
	wxBitmapButton* itemBitmapButton005 = new wxBitmapButton( itemPanel6, ID_SYNC_DEST_EXPAND, GetBitmapResource(wxT("expandall.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	itemBoxSizer2001->Add(itemBitmapButton005, 0, wxALIGN_CENTER_VERTICAL, 0);

	//Backup section
	wxPanel* BackupPanel = new wxPanel(m_Notebook, ID_PANEL_BACKUP, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL);
	
	wxBoxSizer* BackupSizer = new wxBoxSizer(wxVERTICAL);
	BackupPanel->SetSizer(BackupSizer);
	
	wxBoxSizer* BackupTop = new wxBoxSizer(wxHORIZONTAL);
	BackupSizer->Add(BackupTop, 0, wxALIGN_LEFT|wxALL, 5);
	
	wxBoxSizer* BackupJobsRules = new wxBoxSizer(wxVERTICAL);
	BackupTop->Add(BackupJobsRules, 0, wxALIGN_LEFT|wxALL, 0);
	
	//Jobs
	wxStaticBox* BackupJobsBox = new wxStaticBox(BackupPanel, wxID_ANY, _("Job Name"));
	wxStaticBoxSizer* BackupJobsBoxSizer = new wxStaticBoxSizer(BackupJobsBox, wxHORIZONTAL);
	BackupJobsRules->Add(BackupJobsBoxSizer, 0, wxALIGN_TOP|wxALL, 5);
	
	wxArrayString m_Backup_Job_SelectStrings;
	m_Backup_Job_Select = new wxComboBox(BackupPanel, ID_BACKUP_JOB_SELECT, _T(""), wxDefaultPosition, wxDefaultSize, m_Backup_Job_SelectStrings, wxCB_DROPDOWN|wxCB_READONLY);
	BackupJobsBoxSizer->Add(m_Backup_Job_Select, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBitmapButton* BackupJobSave = new wxBitmapButton(BackupPanel, ID_BACKUP_JOB_SAVE, GetBitmapResource(wxT("save.png")));
	BackupJobsBoxSizer->Add(BackupJobSave, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBitmapButton* BackupJobAdd = new wxBitmapButton(BackupPanel, ID_BACKUP_JOB_ADD, GetBitmapResource(wxT("add.png")));
	BackupJobsBoxSizer->Add(BackupJobAdd, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBitmapButton* BackupJobRemove = new wxBitmapButton(BackupPanel, ID_BACKUP_JOB_REMOVE, GetBitmapResource(wxT("remove.png")));
	BackupJobsBoxSizer->Add(BackupJobRemove, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
	//Rules
	wxStaticBox* BackupRulesBox = new wxStaticBox(BackupPanel, wxID_ANY, _("Rules"));
	wxStaticBoxSizer* BackupRulesSizer = new wxStaticBoxSizer(BackupRulesBox, wxHORIZONTAL);
	BackupJobsRules->Add(BackupRulesSizer, 0, wxALL|wxEXPAND, 5);

	wxArrayString m_Backup_RulesStrings;
	m_Backup_Rules = new wxComboBox(BackupPanel, ID_BACKUP_RULES, _T(""), wxDefaultPosition, wxDefaultSize, m_Backup_RulesStrings, wxCB_DROPDOWN|wxCB_READONLY);
	BackupRulesSizer->Add(m_Backup_Rules, 1, wxALL, 5);
	
	//Options
	wxArrayString m_Backup_FunctionStrings;
	m_Backup_FunctionStrings.Add(_("Complete"));
	m_Backup_FunctionStrings.Add(_("Update"));
	m_Backup_FunctionStrings.Add(_("Differential"));
	m_Backup_Function = new wxRadioBox(BackupPanel, ID_BACKUP_FUNCTION, _("Type"), wxDefaultPosition, wxDefaultSize, m_Backup_FunctionStrings, 1, wxRA_SPECIFY_COLS );
	m_Backup_Function->SetSelection(0);
	BackupTop->Add(m_Backup_Function, 0, wxALIGN_TOP|wxALL, 5);

	wxArrayString m_Backup_FormatStrings;
	m_Backup_FormatStrings.Add(_("Zip"));
	m_Backup_FormatStrings.Add(_("7-Zip"));
	m_Backup_Format = new wxRadioBox(BackupPanel, ID_BACKUP_FORMAT, _("Format"), wxDefaultPosition, wxDefaultSize, m_Backup_FormatStrings, 1, wxRA_SPECIFY_COLS );
	m_Backup_Format->SetSelection(0);
	BackupTop->Add(m_Backup_Format, 0, wxALIGN_TOP|wxALL, 5);

	wxStaticBox* BackupRatioStatic = new wxStaticBox(BackupPanel, wxID_ANY, _("Compression Level"));
	wxStaticBoxSizer* BackupRatioSizer = new wxStaticBoxSizer(BackupRatioStatic, wxVERTICAL);
	
	m_Backup_Ratio_Text = new wxStaticText(BackupPanel, wxID_STATIC, _("Default"));
	m_Backup_Ratio = new wxSlider(BackupPanel, ID_BACKUP_RATIO, 3, 0, 5);
	
	BackupRatioSizer->Add(m_Backup_Ratio_Text);
	BackupRatioSizer->Add(m_Backup_Ratio);
	
	BackupTop->Add(BackupRatioSizer, 0, wxALIGN_TOP|wxALL, 5);

	wxStaticBox* BackupOther = new wxStaticBox(BackupPanel, wxID_ANY, _("Other"));
	wxStaticBoxSizer* BackupOtherSizer = new wxStaticBoxSizer(BackupOther, wxVERTICAL);
	BackupTop->Add(BackupOtherSizer, 0, wxALIGN_TOP|wxALL, 5);

	m_Backup_IsPass = new wxCheckBox(BackupPanel, ID_BACKUP_ISPASS, _("Password"));
	BackupOtherSizer->Add(m_Backup_IsPass, 0, wxALIGN_TOP|wxALL, 5);

	//Buttons
	wxBoxSizer* BackupButtons = new wxBoxSizer(wxVERTICAL);
	BackupTop->Add(BackupButtons, 1, wxGROW|wxALL|wxALIGN_CENTER_VERTICAL, 5);	
	
	wxButton* BackupOK = new wxButton(BackupPanel, ID_BACKUP_OK, _("OK"));
	BackupButtons->Add(BackupOK, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
	wxButton* BackupPreview = new wxButton(BackupPanel, ID_BACKUP_PREVIEW , _("Preview"));
	BackupButtons->Add(BackupPreview, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);	
	
	wxButton* BackupRestore = new wxButton(BackupPanel, ID_BACKUP_RESTORE , _("Restore"));
	BackupButtons->Add(BackupRestore, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);	
	
	//Main
	wxBoxSizer* BackupMain = new wxBoxSizer(wxHORIZONTAL);
	BackupSizer->Add(BackupMain, 1, wxEXPAND|wxLEFT|wxRIGHT|wxBOTTOM, 5);	
	
	wxBoxSizer* BackupMainLeft = new wxBoxSizer(wxVERTICAL);	
	BackupMain->Add(BackupMainLeft, 1, wxEXPAND|wxALL, 5);

	m_Backup_DirCtrl = new ExtendedDirCtrl(BackupPanel, ID_BACKUP_DIRCTRL, _T(""), wxDefaultPosition, wxDefaultSize, wxBORDER_THEME);
	m_Backup_DirCtrl->ShowHidden(true);
	BackupMainLeft->Add(m_Backup_DirCtrl, 1, wxGROW|wxALL, 5);

	wxBoxSizer* itemBoxSizer55 = new wxBoxSizer(wxVERTICAL);
	BackupMain->Add(itemBoxSizer55, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);
	
	wxBitmapButton* itemBitmapButton56 = new wxBitmapButton(BackupPanel, ID_BACKUP_ADD, itemFrame1->GetBitmapResource(wxT("add.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	itemBoxSizer55->Add(itemBitmapButton56, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxBitmapButton* itemBitmapButton57 = new wxBitmapButton(BackupPanel, ID_BACKUP_REMOVE, itemFrame1->GetBitmapResource(wxT("remove.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	itemBoxSizer55->Add(itemBitmapButton57, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxBoxSizer* BackupMainRight = new wxBoxSizer(wxVERTICAL);	
	BackupMain->Add(BackupMainRight, 1, wxEXPAND|wxALL, 5);
		
	wxStaticText* BackupStaticLocation = new wxStaticText(BackupPanel, wxID_ANY, _("Backup Location"));
	BackupMainRight->Add(BackupStaticLocation, 0, wxALL, 5);

	wxBoxSizer* BackupLocation = new wxBoxSizer(wxHORIZONTAL);
	BackupMainRight->Add(BackupLocation, 0, wxEXPAND|wxALL, 0);

	m_Backup_Location = new wxTextCtrl(BackupPanel, ID_BACKUP_LOCATION_TEXT, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
	BackupLocation->Add(m_Backup_Location,1, wxALL, 5);

	wxButton* itemButton49 = new wxButton(BackupPanel, ID_BACKUP_LOCATION, wxT("..."), wxDefaultPosition, wxSize(25, -1), 0 );
	BackupLocation->Add(itemButton49, 0, wxALL, 5);

	m_Backup_TreeCtrl = new wxVirtualDirTreeCtrl(BackupPanel, ID_BACKUP_TREECTRL, wxDefaultPosition, wxSize(100, 100), wxTR_HAS_BUTTONS |wxTR_LINES_AT_ROOT|wxTR_HIDE_ROOT|wxTR_SINGLE| BORDER );
	BackupMainRight->Add(m_Backup_TreeCtrl, 1, wxGROW|wxALL, 5);

	wxBoxSizer* itemBoxSizer59 = new wxBoxSizer(wxVERTICAL);
	BackupMain->Add(itemBoxSizer59, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);
	wxBitmapButton* itemBitmapButton60 = new wxBitmapButton(BackupPanel, ID_BACKUP_ADDVAR, GetBitmapResource(wxT("addvar.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	itemBoxSizer59->Add(itemBitmapButton60, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 0);
	
	wxBitmapButton* itemBitmapButton001 = new wxBitmapButton(BackupPanel, ID_BACKUP_EXPAND, GetBitmapResource(wxT("expandall.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	itemBoxSizer59->Add(itemBitmapButton001, 0, wxALIGN_CENTER_HORIZONTAL|wxTOP, 10);
	
	//Secure

	wxPanel* itemPanel68 = new wxPanel( m_Notebook, ID_PANEL_SECURE, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );
	wxBoxSizer* itemBoxSizer69 = new wxBoxSizer(wxVERTICAL);
	itemPanel68->SetSizer(itemBoxSizer69);

	wxBoxSizer* itemBoxSizer87 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer69->Add(itemBoxSizer87, 0, wxALIGN_LEFT|wxALL, 5);
	
	wxBoxSizer* itemBoxSizer3000 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer87->Add(itemBoxSizer3000, 0, wxALIGN_LEFT|wxALL, 0);	
	
	//Jobs section

	wxStaticBox* itemStaticBoxSizer73Static = new wxStaticBox(itemPanel68, wxID_ANY, _("Job Name"));
	wxStaticBoxSizer* itemStaticBoxSizer73 = new wxStaticBoxSizer(itemStaticBoxSizer73Static, wxHORIZONTAL);
	itemBoxSizer3000->Add(itemStaticBoxSizer73, 0, wxALIGN_TOP|wxALL, 5);
	wxArrayString m_Secure_Job_SelectStrings;
	m_Secure_Job_Select = new wxComboBox( itemPanel68, ID_SECURE_JOB_SELECT, _T(""), wxDefaultPosition, wxDefaultSize, m_Secure_Job_SelectStrings, wxCB_DROPDOWN|wxCB_READONLY);
	itemStaticBoxSizer73->Add(m_Secure_Job_Select, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBitmapButton* itemBitmapButton75 = new wxBitmapButton( itemPanel68, ID_SECURE_JOB_SAVE, itemFrame1->GetBitmapResource(wxT("save.png")));
	itemStaticBoxSizer73->Add(itemBitmapButton75, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBitmapButton* itemBitmapButton76 = new wxBitmapButton( itemPanel68, ID_SECURE_JOB_ADD, itemFrame1->GetBitmapResource(wxT("add.png")));
	itemStaticBoxSizer73->Add(itemBitmapButton76, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBitmapButton* itemBitmapButton77 = new wxBitmapButton( itemPanel68, ID_SECURE_JOB_REMOVE, itemFrame1->GetBitmapResource(wxT("remove.png")));
	itemStaticBoxSizer73->Add(itemBitmapButton77, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
	//Rules section

	wxStaticBox* itemStaticBoxSizer71Static = new wxStaticBox(itemPanel68, wxID_ANY, _("Rules"));
	wxStaticBoxSizer* itemStaticBoxSizer71 = new wxStaticBoxSizer(itemStaticBoxSizer71Static, wxHORIZONTAL);
	itemBoxSizer3000->Add(itemStaticBoxSizer71, 1, wxALIGN_TOP|wxALL|wxGROW, 5);
	wxArrayString m_Secure_RulesStrings;
	m_Secure_Rules = new wxComboBox( itemPanel68, ID_SECURE_RULES, _T(""), wxDefaultPosition, wxDefaultSize, m_Secure_RulesStrings, wxCB_DROPDOWN|wxCB_READONLY);
	itemStaticBoxSizer71->Add(m_Secure_Rules, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
	//Options
	
	wxBoxSizer* itemBoxSizer3001 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer87->Add(itemBoxSizer3001, 0, wxALIGN_LEFT|wxALL, 0);	
	
	wxArrayString m_Secure_FunctionStrings;
	m_Secure_FunctionStrings.Add(_("Encrypt"));
	m_Secure_FunctionStrings.Add(_("Decrypt"));
	m_Secure_Function = new wxRadioBox( itemPanel68, ID_SECURE_FUNCTION, _("Function"), wxDefaultPosition, wxDefaultSize, m_Secure_FunctionStrings, 1, wxRA_SPECIFY_COLS );
	m_Secure_Function->SetSelection(0);
	itemBoxSizer3001->Add(m_Secure_Function, 1, wxGROW|wxALIGN_TOP|wxALL, 5);

	wxBoxSizer* itemBoxSizer3002 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer87->Add(itemBoxSizer3002, 1, wxGROW|wxALL|wxALIGN_CENTER_VERTICAL, 5);	
	
	wxButton* itemButtonSecureOK = new wxButton( itemPanel68, ID_SECURE_OK, _("OK"));
	itemBoxSizer3002->Add(itemButtonSecureOK, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
	wxButton* itemButtonSecurePreview = new wxButton( itemPanel68, ID_SECURE_PREVIEW , _("Preview"));
	itemBoxSizer3002->Add(itemButtonSecurePreview, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);	

	//Main secure stuff
	wxBoxSizer* itemBoxSizer78 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer69->Add(itemBoxSizer78, 1, wxGROW|wxALL, 5);
	
	
	m_Secure_DirCtrl = new ExtendedDirCtrl( itemPanel68, ID_SECURE_DIRCTRL, _T(""), wxDefaultPosition, wxDefaultSize, wxBORDER_THEME);
	m_Secure_DirCtrl->ShowHidden(true);	
	itemBoxSizer78->Add(m_Secure_DirCtrl, 1, wxGROW|wxALL, 5);

	wxBoxSizer* itemBoxSizer80 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer78->Add(itemBoxSizer80, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	wxBitmapButton* itemBitmapButton81 = new wxBitmapButton( itemPanel68, ID_SECURE_ADD, itemFrame1->GetBitmapResource(wxT("add.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	itemBoxSizer80->Add(itemBitmapButton81, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxBitmapButton* itemBitmapButton82 = new wxBitmapButton( itemPanel68, ID_SECURE_REMOVE, itemFrame1->GetBitmapResource(wxT("remove.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	itemBoxSizer80->Add(itemBitmapButton82, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	m_Secure_TreeCtrl = new wxVirtualDirTreeCtrl( itemPanel68, ID_SECURE_TREECTRL, wxDefaultPosition, wxSize(100, 100), wxTR_HAS_BUTTONS |wxTR_LINES_AT_ROOT|wxTR_HIDE_ROOT|wxTR_SINGLE|BORDER);
	itemBoxSizer78->Add(m_Secure_TreeCtrl, 1, wxGROW|wxALL, 5);

	wxBoxSizer* itemBoxSizer84 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer78->Add(itemBoxSizer84, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);
	wxBitmapButton* itemBitmapButton85 = new wxBitmapButton( itemPanel68, ID_SECURE_ADDVAR, GetBitmapResource(wxT("addvar.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	itemBoxSizer84->Add(itemBitmapButton85, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 0);
	
	wxBitmapButton* itemBitmapButton002 = new wxBitmapButton( itemPanel68, ID_SECURE_EXPAND, GetBitmapResource(wxT("expandall.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	itemBoxSizer84->Add(itemBitmapButton002, 0, wxALIGN_CENTER_HORIZONTAL|wxTOP, 10);

	
	//Rules section

	wxPanel* itemPanel93 = new wxPanel( m_Notebook, ID_PANEL_RULES, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );
	wxBoxSizer* itemBoxSizer94 = new wxBoxSizer(wxVERTICAL);
	itemPanel93->SetSizer(itemBoxSizer94);

	wxBoxSizer* itemBoxSizer97 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer94->Add(itemBoxSizer97, 0, wxALIGN_LEFT|wxALL, 5);
	
	wxStaticText* itemStaticText96 = new wxStaticText( itemPanel93, wxID_STATIC, _("Name"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer97->Add(itemStaticText96, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 10);	
	
	wxArrayString m_Rules_ComboStrings;
	m_Rules_Name = new wxComboBox( itemPanel93, ID_RULES_COMBO, _T(""), wxDefaultPosition, wxDefaultSize, m_Rules_ComboStrings, wxCB_READONLY );
	itemBoxSizer97->Add(m_Rules_Name, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBitmapButton* itemBitmapButton99 = new wxBitmapButton( itemPanel93, ID_RULES_SAVE, itemFrame1->GetBitmapResource(wxT("save.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	itemBoxSizer97->Add(itemBitmapButton99, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBitmapButton* itemBitmapButton100 = new wxBitmapButton( itemPanel93, ID_RULES_ADD, itemFrame1->GetBitmapResource(wxT("add.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	itemBoxSizer97->Add(itemBitmapButton100, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBitmapButton* itemBitmapButton101 = new wxBitmapButton( itemPanel93, ID_RULES_REMOVE, itemFrame1->GetBitmapResource(wxT("remove.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	itemBoxSizer97->Add(itemBitmapButton101, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer102 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer94->Add(itemBoxSizer102, 1, wxGROW|wxALL, 5);
	wxStaticText* itemStaticText103 = new wxStaticText( itemPanel93, wxID_STATIC, _("Files to exclude (file name, path or extension)"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer102->Add(itemStaticText103, 0, wxALIGN_LEFT|wxLEFT, 10);

	wxBoxSizer* itemBoxSizer104 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer102->Add(itemBoxSizer104, 1, wxGROW|wxALL, 5);
	wxArrayString m_Rules_FileExcludeStrings;
	m_Rules_FileExclude = new wxListBox( itemPanel93, ID_RULE_FILE_EXCLUDE, wxDefaultPosition, wxDefaultSize, m_Rules_FileExcludeStrings, wxLB_SINGLE|BORDER );
	itemBoxSizer104->Add(m_Rules_FileExclude, 1, wxGROW|wxALL, 5);

	wxBoxSizer* itemBoxSizer106 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer104->Add(itemBoxSizer106, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	wxBitmapButton* itemBitmapButton107 = new wxBitmapButton( itemPanel93, ID_RULES_ADD_FILEEXCLUDE, itemFrame1->GetBitmapResource(wxT("add.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	itemBoxSizer106->Add(itemBitmapButton107, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxBitmapButton* itemBitmapButton108 = new wxBitmapButton( itemPanel93, ID_RULES_REMOVE_FILEEXCLUDE, itemFrame1->GetBitmapResource(wxT("remove.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	itemBoxSizer106->Add(itemBitmapButton108, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer109 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer94->Add(itemBoxSizer109, 1, wxGROW|wxALL, 5);
	wxStaticText* itemStaticText110 = new wxStaticText( itemPanel93, wxID_STATIC, _("Folders to exclude (folder name or path)"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer109->Add(itemStaticText110, 0, wxALIGN_LEFT|wxLEFT, 10);

	wxBoxSizer* itemBoxSizer111 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer109->Add(itemBoxSizer111, 1, wxGROW|wxALL, 5);
	wxArrayString m_Rules_FolderExcludeStrings;
	m_Rules_FolderExclude = new wxListBox( itemPanel93, ID_RULES_FOLDER_EXCLUDE, wxDefaultPosition, wxDefaultSize, m_Rules_FolderExcludeStrings, wxLB_SINGLE|BORDER );
	itemBoxSizer111->Add(m_Rules_FolderExclude, 1, wxGROW|wxALL, 5);

	wxBoxSizer* itemBoxSizer113 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer111->Add(itemBoxSizer113, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	wxBitmapButton* itemBitmapButton114 = new wxBitmapButton( itemPanel93, ID_RULES_ADD_FOLDEREXCLUDE, itemFrame1->GetBitmapResource(wxT("add.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	itemBoxSizer113->Add(itemBitmapButton114, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxBitmapButton* itemBitmapButton115 = new wxBitmapButton( itemPanel93, ID_RULES_REMOVE_FOLDEREXCLUDE, itemFrame1->GetBitmapResource(wxT("remove.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	itemBoxSizer113->Add(itemBitmapButton115, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer116 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer94->Add(itemBoxSizer116, 1, wxGROW|wxALL, 5);
	wxStaticText* itemStaticText117 = new wxStaticText( itemPanel93, wxID_STATIC, _("Locations to always include (file/folder name, path or  file extension)"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer116->Add(itemStaticText117, 0, wxALIGN_LEFT|wxLEFT, 10);

	wxBoxSizer* itemBoxSizer118 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer116->Add(itemBoxSizer118, 1, wxGROW|wxALL, 5);
	wxArrayString m_Rules_LocationIncludeStrings;
	m_Rules_LocationInclude = new wxListBox( itemPanel93, ID_RULES_LOCATION_INCLUDE, wxDefaultPosition, wxDefaultSize, m_Rules_LocationIncludeStrings, wxLB_SINGLE|BORDER );
	itemBoxSizer118->Add(m_Rules_LocationInclude, 1, wxGROW|wxALL, 5);

	wxBoxSizer* itemBoxSizer120 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer118->Add(itemBoxSizer120, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	wxBitmapButton* itemBitmapButton121 = new wxBitmapButton( itemPanel93, ID_RULES_ADD_LOCATIONINCLUDE, itemFrame1->GetBitmapResource(wxT("add.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	itemBoxSizer120->Add(itemBitmapButton121, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxBitmapButton* itemBitmapButton122 = new wxBitmapButton( itemPanel93, ID_RULES_REMOVE_LOCATIONINCLUDE, itemFrame1->GetBitmapResource(wxT("remove.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	itemBoxSizer120->Add(itemBitmapButton122, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	//Variables
	wxPanel* VariablesPanel = new wxPanel(m_Notebook, ID_PANEL_VARIABLES, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL);

	wxBoxSizer* VariablesSizer = new wxBoxSizer(wxVERTICAL);
    VariablesPanel->SetSizer(VariablesSizer);

    wxBoxSizer* VariablesTop = new wxBoxSizer(wxHORIZONTAL);
    VariablesSizer->Add(VariablesTop, 0, wxALIGN_LEFT|wxALL, 5);

    wxStaticText* VariablesStatic = new wxStaticText(VariablesPanel, wxID_STATIC, _("Name"));
    VariablesTop->Add(VariablesStatic, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 10);

    wxArrayString arrVariables;
    m_Variables_Name = new wxComboBox(VariablesPanel, ID_VARIABLES_NAME, _T(""), wxDefaultPosition, wxDefaultSize, arrVariables, wxCB_DROPDOWN|wxCB_READONLY);
    VariablesTop->Add(m_Variables_Name, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxBitmapButton* VariablesAdd = new wxBitmapButton(VariablesPanel, ID_VARIABLES_ADD, itemFrame1->GetBitmapResource(wxT("add.png")));
    VariablesTop->Add(VariablesAdd, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBitmapButton* VariablesRemove = new wxBitmapButton(VariablesPanel, ID_VARIABLES_REMOVE, itemFrame1->GetBitmapResource(wxT("remove.png")));
    VariablesTop->Add(VariablesRemove, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* VariablesMain = new wxBoxSizer(wxHORIZONTAL);
    VariablesSizer->Add(VariablesMain, 1, wxGROW|wxALL, 5);

    m_Variables_List = new wxListCtrl(VariablesPanel, ID_VARIABLES_LIST, wxDefaultPosition, wxSize(100, 100), wxLC_REPORT|BORDER);
    VariablesMain->Add(m_Variables_List, 1, wxGROW|wxALL, 5);

    wxBoxSizer* VariablesRight = new wxBoxSizer(wxVERTICAL);
    VariablesMain->Add(VariablesRight, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBitmapButton* VariablesAddItem = new wxBitmapButton(VariablesPanel, ID_VARIABLES_ADDITEM, itemFrame1->GetBitmapResource(wxT("add.png")));
    VariablesRight->Add(VariablesAddItem, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBitmapButton* VariablesRemoveItem = new wxBitmapButton(VariablesPanel, ID_VARIABLES_REMOVEITEM, itemFrame1->GetBitmapResource(wxT("remove.png")));
    VariablesRight->Add(VariablesRemoveItem, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	//Scripting

	wxPanel* itemPanel131 = new wxPanel( m_Notebook, ID_PANEL_SCRIPT, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );
	wxBoxSizer* itemBoxSizer132 = new wxBoxSizer(wxVERTICAL);
	itemPanel131->SetSizer(itemBoxSizer132);
	
	wxBoxSizer* itemBoxSizer140 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer132->Add(itemBoxSizer140, 0, wxALIGN_LEFT|wxALL, 5);

	wxStaticBox* ScriptStaticBox = new wxStaticBox(itemPanel131, wxID_ANY, _("Name"));
	wxStaticBoxSizer* ScriptStaticBoxSizer = new wxStaticBoxSizer(ScriptStaticBox, wxHORIZONTAL);
	itemBoxSizer140->Add(ScriptStaticBoxSizer, 0, wxALIGN_TOP|wxALL, 5);


	wxArrayString itemComboBox134Strings;
	m_Script_Name = new wxComboBox( itemPanel131, ID_SCRIPT_NAME, _T(""), wxDefaultPosition, wxDefaultSize, itemComboBox134Strings, wxCB_DROPDOWN|wxCB_READONLY);
	ScriptStaticBoxSizer->Add(m_Script_Name, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
	wxBitmapButton* itemBitmapButton135 = new wxBitmapButton( itemPanel131, ID_SCRIPT_SAVE, itemFrame1->GetBitmapResource(wxT("save.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	ScriptStaticBoxSizer->Add(itemBitmapButton135, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBitmapButton* itemBitmapButton136 = new wxBitmapButton( itemPanel131, ID_SCRIPT_ADD, itemFrame1->GetBitmapResource(wxT("add.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	ScriptStaticBoxSizer->Add(itemBitmapButton136, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBitmapButton* itemBitmapButton137 = new wxBitmapButton( itemPanel131, ID_SCRIPT_REMOVE, itemFrame1->GetBitmapResource(wxT("remove.png")), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	ScriptStaticBoxSizer->Add(itemBitmapButton137, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxButton* itemButton142 = new wxButton( itemPanel131, ID_SCRIPT_EXECUTE, _("Execute"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer140->Add(itemButton142, 0, wxALIGN_TOP|wxALL, 10);

	wxBoxSizer* itemBoxSizer138 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer132->Add(itemBoxSizer138, 1, wxGROW|wxALL, 5);
	m_Script_Rich = new wxTextCtrl( itemPanel131, ID_SCRIPT_RICH, _T(""), wxDefaultPosition, wxSize(100, 100), wxTE_MULTILINE|BORDER);
	itemBoxSizer138->Add(m_Script_Rich, 1, wxGROW|wxALL, 5);

	//Settings
	wxPanel* SettingsPanel = new wxPanel(m_Notebook, ID_PANEL_SETTINGS, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL);

	wxBoxSizer* SettingsSizer = new wxBoxSizer(wxVERTICAL);
	SettingsPanel->SetSizer(SettingsSizer);
	
	wxStaticBox* FontStaticBox = new wxStaticBox(SettingsPanel, wxID_ANY, _("Font"));
	wxStaticBoxSizer* FontStaticBoxSizer = new wxStaticBoxSizer(FontStaticBox, wxHORIZONTAL);
	SettingsSizer->Add(FontStaticBoxSizer, 0, wxALIGN_TOP|wxALL, 5);

	m_Settings_Font = new wxFontPickerCtrl(SettingsPanel, ID_SETTINGS_FONT, wxNullFont, wxDefaultPosition, wxDefaultSize, 0);
	m_Settings_Font->SetSelectedFont(font);
	FontStaticBoxSizer->Add(m_Settings_Font, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxArrayString arrTabStyles;
	arrTabStyles.Add(_("Icons and Text"));
	arrTabStyles.Add(_("Text"));
	m_Settings_TabStyle = new wxRadioBox(SettingsPanel, ID_SETTINGS_TABSTYLE, _("Tab style"), wxDefaultPosition, wxDefaultSize, arrTabStyles, 2, wxRA_SPECIFY_ROWS);
	m_Settings_TabStyle->SetStringSelection(wxGetApp().m_Settings->GetTabStyle());
	SettingsSizer->Add(m_Settings_TabStyle, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);	
	
	wxStaticBox* LanguageStaticBox = new wxStaticBox(SettingsPanel, wxID_ANY, _("Language"));
	wxStaticBoxSizer* LanguageStaticBoxSizer = new wxStaticBoxSizer(LanguageStaticBox, wxHORIZONTAL);
	SettingsSizer->Add(LanguageStaticBoxSizer, 0, wxALIGN_TOP|wxALL, 5);
	
	wxArrayString m_Settings_LanguageStrings = GetLanguages();
	m_Settings_Language = new wxComboBox(SettingsPanel, ID_SETTINGS_LANGUAGE, _T(""), wxDefaultPosition, wxDefaultSize, m_Settings_LanguageStrings, wxCB_DROPDOWN|wxCB_READONLY);
	m_Settings_Language->SetMinSize(wxSize(125, -1));
	m_Settings_Language->SetStringSelection(wxLocale::FindLanguageInfo(wxGetApp().m_Settings->GetLanguageCode())->Description);
	LanguageStaticBoxSizer->Add(m_Settings_Language, 1, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5);

	//Need to solve this properly
	wxScreenDC textdc;
	textdc.SetFont(wxGetApp().m_Settings->GetFont());
	wxSize textsize = textdc.GetTextExtent(_("Remember Entered Information"));
	
	wxStaticBox* FieldsStaticBox = new wxStaticBox(SettingsPanel, wxID_ANY, _("Remember Entered Information"));
	wxStaticBoxSizer* FieldsStaticBoxSizer = new wxStaticBoxSizer(FieldsStaticBox, wxVERTICAL);
	FieldsStaticBoxSizer->SetMinSize(wxSize(textsize.GetWidth() + 20, -1));
	SettingsSizer->Add(FieldsStaticBoxSizer, 0, wxALIGN_TOP|wxALL, 5);
	
	m_Settings_RememberSync = new wxCheckBox(SettingsPanel, ID_SETTINGS_REMEMBERSYNC, _("Sync"));
	FieldsStaticBoxSizer->Add(m_Settings_RememberSync, 0, wxALIGN_TOP|wxALL, 5);
	
	m_Settings_RememberBackup = new wxCheckBox(SettingsPanel, ID_SETTINGS_REMEMBERBACKUP, _("Backup"));
	FieldsStaticBoxSizer->Add(m_Settings_RememberBackup, 0, wxALIGN_TOP|wxALL, 5);
	
	m_Settings_RememberSecure = new wxCheckBox(SettingsPanel, ID_SETTINGS_REMEMBERSECURE, _("Secure"));
	FieldsStaticBoxSizer->Add(m_Settings_RememberSecure, 0, wxALIGN_TOP|wxALL, 5);

	//Help
	wxPanel* HelpPanel = new wxPanel(m_Notebook, ID_PANEL_HELP, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL);
	
	wxBoxSizer* HelpSizer = new wxBoxSizer(wxVERTICAL);
	HelpPanel->SetSizer(HelpSizer);
	
	wxButton* AboutButton = new wxButton(HelpPanel, wxID_ABOUT, _("About"));
	HelpSizer->Add(AboutButton, 0, wxALIGN_TOP|wxALL, 5);
	
	m_HelpWindow = new wxHtmlHelpWindow;
    wxGetApp().m_Help->SetHelpWindow(m_HelpWindow); 
    wxGetApp().m_Help->AddBook(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("toucan.htb"));
    m_HelpWindow->Create(HelpPanel, ID_HELP, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxNO_BORDER, wxHF_CONTENTS|wxHF_INDEX|wxHF_SEARCH);
	m_HelpWindow->DisplayContents();
	HelpSizer->Add(m_HelpWindow, 1, wxALIGN_TOP|wxTOP|wxEXPAND, 5);

	//Add the panels
	wxBitmap syncbitmap = GetBitmapResource(wxT("sync.png"));
	wxBitmap backupbitmap = GetBitmapResource(wxT("backup.png"));
	wxBitmap securebitmap = GetBitmapResource(wxT("secure.png"));
	wxBitmap settingsbitmap = GetBitmapResource(wxT("settings.png"));
	wxBitmap scriptbitmap = GetBitmapResource(wxT("script.png"));
	wxBitmap rulesbitmap = GetBitmapResource(wxT("rules.png"));
	wxBitmap pvarsbitmap = GetBitmapResource(wxT("pvars.png"));
	wxBitmap helpbitmap = GetBitmapResource(wxT("help.png"));
	
	if(wxGetApp().m_Settings->GetTabStyle() == _("Text")){
		syncbitmap = wxNullBitmap;
		backupbitmap = wxNullBitmap;
		securebitmap = wxNullBitmap;
		settingsbitmap = wxNullBitmap;
		scriptbitmap = wxNullBitmap;
		rulesbitmap = wxNullBitmap;
		pvarsbitmap = wxNullBitmap;
		helpbitmap = wxNullBitmap;
	}

	m_Notebook->AddPage(itemPanel6, _("Sync"), false, syncbitmap);
	m_Notebook->AddPage(BackupPanel, _("Backup"), false, backupbitmap);
	m_Notebook->AddPage(itemPanel68, _("Secure"), false, securebitmap);
	m_Notebook->AddPage(itemPanel93, _("Rules"), false, rulesbitmap);
	m_Notebook->AddPage(VariablesPanel, _("Variables"), false, pvarsbitmap);
	m_Notebook->AddPage(itemPanel131, _("Script"), false, scriptbitmap);
	m_Notebook->AddPage(SettingsPanel, _("Settings"), false, settingsbitmap);
	m_Notebook->AddPage(HelpPanel, _("Help"), false, helpbitmap);

	itemFrame1->m_auiManager.AddPane(m_Notebook, wxAuiPaneInfo()
	                                    .Name(_T("Pane3")).Centre().CaptionVisible(false).CloseButton(false).DestroyOnClose(false).Resizable(true).Floatable(false).PaneBorder(false));

	m_auiManager.Update();

	//Set the settings
	unsigned int i = 0;
	unsigned int count = m_Notebook->GetPageCount();
	while(m_Notebook->GetPageText(m_Notebook->GetSelection()) != wxGetApp().m_Settings->GetPosition()){
		
		i ++;
		if(i < count){
			m_Notebook->AdvanceSelection();
		}
		else{
			m_Notebook->SetSelection(0);
			break;
		}
	}
	
	m_Settings_RememberSync->SetValue(wxGetApp().m_Settings->GetRememberSync());
	m_Settings_RememberBackup->SetValue(wxGetApp().m_Settings->GetRememberBackup());
	m_Settings_RememberSecure->SetValue(wxGetApp().m_Settings->GetRememberSecure());

	//Set the drag and drop targets
	m_Sync_Source_Txt->SetDropTarget(new DnDFileTreeText(m_Sync_Source_Txt, m_Sync_Source_Tree));
	m_Sync_Dest_Txt->SetDropTarget(new DnDFileTreeText(m_Sync_Dest_Txt, m_Sync_Dest_Tree));
	
	m_Sync_Source_Tree->SetDropTarget(new DnDFileTreeText(m_Sync_Source_Txt, m_Sync_Source_Tree));
	m_Sync_Dest_Tree->SetDropTarget(new DnDFileTreeText(m_Sync_Dest_Txt, m_Sync_Dest_Tree));
	
	m_Backup_TreeCtrl->SetDropTarget(new DnDFileTree(m_Backup_TreeCtrl));
	m_Secure_TreeCtrl->SetDropTarget(new DnDFileTree(m_Secure_TreeCtrl));
	
	m_Sync_Source_Tree->SetSync(true);
	m_Sync_Dest_Tree->SetSync(true);

	//Set up the rules boxes
	SetRulesBox(m_Sync_Rules);
	SetRulesBox(m_Backup_Rules);
	SetRulesBox(m_Secure_Rules);
	SetRulesBox(m_Rules_Name);

	//Set up the jobs boxes
	SetJobsBox(m_Sync_Job_Select, wxT("Sync"));
	SetJobsBox(m_Backup_Job_Select, wxT("Backup"));
	SetJobsBox(m_Secure_Job_Select, wxT("Secure"));
	
	//Set the portable variables box
	SetVariablesBox(m_Variables_Name);
	
	//Set the scripts box
	SetScriptsBox(m_Script_Name);
	
	wxListItem column;
	m_Variables_List->InsertColumn(0, column);
	m_Variables_List->InsertColumn(1, column);
	
	//Add some blank items to the combo box to allow deselection
	m_Sync_Rules->Append(wxEmptyString);
	m_Backup_Rules->Append(wxEmptyString);
	m_Secure_Rules->Append(wxEmptyString);

	this->SetIcon(wxIcon(wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH + wxT("Toucan.ico"), wxBITMAP_TYPE_ICO));	
}

//Get bitmap resources
wxBitmap frmMain::GetBitmapResource(const wxString& name)
{
	wxString strPath = wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + wxFILE_SEP_PATH;
	wxUnusedVar(name);
	if (name == _T("save.png")) {
		wxBitmap bitmap(strPath + _T("save.png"), wxBITMAP_TYPE_PNG);
		return bitmap;
	} 
	else if (name == _T("add.png")) {
		wxBitmap bitmap(strPath + _T("add.png"), wxBITMAP_TYPE_PNG);
		return bitmap;
	} 
	else if (name == _T("expandall.png")) {
		wxBitmap bitmap(strPath + _T("expandall.png"), wxBITMAP_TYPE_PNG);
		return bitmap;
	} 
	else if (name == _T("ok.png")) {
		wxBitmap bitmap(strPath + _T("ok.png"), wxBITMAP_TYPE_PNG);
		return bitmap;
	}
	else if (name == _T("preview.png")) {
		wxBitmap bitmap(strPath + _T("preview.png"), wxBITMAP_TYPE_PNG);
		return bitmap;
	}
	else if (name == _T("remove.png")) {
		wxBitmap bitmap(strPath + _T("remove.png"), wxBITMAP_TYPE_PNG);
		return bitmap;
	}
	else if (name == _T("sync.png")) {
		wxBitmap bitmap(strPath + _T("sync.png"), wxBITMAP_TYPE_PNG);
		return bitmap;
	}
	else if (name == _T("backup.png")) {
		wxBitmap bitmap(strPath + _T("backup.png"), wxBITMAP_TYPE_PNG);
		return bitmap;
	}
	else if (name == _T("secure.png")) {
		wxBitmap bitmap(strPath + _T("secure.png"), wxBITMAP_TYPE_PNG);
		return bitmap;
	}
	else if (name == _T("settings.png")) {
		wxBitmap bitmap(strPath + _T("settings.png"), wxBITMAP_TYPE_PNG);
		return bitmap;
	}
	else if (name == _T("rules.png")) {
		wxBitmap bitmap(strPath + _T("rules.png"), wxBITMAP_TYPE_PNG);
		return bitmap;
	}
	else if (name == _T("script.png")) {
		wxBitmap bitmap(strPath + _T("script.png"), wxBITMAP_TYPE_PNG);
		return bitmap;
	}
	else if (name == _T("pvars.png")) {
		wxBitmap bitmap(strPath + _T("pvars.png"), wxBITMAP_TYPE_PNG);
		return bitmap;
	}
	else if (name == _T("addvar.png")) {
		wxBitmap bitmap(strPath + _T("addvar.png"), wxBITMAP_TYPE_PNG);
		return bitmap;
	}
	else if (name == _T("help.png")) {
		wxBitmap bitmap(strPath + _T("help.png"), wxBITMAP_TYPE_PNG);
		return bitmap;
	}
	return wxNullBitmap;
}

//ID_BACKUP_ADD
void frmMain::OnBackupAddClick(wxCommandEvent& event){
	wxBusyCursor cursor;
	wxArrayString arrPaths = m_Backup_DirCtrl->GetSelectedPaths();
	for(unsigned int i = 0; i < arrPaths.Count(); i++){
		m_BackupLocations->Add(arrPaths.Item(i));
		m_Backup_TreeCtrl->AddNewPath(arrPaths.Item(i));		
	}
}

//ID_SECURE_ADD
void frmMain::OnSecureAddClick(wxCommandEvent& event){
	wxBusyCursor cursor;
	wxArrayString arrPaths = m_Secure_DirCtrl->GetSelectedPaths();
	for(unsigned int i = 0; i < arrPaths.Count(); i++){
		m_SecureLocations->Add(arrPaths.Item(i));
		m_Secure_TreeCtrl->AddNewPath(arrPaths.Item(i));		
	}
}

//ID_BACKUP_REMOVE
void frmMain::OnBackupRemoveClick(wxCommandEvent& event){
	//Checks to see if it is a top level item that is being removed
	if (m_Backup_TreeCtrl->GetItemParent(m_Backup_TreeCtrl->GetSelection()) == m_Backup_TreeCtrl->GetRootItem()){
		//A loop to compare to normalised and non-normalised files
		for(unsigned int i = 0; i < m_BackupLocations->GetCount(); i++){
			if(Normalise(Normalise(m_BackupLocations->Item(i))) == m_Backup_TreeCtrl->GetItemText(m_Backup_TreeCtrl->GetSelection())){
				m_BackupLocations->RemoveAt(i);
			}
		}
		m_Backup_TreeCtrl->Delete(m_Backup_TreeCtrl->GetSelection());
	}
}

//ID_SECURE_REMOVE
void frmMain::OnSecureRemoveClick(wxCommandEvent& event){
	//Checks to see if it is a top level item that is being removed
	if (m_Secure_TreeCtrl->GetItemParent(m_Secure_TreeCtrl->GetSelection()) == m_Secure_TreeCtrl->GetRootItem()){
		//A loop to compare to normalised and non-normalised files
		for(unsigned int i = 0; i < m_SecureLocations->GetCount(); i++){
			if(Normalise(Normalise(m_SecureLocations->Item(i))) == m_Secure_TreeCtrl->GetItemText(m_Secure_TreeCtrl->GetSelection())){
				m_SecureLocations->RemoveAt(i);
			}
		}
		m_Secure_TreeCtrl->Delete(m_Secure_TreeCtrl->GetSelection());
	}
}

//ID_SYNC_SOURCE_BTN
void frmMain::OnSyncSourceBtnClick(wxCommandEvent& event){
	//Need to replace this with a better browser
	wxDirDialog dialog(this,_("Please select the source folder."), Normalise(Normalise(m_Sync_Source_Txt->GetValue())));
	if (dialog.ShowModal() == wxID_OK) {
		wxBusyCursor cursor;
		m_Sync_Source_Tree->DeleteAllItems();
		m_Sync_Source_Tree->AddRoot(wxT("Hidden root"));
		m_Sync_Source_Tree->SetRoot(Normalise(Normalise(dialog.GetPath())));
		m_Sync_Source_Tree->AddNewPath(Normalise(Normalise(dialog.GetPath())));
		m_Sync_Source_Txt->SetValue(dialog.GetPath());
	}
}

//ID_SYNC_DEST_BTN
void frmMain::OnSyncDestBtnClick(wxCommandEvent& event){
	//Need to replace this with a better browser	
	wxDirDialog dialog(this,_("Please select the desination folder."), Normalise(Normalise(m_Sync_Dest_Txt->GetValue())));
	if (dialog.ShowModal() == wxID_OK) {
		wxBusyCursor cursor;
		m_Sync_Dest_Tree->DeleteAllItems();
		m_Sync_Dest_Tree->AddRoot(wxT("Hidden root"));
		m_Sync_Dest_Tree->SetRoot(Normalise(Normalise(dialog.GetPath())));
		m_Sync_Dest_Tree->AddNewPath(Normalise(Normalise(dialog.GetPath())));
		m_Sync_Dest_Txt->SetValue(dialog.GetPath());
	}
}

//ID_RULES_ADD_FILEEXCLUDE
void frmMain::OnRulesAddFileexcludeClick(wxCommandEvent& event){
	wxTextEntryDialog *dialog = new wxTextEntryDialog(this, _("File to exclude"), wxEmptyString);
	if (dialog->ShowModal() == wxID_OK) {
		m_Rules_FileExclude->Append(dialog->GetValue());
	}
	delete dialog;
}

//ID_RULES_ADD_FOLDEREXCLUDE
void frmMain::OnRulesAddFolderexcludeClick(wxCommandEvent& event){
	wxTextEntryDialog *dialog = new wxTextEntryDialog(this, _("Folder to exclude"),wxEmptyString);
	if (dialog->ShowModal() == wxID_OK) {
		m_Rules_FolderExclude->Append(dialog->GetValue());
	}
	delete dialog;
}

//ID_RULES_REMOVE_FILEEXCLUDE
void frmMain::OnRulesRemoveFileexcludeClick(wxCommandEvent& event){
	m_Rules_FileExclude->Delete(m_Rules_FileExclude->GetSelection());
}

//ID_RULES_REMOVE_FOLDEREXCLUDE
void frmMain::OnRulesRemoveFolderexcludeClick(wxCommandEvent& event){
	m_Rules_FolderExclude->Delete(m_Rules_FolderExclude->GetSelection());
}

//ID_RULES_SAVE
void frmMain::OnRulesSaveClick(wxCommandEvent& event){
	//Get all of the form data into arraystrings
	wxArrayString arrLocationsInclude, arrFolderExclude, arrFileExclude, arrFileDelete;
	for (unsigned int i = 0; i < m_Rules_LocationInclude->GetCount(); i++) {
		arrLocationsInclude.Add(m_Rules_LocationInclude->GetString(i));
	}
	for (unsigned int j = 0; j < m_Rules_FolderExclude->GetCount(); j++) {
		arrFolderExclude.Add(m_Rules_FolderExclude->GetString(j));
	}
	for (unsigned int k = 0; k < m_Rules_FileExclude->GetCount(); k++) {
		arrFileExclude.Add(m_Rules_FileExclude->GetString(k));
	}

	//Create the rule set and add the arraystrings to it
	Rules rules;
	rules.SetLocationsToInclude(arrLocationsInclude);
	rules.SetFilesToExclude(arrFileExclude);
	rules.SetFoldersToExclude(arrFolderExclude);
	//rules.SetFilesToDelete(arrFileDelete);
	if (m_Rules_Name->GetStringSelection() != wxEmptyString) {
		rules.TransferToFile(m_Rules_Name->GetStringSelection());
	} else {
		ErrorBox(_("You must select a name for this set of Rules"));
	}
	//Set up the rules boxes
	SetRulesBox(m_Sync_Rules);
	SetRulesBox(m_Backup_Rules);
	SetRulesBox(m_Secure_Rules);
}

//ID_RULES_ADD
void frmMain::OnRulesAddClick(wxCommandEvent& event){
	wxTextEntryDialog *dialog = new wxTextEntryDialog(this, _("You must select a name for this set of Rules"), wxEmptyString);
	if (dialog->ShowModal() == wxID_OK) {
		m_Rules_Name->AppendString(dialog->GetValue());
		m_Rules_Name->SetStringSelection(dialog->GetValue());
		m_Rules_LocationInclude->Clear();
		m_Rules_FileExclude->Clear();
		m_Rules_FolderExclude->Clear();
	}
	delete dialog;
}

//ID_RULES_REMOVE
void frmMain::OnRulesRemoveClick(wxCommandEvent& event){
	m_Rules_LocationInclude->Clear();
	m_Rules_FileExclude->Clear();
	m_Rules_FolderExclude->Clear();
	wxGetApp().m_Rules_Config->DeleteGroup(m_Rules_Name->GetStringSelection());
	wxGetApp().m_Rules_Config->Flush();
	m_Rules_Name->Delete(m_Rules_Name->GetSelection());
}

//ID_RULES_COMBO
void frmMain::OnRulesComboSelected(wxCommandEvent& event){
	//Clear the existing rules
	m_Rules_LocationInclude->Clear();
	m_Rules_FileExclude->Clear();
	m_Rules_FolderExclude->Clear();
	
	//Create a new rule set
	Rules rules;
	if (rules.TransferFromFile(m_Rules_Name->GetStringSelection())) {
		//Loop through the array of rules and add them to the form
		for (unsigned int i = 0; i < rules.GetLocationsToInclude().GetCount(); i++) {
			m_Rules_LocationInclude->Append(rules.GetLocationsToInclude().Item(i));
		}
		for (unsigned int j = 0; j < rules.GetFilesToExclude().GetCount(); j++) {
			m_Rules_FileExclude->Append(rules.GetFilesToExclude().Item(j));
		}
		for (unsigned int k = 0; k < rules.GetFoldersToExclude().GetCount(); k++) {
			m_Rules_FolderExclude->Append(rules.GetFoldersToExclude().Item(k));
		}
	}
	SetTitleBarText();
}

//ID_SYNC_JOB_SAVE
void frmMain::OnSyncJobSaveClick(wxCommandEvent& event){
	JobSave(m_Sync_Job_Select->GetStringSelection(), m_Sync_Rules->GetStringSelection(), wxT("Sync"));
}

//ID_BACKUP_JOB_SAVE
void frmMain::OnBackupJobSaveClick(wxCommandEvent& event){
	JobSave(m_Backup_Job_Select->GetStringSelection(), m_Backup_Rules->GetStringSelection(), wxT("Backup"));
}

//ID_SECURE_JOB_SAVE
void frmMain::OnSecureJobSaveClick(wxCommandEvent& event){
	JobSave(m_Secure_Job_Select->GetStringSelection(), m_Secure_Rules->GetStringSelection(), wxT("Secure"));
}

//ID_SYNC_JOB_ADD
void frmMain::OnSyncJobAddClick(wxCommandEvent& event){
	JobAdd(m_Sync_Job_Select);
}

//ID_BACKUP_JOB_ADD
void frmMain::OnBackupJobAddClick(wxCommandEvent& event){
	JobAdd(m_Backup_Job_Select);
}

//ID_SECURE_JOB_ADD
void frmMain::OnSecureJobAddClick(wxCommandEvent& event){
	JobAdd(m_Secure_Job_Select);
}

//ID_SYNC_JOB_REMOVE
void frmMain::OnSyncJobRemoveClick(wxCommandEvent& event){
	JobRemove(m_Sync_Job_Select);	
}

//ID_BACKUP_JOB_REMOVE
void frmMain::OnBackupJobRemoveClick(wxCommandEvent& event){
	JobRemove(m_Backup_Job_Select);
}

//ID_SECURE_JOB_REMOVE
void frmMain::OnSecureJobRemoveClick(wxCommandEvent& event){
	JobRemove(m_Secure_Job_Select);
}

//ID_BACKUP_JOB_SELECT
void frmMain::OnBackupJobSelectSelected(wxCommandEvent& event){
	JobLoad(m_Backup_Job_Select->GetStringSelection(), m_Backup_Rules, wxT("Backup"));
}

//ID_SECURE_JOB_SELECT
void frmMain::OnSecureJobSelectSelected(wxCommandEvent& event){
	JobLoad(m_Secure_Job_Select->GetStringSelection(), m_Secure_Rules, wxT("Secure"));
}

//ID_SYNC_JOB_SELECT
void frmMain::OnSyncJobSelectSelected(wxCommandEvent& event){
	JobLoad(m_Sync_Job_Select->GetStringSelection(), m_Sync_Rules, wxT("Sync"));
}

//ID_RULES_ADD_FILEINCLUDE
void frmMain::OnRulesAddLocationincludeClick(wxCommandEvent& event){
	wxTextEntryDialog *dialog = new wxTextEntryDialog(this, _("Location to include"),wxEmptyString);
	if (dialog->ShowModal() == wxID_OK) {
		m_Rules_LocationInclude->Append(dialog->GetValue());
	}
	delete dialog;
}

//ID_RULES_REMOVE_FILEINCLUDE
void frmMain::OnRulesRemoveLocationincludeClick(wxCommandEvent& event){
	m_Rules_LocationInclude->Delete(m_Rules_LocationInclude->GetSelection());
}

//ID_BACKUP_LOCATION
void frmMain::OnBackupLocationClick(wxCommandEvent& event){
	if(m_Backup_Function->GetStringSelection() == _("Differential")){
		wxDirDialog dialog(this,_("Please select the folder to store your backups"),m_Backup_Location->GetValue());
		if (dialog.ShowModal() == wxID_OK){
			m_Backup_Location->SetValue(dialog.GetPath());
		}
	}
	else{
		wxString strWildcard;
		if(m_Backup_Format->GetStringSelection() == _("7-Zip")){
			strWildcard = wxT("7 Zip (*.7z)|*.7z|All Files (*.*)|*.*");
		}
		else if(m_Backup_Format->GetStringSelection() == _("Zip")){
			strWildcard = wxT("Zip Files (*.zip)|*.zip|All Files (*.*)|*.*");
		}		
		wxFileDialog dialog(this,_("Please choose a file to backup to"), wxPathOnly(m_Backup_Location->GetValue()), wxEmptyString, strWildcard, wxFD_SAVE);
		if (dialog.ShowModal() == wxID_OK) {
			m_Backup_Location->SetValue(dialog.GetPath());
		}
	}
}

//ID_SYNC_OK
void frmMain::OnSyncOKClick(wxCommandEvent& event){
	Run(m_Sync_Rules->GetStringSelection(), wxT("Sync"));
}

//ID_BACKUP_OK
void frmMain::OnBackupOKClick(wxCommandEvent& event){
	Run(m_Backup_Rules->GetStringSelection(), wxT("Backup"));
}

//ID_SECURE_OK
void frmMain::OnSecureOKClick(wxCommandEvent& event){
	Run(m_Secure_Rules->GetStringSelection(), wxT("Secure"));
	m_Secure_DirCtrl->ReCreateTree();
}

//ID_SYNC_PREVIEW
void frmMain::OnSyncPreviewClick(wxCommandEvent& event){
	wxString strPath = m_Sync_Source_Txt->GetValue();
	/*Stupid horrible hack as folders do not have a trailing slash whereas roots do, nasty horrible code that needs to be changed
	in the future in the vdtc code*/
	if (strPath[strPath.length()-1] == wxFILE_SEP_PATH) {
		strPath = strPath.Left(strPath.Length() - 1); 
	}
	m_Sync_Source_Txt->SetValue(strPath);
	strPath = m_Sync_Dest_Txt->GetValue();
	if (strPath[strPath.length()-1] == wxFILE_SEP_PATH) {
		strPath = strPath.Left(strPath.Length() - 1); 
	}
	//Get the rules
	
	Rules rules;
	if (m_Sync_Rules->GetStringSelection() != wxEmptyString) {
		rules.TransferFromFile(m_Sync_Rules->GetStringSelection());
	}
	
	m_Sync_Dest_Tree->SetRules(rules);
	
	//Set the other tree too incase it is a unidirectional function
	m_Sync_Source_Tree->SetMode(m_Sync_Function->GetStringSelection());	
	
	m_Sync_Dest_Txt->SetValue(strPath);
	m_Sync_Dest_Tree->DeleteAllItems();
	m_Sync_Dest_Tree->AddRoot(wxT("Hidden root"));
	m_Sync_Dest_Tree->SetRoot(Normalise(Normalise(m_Sync_Dest_Txt->GetValue())));
	m_Sync_Dest_Tree->SetRootOpp(Normalise(Normalise(m_Sync_Source_Txt->GetValue())));
	m_Sync_Dest_Tree->SetPreview(true);
	m_Sync_Dest_Tree->SetMode(m_Sync_Function->GetStringSelection());
	m_Sync_Dest_Tree->AddNewPath(Normalise(Normalise(m_Sync_Dest_Txt->GetValue())));
			
	//Code for equalise function
	if(m_Sync_Function->GetStringSelection() == _("Equalise")){
		m_Sync_Source_Tree->SetRules(rules);
		m_Sync_Source_Tree->DeleteAllItems();
		m_Sync_Source_Tree->AddRoot(wxT("Hidden root"));
		m_Sync_Source_Tree->SetRoot(Normalise(Normalise(m_Sync_Source_Txt->GetValue())));
		m_Sync_Source_Tree->SetRootOpp(Normalise(Normalise(m_Sync_Dest_Txt->GetValue())));
		m_Sync_Source_Tree->SetPreview(true);
		m_Sync_Source_Tree->SetMode(m_Sync_Function->GetStringSelection());
		m_Sync_Source_Tree->AddNewPath(Normalise(Normalise(m_Sync_Source_Txt->GetValue())));
	}
}

//ID_BACKUP_PREVIEW
void frmMain::OnBackupPreviewClick(wxCommandEvent& event){
	if(m_Backup_Rules->GetStringSelection() != wxEmptyString){
		//Create a new rule set and populate it from the form
		Rules rules;
		if (m_Backup_Rules->GetStringSelection() != wxEmptyString){
			rules.TransferFromFile(m_Backup_Rules->GetStringSelection());
		}
		//Set up the tree ctrl for previewing
		m_Backup_TreeCtrl->SetPreview(true);
		m_Backup_TreeCtrl->SetRules(rules);
		//Delete all items and re-add the root
		m_Backup_TreeCtrl->DeleteAllItems();
		m_Backup_TreeCtrl->AddRoot(wxT("Hidden root"));
		for (unsigned int i = 0; i < m_BackupLocations->GetCount(); i++){
			//Loop through all the the filenames listed in the array and read them to the tree
			m_Backup_TreeCtrl->AddNewPath(Normalise(Normalise(m_BackupLocations->Item(i))));
		}
		//Turn off preview
		m_Backup_TreeCtrl->SetPreview(false);
	}
}

//ID_SECURE_PREVIEW
void frmMain::OnSecurePreviewClick(wxCommandEvent& event){
	if(m_Secure_Rules->GetStringSelection() != wxEmptyString){
		//Create a new rule set and populate it from the form
		Rules rules;
		if (m_Secure_Rules->GetStringSelection() != wxEmptyString) {
			rules.TransferFromFile(m_Secure_Rules->GetStringSelection());
		}
		//Set up the tree ctrl for previewing
		m_Secure_TreeCtrl->SetPreview(true);
		m_Secure_TreeCtrl->SetRules(rules);
		//Delete all items and re-add the root
		m_Secure_TreeCtrl->DeleteAllItems();
		m_Secure_TreeCtrl->AddRoot(wxT("Hidden root"));
		for (unsigned int i = 0; i < m_SecureLocations->GetCount(); i++) {
			//Loop through all the the filenames listed in the array and readd them to the tree
			m_Secure_TreeCtrl->AddNewPath(Normalise(Normalise(m_SecureLocations->Item(i))));
		}
		//Turn off preview
		m_Secure_TreeCtrl->SetPreview(false);
	}
}

//ID_BACKUP_RESTORE
void frmMain::OnBackupRestoreClick(wxCommandEvent& event){
	frmRestore *window = new frmRestore(NULL, ID_FRMRESTORE, _("Restore"));	
	window->ShowModal();
}

void frmMain::OnCloseWindow(wxCloseEvent& event){
	wxGetApp().m_Settings->SetPosition(m_Notebook->GetPageText(m_Notebook->GetSelection()));
	wxGetApp().m_Settings->SetTabStyle(m_Settings_TabStyle->GetStringSelection());
	wxGetApp().m_Settings->SetLanguageCode(wxLocale::FindLanguageInfo(m_Settings_Language->GetStringSelection())->CanonicalName);
	wxGetApp().m_Settings->SetFont(m_Settings_Font->GetSelectedFont().GetNativeFontInfoDesc());
	wxGetApp().m_Settings->SetRememberSync(m_Settings_RememberSync->GetValue());
	wxGetApp().m_Settings->SetRememberBackup(m_Settings_RememberBackup->GetValue());
	wxGetApp().m_Settings->SetRememberSecure(m_Settings_RememberSecure->GetValue());
	
	SyncData data;
	data.SetName(wxT("SyncRemember"));
	data.TransferFromForm();
	data.TransferToFile();
	
	BackupData bdata;
	bdata.SetName(wxT("BackupRemember"));
	bdata.TransferFromForm();
	bdata.TransferToFile();
	
	SecureData sdata;
	sdata.SetName(wxT("SecureRemember"));
	sdata.TransferFromForm();
	sdata.TransferToFile();
	
	//Set the height and width to be relative to allow Toucan to fit properly when resolution is changed
	int height, width, x, y;
	wxClientDisplayRect(&x, &y, &width, &height);
	
	wxGetApp().m_Settings->SetHeight((double)(this->GetSize().GetHeight())/(height));
	wxGetApp().m_Settings->SetWidth((double)(this->GetSize().GetWidth())/(width));
	
	wxGetApp().m_Settings->SetX((double)(this->GetScreenPosition().x)/(width));
	wxGetApp().m_Settings->SetY((double)(this->GetScreenPosition().y)/(height));
	
	wxGetApp().m_Settings->TransferToFile();
	
	menuTree = NULL;
	menuRules = NULL;
	
	delete m_BackupLocations;
	delete m_SecureLocations;
	
	wxGetApp().MainWindow->Destroy();
	wxGetApp().ProgressWindow->Destroy();
}

//ID_VARIABLES_ADD
void frmMain::OnVariablesAddClick(wxCommandEvent& event){	
    wxTextEntryDialog dialog(this, _("Please enter the name for the new portable variable"), _("New Portable Variable") ,wxEmptyString, wxOK|wxCANCEL);
	if(dialog.ShowModal() == wxID_OK){
        if(!wxGetApp().m_Variables_Config->HasGroup(dialog.GetValue())){
            m_Variables_List->ClearAll();
            wxListItem column;
            m_Variables_List->InsertColumn(0, column);
            m_Variables_List->InsertColumn(1, column);
            wxGetApp().m_Variables_Config->Write(dialog.GetValue() + wxT("/") + wxGetFullHostName() , wxEmptyString);
            wxGetApp().m_Variables_Config->Write(dialog.GetValue() + wxT("/") + _("Other") , wxEmptyString);
            wxGetApp().m_Variables_Config->Flush();
            m_Variables_Name->Append(dialog.GetValue());
            m_Variables_Name->SetStringSelection(dialog.GetValue());
            m_Variables_List->InsertItem(0, wxT("Test"));
            m_Variables_List->SetItem(0, 0, wxGetFullHostName() );
            m_Variables_List->InsertItem(1, wxT("Test"));
            m_Variables_List->SetItem(1, 0, _("Other"));
        }
        else{
            ErrorBox(_("There is already a Portable Variable with this name."));
        }
    }
}

//ID_VARIABLES_REMOVE
void frmMain::OnVariablesRemoveClick(wxCommandEvent& event){	
	wxGetApp().m_Variables_Config->DeleteGroup(m_Variables_Name->GetValue());
	wxGetApp().m_Variables_Config->Flush();
	m_Variables_Name->Delete(m_Variables_Name->GetSelection());
	m_Variables_Name->SetValue(wxEmptyString);
	m_Variables_List->ClearAll();
	wxListItem column;
	m_Variables_List->InsertColumn(0, column);
	m_Variables_List->InsertColumn(1, column);
}

//ID_VARIABLES_NAME
void frmMain::OnVariablesNameSelected(wxCommandEvent& event){	
	m_Variables_List->DeleteAllItems();
	
    wxGetApp().m_Variables_Config->SetPath(m_Variables_Name->GetValue());
    
	long dummy;
    wxString str;
    int i = 0;
	
    bool bCont = wxGetApp().m_Variables_Config->GetFirstEntry(str, dummy);
    while(bCont) {
        m_Variables_List->InsertItem(i, wxT("Test"));
        m_Variables_List->SetItem(i, 0, str);
		//Return the config to the top level
        wxGetApp().m_Variables_Config->SetPath(wxT("/"));
        wxString strTest = wxGetApp().m_Variables_Config->Read(m_Variables_Name->GetValue() + wxT("/") + str, wxT("Cannot Find Value"));
        m_Variables_List->SetItem(i, 1, strTest);
		//Put the config location back
        wxGetApp().m_Variables_Config->SetPath(m_Variables_Name->GetValue());    
        bCont = wxGetApp().m_Variables_Config->GetNextEntry(str, dummy);
        i++;
    }
	m_Variables_List->SetColumnWidth(0, -1);
	m_Variables_List->SetColumnWidth(1, -1);
	SetTitleBarText();
    wxGetApp().m_Variables_Config->SetPath(wxT("/"));
}

//ID_VARIABLES_ADDITEM
void frmMain::OnVariablesAddItemClick(wxCommandEvent& event){	
	int j = m_Variables_List->GetItemCount();
	m_Variables_List->InsertItem(j, wxT("Test"));
	m_Variables_List->SetItem(j, 0, wxGetFullHostName() );
	wxString caption = _("Choose a directory");
	wxString wildcard = wxT("All Files (*.*)|*.*");
	wxString defaultFilename = wxEmptyString;
	wxString defaultDir = wxT("/");
	wxString strPath;
	wxDirDialog dialog(this, caption, defaultDir);
	if (dialog.ShowModal() == wxID_OK){
		m_Variables_List->SetItem(j, 1, dialog.GetPath());
	}
	else{
		m_Variables_List->SetItem(j, 1, wxEmptyString);
	}
	wxGetApp().m_Variables_Config->Write(m_Variables_Name->GetValue() + wxT("/") + wxGetFullHostName() , dialog.GetPath());
	wxGetApp().m_Variables_Config->Flush();
}

//ID_VARIABLES_REMOVEITEM
void frmMain::OnVariablesRemoveItemClick(wxCommandEvent& event){	
	wxString selected;
	long item = -1;
	for ( ;; )
	{
		item = m_Variables_List->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
		if ( item == -1 )
		break;
		selected = m_Variables_List->GetItemText(item);
		m_Variables_List->DeleteItem(item);
	}
	wxGetApp().m_Variables_Config->DeleteEntry(m_Variables_Name->GetValue() + wxT("/") + selected);
	wxGetApp().m_Variables_Config->Flush();
}

//ID_VARIABLES_LIST
void frmMain::OnVariablesListActivated(wxListEvent& event){	
	long item = -1;
	for ( ;; )
	{
		item = m_Variables_List->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
		if ( item == -1 )
		break;
		
		wxListItem PreviousColumn1;
		PreviousColumn1.m_itemId = item;
		PreviousColumn1.m_col = 1;
		PreviousColumn1.m_mask = wxLIST_MASK_TEXT;
		m_Variables_List->GetItem(PreviousColumn1);
		wxString strPreviousColumn1 = PreviousColumn1.m_text;
		wxTextEntryDialog location(this, _("Please insert the location you want to point to."), _("Location"), strPreviousColumn1, wxOK);
		if(location.ShowModal() == wxID_OK){
			m_Variables_List->SetItem(item , 1, location.GetValue());
		}
		m_Variables_List->SetColumnWidth(0, -1);
		m_Variables_List->SetColumnWidth(1, -1);
		wxGetApp().m_Variables_Config->Write(m_Variables_Name->GetValue() + wxT("/") + m_Variables_List->GetItemText(item), location.GetValue());
		wxGetApp().m_Variables_Config->Flush();
	}
}

//ID_SCRIPT_EXECUTE
void frmMain::OnScriptExecute(wxCommandEvent& event){	
	wxArrayString arrLines;
	for(signed int i = 0; i < m_Script_Rich->GetNumberOfLines(); i++){
		arrLines.Add(m_Script_Rich->GetLineText(i));
	}
	wxGetApp().m_Script->SetScript(arrLines);
	wxGetApp().m_Script->Execute();
}

//ID_SCRIPT_NAME
void frmMain::OnScriptSelected(wxCommandEvent& event){	
	m_Script_Rich->Clear();
	wxString strFile = 	wxGetApp().m_Scripts_Config->Read(m_Script_Name->GetStringSelection() + wxT("/") + wxT("Script"));
	wxArrayString arrContents = StringToArrayString(strFile, wxT("|"));
	unsigned int i;
	for(i = 0; i < arrContents.Count() - 1; i++){
		m_Script_Rich->AppendText(arrContents.Item(i) + wxT("\r\n"));
	}
	m_Script_Rich->AppendText(arrContents.Item(i));
	SetTitleBarText();
}

//ID_SCRIPT_SAVE
void frmMain::OnScriptSaveClick(wxCommandEvent& event){	
	wxArrayString arrContents;
	for(signed int i = 0; i < m_Script_Rich->GetNumberOfLines(); i++){
		arrContents.Add(m_Script_Rich->GetLineText(i));
	}
	wxGetApp().m_Scripts_Config->Write(m_Script_Name->GetStringSelection() + wxT("/") + wxT("Script"), ArrayStringToString(arrContents, wxT("|")));
	wxGetApp().m_Scripts_Config->Flush();
}

//ID_SCRIPT_REMOVE
void frmMain::OnScriptRemoveClick(wxCommandEvent& event){	
	wxGetApp().m_Scripts_Config->DeleteGroup(m_Script_Name->GetStringSelection());
	wxGetApp().m_Scripts_Config->Flush();
	m_Script_Name->Delete(m_Script_Name->GetSelection());
}

//ID_SCRIPT_ADD
void frmMain::OnScriptAddClick(wxCommandEvent& event){	
	wxTextEntryDialog* dialog = new wxTextEntryDialog(this, _("Please enter the name for the new script"), _("New Script") ,wxEmptyString, wxOK|wxCANCEL);
	if (dialog->ShowModal() == wxID_OK) {
		m_Script_Name->Append(dialog->GetValue());
		m_Script_Name->SetStringSelection(dialog->GetValue());
	}
	delete dialog;
}

//ID_BACKUP_RATIO
void frmMain::OnBackupRatioChanged(wxScrollEvent& event){
	SetSliderText();
}

//ID_BACKUP_ADDVAR
void frmMain::OnBackupAddVarClick(wxCommandEvent& event){
	frmVariable* window = new frmVariable(NULL, ID_FRMVARIABLE, _("Insert Variable"));
	if(window->ShowModal() == wxID_OK){
		m_BackupLocations->Add(window->m_Location_Text->GetValue());
		m_Backup_TreeCtrl->AddNewPath(Normalise(Normalise(window->m_Preview_Text->GetValue())));
	}
	delete window;
}

//ID_SECURE_ADDVAR
void frmMain::OnSecureAddVarClick(wxCommandEvent& event){
	frmVariable* window = new frmVariable(NULL, ID_FRMVARIABLE, _("Insert Variable"));
	if(window->ShowModal() == wxID_OK){
		m_SecureLocations->Add(window->m_Location_Text->GetValue());
		m_Secure_TreeCtrl->AddNewPath(Normalise(Normalise(window->m_Preview_Text->GetValue())));
	}
	delete window;	
}

//wxID_ABOUT
void frmMain::OnAboutClick(wxCommandEvent& event){
	wxAboutDialogInfo info;
	info.SetName(wxT("Toucan"));
	info.SetVersion(wxT("2.0.6"));
	info.SetCopyright(wxT("(C) 2006-2008 Steven Lamerton \nName by Danny Mensingh\nMain icons by Neorame\nOther icons by Silvestre Herrera\nExtra thanks to Jorgen Bodde for his awesome wxVirtualDirTreeCtrl\n7Zip, hashlib++ and ccrypt are by their respective teams.\nAll items (C) their owners."));
	info.SetWebSite(wxT("http://portableapps.com/toucan"));
	info.SetLicense(wxT("Toucan and its component parts are all licensed under the GNU GPL Version 2 or a compatible license."));
	info.SetTranslators(GetTranslatorNames());
	wxAboutBox(info);
}

//ID_BACKUP_TREECTRL
void frmMain::OnBackupTreeCtrlTooltip(wxTreeEvent& event){
	if(m_Backup_TreeCtrl->GetItemParent(event.GetItem()) == m_Backup_TreeCtrl->GetRootItem()){
		for(unsigned int i = 0; i < m_BackupLocations->GetCount(); i++){
			if(Normalise(Normalise(m_BackupLocations->Item(i))) == m_Backup_TreeCtrl->GetItemText(event.GetItem())){
				event.SetToolTip(m_BackupLocations->Item(i));
			}
		}
	}
}

//ID_SECURE_TREECTRL
void frmMain::OnSecureTreeCtrlTooltip(wxTreeEvent& event){
	if(m_Secure_TreeCtrl->GetItemParent(event.GetItem()) == m_Secure_TreeCtrl->GetRootItem()){
		for(unsigned int i = 0; i < m_SecureLocations->GetCount(); i++){
			if(Normalise(Normalise(m_SecureLocations->Item(i))) == m_Secure_TreeCtrl->GetItemText(event.GetItem())){
				event.SetToolTip(m_SecureLocations->Item(i));
			}
		}
	}	
}

//ID_SYNC_SOURCE_TREE
void frmMain::OnSyncSourceTreeRightClick(wxTreeEvent& event){
	menuTree = m_Sync_Source_Tree;
	menuRules = m_Sync_Rules;
	CreateMenu(event);
}

//ID_SYNC_DEST_TREE
void frmMain::OnSyncDestTreeRightClick(wxTreeEvent& event){
	menuTree = m_Sync_Dest_Tree;
	menuRules = m_Sync_Rules;
	CreateMenu(event);
}

//ID_BACKUP_TREECTRL
void frmMain::OnBackupTreeRightClick(wxTreeEvent& event){
	menuTree = m_Backup_TreeCtrl;
	menuRules = m_Backup_Rules;
	CreateMenu(event);
}

//ID_SECURE_TREECTRL
void frmMain::OnSecureTreeRightClick(wxTreeEvent& event){
	menuTree = m_Secure_TreeCtrl;
	menuRules = m_Secure_Rules;
	CreateMenu(event);
}

//ID_SYNC_SOURCE_EXPAND
void frmMain::OnSyncSourceExpandClick(wxCommandEvent& event){
	m_Sync_Source_Tree->NeatExpandAll(this);
}

//ID_SYNC_DEST_EXPAND
void frmMain::OnSyncDestExpandClick(wxCommandEvent& event){
	m_Sync_Dest_Tree->NeatExpandAll(this);
}

//ID_BACKUP_EXPAND
void frmMain::OnBackupExpandClick(wxCommandEvent& event){
	m_Backup_TreeCtrl->NeatExpandAll(this);
}

//ID_SECURE_EXPAND
void frmMain::OnSecureExpandClick(wxCommandEvent& event){
	m_Secure_TreeCtrl->NeatExpandAll(this);
}
//ID_SCRIPTFINISH
void frmMain::OnScriptFinish(wxCommandEvent& event){
	if(wxGetApp().m_Script->GetCommand() < wxGetApp().m_Script->GetCount()){
		wxGetApp().m_Script->SetCommand(wxGetApp().m_Script->GetCommand() + 1);
		wxGetApp().m_Script->ParseCommand(wxGetApp().m_Script->GetCommand() - 1);
	}
	else{
		wxGetApp().m_Script->CleanUp();
		wxGetApp().m_Script->SetCommand(wxGetApp().m_Script->GetCommand() + 1);
	}
}

//ID_AUINOTEBOOK
void frmMain::OnTabChanged(wxAuiNotebookEvent& event){
	SetTitleBarText();
}

void frmMain::SetTitleBarText(){
	wxString strTitle = wxT("Toucan - ");
	if(m_Notebook->GetPageText(m_Notebook->GetSelection()) == _("Sync")){
		strTitle = strTitle + m_Sync_Job_Select->GetStringSelection();
	}
	else if(m_Notebook->GetPageText(m_Notebook->GetSelection()) == _("Backup")){
		strTitle = strTitle + m_Backup_Job_Select->GetStringSelection();
	}
	else if(m_Notebook->GetPageText(m_Notebook->GetSelection()) == _("Secure")){
		strTitle = strTitle + m_Secure_Job_Select->GetStringSelection();
	}
	else if(m_Notebook->GetPageText(m_Notebook->GetSelection()) == _("Script")){
		strTitle = strTitle + m_Script_Name->GetStringSelection();
	}
	else if(m_Notebook->GetPageText(m_Notebook->GetSelection()) == _("Rules")){
		strTitle = strTitle + m_Rules_Name->GetStringSelection();
	}
	else if(m_Notebook->GetPageText(m_Notebook->GetSelection()) == _("Variables")){
		strTitle = strTitle +m_Variables_Name->GetStringSelection();
	}	
	if(strTitle == wxT("Toucan - ")){
		this->SetTitle(wxT("Toucan"));
	}
	else{
		this->SetTitle(strTitle);
	}
}

void frmMain::JobAdd(wxComboBox* box){
	wxTextEntryDialog dialog(this, wxEmptyString, _("Job name"));
	if (dialog.ShowModal() == wxID_OK){
		box->Append(dialog.GetValue());
		box->SetStringSelection(dialog.GetValue());
	}
	SetTitleBarText();
	ClearToDefault();
}

void frmMain::JobRemove(wxComboBox* box){
	if (box->GetStringSelection() != wxEmptyString) {
		wxGetApp().m_Jobs_Config->DeleteGroup(box->GetStringSelection());
		wxGetApp().m_Jobs_Config->Flush();
		box->Delete(box->GetSelection());
	}	
	SetTitleBarText();
	ClearToDefault();
}

void frmMain::JobSave(const wxString name, const wxString rules, const wxString type){
	wxBusyCursor cursor;
	RootData* data;
	if(type == wxT("Sync")){
		data = new SyncData();
	}
	else if(type == wxT("Backup")){
		data = new BackupData();
	}
	else if(type == wxT("Secure")){
		data = new SecureData();
	}
	else{
		return;
	}
	data->SetName(name);
	if (data->TransferFromForm()){
		if (data->GetName() != wxEmptyString) {
			data->TransferToFile();
			wxGetApp().m_Jobs_Config->Write(data->GetName() + wxT("/Rules"),  rules);
			wxGetApp().m_Jobs_Config->Write(data->GetName() + wxT("/Type"),  type);
			wxGetApp().m_Jobs_Config->Flush();
		}
		else {
			ErrorBox(_("Please chose a job to save to"));
		}
	} 
	delete data;
}

void frmMain::JobLoad(const wxString name, wxComboBox* rules, const wxString type){
	wxBusyCursor cursor;
	ClearToDefault();
	RootData* data;
	if(type == wxT("Sync")){
		data = new SyncData();
	}
	else if(type == wxT("Backup")){
		data = new BackupData();
	}
	else if(type == wxT("Secure")){
		data = new SecureData();
	}
	else{
		return;
	}
	data->SetName(name);
	if (data->TransferFromFile()){
		data->TransferToForm();
		rules->SetStringSelection(wxGetApp().m_Jobs_Config->Read(data->GetName() + wxT("/Rules")));
	}
	SetTitleBarText();
}

void frmMain::ClearToDefault(){
	if(m_Notebook->GetPageText(m_Notebook->GetSelection()) == _("Sync")){
		m_Sync_Function->SetStringSelection(_("Copy"));
		m_Sync_Timestamp->SetValue(false);
		m_Sync_Attributes->SetValue(false);
		m_Sync_Ignore_Readonly->SetValue(false);
		m_Sync_Ignore_DaylightS->SetValue(false);
		m_Sync_Rules->SetStringSelection(wxEmptyString);
		m_Sync_Source_Txt->SetValue(wxEmptyString);
		m_Sync_Dest_Txt->SetValue(wxEmptyString);
		m_Sync_Source_Tree->DeleteAllItems();
		m_Sync_Dest_Tree->DeleteAllItems();
	}
	if(m_Notebook->GetPageText(m_Notebook->GetSelection()) == _("Backup")){
		m_Backup_Function->SetStringSelection(_("Complete"));
		m_Backup_Format->SetStringSelection(_("Zip"));
		m_Backup_Ratio->SetValue(3);
		m_Backup_IsPass->SetValue(false);
		m_Backup_Location->SetValue(wxEmptyString);
		m_Backup_TreeCtrl->DeleteAllItems();
		m_BackupLocations->Clear();
	}
	if(m_Notebook->GetPageText(m_Notebook->GetSelection()) == _("Secure")){
		m_Secure_Function->SetStringSelection(_("Encrypt"));
		m_Secure_Rules->SetStringSelection(wxEmptyString);
		m_Secure_TreeCtrl->DeleteAllItems();
		m_SecureLocations->Clear();
	}
}

void frmMain::Run(const wxString rules, const wxString type){
	RootData* data;
	if(type == wxT("Sync")){
		data = new SyncData();
	}
	else if(type == wxT("Backup")){
		data = new BackupData();
	}
	else if(type == wxT("Secure")){
		data = new SecureData();
	}
	else{
		return;
	}
	data->SetName(wxT("Last") + type + wxT("Job"));
	if(data->TransferFromForm()){
		if(data->NeededFieldsFilled()){
			if(data->TransferToFile()){
				wxGetApp().m_Jobs_Config->Write(wxT("Last") + type + wxT("Job/Rules"),  rules);
				wxGetApp().m_Jobs_Config->Write(wxT("Last") + type + wxT("Job/Type"),  type);
				wxGetApp().m_Jobs_Config->Flush();	
				wxArrayString arrScript;
				arrScript.Add(type + wxT(" \"Last") + type + wxT("Job\""));
				wxGetApp().SetAbort(false);
				wxGetApp().m_Script->SetScript(arrScript);
				wxGetApp().m_Script->Execute();
			}
		}
		else{
			ErrorBox(_("Not all of the required fields are filled"));
		}
	}
	delete data;
}

void frmMain::CreateMenu(wxTreeEvent& event){
	menuTree->SelectItem(event.GetItem());
	wxString strMenuTitle = menuRules->GetStringSelection();
	if(strMenuTitle == wxEmptyString){
		strMenuTitle = _("Error");
	}
	wxMenu menu(strMenuTitle);
	if(wxFileExists(m_Secure_TreeCtrl->GetFullPath(event.GetItem()).GetFullPath())){
		menu.Append(ID_MENU_FILEEXCLUDE_EXTENSION, wxT("Exclude by extension"));
		menu.Append(ID_MENU_FILEEXCLUDE_NAME, wxT("Exclude by name"));
		menu.Append(ID_MENU_LOCATIONINCLUDE_EXTENSION, wxT("Include by extension"));
		menu.Append(ID_MENU_LOCATIONINCLUDE_NAME, wxT("Include by name"));
	}	
	else if(wxDirExists(m_Secure_TreeCtrl->GetFullPath(event.GetItem()).GetFullPath())){
		menu.Append(ID_MENU_FOLDEREXCLUDE_NAME, wxT("Exclude by name"));
		menu.Append(ID_MENU_LOCATIONINCLUDE_NAME, wxT("Include by name"));
	}	
	this->PopupMenu(&menu, event.GetPoint() + menuTree->GetPosition());
}

//ID_MENU_FILEEXCLUDE_EXTENSION
void frmMain::OnMenuFileExcludeExtensionClick(wxCommandEvent& event){
	if(menuRules->GetStringSelection() != wxEmptyString){
		Rules rules;
		rules.TransferFromFile(menuRules->GetStringSelection());
		wxArrayString arrFileExclude = rules.GetFilesToExclude();
		arrFileExclude.Add(wxT(".") + menuTree->GetFullPath(menuTree->GetSelection()).GetExt());
		rules.SetFilesToExclude(arrFileExclude);
		rules.TransferToFile(menuRules->GetStringSelection());
	}	
}

//ID_MENU_FILEEXCLUDE_NAME
void frmMain::OnMenuFileExcludeNameClick(wxCommandEvent& event){
	if(menuRules->GetStringSelection() != wxEmptyString){
		Rules rules;
		rules.TransferFromFile(menuRules->GetStringSelection());
		wxArrayString arrFileExclude = rules.GetFoldersToExclude();
		arrFileExclude.Add(menuTree->GetItemText(menuTree->GetSelection()));
		rules.SetFoldersToExclude(arrFileExclude);
		rules.TransferToFile(menuRules->GetStringSelection());
	}	
}

//ID_MENU_LOCATIONINCLUDE_EXTENSION
void frmMain::OnMenuLocationIncludeExtensionClick(wxCommandEvent& event){
	if(menuRules->GetStringSelection() != wxEmptyString){
		Rules rules;
		rules.TransferFromFile(menuRules->GetStringSelection());
		wxArrayString arrLocationInclude = rules.GetLocationsToInclude();
		arrLocationInclude.Add(wxT(".") + menuTree->GetFullPath(menuTree->GetSelection()).GetExt());
		rules.SetLocationsToInclude(arrLocationInclude);
		rules.TransferToFile(menuRules->GetStringSelection());
	}		
}

//ID_MENU_LOCATIONINCLUDE_NAME
void frmMain::OnMenuLocationIncludeNameClick(wxCommandEvent& event){
	if(menuRules->GetStringSelection() != wxEmptyString){
		Rules rules;
		rules.TransferFromFile(menuRules->GetStringSelection());
		wxArrayString arrLocationInclude = rules.GetLocationsToInclude();
		arrLocationInclude.Add(menuTree->GetItemText(menuTree->GetSelection()));
		rules.SetLocationsToInclude(arrLocationInclude);
		rules.TransferToFile(menuRules->GetStringSelection());
	}		
}

//ID_MENU_FOLDEREXCLUDE_NAME
void frmMain::OnMenuFolderExcludeNameClick(wxCommandEvent& event){
	if(menuRules->GetStringSelection() != wxEmptyString){
		Rules rules;
		rules.TransferFromFile(menuRules->GetStringSelection());
		wxArrayString arrFolderExclude = rules.GetFoldersToExclude();
		arrFolderExclude.Add(menuTree->GetItemText(menuTree->GetSelection()));
		rules.SetFoldersToExclude(arrFolderExclude);
		rules.TransferToFile(menuRules->GetStringSelection());
	}		
}

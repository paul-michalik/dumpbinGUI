// dumpbinGUI v 1.1.0.1
// Copyright(c) 2000-2003 tim tabor.  
// All rights reserved.  No Warranties extended.

// ContextMenu.h : Declaration of the CContextMenu

#pragma once
#include "resource.h"       // main symbols
#include "ContextMenuImpl.h"

// IAutoObject
[
	object,
	uuid("13447646-25E9-40cb-921B-967999167CD5"),
	oleautomation,	
	helpstring("IRegDialog Interface"),
	pointer_default(unique)
]
__interface IRegDialog : IUnknown
{
	[id(1), helpstring("method ShowDialog")] HRESULT ShowDialog();
};

// CContextMenu

[
	coclass,
	threading("apartment"),
	vi_progid("dumpbinCMH.ContextMenu"),
	progid("dumpbinCMH.ContextMenu.1"),
	version(1.0),
	uuid("8F697706-E1B2-4BF7-8C64-89904C409DE6"),
	com_interface_entry ("COM_INTERFACE_ENTRY_IID(IID_IContextMenu, IContextMenu)"),
	helpstring("ContextMenu Class")
]
class ATL_NO_VTABLE CContextMenu : 
	public CContextMenuImpl<CContextMenu, IDR_MENU, 0>,
	public IRegDialog,
	public CMessageMap
{
public:
public:
	CContextMenu();
	~CContextMenu();

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_MSG_MAP(CContextMenu)
		COMMAND_ID_HANDLER(ID_ABOUT, OnAbout)
		COMMAND_ID_HANDLER(ID_ASSOCIATE, OnAssociate)
		COMMAND_ID_HANDLER(ID_README, OnReadMe)
		MESSAGE_HANDLER(WM_COMMAND, OnCommand)	// all other commands
	END_MSG_MAP()

	HRESULT FinalConstruct();
	void FinalRelease();

public:
	// CMessageMap
	LRESULT OnReadMe(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnAbout(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnAssociate(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

// IRegDialog
	HRESULT	ShowDialog();

private:
	void SpawnDumpbinGUI(LPCTSTR pszFlag);
	bool GetDumpbinGUIPath(CString& strPath);

};


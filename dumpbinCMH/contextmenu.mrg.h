// Created by Microsoft (R) C/C++ Compiler Version 13.00.9466
//
// e:\projectsatl\dumpbingui\source\dumpbincmh\contextmenu.mrg.h
// compiler-generated file created 11/08/03 at 22:41:39
//
// This C++ source file is intended to be a representation of the
// source code injected by the compiler.  It may not compile or
// run exactly as the original source file.
//


//+++ Start Injected Code
[no_injected_text(true)];      // Suppress injected text, it has already been injected
#pragma warning(disable: 4543) // Suppress warnings about skipping injected text
#pragma warning(disable: 4199) // Suppress warnings from attribute providers

#pragma message("\n\nNOTE: This merged source file should be visually inspected for correctness.\n\n")
//--- End Injected Code

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
,
    /*+++ Added Baseclass */ public CComCoClass<CContextMenu, &__uuidof(CContextMenu)>,
    /*+++ Added Baseclass */ public CComObjectRootEx<CComSingleThreadModel>,
    /*+++ Added Baseclass */ public IProvideClassInfoImpl<&__uuidof(CContextMenu)>
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


	//+++ Start Injected Code For Attribute 'coclass'
#injected_line 34 "e:\\projectsatl\\dumpbingui\\source\\dumpbincmh\\contextmenu.h"
    static DWORD* GetOpCodes() 
    {
        static DWORD rgOps[] = 
            					{
            						0x70000000,
            0x30004000,
            0x80000002,
            0x50000000,
            0x3000c000,
            0x80000004,
            0x60000000,
            0x30014000,
            0x80000002,
            0x50000000,
            0x3000c000,
            0x80000004,
            0x30018000,
            0x80000001,
            0x60000000,
            0x1000c000,
            0x3000c000,
            0x50000000,
            0x20010000,
            0x30010000,
            0x80000002,
            0x50000000,
            0x3001c000,
            0x80000001,
            0x30020000,
            0x80000005,
            0x20024000,
            0x30024000,
            0x30028000,
            0x8000000b,
            0x50000000,
            0x8003000d,
            0x60000000,
            0x8003800f,
            0x30040000,
            0x80000011,
            0x60000000,
            0x60000000,
            0x60000000,
            0
            					};
        return rgOps;
    }
#injected_line 34 "e:\\projectsatl\\dumpbingui\\source\\dumpbincmh\\contextmenu.h"
    static RGSDWORD* GetOpcodeDWORDVals() 
    {
        static RGSDWORD rgDWORDS[] = 
            					{
            						{0, 0}
            					};
        return rgDWORDS;
    }
#injected_line 34 "e:\\projectsatl\\dumpbingui\\source\\dumpbincmh\\contextmenu.h"
    static RGSBinary* GetOpcodeBinaryVals() 
    {

        static RGSBinary rgBinary[] = 
            					{
            						{0, 0}
            					};
        return rgBinary;
    }
#injected_line 34 "e:\\projectsatl\\dumpbingui\\source\\dumpbincmh\\contextmenu.h"
    static RGSStrings* GetOpcodeStringVals() 
    {
        static RGSStrings rgStrings[] = 
            					{
            						{_T(""),0 }, 
            {_T("dumpbinCMH.ContextMenu.1"),0 }, 
            {_T("%FriendlyName%"),1 }, 
            {_T("CLSID"),0 }, 
            {_T("{8F697706-E1B2-4BF7-8C64-89904C409DE6}"),0 }, 
            {_T("dumpbinCMH.ContextMenu"),0 }, 
            {_T("CurVer"),0 }, 
            {_T("ProgID"),0 }, 
            {_T("VersionIndependentProgID"),0 }, 
            {_T("Programmable"),0 }, 
            {_T("%MODULETYPE%"),1 }, 
            {_T("%MODULE%"),1 }, 
            {_T("ThreadingModel"),0 }, 
            {_T("apartment"),0 }, 
            {_T("AppID"),0 }, 
            {_T("%APPID%"),1 }, 
            {_T("TypeLib"),0 }, 
            {_T("%MODULEGUID%"),1 }, 
            {NULL, 0}
            					};
        return rgStrings;
    }
#injected_line 34 "e:\\projectsatl\\dumpbingui\\source\\dumpbincmh\\contextmenu.h"
    static HRESULT WINAPI UpdateRegistry(BOOL bRegister) 
    {
        CRegistryVirtualMachine rvm;
        HRESULT hr;
        if (FAILED(hr = rvm.AddStandardReplacements()))
            return hr;
        rvm.AddReplacement(_T("FriendlyName"), GetObjectFriendlyName());
        return rvm.VMUpdateRegistry(GetOpCodes(), GetOpcodeStringVals(), GetOpcodeDWORDVals(), GetOpcodeBinaryVals(), bRegister);
    }
#injected_line 34 "e:\\projectsatl\\dumpbingui\\source\\dumpbincmh\\contextmenu.h"
    static const TCHAR* GetObjectFriendlyName() 
    {
        return _T("CContextMenu Object");
    }
#injected_line 34 "e:\\projectsatl\\dumpbingui\\source\\dumpbincmh\\contextmenu.h"
    static const TCHAR* GetProgID() 
    {
        return _T("dumpbinCMH.ContextMenu.1");
    }
#injected_line 34 "e:\\projectsatl\\dumpbingui\\source\\dumpbincmh\\contextmenu.h"
    static const TCHAR* GetVersionIndependentProgID() 
    {
        return _T("dumpbinCMH.ContextMenu");
    }
#injected_line 34 "e:\\projectsatl\\dumpbingui\\source\\dumpbincmh\\contextmenu.h"
    BEGIN_COM_MAP(CContextMenu)
        COM_INTERFACE_ENTRY_IID(IID_IContextMenu, IContextMenu)
        COM_INTERFACE_ENTRY(IShellExtInit)
        COM_INTERFACE_ENTRY(IRegDialog)
        COM_INTERFACE_ENTRY(IProvideClassInfo)
    END_COM_MAP()

	//--- End Injected Code For Attribute 'coclass'
};

//+++ Start Injected Code For Attribute 'coclass'
#injected_line 34 "e:\\projectsatl\\dumpbingui\\source\\dumpbincmh\\contextmenu.h"

				OBJECT_ENTRY_AUTO(__uuidof(CContextMenu), CContextMenu)
			
//--- End Injected Code For Attribute 'coclass'



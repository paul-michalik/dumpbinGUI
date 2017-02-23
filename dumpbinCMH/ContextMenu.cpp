// dumpbinGUI v 1.1.0.1
// Copyright(c) 2000-2003 tim tabor.  
// All rights reserved.  No Warranties extended.

// ContextMenu.cpp : Implementation of ContextMenu

#include "stdafx.h"
#include "resource.h"
#include "ContextMenu.h"
#include "About.h"
#include "AssociateDlg.h"
#include "..\dumpbinGUI\Msg.h"

#pragma warning(disable : 4311)	// type cast - pointer truncation

// ContextMenu

CContextMenu::CContextMenu()
{
}

CContextMenu::~CContextMenu()
{
	if (m_hMenuBar)
	{
	}
}

HRESULT CContextMenu::FinalConstruct()
{
	return S_OK;
}

void CContextMenu::FinalRelease() 
{
}

bool CContextMenu::GetDumpbinGUIPath(CString& strPath)
{
	TCHAR szShortPath[MAX_PATH];
	GetModuleFileName(_AtlBaseModule.GetModuleInstance(), szShortPath, LENGTHOF(szShortPath));

	GetLongPath(szShortPath, strPath);
	LPTSTR psz = strPath.GetBuffer(512);
	PathRemoveFileSpec(psz);
	PathAppend(psz, _T("dumpbinGUI.exe"));
	strPath.ReleaseBuffer();

	return PathFileExists(strPath) == TRUE;
}

LRESULT CContextMenu::OnAbout(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CAbout().DoModal();
	return 0;
}

LRESULT CContextMenu::OnAssociate(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CAssociateDlg().DoModal();
	return 0;
}

LRESULT CContextMenu::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CString strFlag;
	CString strHelp;

	if (!GetCommandStrings(LOWORD(wParam), strFlag, strHelp)) return 0;

	strFlag.Insert(0, _T('/'));
	SpawnDumpbinGUI(strFlag);

	return 0;
}

LRESULT CContextMenu::OnReadMe(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CString strPath;
	if ( !GetDumpbinGUIPath(strPath) )
	{
		ErrMsg(_T("Can't find dumpbinGUI.exe"), ERROR_FILE_NOT_FOUND);
		return 0;
	}

	CComPtr<IWebBrowser2>spIE;
	HRESULT hr = spIE.CoCreateInstance(CLSID_InternetExplorer);
	if (FAILED(hr))
	{
		ErrMsg(_T("Can't start Internet Explorer"), hr);
		return 0;
	}

	spIE->put_Visible(VARIANT_TRUE);
	HWND hWndIE;
	spIE->get_HWND((long*)&hWndIE);
	::SetForegroundWindow(hWndIE);

	CComBSTR bstrUrl(L"res://");
	bstrUrl += (LPCTSTR)strPath;
	bstrUrl += L"/ReadMe.htm";

	CComVariant vEmpty;
	spIE->Navigate(bstrUrl, &vEmpty, &vEmpty, &vEmpty, &vEmpty);

	return 0;

}

HRESULT CContextMenu::ShowDialog(void)
{
	CAssociateDlg().DoModal();

	return S_OK;
}

void CContextMenu::SpawnDumpbinGUI(LPCTSTR pszFlag)
{
	if (m_rgstrFileNames.GetCount() == 0) return;

	CString strPath;
	if ( !GetDumpbinGUIPath(strPath) )
	{
		ErrMsg(_T("Can't find dumpbinGUI.exe"), ERROR_FILE_NOT_FOUND);
		return;
	}

	CString strArgs;
	strArgs.Format(_T("%s \"%s\""), pszFlag, (LPCTSTR)m_rgstrFileNames.GetAt(0));

	LONG lr = (LONG)ShellExecute(0, _T("open"), strPath, strArgs, 0, SW_SHOW);
	if (lr < 32)
	{
		ErrMsg(_T("Can't start dumpbinGUI.exe"), lr);
	}
}
 


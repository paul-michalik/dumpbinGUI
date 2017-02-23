// dumpbinGUI v 1.1.0.1
// Copyright(c) 2000-2003 tim tabor.  
// All rights reserved.  No Warranties extended.

#pragma once

#include "shlobj.h"
#include <atlcoll.h>

#pragma warning(disable : 4311)	// type cast - pointer truncation

#ifndef HR
#define HR(_ex) { HRESULT _hr = _ex; if (FAILED(_hr)) return _hr; }
#endif

template <class T, UINT t_nMenuResId, int t_nSubmenuIndex>
class ATL_NO_VTABLE CContextMenuImpl : 
	public IShellExtInit,
	public IContextMenu
{
public:

	CContextMenuImpl(void) :
		m_hMenuBar(0),
		m_hShellExMenu(0)
	{
	}

	~CContextMenuImpl(void)
	{
		DestroyMenu(m_hMenuBar);
		m_hMenuBar = 0;
		m_hShellExMenu = 0;
	}

	STDMETHOD(Initialize)(LPCITEMIDLIST pidlFolder, IDataObject *lpdobj, HKEY hkeyProgID)
	{
		ATLTRACE(_T("CContextMenuImpl::IShellExtInit::Initialize"));

		HR(DragQueryFiles(lpdobj));

		return S_OK;
	}

	STDMETHOD(QueryContextMenu)(HMENU hContextMenu, UINT iContextMenuFirstItem, UINT idCmdFirst, UINT idCmdLast, UINT uFlags)
	{
		ATLTRACE(_T("CContextMenuImpl::ICContextMenu::QueryContextMenu\n"));

		if (uFlags & CMF_DEFAULTONLY)
		{
			return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, 0);
		}

		if (! GetShellExMenu()) return E_FAIL;

		CacheCommands(m_hShellExMenu, idCmdFirst);

		int nMenuItems = GetMenuItemCount(m_hShellExMenu);
		for (int i=0; i<nMenuItems; ++i)
		{
			MENUITEMINFO mii = { sizeof mii };
			TCHAR sz[128] = {0};
			mii.fMask = MIIM_TYPE | MIIM_ID | MIIM_SUBMENU;
			mii.fType = MFT_STRING;
			mii.cch = 128;
			mii.dwTypeData = sz;
			GetMenuItemInfo(m_hShellExMenu, i, TRUE, &mii);

			if (mii.hSubMenu == 0)
			{
				InsertMenu(hContextMenu, iContextMenuFirstItem + i, MF_STRING|MF_BYPOSITION, mii.wID, mii.dwTypeData);
			}
			else
			{
				InsertMenu(hContextMenu, iContextMenuFirstItem + i, MF_POPUP|MF_BYPOSITION, (UINT_PTR)mii.hSubMenu, mii.dwTypeData);
			}
		}

		return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, m_rgCmdId.GetCount());
	}


	// idCmd is either an index into my commands, it's NEVER the shell's cmdId
	STDMETHOD(GetCommandString)(UINT idCmd, UINT uStrType, UINT* pwReserved, LPSTR pszName, UINT cchMax)
	{
		ATLTRACE(_T("CContextMenuImpl::ICContextMenu::GetCommandString"));
		return E_NOTIMPL;

		//preserved for posterity
		//if (idCmd > m_rgCmdId.GetCount()) return E_INVALIDARG;

		//return m_rgCmdId[idCmd].GetCommandString(uStrType, pszName, cchMax);

	}

	// lpici->lpVerb is either an index into my commands or a string - it's NEVER the shell's cmdId
	STDMETHOD(InvokeCommand)(LPCMINVOKECOMMANDINFO lpici)
	{
		T* pT = static_cast<T*>(this);

		ATLTRACE(_T("CContextMenuImpl::ICContextMenu::InvokeCommand\n"));

		if (HIWORD(lpici->lpVerb) != 0) return 0;	//!! I should support verb strings

		int iCmd = (int)lpici->lpVerb;

		if (iCmd >= (int)m_rgCmdId.GetCount()) return E_INVALIDARG;

		LRESULT lRes = 0;
		pT->ProcessWindowMessage(0, WM_COMMAND, m_rgCmdId[(UINT)lpici->lpVerb], 0, lRes, 0);

		return S_OK;
	}

	bool GetShellExMenu()
	{
		m_hMenuBar = LoadMenu(_AtlBaseModule.GetResourceInstance(), MAKEINTRESOURCE(t_nMenuResId));
		if (m_hMenuBar == 0) return false;

		m_hShellExMenu = GetSubMenu(m_hMenuBar, t_nSubmenuIndex);

		return m_hShellExMenu != 0;
	}

	// change my command ID's to the values expected by the shell
	// cache my ID's so I can route the shell's commands to my handlers
	UINT CacheCommands(HMENU hMenu, UINT uShellCmdID)
	{
		int nMenuItems = GetMenuItemCount(hMenu);
		for (int i=0; i<nMenuItems; ++i)
		{
			MENUITEMINFO mii = { sizeof mii };
			TCHAR sz[128] = {0};
			mii.fMask = MIIM_ID | MIIM_SUBMENU;
			GetMenuItemInfo(hMenu, i, TRUE, &mii);
			if (mii.wID == 0)
			{
				// MFT_SEPARATOR
				continue;
			}
			else if (mii.hSubMenu != 0)
			{
				uShellCmdID = CacheCommands(mii.hSubMenu, uShellCmdID);
			}
			else
			{
				m_rgCmdId.Add(mii.wID);

				// adjust the menu's commands so the shell can use them
				mii.fMask = MIIM_ID;
				mii.wID = uShellCmdID++;
				SetMenuItemInfo(hMenu, i, TRUE, &mii);
			}
		}
		return uShellCmdID;
	}

	bool GetCommandStrings(UINT nId, CString& strVerb, CString& strHelp)
	{
		BOOL bOK = strVerb.LoadString(nId);
		if (!bOK) return false;

		int iSemiColon = strVerb.Find(_T(';'));
		if (iSemiColon != -1){
			strHelp = strVerb.Right(strVerb.GetLength() - iSemiColon - 1);
			strVerb = strVerb.Left(iSemiColon);
		}
		return true;
	}

private:
	inline HRESULT DragQueryFiles(IDataObject* pDataObj)
	{
		m_rgstrFileNames.RemoveAll();

		FORMATETC fe = {CF_HDROP, 0, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};
		STGMEDIUM stm;
		HRESULT hr = pDataObj->GetData(&fe, &stm);
		if (FAILED(hr)) return hr;

		HDROP hDrop = (HDROP)stm.hGlobal;

		int nFiles = DragQueryFile(hDrop, -1, 0, 0);
		for (int i=0; i < nFiles; ++i)
		{
			int nLen = DragQueryFile(hDrop, i, 0, 0);	// get string length
			nLen++;										// terminator

			TCHAR szPath[MAX_PATH];
			nLen = DragQueryFile(hDrop, i, szPath, nLen);
			if (nLen != 0){
				m_rgstrFileNames.Add(szPath);
			}
		}

		ReleaseStgMedium(&stm);

		return m_rgstrFileNames.GetCount() != 0 ? S_OK : E_FAIL;
	}

protected:
	inline DWORD GetLongPath(LPCTSTR pszShortPath, CString& strLongPath)
	{
		if (pszShortPath == 0 || pszShortPath[0] == 0) return 0;

		SetLastError(ERROR_GEN_FAILURE);
		CComPtr<IShellFolder> spShellFolder;
		HRESULT hr = SHGetDesktopFolder(&spShellFolder);
		if( FAILED(hr) ) return 0;

		ULONG dwEaten;
		LPITEMIDLIST pidl;
		hr = spShellFolder->ParseDisplayName(0, 0, CT2W(pszShortPath), &dwEaten, &pidl, 0);
		if( FAILED(hr) ) return 0;

		TCHAR szShellPathName[MAX_PATH];
		if (SHGetPathFromIDList(pidl, szShellPathName)){
			strLongPath = szShellPathName;
		}
		
		CComPtr<IMalloc> spMalloc;
		SHGetMalloc(&spMalloc);
		spMalloc->Free(pidl);
		pidl = 0;

		return strLongPath.GetLength();
	}

protected:

	CAtlArray<UINT> m_rgCmdId;
	CAtlArray<CString> m_rgstrFileNames;
	HMENU m_hMenuBar;
	HMENU m_hShellExMenu;

};

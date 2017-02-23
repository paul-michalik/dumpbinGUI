// dumpbinGUI v 1.1.0.1
// Copyright(c) 2000-2003 tim tabor.  
// All rights reserved.  No Warranties extended.

#include "stdafx.h"
#include "mainfrm.h"
#include "CmdArgs.h"
#include "about.h"
#include "msg.h"
#include "myshell.h"

#include "..\dumpbinCMH\_dumpbinCMH.h"

#pragma comment(lib, "shlwapi.lib")

CMainFrame::CMainFrame() :
	m_bGUIEnabled(true),
	m_nToolBarButtons(0)
{}

LRESULT CMainFrame::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CenterWindow();

	bool bOk = FindLinkExe();
	if (!bOk) return -1;

	HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
	CMenuHandle mainmenu = GetMenu();
	m_CmdBar.AttachMenu(mainmenu);
	SetMenu(0);
	m_CmdBar.LoadImages(IDR_MAINFRAME);
	m_viewmenu = mainmenu.GetSubMenu(VIEW_MENU_POSITION);

	m_toolbar = CreateSimpleToolBarCtrl(m_hWnd, IDR_MAINFRAME, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);
	m_nToolBarButtons = m_toolbar.GetButtonCount();
	UIAddToolBar(m_toolbar);

	CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
	AddSimpleReBarBand(hWndCmdBar);
	AddSimpleReBarBand(m_toolbar, 0, TRUE);
	CreateSimpleStatusBar();

	m_hWndClient = m_view.Create(m_hWnd, rcDefault, 0, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_HSCROLL | WS_VSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | ES_NOHIDESEL | ES_SAVESEL, WS_EX_CLIENTEDGE);

	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	ParseCommandLine();

	PostMessage(AM_POSTCREATE);

	return 0;
}

// this is handled *after* the window is created
LRESULT CMainFrame::OnPostCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	UpdateView();

	return 0;
}

LRESULT CMainFrame::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (m_bGUIEnabled) 
	{
		bHandled = FALSE;
	}
	else 
	{
		CString strCaption;
		strCaption.LoadString(IDR_MAINFRAME);

		Msg(_T("Busy!"), MB_ICONEXCLAMATION);
	}

	return 0;
}

LRESULT CMainFrame::OnDropFiles(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HDROP hDrop = (HDROP)wParam;
	UINT nFiles = DragQueryFile(hDrop, 0xffffffff, 0, 0);
	if (nFiles == 0) return 0;

	m_strOutFile.Empty();
	SetCaption();

	m_strTargets.Empty();
	for (UINT i=0; i<nFiles; ++i)
	{
		CString strFile;
		DragQueryFile(hDrop, i, strFile.GetBuffer(MAX_PATH), MAX_PATH);
		strFile.ReleaseBuffer();

		// use the first file's folder for the default
		if (i == 0) SetInFolder(strFile);

		PathQuoteSpaces(strFile.GetBuffer(MAX_PATH));
		strFile.ReleaseBuffer();

		m_strTargets += strFile;
	}

	UpdateView();

	return 0;
}

LRESULT CMainFrame::OnHelp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SendMessage(WM_COMMAND, ID_HELP_README);
	return 0;
}

LRESULT CMainFrame::OnFileOpen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	LPCTSTR pszFilter = 
		_T("bin files (*.exe;*.dll;*.ocx;*.lib;*.obj)\0*.exe;*.dll;*.ocx;*.lib;*.obj\0")
		_T("All Files (*.*)\0*.*\0")
		_T("");	

	CFileDialog dlg(TRUE, 0, _T(""), OFN_HIDEREADONLY, pszFilter);
	dlg.m_ofn.lpstrInitialDir = m_strInFolder;
	int nRet = dlg.DoModal();
	if (nRet != IDOK) return 0;

	m_strOutFile.Empty();
	SetCaption();

	SetInFolder(dlg.m_szFileName);

	m_strTargets = dlg.m_szFileName;
	PathQuoteSpaces(m_strTargets.GetBuffer(MAX_PATH));
	m_strTargets.ReleaseBuffer();

	UpdateView();

	return 0;
}

LRESULT CMainFrame::OnFileSave(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (m_strOutFile.GetLength() != 0)
	{
		DWORD dwErr = m_view.Save(m_strOutFile);
		if (dwErr != NOERROR) ErrMsg(_T("Save failed"), dwErr);
	}
	else
	{
		SendMessage(WM_COMMAND, ID_FILE_SAVE_AS);
	}

	return 0;
}

LRESULT CMainFrame::OnFileSaveAs(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	LPCTSTR pszFilter = 
		_T("Text Files (*.txt)\0*.txt\0")
		_T("All Files (*.*)\0*.*\0")
		_T("");	

	CFileDialog dlg(FALSE, _T(".txt"), _T(""), OFN_OVERWRITEPROMPT, pszFilter);
	dlg.m_ofn.lpstrInitialDir = m_strOutFolder;
	int nRet = dlg.DoModal();
	if (nRet != IDOK) return 0;

	m_strOutFolder = dlg.m_szFileName;
	PathRemoveFileSpec(m_strOutFolder.LockBuffer());
	m_strOutFolder.ReleaseBuffer();

	m_strOutFile = dlg.m_szFileName;
	SetCaption();

	DWORD dwErr = m_view.Save(m_strOutFile);
	if (dwErr != 0) ErrMsg(_T("Save failed"), dwErr);

	return 0;
}

LRESULT CMainFrame::OnFilePrint(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	DWORD dwErr = m_view.Print();
	if (dwErr != 0) ErrMsg(_T("Print failed"), dwErr);

	return  0;
}

LRESULT CMainFrame::OnFileAssociate(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CComPtr<IRegDialog> spDlg;
	HRESULT hr = spDlg.CoCreateInstance(L"dumpbinCMH.ContextMenu");
	if (FAILED(hr)) return 0;

	spDlg->ShowDialog();

	return 0;
}

LRESULT CMainFrame::OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	PostMessage(WM_CLOSE);
	return 0;
}

LRESULT CMainFrame::OnEditSelectAll(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_view.SetSel(0, -1);
	return 0;
}

LRESULT CMainFrame::OnEditCopy(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_view.SendMessage(WM_COPY);
	return 0;
}

LRESULT CMainFrame::OnEditFind(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_view.Find();
	return 0;
}

LRESULT CMainFrame::OnViewCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CString strCommand;
	m_viewmenu.GetMenuString(wID, strCommand, MF_BYCOMMAND);

	// chop accelerator if there is one
	int iTab = strCommand.Find(_T('\t'));
	if (iTab != -1) strCommand = strCommand.Left(iTab);

	m_strFlags.Format(_T("/%s "), strCommand);

	UpdateView();

	return 0;
}

LRESULT CMainFrame::OnHelpReadMe(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
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

	TCHAR szShortPath[MAX_PATH];
	GetModuleFileName(_Module.GetModuleInstance(), szShortPath, LENGTHOF(szShortPath));
	TCHAR szLongPath[MAX_PATH];
	MyGetLongPathName(szShortPath, szLongPath, LENGTHOF(szLongPath));

	CComBSTR bstrUrl(L"res://");
	bstrUrl += szLongPath;
	bstrUrl += L"/ReadMe.htm";

	CComVariant vEmpty;
	spIE->Navigate(bstrUrl, &vEmpty, &vEmpty, &vEmpty, &vEmpty);
	
	return S_OK;

}

LRESULT CMainFrame::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAbout().DoModal();
	return 0;
}

BOOL CMainFrame::OnIdle()
{
	if (m_bGUIEnabled)
	{
		UIEnable(ID_EDIT_COPY, m_view.HasSelection());
		UIUpdateToolBar();
	}

	return FALSE;
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	if (m_view.PreTranslateMessage(pMsg)) return TRUE;

	return CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg);
}

// we need to pump messages while we're running the child process
// but we can't abort while the child is active - or can we, by
// just closing pipe?
void CMainFrame::EnableGUI(bool bEnable)
{
	m_bGUIEnabled = bEnable;

	// enable/disable toolbar buttons
	for (int i=0; i<m_nToolBarButtons; ++i)
	{
		TBBUTTON tbb = {0};
		m_toolbar.GetButton(i, &tbb);
		int nState = m_toolbar.GetState(tbb.idCommand);
		if (bEnable)
		{
			nState |= TBSTATE_ENABLED;
		}
		else
		{
			nState &= ~TBSTATE_ENABLED;
		}
		m_toolbar.SetState(tbb.idCommand, nState);
	}

	// enable/disable menuitems
	EnableMenu(m_CmdBar.GetMenu(), bEnable);
}

void CMainFrame::EnableMenu(HMENU hMenu, bool bEnable)
{
	UINT uFlags = MF_BYPOSITION;
	uFlags |= bEnable ? MF_ENABLED : MF_GRAYED;

	CMenuHandle menu(hMenu);
	int n = menu.GetMenuItemCount();
	for (int i=0; i<n; ++i)
	{
		HMENU hSubMenu = menu.GetSubMenu(i);
		if (hSubMenu != 0) EnableMenu(hSubMenu, bEnable);
		else menu.EnableMenuItem(i, uFlags);
	}
}

// this searches the specified path for link.exe
// if found, the path is added to PATH if necessary
// note - it's possible that link.exe will be found on the path, 
//		but its import dll's won't be found on the path
bool CMainFrame::TestPath(LPCTSTR pszTestPath)
{
	TCHAR szFoundPath[MAX_PATH];

	// look for exe on path
	bool bFound = false;
	LPTSTR pFilePart = 0;

	int nLen = SearchPath(pszTestPath, _T("link.exe"), 0, MAX_PATH, szFoundPath, &pFilePart);
	if (nLen == 0) return false;	// couldn't find the file on specified path

	if (pszTestPath == 0) return true;	// specified path is current path

	CString strOldPath;
	DWORD dwLen = GetEnvironmentVariable(_T("PATH"), 0, 0);
	GetEnvironmentVariable(_T("PATH"), strOldPath.GetBuffer(dwLen), dwLen);
	strOldPath.ReleaseBuffer();

	CString strNewPath;
	strNewPath.Format(_T("%s;%s"), pszTestPath, strOldPath);
	SetEnvironmentVariable(_T("PATH"), strNewPath);

	return true;
}

bool CMainFrame::FindLinkExe()
{
	TCHAR szFoundPath[MAX_PATH] = {0};
	LPTSTR pFilePart = 0;
	int nLen = SearchPath(0, _T("link.exe"), 0, MAX_PATH, szFoundPath, &pFilePart);
	if (nLen != 0) return true;

	CString strMsg(_T("link.exe cannot be found.\nAdd the tool's directory to your path and try again."));
	MessageBox(strMsg, _T("dumpbinGUI"), MB_OK | MB_ICONERROR);
	return false;
}

void CMainFrame::ParseCommandLine()
{
	CCmdArgs cmdargs;
	if (!cmdargs.Parse()) return;

	cmdargs.Next();				// eat the appname

	LPCTSTR pszArg = 0;
	while ((pszArg = cmdargs.Next()) != 0)
	{
		CString strArg = pszArg;

		// flags start with a switch
		if (strArg.Left(1).FindOneOf(_T("/-")) != -1)
		{
			m_strFlags += strArg;
			m_strFlags += _T(' ');
		}
		else
		{
			PathUnquoteSpaces(strArg.GetBuffer(MAX_PATH));
			strArg.ReleaseBuffer();
			if (strArg.Find(_T('\\')) == -1)
			{
				CString strDir;
				GetCurrentDirectory(MAX_PATH, strDir.GetBuffer(MAX_PATH));
				PathAddBackslash(strDir.GetBuffer(MAX_PATH));
				strDir.ReleaseBuffer();
	
				strArg = strDir + strArg;
			}
			PathQuoteSpaces(strArg.GetBuffer(MAX_PATH));
			strArg.ReleaseBuffer();
			m_strTargets += strArg + _T(' ');
			if (m_strInFolder.GetLength() == 0) SetInFolder(strArg);
		}
	}
	if (m_strInFolder.GetLength() == 0)
	{
		GetSystemDirectory(m_strInFolder.GetBuffer(MAX_PATH), MAX_PATH);
		PathAddBackslash(m_strInFolder.LockBuffer());
		m_strInFolder.ReleaseBuffer();
	}
}

void CMainFrame::SetCaption()
{
	CString strAppName;
	strAppName.LoadString(IDR_MAINFRAME);

	CString strCaption = strAppName;
	if(m_strOutFile.GetLength() != 0)
	{
		strCaption += _T(" - ");
		strCaption += m_strOutFile;
	}
	SetWindowText(strCaption);
}

void CMainFrame::SetInFolder(LPCTSTR pszFile)
{
	m_strInFolder = pszFile;
	PathUnquoteSpaces(m_strInFolder.GetBuffer(MAX_PATH));
	PathRemoveFileSpec(m_strInFolder.LockBuffer());
	m_strInFolder.ReleaseBuffer();
}

void CMainFrame::UpdateView()
{
	CString strStatus(_T("  "));
	strStatus += m_strFlags;
	strStatus += m_strTargets;

	CWindow(m_hWndStatusBar).SetWindowText(strStatus);

	DragAcceptFiles(FALSE);
	EnableGUI(false);

	DWORD dwErr = m_view.Open(m_strTargets, m_strFlags);
	if (dwErr != 0) ErrMsg(_T("Could not start link.exe"), dwErr);

	EnableGUI(true);
	DragAcceptFiles(TRUE);
}


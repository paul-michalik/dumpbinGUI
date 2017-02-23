// dumpbinCMH v 1.1.0.1
// Copyright(c) 2000-2003 tim tabor.  
// All rights reserved.  No Warranties extended.

#include "stdafx.h"
#include "AssociateDlg.h"
#include "process.h"
#include "..\dumpbinGUI\Msg.h"

CAssociateDlg::CAssociateDlg()
{}

void CAssociateDlg::AddLvColumn(int iColumn, LPTSTR pszName)
{
	LV_COLUMN lvcol = {0};
	lvcol.mask = LVCF_TEXT;
	lvcol.pszText = pszName;

	m_lv.SendMessage(LVM_INSERTCOLUMN, iColumn, (LPARAM)&lvcol);
}

void CAssociateDlg::AddLvRow(LPCTSTR pszFileType)
{
	LVITEM lvi = {0};
	lvi.mask = LVIF_TEXT;
	lvi.iItem = (int)m_lv.SendMessage(LVM_GETITEMCOUNT);
	lvi.pszText = (LPTSTR)pszFileType;
	lvi.iItem = (int)m_lv.SendMessage(LVM_INSERTITEM, 0, (LPARAM)&lvi);

	m_lv.SendMessage(LVM_SETITEM, lvi.iItem, (LPARAM)&lvi);
}

LRESULT CAssociateDlg::OnAdd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CString strFileType;
	int nChars = m_edit.GetWindowText(strFileType);
	if (nChars == 0) return 0;

	_ShellReg.NormalizeFileType(strFileType);

	m_edit.SetWindowText(_T(""));

	CString strRegFileType;
	if (_ShellReg.IsRegistered(strFileType, strRegFileType)){
		CString strMsg;
		strMsg.Format(_T("%s was already registered as %s"), (LPCTSTR)strFileType, (LPCTSTR)strRegFileType);
		MessageBox(strMsg, _T("dumpbinGUI"), MB_OK | MB_ICONINFORMATION);
		return 0;
	}

	HRESULT hr = _ShellReg.CreateHandlerKey(strFileType, strRegFileType);
	if (hr == S_FALSE) return 0;
	if (FAILED(hr)){
		CString strMsg;
		strMsg.Format(_T("%s could not be registered (0x%8.8x)\n"), (LPCTSTR)strFileType, hr);
		MessageBox(strMsg, _T("dumpbinGUI"), MB_OK | MB_ICONERROR);
		return 0;
	}

	AddLvRow(strRegFileType);
	if (strRegFileType != strFileType){
		CString strMsg;
		strMsg.Format(_T("dumpbinGUI is now registered under %s"), (LPCTSTR)strRegFileType);
		MessageBox(strMsg, _T("dumpbinGUI"));
	}

	return 0;
}

LRESULT CAssociateDlg::OnEditCommand(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	bHandled = FALSE;
	SetButtonStates();
	return 0;
}

LRESULT CAssociateDlg::OnListNotify(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	bHandled = FALSE;
	SetButtonStates();
	return 0;
}

LRESULT CAssociateDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_lv = GetDlgItem(IDC_LIST);
	m_edit = GetDlgItem(IDC_EDIT);
	m_ok = GetDlgItem(IDOK);
	m_add = GetDlgItem(IDC_ADD);
	m_remove = GetDlgItem(IDC_REMOVE);

	m_lv.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

	AddLvColumn(0, _T("File Type"));

	CRect r;
	m_lv.GetClientRect(&r);
	m_lv.SendMessage(LVM_SETCOLUMNWIDTH, 0, MAKELPARAM(r.Width(), 0));

	CAtlArray<CString> rgstrFileTypes;
	int nTypes = _ShellReg.GetRegisteredFileTypes(rgstrFileTypes);

	size_t n = rgstrFileTypes.GetCount();
	for (size_t i=0; i<n; ++i)
	{
		AddLvRow(rgstrFileTypes.GetAt(i));
	}
	return TRUE;
}

LRESULT CAssociateDlg::OnOk(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

LRESULT CAssociateDlg::OnRemove(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int iSelected = (int)m_lv.SendMessage(LVM_GETNEXTITEM, (WPARAM)-1, MAKELPARAM(LVNI_ALL | LVNI_SELECTED, 0));;
	if (iSelected == -1) return 0;

	TCHAR szClass[128];
	LVITEM lvi = {0};
	lvi.mask = LVIF_TEXT;
	lvi.pszText = szClass;
	lvi.cchTextMax = LENGTHOF(szClass);

	int nChars = (int)m_lv.SendMessage(LVM_GETITEMTEXT, iSelected, (LPARAM)&lvi);
	if (nChars == 0) return 0;

	// test
	_ShellReg.DeleteHandlerKey(lvi.pszText);

	m_lv.SendMessage(LVM_DELETEITEM, iSelected);

	return 0;
}

void CAssociateDlg::SetButtonStates()
{
	BOOL bEditHasText = (BOOL)m_edit.SendMessage(WM_GETTEXTLENGTH) != 0;
	m_add.EnableWindow(bEditHasText);

	int nLvItems = (int)m_lv.SendMessage(LVM_GETITEMCOUNT);
	int iLvItemSelected = (int)m_lv.SendMessage(LVM_GETNEXTITEM, (WPARAM)-1, MAKELPARAM(LVNI_ALL | LVNI_SELECTED, 0));;
	m_remove.EnableWindow(nLvItems > 1 && iLvItemSelected != -1);

	HWND hWndFocus = ::GetFocus();
	WORD wDefID = 0;
	if (hWndFocus == m_edit && m_add.IsWindowEnabled()) wDefID = IDC_ADD;
	else if (hWndFocus == m_lv && m_remove.IsWindowEnabled()) wDefID = IDC_REMOVE;
	else wDefID = IDOK;

	SetDefaultButton(wDefID);
}

void CAssociateDlg::SetDefaultButton(WORD wID)
{
	WORD wOldDefID = (UINT)LOWORD(SendMessage(DM_GETDEFID));
	if (wID == wOldDefID) return;

	SendDlgItemMessage(wOldDefID, BM_SETSTYLE, BS_PUSHBUTTON, (LPARAM)TRUE);
	SendMessage(DM_SETDEFID, wID);
	SendDlgItemMessage(wID, BM_SETSTYLE, BS_DEFPUSHBUTTON, (LPARAM)TRUE);
}




// dumpbinGUI v 1.1.0.1
// Copyright(c) 2000-2003 tim tabor.  
// All rights reserved.  No Warranties extended.

#include "stdafx.h"
#include "FindDlg.h"

LRESULT CFindDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CenterWindow(GetParent());

	m_view = (HWND)lParam;

	m_edit = GetDlgItem(IDC_EDIT);
	m_fromtop = GetDlgItem(IDC_FROMTOP);
	m_next = GetDlgItem(IDC_NEXT);

	CheckDlgButton(IDC_DIRDOWN, 1);
	m_next.EnableWindow(FALSE);

	return TRUE;
}

LRESULT CFindDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	DestroyWindow();
	return 0;
}

LRESULT CFindDlg::OnNext(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	int nLen = m_edit.GetWindowTextLength();
	if (nLen == 0) return 0;


	m_view.SendMessage(AM_FINDNEXT);

	m_fromtop.SetCheck(0);	// clear check for next search
	return 0;
}

LRESULT CFindDlg::OnEditCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	BOOL bEnableNext = m_edit.GetWindowTextLength() != 0;

	m_next.EnableWindow(bEnableNext);
	SetDefaultButton(bEnableNext ? IDC_NEXT : IDCANCEL);

	return 0;
}

bool CFindDlg::IsDirDown()
{
	return IsDlgButtonChecked(IDC_DIRDOWN) == TRUE;
}

bool CFindDlg::IsFromTop()
{
	return IsDlgButtonChecked(IDC_FROMTOP) == TRUE;
}

bool CFindDlg::GetText(CString& strText)
{
	strText.Empty();

	int nLen = m_edit.GetWindowTextLength();
	if (nLen == 0) return false;

	m_edit.GetWindowText(strText.GetBuffer(nLen+1), nLen+1);
	strText.ReleaseBuffer();

	return true;
}

void CFindDlg::SetDefaultButton(WORD wID)
{
	WORD wOldDefID = (UINT)LOWORD(SendMessage(DM_GETDEFID));
	if (wID == wOldDefID) return;

	SendDlgItemMessage(wOldDefID, BM_SETSTYLE, BS_PUSHBUTTON, (LPARAM)TRUE);
	SendMessage(DM_SETDEFID, wID);
	SendDlgItemMessage(wID, BM_SETSTYLE, BS_DEFPUSHBUTTON, (LPARAM)TRUE);
}

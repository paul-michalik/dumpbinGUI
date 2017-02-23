// dumpbinGUI v 1.1.0.1
// Copyright(c) 2000-2003 tim tabor.  
// All rights reserved.  No Warranties extended.

#pragma once

#include "resource.h"

using namespace ATL;

class CFindDlg : public CDialogImpl<CFindDlg>
{

private:
	CEdit m_edit;
	CButton m_fromtop;
	CButton m_next;
	CWindow m_view;

public:

	enum { IDD = IDD_FIND };
	enum { AM_FINDNEXT = WM_APP };

	BEGIN_MSG_MAP(CFindDlg)		//todo
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_ID_HANDLER(IDC_NEXT, OnNext)
		COMMAND_ID_HANDLER(IDC_EDIT, OnEditCmd)
	END_MSG_MAP()

	//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnNext(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEditCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	bool IsDirDown();
	bool IsFromTop();
	bool GetText(CString& strText);

private:
	void SetDefaultButton(WORD wID);

};
// dumpbinCMH v 1.1.0.1
// Copyright(c) 2000-2003 tim tabor.  
// All rights reserved.  No Warranties extended.

#pragma once
#include "Resource.h"

class CAssociateDlg : 
	public CDialogImpl<CAssociateDlg>
{
private:

	CWindow m_lv;
	CWindow m_edit;
	CWindow m_ok;
	CWindow m_add;
	CWindow m_remove;

	HANDLE m_hThread;

	CAtlArray<CString> m_rgstrFileTypes;

public:
	
	enum { IDD = IDD_ASSOCIATE };

	BEGIN_MSG_MAP(CAssociateDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOk)
		COMMAND_ID_HANDLER(IDCANCEL, OnOk)
		COMMAND_ID_HANDLER(IDC_ADD, OnAdd)
		COMMAND_ID_HANDLER(IDC_REMOVE, OnRemove)
		COMMAND_ID_HANDLER(IDC_EDIT, OnEditCommand)
		NOTIFY_ID_HANDLER(IDC_LIST, OnListNotify)
	END_MSG_MAP()

	CAssociateDlg();

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOk(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnAdd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnRemove(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnEditCommand(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnListNotify(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

private:

	void AddLvColumn(int iColumn, LPTSTR pszName);
	void AddLvRow(LPCTSTR pszFileType);
	void SetButtonStates();
	void SetDefaultButton(WORD wID);

};

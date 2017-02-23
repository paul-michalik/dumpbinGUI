// dumpbinGUI v 1.1.0.1
// Copyright(c) 2000-2003 tim tabor.  
// All rights reserved.  No Warranties extended.

#pragma once

using namespace ATL;

class CAbout : public CDialogImpl<CAbout>
{
private:
	CHyperLink m_mailto;
	CHyperLink m_http;

public:
	enum { IDD = IDD_ABOUT };

BEGIN_MSG_MAP(CAbout)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnOK)
END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

private:

	void CreateHyperLink(CHyperLink& hlink, UINT uCtrlID, LPCTSTR pszLabel, LPCTSTR pszAddress);
};



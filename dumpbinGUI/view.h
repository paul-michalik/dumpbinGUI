// dumpbinGUI v 1.1.0.1
// Copyright(c) 2000-2003 tim tabor.  
// All rights reserved.  No Warranties extended.

#pragma once

#include "finddlg.h"

using namespace ATL;

typedef CWinTraitsOR<ES_READONLY> CViewTraits;

class CView : 
	public CWindowImpl<CView, CRichEditCtrl, CViewTraits>,
	public CPrintJobInfo
{
public:
	CFindDlg m_finddlg;
	
	// printer stuff
	CPrinterT<true> printer;
	CDevModeT<true> devmode;
	CRect m_rcMargin;
	CSimpleValArray<long> m_arrPages;

	DECLARE_WND_SUPERCLASS(_T("dumpbinGUIview"), CRichEditCtrl::GetWndClassName())

	BEGIN_MSG_MAP(CView)
		MESSAGE_HANDLER(WM_CREATE, OnCreate);
		MESSAGE_HANDLER(CFindDlg::AM_FINDNEXT, OnFindNext);
	END_MSG_MAP()

	CView();

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnFindNext(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	BOOL PreTranslateMessage(MSG* pMsg);

	DWORD Open(LPCTSTR pszTargets, LPCTSTR pszFlags);
	DWORD Save(LPCTSTR pszPath);
	DWORD Print();	// largely from WTL MTPad.h sample code

	void Find();

	bool HasSelection();

	static DWORD CALLBACK StreamWriteCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LPLONG pcb);

private:
	bool PrintPage(UINT nPage, HDC hDC);

};

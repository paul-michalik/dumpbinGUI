// dumpbinGUI v 1.1.0.1
// Copyright(c) 2000-2003 tim tabor.  
// All rights reserved.  No Warranties extended.

#include "stdafx.h"
#include "resource.h"
#include "About.h"

LRESULT CAbout::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CenterWindow(GetParent());

	CreateHyperLink(m_mailto, IDC_MAILTO, _T("tltabor@nyc.rr.com"), _T("mailto:tltabor@nyc.rr.com?subject=dumpbinGUI"));
	CreateHyperLink(m_http, IDC_HTTP, _T("www.cheztabor.com/dumpbinGUI"), _T("http://www.cheztabor.com/dumpbinGUI"));

	return 1;  // Let the system set the focus
}

LRESULT CAbout::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

void CAbout::CreateHyperLink(CHyperLink& hlink, UINT uCtrlID, LPCTSTR pszLabel, LPCTSTR pszAddress)
{
	CWindow ctl(GetDlgItem(uCtrlID));
	ctl.ModifyStyle(0, SS_CENTER);	// so CHyperlink::DoPaint works properly

	hlink.SubclassWindow(ctl);
	hlink.SetHyperLink(pszAddress);
	hlink.SetLabel(pszLabel);

	// center the label in the control window
	// this is a workaround for a DrawText(...,DT_CENTER | DT_CALCREC) bug
	CRect rLinkLabel(hlink.m_rcLink);
	CRect rLinkCtl;
	ctl.GetClientRect(&rLinkCtl);
	rLinkLabel.OffsetRect((rLinkCtl.Width() - rLinkLabel.Width()) / 2, 0);
	CopyRect(&hlink.m_rcLink, rLinkCtl);

}


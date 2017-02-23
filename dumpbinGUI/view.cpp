// dumpbinGUI v 1.1.0.1
// Copyright(c) 2000-2003 tim tabor.  
// All rights reserved.  No Warranties extended.

#include "stdafx.h"
#include "view.h"

CView::CView() : m_rcMargin(1000, 1000, 1000, 1000)
{
	printer.OpenDefaultPrinter();
	devmode.CopyFromPrinter(printer);
}

BOOL CView::PreTranslateMessage(MSG* pMsg)
{
	if(m_finddlg.IsWindow() && ::IsDialogMessage(m_finddlg.m_hWnd, pMsg))
	{
		return TRUE;
	}

	return FALSE;
}

LRESULT CView::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT lr = DefWindowProc();
	if (lr == -1) return lr;

	CFont font((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	LOGFONT lf = {0};
	font.GetLogFont(&lf);
	lstrcpy(lf.lfFaceName, _T("Courier New"));
	font.Detach();

	font.CreateFontIndirect(&lf);
	SetFont(font);

	return lr;
}

LRESULT CView::OnFindNext(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	CString strText;
	if (!m_finddlg.GetText(strText)) return 0;

	CHARRANGE chrg;
	GetSel(chrg);

	bool bFromTop = m_finddlg.IsFromTop();
	bool bDirDown = m_finddlg.IsDirDown();

	FINDTEXTEX ft;
	ft.lpstrText = strText;

	if (bFromTop)
	{
		ft.chrg.cpMin = 0;
		ft.chrg.cpMax = -1;
	}
	else 
	{
		if (bDirDown) 
		{
			ft.chrg.cpMin = chrg.cpMin + 1;
			ft.chrg.cpMax = -1;
		}
		else
		{
			ft.chrg.cpMin = chrg.cpMax - 1;
			ft.chrg.cpMax = -1;
		}
	}

	DWORD dwFlags = bDirDown ? FR_DOWN : 0;
	if (FindText(dwFlags, ft) != -1)
	{
		SetSel(ft.chrgText);
	}

	return 0;
}	

DWORD CView::Open(LPCTSTR pszTargets, LPCTSTR pszFlags)
{
	SetWindowText(_T(""));

	SECURITY_ATTRIBUTES sa = { sizeof sa };
	sa.bInheritHandle = TRUE;

	HANDLE hReadPipe = 0;
	HANDLE hWritePipe = 0;
	BOOL bOK = CreatePipe(&hReadPipe, &hWritePipe, &sa, 0);
	if (!bOK) return GetLastError();

	STARTUPINFO si = { sizeof si };
	si.dwFlags     = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	si.wShowWindow = SW_HIDE;
	si.hStdError = hWritePipe;
	si.hStdOutput = hWritePipe;

	PROCESS_INFORMATION pi = {0};

	CString strLinkArgs(_T("\"DUMPBIN\" /DUMP "));
	if (strLinkArgs.GetLength() != 0) strLinkArgs += pszFlags;
	strLinkArgs += pszTargets;

	CString strLinkPath(_T("link.exe"));
	bOK = PathFindOnPath(strLinkPath.GetBuffer(MAX_PATH), 0);
	strLinkPath.ReleaseBuffer();

	if (bOK)
	{
		bOK = CreateProcess(strLinkPath, strLinkArgs.LockBuffer(), 0, 0, TRUE, 0, 0, 0, &si, &pi);
		strLinkArgs.ReleaseBuffer();

		CloseHandle(hWritePipe);
		hWritePipe = 0;

		if (bOK)
		{
			CloseHandle (pi.hThread);

			while (true)
			{
				MSG msg;
				while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

 				BYTE buf[1024] = {0};
				DWORD dwBytesToRead = sizeof buf - sizeof(TCHAR);
				DWORD dwBytesRead = 0;
				if (!ReadFile(hReadPipe, buf, dwBytesToRead, &dwBytesRead, 0) || dwBytesRead == 0)
				{
					break;
				}		
				SetSel(-1, -1);
				ReplaceSel((LPCTSTR)buf);
			}

			CloseHandle (pi.hProcess);
		}
	}

	CloseHandle(hReadPipe);
	hReadPipe = 0;
	SetSel(0, 0);
	SetFocus();

	return bOK ? NOERROR : GetLastError();
}

DWORD CView::Save(LPCTSTR pszPath)
{
	HANDLE hFile = ::CreateFile(pszPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_ARCHIVE | FILE_FLAG_SEQUENTIAL_SCAN, 0);
	if (hFile == INVALID_HANDLE_VALUE) return false;

	EDITSTREAM es;
	es.dwCookie = (DWORD)hFile;
	es.dwError = 0;
	es.pfnCallback = StreamWriteCallback;

	StreamOut(SF_TEXT, es);

	CloseHandle(hFile);

	return es.dwError;
}

// The print stuff is largely from WTL's MTPad sample
DWORD CView::Print()
{
	CDC dc = printer.CreatePrinterDC(devmode);
	if (dc.IsNull()) return GetLastError();

	RECT rcPage;
	rcPage.left = rcPage.top = 0;
	rcPage.bottom = GetDeviceCaps(dc, PHYSICALHEIGHT);
	rcPage.right = GetDeviceCaps(dc, PHYSICALWIDTH);

	// We have to adjust the origin because 0,0 is not at the corner of the paper
	// but is at the corner of the printable region
	int nOffsetX = dc.GetDeviceCaps(PHYSICALOFFSETX);
	int nOffsetY = dc.GetDeviceCaps(PHYSICALOFFSETY);
	dc.SetViewportOrg(-nOffsetX, -nOffsetY);
	rcPage.right = MulDiv(rcPage.right, 1440, GetDeviceCaps(dc, LOGPIXELSX));
	rcPage.bottom = MulDiv(rcPage.bottom, 1440, GetDeviceCaps(dc, LOGPIXELSY));

	RECT rcOutput = rcPage;
	//convert from 1/1000" to twips
	rcOutput.left += MulDiv(m_rcMargin.left, 1440, 1000);
	rcOutput.right -= MulDiv(m_rcMargin.right, 1440, 1000);
	rcOutput.top += MulDiv(m_rcMargin.top, 1440, 1000);
	rcOutput.bottom -= MulDiv(m_rcMargin.bottom, 1440, 1000);
	
	FORMATRANGE fr;
	fr.hdc = dc;
	fr.hdcTarget = dc;
	fr.rc = rcOutput;
	fr.rcPage = rcPage;
	fr.chrg.cpMin = 0;
	fr.chrg.cpMax = -1;

	LONG n = (LONG)GetTextLength();
	m_arrPages.RemoveAll();
	while (1)
	{
		m_arrPages.Add(fr.chrg.cpMin);
		LONG lRet = (LONG)SendMessage(EM_FORMATRANGE, FALSE, (LPARAM)&fr);
		if((lRet - fr.chrg.cpMin) == -1)
		{
			m_arrPages.RemoveAt(m_arrPages.GetSize()-1);
			break;
		}
		else
			fr.chrg.cpMin = lRet;
		if (fr.chrg.cpMin >= n)
			break;
	}

	SendMessage(EM_FORMATRANGE, FALSE, 0);

	CPrintDialog dlg(FALSE);
	dlg.m_pd.hDevMode = devmode.CopyToHDEVMODE();
	dlg.m_pd.hDevNames = printer.CopyToHDEVNAMES();
	dlg.m_pd.nMinPage = 1;
	dlg.m_pd.nMaxPage = m_arrPages.GetSize();
	dlg.m_pd.nFromPage = 1;
	dlg.m_pd.nToPage = m_arrPages.GetSize();
	dlg.m_pd.Flags &= ~PD_NOPAGENUMS;

	if (dlg.DoModal() == IDOK)
	{
		devmode.CopyFromHDEVMODE(dlg.m_pd.hDevMode);
		printer.ClosePrinter();
		printer.OpenPrinter(dlg.m_pd.hDevNames, devmode.m_pDevMode);

		CPrintJob job;
		int nMin=0;
		int nMax = m_arrPages.GetSize()-1;
		if (dlg.m_pd.Flags | PD_PAGENUMS)
		{
			nMin = dlg.m_pd.nFromPage-1;
			nMax = dlg.m_pd.nToPage-1;
		}

		job.StartPrintJob(false, printer, devmode.m_pDevMode, this, _T("MTPad Document"), nMin, nMax);
	}

	GlobalFree(dlg.m_pd.hDevMode);
	GlobalFree(dlg.m_pd.hDevNames);

	return 0;
}

void CView::Find()
{
	m_finddlg.Create(m_hWnd, (LPARAM)m_hWnd);
}

bool CView::HasSelection()
{
	return GetSelectionType() != SEL_EMPTY;
}

bool CView::PrintPage(UINT nPage, HDC hDC)
{
	if (nPage >= (UINT)m_arrPages.GetSize())
		return false;

	RECT rcPage;
	rcPage.left = rcPage.top = 0;
	rcPage.bottom = GetDeviceCaps(hDC, PHYSICALHEIGHT);
	rcPage.right = GetDeviceCaps(hDC, PHYSICALWIDTH);

	rcPage.right = MulDiv(rcPage.right, 1440, GetDeviceCaps(hDC, LOGPIXELSX));
	rcPage.bottom = MulDiv(rcPage.bottom, 1440, GetDeviceCaps(hDC, LOGPIXELSY));

	RECT rcOutput = rcPage;
	//convert from 1/1000" to twips
	rcOutput.left += MulDiv(m_rcMargin.left, 1440, 1000);
	rcOutput.right -= MulDiv(m_rcMargin.right, 1440, 1000);
	rcOutput.top += MulDiv(m_rcMargin.top, 1440, 1000);
	rcOutput.bottom -= MulDiv(m_rcMargin.bottom, 1440, 1000);
	
	FORMATRANGE fr;
	fr.hdc = hDC;
	fr.hdcTarget = hDC;
	fr.rc = rcOutput;
	fr.rcPage = rcPage;
	fr.chrg.cpMin = 0;
	fr.chrg.cpMax = -1;
	fr.chrg.cpMin = m_arrPages[nPage];

	// We have to adjust the origin because 0,0 is not at the corner of the paper
	// but is at the corner of the printable region
	int nOffsetX = GetDeviceCaps(hDC, PHYSICALOFFSETX);
	int nOffsetY = GetDeviceCaps(hDC, PHYSICALOFFSETY);
	SetViewportOrgEx(hDC, -nOffsetX, -nOffsetY, NULL);
	int n = (LONG)SendMessage(EM_FORMATRANGE, TRUE, (LPARAM)&fr);
	SendMessage(EM_DISPLAYBAND, 0, (LPARAM)&rcOutput);

	//Cleanup cache in richedit
	SendMessage(EM_FORMATRANGE, FALSE, 0);
	return true;
}

DWORD CALLBACK CView::StreamWriteCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG FAR *pcb)
{
	BOOL bOK = WriteFile((HANDLE)dwCookie, pbBuff, cb, (LPDWORD)pcb, 0);
	return bOK ? NOERROR : GetLastError();
}


// dumpbinGUI v 1.1.0.1
// Copyright(c) 2000-2003 tim tabor.  
// All rights reserved.  No Warranties extended.

#include "stdafx.h"
#include "resource.h"
#include "MainFrm.h"

CAppModule _Module;

int Run(LPTSTR /*lpstrCmdLine*/ = 0, int nCmdShow = SW_SHOWDEFAULT)
{
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	CMainFrame wndMain;

	if(wndMain.CreateEx() == 0)
	{
		ATLTRACE(_T("Main window creation failed!\n"));
		return 0;
	}

	wndMain.ShowWindow(nCmdShow);

	int nRet = theLoop.Run();

	_Module.RemoveMessageLoop();
	return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	HRESULT hRes = ::CoInitialize(0);

	ATLASSERT(SUCCEEDED(hRes));
	INITCOMMONCONTROLSEX iccx;
	iccx.dwSize = sizeof(iccx);
	iccx.dwICC = ICC_COOL_CLASSES | ICC_BAR_CLASSES;
	BOOL bRet = ::InitCommonControlsEx(&iccx);
	bRet;
	ATLASSERT(bRet);

	hRes = _Module.Init(0, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	HINSTANCE hInstRich = ::LoadLibrary(CRichEditCtrl::GetLibraryName());
	ATLASSERT(hInstRich != 0);

	int nRet = Run(lpstrCmdLine, nCmdShow);

	::FreeLibrary(hInstRich);
	_Module.Term();
	::CoUninitialize();

	return nRet;
}

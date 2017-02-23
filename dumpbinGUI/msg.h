// dumpbinGUI v 1.1.0.1
// Copyright(c) 2000-2003 tim tabor.  
// All rights reserved.  No Warranties extended.

#pragma once

#ifndef LENGTHOF
#define LENGTHOF(a) (sizeof(a) == sizeof(&*a)) ? 0 : (sizeof(a) / sizeof(*a))
#endif

inline void Msg(LPCTSTR pszMsg, UINT uIcon)
{
	TCHAR szCaption[128];
	LoadString(_AtlBaseModule.GetResourceInstance(), IDR_MAINFRAME, szCaption, LENGTHOF(szCaption));

	::MessageBox(GetActiveWindow(), pszMsg, szCaption, uIcon | MB_OK);
}

inline void ErrMsg(LPCTSTR pszText, LONG lr)
{
	TCHAR szMsg[512];
	if (lr >=0)
	{
		wsprintf(szMsg, _T("%s (%d)."), pszText, lr);
	}
	else
	{
		wsprintf(szMsg, _T("%s (0x%8.8x)."), pszText, lr);
	}

	Msg(szMsg, MB_ICONERROR);
}
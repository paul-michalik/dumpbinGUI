// dumpbinGUI v 1.1.0.1
// Copyright(c) 2000-2003 tim tabor.  
// All rights reserved.  No Warranties extended.

#pragma once

#include <ShlObj.h>
#include <ExDisp.h>

#ifndef HR
#define HR(_ex) { HRESULT _hr = _ex; if (FAILED(_hr)) return _hr; }
#endif

inline DWORD MyGetLongPathName(LPCTSTR	pszShortPath, LPTSTR pszLongPath, DWORD	nBufferLen)
{
    SetLastError(ERROR_INVALID_PARAMETER);
    if (pszShortPath == 0 || pszShortPath[0] == 0) return 0;
    if (pszLongPath == 0) return 0;

    SetLastError(ERROR_GEN_FAILURE);
    CComPtr<IShellFolder> spShellFolder;
    HRESULT hr = SHGetDesktopFolder(&spShellFolder);
    if( FAILED(hr) ) return 0;

	ULONG			dwEaten;
	LPITEMIDLIST	pidl;
    USES_CONVERSION;
    hr = spShellFolder->ParseDisplayName(0, 0, (LPWSTR)T2CW(pszShortPath), &dwEaten, &pidl, 0);
    if( FAILED(hr) ) return 0;

    TCHAR szShellPathName[MAX_PATH];
    if (!SHGetPathFromIDList(pidl, szShellPathName)) return 0;

    int nRequiredLen = lstrlen(szShellPathName);
    if (nRequiredLen < static_cast<int>(nBufferLen))
    {
        lstrcpy(pszLongPath, szShellPathName);
        return nRequiredLen;
    }

    SetLastError(0);
    return nRequiredLen;
}


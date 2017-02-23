// dumpbinGUI v 1.1.0.1
// Copyright(c) 2000-2003 tim tabor.  
// All rights reserved.  No Warranties extended.

#pragma once

#ifndef HR
#define HR(_ex) { HRESULT _hr = _ex; if (FAILED(_hr)) return _hr; }
#endif

#ifndef HRLR
#define HRLR(_ex) { LRESULT _lr = _ex; if (_lr != NOERROR) return HRESULT_FROM_WIN32(_lr); }
#endif

#ifndef LENGTHOF
#define LENGTHOF(a) (sizeof(a) == sizeof(&*a)) ? 0 : (sizeof(a) / sizeof(*a))
#endif

class CShellReg
{
public:
	CShellReg(LPCTSTR pszProgId);

	HRESULT Unregister();
	HRESULT Register(LPCTSTR pMultiFileTypes);

	HRESULT CreateHandlerKey(LPCTSTR pszType, CString& strRegType);
	HRESULT DeleteHandlerKey(LPCTSTR pszFileClass);
	HRESULT GetRegisteredFileTypes(CAtlArray<CString>& rgstrFileTypes);
	bool IsRegistered(LPCTSTR pszType, CString& strRegType);
	bool ResolveFileType(LPCTSTR pszType, CString& strResolvedType);
	void NormalizeFileType(CString& strType);

private:
	HRESULT EnsureDataMembers();
	HRESULT SetApprovedValue();
	HRESULT DeleteApprovedValue();
	HRESULT CreateFileType(LPCTSTR pszType, CString& strRegType);

	bool IsNT4Plus();
	bool FindRegisteredFileType(LPCTSTR pszType);

	CString m_strProgId;
	CString m_strHandlerType;
	CString m_strCLSID;
};


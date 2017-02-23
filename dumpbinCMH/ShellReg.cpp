// dumpbinCMH v 1.1.0.1
// Copyright(c) 2000-2003 tim tabor.  
// All rights reserved.  No Warranties extended.

#include "stdafx.h"
#include "ShellReg.h"

CShellReg::CShellReg(LPCTSTR pszProgId) :
	m_strProgId(pszProgId),
	m_strHandlerType(_T("ContextMenuHandlers"))
{
}

HRESULT CShellReg::CreateFileType(LPCTSTR pszType, CString& strRegType)
{
	ATLASSERT(pszType != 0);
	if (pszType == 0) return E_INVALIDARG;
	strRegType.Empty();

	CRegKey key;
	LRESULT lr = key.Create(HKEY_CLASSES_ROOT, pszType);
	if (lr != NOERROR) return HRESULT_FROM_WIN32(lr);

	if (pszType[0] != _T('.')) return S_OK;

	strRegType.Format(_T("%sfile"), &pszType[1]);
	lr = key.SetStringValue(_T(""), strRegType);
	if (lr != NOERROR) return HRESULT_FROM_WIN32(lr);

	key.Close();
	lr = key.Create(HKEY_CLASSES_ROOT, strRegType);
	return lr == NOERROR ? S_OK : HRESULT_FROM_WIN32(lr);
}

HRESULT CShellReg::CreateHandlerKey(LPCTSTR pszType, CString& strRegType)
{
	HR(EnsureDataMembers());

	if (!ResolveFileType(pszType, strRegType)) {
		CString strMsg;
		strMsg.Format(_T("%s is not a registered file type.\nWould you like to register it?"), pszType);
		int nReply = MessageBox(0, strMsg, _T("dumpbinGUI"), MB_YESNO | MB_ICONQUESTION);
		if (nReply == IDNO) return S_FALSE;		// cancelled

		HR(CreateFileType(pszType, strRegType));
	}

	CString strKey;
	strKey.Format(_T("%s\\shellex\\%s\\%s"), (LPCTSTR)strRegType, (LPCTSTR)m_strHandlerType, (LPCTSTR)m_strProgId);
	CRegKey key;
	HRLR(key.Create(HKEY_CLASSES_ROOT, strKey));
	HRLR(key.SetStringValue(_T(""), m_strCLSID));

	return S_OK;
}



HRESULT CShellReg::DeleteApprovedValue()
{
	if (!IsNT4Plus()) return S_FALSE;

	CRegKey key;
	CString strKey(_T("Software\\Microsoft\\Windows\\CurrentVersion\\Shell Extensions\\Approved"));
	HRLR(key.Open(HKEY_LOCAL_MACHINE, strKey));

	HR(key.DeleteValue(m_strCLSID));

	return S_OK;
}

HRESULT CShellReg::DeleteHandlerKey(LPCTSTR pszFileClass)
{
	CString strRegPath;
	strRegPath.Format(_T("%s\\shellex\\%s"), pszFileClass, (LPCTSTR)m_strHandlerType);

	CRegKey key;
	HRLR(key.Open(HKEY_CLASSES_ROOT, strRegPath));

	HR(RegDeleteKey(key, m_strProgId));

	return S_OK;
}

HRESULT CShellReg::EnsureDataMembers()
{
	ATLASSERT( ! m_strHandlerType.IsEmpty());
	ATLASSERT( ! m_strProgId.IsEmpty());
	if (m_strCLSID.IsEmpty()){
		CString strRegPath;
		strRegPath.Format(_T("%s\\CLSID"), (LPCTSTR)m_strProgId);

		CRegKey key;
		LRESULT lr = key.Open(HKEY_CLASSES_ROOT, strRegPath);
		if (lr != NOERROR) return HRESULT_FROM_WIN32(lr);

		const int CLSIDLEN = 39;
		TCHAR szCLSID[CLSIDLEN] = {0};
		ULONG nChars = CLSIDLEN;
		lr = key.QueryStringValue(_T(""), szCLSID, &nChars);
		if (lr != NOERROR) return HRESULT_FROM_WIN32(lr);

		m_strCLSID = szCLSID;
	}

	return S_OK;
}

bool CShellReg::FindRegisteredFileType(LPCTSTR pszType)
{
	CString strRegPath;
	strRegPath.Format(_T("%s\\shellex\\%s\\%s"), pszType, (LPCTSTR)m_strHandlerType, (LPCTSTR)m_strProgId);

	CRegKey key;
	LRESULT lr = key.Open(HKEY_CLASSES_ROOT, strRegPath);
	return lr == NOERROR;
}

HRESULT CShellReg::GetRegisteredFileTypes(CAtlArray<CString>& rgstrFileTypes)
{
	CRegKey key;
	HRLR(key.Open(HKEY_CLASSES_ROOT, _T("")));

	const int BUFLEN = 256;
	TCHAR szClass[BUFLEN] = {0};
	DWORD nChars = BUFLEN;
	int iKey = 0;
	while (nChars = BUFLEN, RegEnumKey(key, iKey++, szClass, nChars) == NOERROR){
		CString strRegPath;
		strRegPath.Format(_T("%s\\shellex\\%s\\%s"), szClass, (LPCTSTR)m_strHandlerType, (LPCTSTR)m_strProgId);

		CRegKey key;
		LRESULT lr = key.Open(HKEY_CLASSES_ROOT, strRegPath);
		if (lr != NOERROR) continue;

		rgstrFileTypes.Add(szClass);
	}

	return rgstrFileTypes.GetCount() != 0 ? S_OK : S_FALSE;
}

bool CShellReg::IsNT4Plus()
{
	OSVERSIONINFO osvi = { sizeof osvi };
	GetVersionEx(&osvi);
	return (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT) && (osvi.dwMajorVersion >= 4);
}

HRESULT CShellReg::Register(LPCTSTR pMultiFileTypes)
{
	HR(EnsureDataMembers());

	HR(SetApprovedValue());

	CString strUnusedResolvedType;
	for (LPCTSTR psz = pMultiFileTypes; *psz; psz += lstrlen(psz) + 1){
		HR(CreateHandlerKey(psz, strUnusedResolvedType));
	}

	return S_OK;
}

HRESULT CShellReg::SetApprovedValue()
{
	if (!IsNT4Plus()) return S_OK;

	CRegKey key;
	CString strKey(_T("Software\\Microsoft\\Windows\\CurrentVersion\\Shell Extensions\\Approved"));
	HRLR(key.Create(HKEY_LOCAL_MACHINE, strKey));
	HRLR(key.SetStringValue(m_strCLSID, m_strProgId));

	return S_OK;
}

bool CShellReg::IsRegistered(LPCTSTR pszType, CString& strRegType)
{
	strRegType.Empty();

	if (FindRegisteredFileType(pszType)){
		strRegType = pszType;
		return true;
	}

	CRegKey key;
	LRESULT lr = key.Open(HKEY_CLASSES_ROOT, pszType);
	if (lr != NOERROR) return false;

	const int BUFLEN = 256;
	TCHAR szTypeVal[BUFLEN] = {0};
	ULONG nChars = 0;

	while (nChars = BUFLEN, key.QueryStringValue(_T(""), szTypeVal, &nChars) == NOERROR){
		if (nChars == 0) break;

		if (FindRegisteredFileType(szTypeVal)){
			strRegType = szTypeVal;
			return true;
		}

		key.Close();
		lr = key.Open(HKEY_CLASSES_ROOT, szTypeVal);
		if (lr != NOERROR) break;
	}

	return false;
}

void CShellReg::NormalizeFileType(CString& strType)
{
	if (strType.GetLength() > 4) return;
	if (strType.GetAt(0) == _T('.')) return;
	if (strType == _T("DVD")) return;

	strType.Insert(0, _T('.'));
}

bool CShellReg::ResolveFileType(LPCTSTR pszType, CString& strResolvedType)
{
	CRegKey key;
	LRESULT lr = key.Open(HKEY_CLASSES_ROOT, pszType);
	if (lr != NOERROR) return false;

	strResolvedType = pszType;

	const int BUFLEN = 256;
	const int BUFSIZE = BUFLEN * sizeof(TCHAR);
	TCHAR szTypeVal[BUFLEN] = {0};
	ULONG nChars = 0;

	while (nChars = BUFLEN, key.QueryStringValue(_T(""), szTypeVal, &nChars) == NOERROR){
		if (nChars == 0) break;

		key.Close();

		lr = key.Open(HKEY_CLASSES_ROOT, szTypeVal);
		if (lr != NOERROR) break;

		strResolvedType = szTypeVal;
	}

	return !strResolvedType.IsEmpty();
}

HRESULT CShellReg::Unregister()
{
	HR(EnsureDataMembers());

	DeleteApprovedValue();

	CRegKey key;
	HRLR(key.Open(HKEY_CLASSES_ROOT, _T("")));

	const int BUFLEN = 256;
	TCHAR szClass[BUFLEN] = {0};
	DWORD nChars = BUFLEN;
	int iKey = 0;
	while (nChars = BUFLEN, RegEnumKey(key, iKey++, szClass, nChars) == NOERROR){
		DeleteHandlerKey(szClass);
	}

	return S_OK;
}


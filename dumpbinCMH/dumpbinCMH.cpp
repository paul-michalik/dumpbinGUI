// dumpbinGUI v 1.1.0.1
// Copyright(c) 2000-2003 tim tabor.  
// All rights reserved.  No Warranties extended.

// dumpbinCMH.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#include "resource.h"

CShellReg _ShellReg(_T("dumpbinCMH.ContextMenu"));

[ module(dll, 
		 uuid = "{450972BC-C312-416E-8319-A55D0E742B82}", 
		 name = "dumpbinCMH",
		 helpstring = "dumpbinCMH 1.0 Type Library",
		 resource_name = "IDR_DUMPBINCMH") ]
class CDumpbinCMH {
public:

	HRESULT DllRegisterServer(void) 
	{
		HRESULT hr = __super::DllRegisterServer();
		if (FAILED(hr)) return hr;

		hr = _ShellReg.Register(_T(".exe\0.dll\0.obj\0.lib\0.ocx\0"));
		if (FAILED(hr)){
			DllUnregisterServer();
		}

		return hr;
	}

	HRESULT DllUnregisterServer(void) 
	{
		_ShellReg.Unregister();
		return __super::DllUnregisterServer();
	}
};
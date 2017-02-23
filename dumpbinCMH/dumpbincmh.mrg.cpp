// Created by Microsoft (R) C/C++ Compiler Version 13.00.9466
//
// e:\projectsatl\dumpbingui\source\dumpbincmh\dumpbincmh.mrg.cpp
// compiler-generated file created 11/08/03 at 22:41:39
//
// This C++ source file is intended to be a representation of the
// source code injected by the compiler.  It may not compile or
// run exactly as the original source file.
//



//+++ Start Injected Code For Attribute 'module'
#injected_line 12 "e:\\projectsatl\\dumpbingui\\source\\dumpbincmh\\dumpbincmh.cpp"

#ifndef GUID_DEFINED
#define GUID_DEFINED
typedef struct _GUID 
{
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[ 8 ];
} 
GUID;
#endif

extern "C" const __declspec(selectany) GUID __LIBID_ = {0x450972bc,0xc312,0x416e,{0x83,0x19,0xa5,0x5d,0x0e,0x74,0x2b,0x82}};
struct __declspec(uuid("450972bc-c312-416e-8319-a55d0e742b82")) dumpbinCMH;

//--- End Injected Code For Attribute 'module'
// dumpbinGUI v 1.1.0.1
// Copyright(c) 2000-2003 tim tabor.  
// All rights reserved.  No Warranties extended.

// dumpbinCMH.cpp : Implementation of DLL Exports.

#include "stdafx.h"
//+++ Start Injected Code
[no_injected_text(true)];      // Suppress injected text, it has already been injected
#pragma warning(disable: 4543) // Suppress warnings about skipping injected text
#pragma warning(disable: 4199) // Suppress warnings from attribute providers

#pragma message("\n\nNOTE: This merged source file should be visually inspected for correctness.\n\n")
//--- End Injected Code


#include "resource.h"

CShellReg _ShellReg(_T("dumpbinCMH.ContextMenu"));

[ module(dll, 
		 uuid = "{450972BC-C312-416E-8319-A55D0E742B82}", 
		 name = "dumpbinCMH",
		 helpstring = "dumpbinCMH 1.0 Type Library",
		 resource_name = "IDR_DUMPBINCMH") ]
class CDumpbinCMH  :
    /*+++ Added Baseclass */ public CAtlDllModuleT<CDumpbinCMH>
{
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

	//+++ Start Injected Code For Attribute 'module'
    #injected_line 12 "e:\\projectsatl\\dumpbingui\\source\\dumpbincmh\\dumpbincmh.cpp"

						DECLARE_LIBID(__uuidof(dumpbinCMH))
					
	//--- End Injected Code For Attribute 'module'
};

//+++ Start Injected Code For Attribute 'module'
#injected_line 12 "e:\\projectsatl\\dumpbingui\\source\\dumpbincmh\\dumpbincmh.cpp"

extern CDumpbinCMH _AtlModule;

//--- End Injected Code For Attribute 'module'


//+++ Start Injected Code For Attribute 'module'
#injected_line 12 "e:\\projectsatl\\dumpbingui\\source\\dumpbincmh\\dumpbincmh.cpp"
extern "C" STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv);

//--- End Injected Code For Attribute 'module'


//+++ Start Injected Code For Attribute 'module'
#injected_line 12 "e:\\projectsatl\\dumpbingui\\source\\dumpbincmh\\dumpbincmh.cpp"
extern "C" STDAPI DllRegisterServer(void);

//--- End Injected Code For Attribute 'module'


//+++ Start Injected Code For Attribute 'module'
#injected_line 12 "e:\\projectsatl\\dumpbingui\\source\\dumpbincmh\\dumpbincmh.cpp"
extern "C" STDAPI DllUnregisterServer(void);

//--- End Injected Code For Attribute 'module'


//+++ Start Injected Code For Attribute 'module'
#injected_line 12 "e:\\projectsatl\\dumpbingui\\source\\dumpbincmh\\dumpbincmh.cpp"
extern "C" STDAPI DllCanUnloadNow(void);

//--- End Injected Code For Attribute 'module'


//+++ Start Injected Code For Attribute 'module'
#injected_line 12 "e:\\projectsatl\\dumpbingui\\source\\dumpbincmh\\dumpbincmh.cpp"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved);

//--- End Injected Code For Attribute 'module'


//+++ Start Injected Code For Attribute 'module'
#injected_line 12 "e:\\projectsatl\\dumpbingui\\source\\dumpbincmh\\dumpbincmh.cpp"

CDumpbinCMH _AtlModule;

#injected_line 12 "e:\\projectsatl\\dumpbingui\\source\\dumpbincmh\\dumpbincmh.cpp"
extern "C" STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv) 
{
    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}
#injected_line 12 "e:\\projectsatl\\dumpbingui\\source\\dumpbincmh\\dumpbincmh.cpp"
extern "C" STDAPI DllRegisterServer(void) 
{
    return _AtlModule.DllRegisterServer();
}
#injected_line 12 "e:\\projectsatl\\dumpbingui\\source\\dumpbincmh\\dumpbincmh.cpp"
extern "C" STDAPI DllUnregisterServer(void) 
{
    return _AtlModule.DllUnregisterServer();
}
#injected_line 12 "e:\\projectsatl\\dumpbingui\\source\\dumpbincmh\\dumpbincmh.cpp"
extern "C" STDAPI DllCanUnloadNow(void) 
{
    return _AtlModule.DllCanUnloadNow();
}
#injected_line 12 "e:\\projectsatl\\dumpbingui\\source\\dumpbincmh\\dumpbincmh.cpp"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved) 
{
    hInstance;
    return _AtlModule.DllMain(dwReason, lpReserved);
}
#injected_line 12 "e:\\projectsatl\\dumpbingui\\source\\dumpbincmh\\dumpbincmh.cpp"

#if defined(_M_IX86)
#pragma comment(linker, "/EXPORT:DllMain=_DllMain@12,PRIVATE")
#pragma comment(linker, "/EXPORT:DllRegisterServer=_DllRegisterServer@0,PRIVATE")
#pragma comment(linker, "/EXPORT:DllUnregisterServer=_DllUnregisterServer@0,PRIVATE")
#pragma comment(linker, "/EXPORT:DllGetClassObject=_DllGetClassObject@12,PRIVATE")
#pragma comment(linker, "/EXPORT:DllCanUnloadNow=_DllCanUnloadNow@0,PRIVATE")
#elif defined(_M_IA64)
#pragma comment(linker, "/EXPORT:DllMain,PRIVATE")
#pragma comment(linker, "/EXPORT:DllRegisterServer,PRIVATE")
#pragma comment(linker, "/EXPORT:DllUnregisterServer,PRIVATE")
#pragma comment(linker, "/EXPORT:DllGetClassObject,PRIVATE")
#pragma comment(linker, "/EXPORT:DllCanUnloadNow,PRIVATE")
#endif	

//--- End Injected Code For Attribute 'module'

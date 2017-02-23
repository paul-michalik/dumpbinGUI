// ATLProject1.cpp : Implementation of WinMain


#include "stdafx.h"
#include "resource.h"
#include "ATLProject1_i.h"

using namespace ATL;


class B : public CDialogImpl<B> {
};

class CATLProject1Module : public ATL::CAtlExeModuleT< CATLProject1Module >
{
public :
	DECLARE_LIBID(LIBID_ATLProject1Lib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_ATLPROJECT1, "{051F0BD7-C539-44DA-9288-0FF9E9722739}")
	};

CATLProject1Module _AtlModule;



//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
								LPTSTR /*lpCmdLine*/, int nShowCmd)
{
	return _AtlModule.WinMain(nShowCmd);
}


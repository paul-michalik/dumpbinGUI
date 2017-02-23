//// stdafx.h : include file for standard system include files,
////  or project specific include files that are used frequently, but
////      are changed infrequently
////
//
//#if !defined(AFX_STDAFX_H__AAD94953_4CC4_4086_AC4A_945CC01205D1__INCLUDED_)
//#define AFX_STDAFX_H__AAD94953_4CC4_4086_AC4A_945CC01205D1__INCLUDED_
//
//// Change these values to use different versions
//#define _ATL_APARTMENT_THREADED
//
//#define _ATL_NO_AUTOMATIC_NAMESPACE
//
//#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit
//
//
//#define ATL_NO_ASSERT_ON_DESTROY_NONEXISTENT_WINDOW
//
//#include "resource.h"
//#include <Windows.h>
//#include <atlbase.h>
//#include <atlcom.h>
//#include <atlctl.h>
//#include <atlwin.h>
////{{AFX_INSERT_LOCATION}}
//// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
//
//#endif // !defined(AFX_STDAFX_H__AAD94953_4CC4_4086_AC4A_945CC01205D1__INCLUDED_)

#pragma once

#ifndef STRICT
#define STRICT
#endif

#define _ATL_APARTMENT_THREADED

#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit


#define ATL_NO_ASSERT_ON_DESTROY_NONEXISTENT_WINDOW

#define _WTL_NO_CSTRING

#include "resource.h"

#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>
#include <atlstr.h>
#include <atlapp.h>
#include <atlctrls.h>
#include <atlctrlx.h>
#include <atltypes.h>
#include <atlprint.h>
#include <atlframe.h>
#include <atlctrlw.h>
#include <atldlgs.h>
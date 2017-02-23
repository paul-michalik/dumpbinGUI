
#pragma warning( disable: 4049 )  /* more than 64k source lines */

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0347 */
/* at Sat Nov 08 22:55:37 2003
 */
/* Compiler settings for _dumpbinCMH.idl:
    Os, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef ___dumpbinCMH_h__
#define ___dumpbinCMH_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IRegDialog_FWD_DEFINED__
#define __IRegDialog_FWD_DEFINED__
typedef interface IRegDialog IRegDialog;
#endif 	/* __IRegDialog_FWD_DEFINED__ */


#ifndef __CContextMenu_FWD_DEFINED__
#define __CContextMenu_FWD_DEFINED__

#ifdef __cplusplus
typedef class CContextMenu CContextMenu;
#else
typedef struct CContextMenu CContextMenu;
#endif /* __cplusplus */

#endif 	/* __CContextMenu_FWD_DEFINED__ */


/* header files for imported files */
#include "prsht.h"
#include "mshtml.h"
#include "mshtmhst.h"
#include "exdisp.h"
#include "objsafe.h"
#include "shldisp.h"
#include "shobjidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __IRegDialog_INTERFACE_DEFINED__
#define __IRegDialog_INTERFACE_DEFINED__

/* interface IRegDialog */
/* [unique][helpstring][oleautomation][uuid][object] */ 


EXTERN_C const IID IID_IRegDialog;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("13447646-25E9-40cb-921B-967999167CD5")
    IRegDialog : public IUnknown
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ShowDialog( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IRegDialogVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IRegDialog * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IRegDialog * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IRegDialog * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ShowDialog )( 
            IRegDialog * This);
        
        END_INTERFACE
    } IRegDialogVtbl;

    interface IRegDialog
    {
        CONST_VTBL struct IRegDialogVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IRegDialog_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IRegDialog_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IRegDialog_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IRegDialog_ShowDialog(This)	\
    (This)->lpVtbl -> ShowDialog(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IRegDialog_ShowDialog_Proxy( 
    IRegDialog * This);


void __RPC_STUB IRegDialog_ShowDialog_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IRegDialog_INTERFACE_DEFINED__ */



#ifndef __dumpbinCMH_LIBRARY_DEFINED__
#define __dumpbinCMH_LIBRARY_DEFINED__

/* library dumpbinCMH */
/* [helpstring][uuid][version] */ 


EXTERN_C const IID LIBID_dumpbinCMH;

EXTERN_C const CLSID CLSID_CContextMenu;

#ifdef __cplusplus

class DECLSPEC_UUID("8F697706-E1B2-4BF7-8C64-89904C409DE6")
CContextMenu;
#endif
#endif /* __dumpbinCMH_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif



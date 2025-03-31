// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
//

//
// ===========================================================================
// File: oaidl.h
// 
// ===========================================================================

#ifndef __OAIDL_H__
#define __OAIDL_H__

#include "rpc.h"
#include "rpcndr.h"

#include "unknwn.h"

typedef struct tagEXCEPINFO {
    uint16_t wCode;
    uint16_t wReserved;
    BSTR bstrSource;
    BSTR bstrDescription;
    BSTR bstrHelpFile;
    uint32_t dwHelpContext;
    void * pvReserved;
    HRESULT (*pfnDeferredFillIn)(struct tagEXCEPINFO *);
    SCODE scode;
} EXCEPINFO, * LPEXCEPINFO;

typedef interface IErrorInfo IErrorInfo;
typedef /* [unique] */ IErrorInfo *LPERRORINFO;

EXTERN_C const IID IID_IErrorInfo;

    interface
    IErrorInfo : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetGUID( 
            /* [out] */ GUID *pGUID) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetSource( 
            /* [out] */ BSTR *pBstrSource) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDescription( 
            /* [out] */ BSTR *pBstrDescription) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetHelpFile( 
            /* [out] */ BSTR *pBstrHelpFile) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetHelpContext( 
            /* [out] */ uint32_t *pdwHelpContext) = 0;
        
    };
    
typedef interface ICreateErrorInfo ICreateErrorInfo;

EXTERN_C const IID IID_ICreateErrorInfo;

typedef /* [unique] */ ICreateErrorInfo *LPCREATEERRORINFO;

    interface
    ICreateErrorInfo : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetGUID( 
            /* [in] */ REFGUID rguid) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetSource( 
            /* [in] */ LPOLESTR szSource) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetDescription( 
            /* [in] */ LPOLESTR szDescription) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetHelpFile( 
            /* [in] */ LPOLESTR szHelpFile) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetHelpContext( 
            /* [in] */ uint32_t dwHelpContext) = 0;
        
    };
    
STDAPI
SetErrorInfo(uint32_t dwReserved, IErrorInfo * perrinfo);

STDAPI
GetErrorInfo(uint32_t dwReserved, IErrorInfo * * pperrinfo);

STDAPI
CreateErrorInfo(ICreateErrorInfo * * pperrinfo);


typedef interface ISupportErrorInfo ISupportErrorInfo;

typedef /* [unique] */ ISupportErrorInfo *LPSUPPORTERRORINFO;

EXTERN_C const IID IID_ISupportErrorInfo;

    
    interface
    ISupportErrorInfo : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE InterfaceSupportsErrorInfo( 
            /* [in] */ REFIID riid) = 0;
        
    };

#endif //__OAIDL_H__

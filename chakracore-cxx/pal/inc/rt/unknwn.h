// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
//

//
// ===========================================================================
// File: unknwn.h
// 
// ===========================================================================
// simplified unknwn.h for PAL

#ifndef __IUnknown_INTERFACE_DEFINED__
#define __IUnknown_INTERFACE_DEFINED__

typedef interface IUnknown IUnknown;

typedef /* [unique] */ IUnknown *LPUNKNOWN;

struct IUnknown
{
    virtual int32_t STDMETHODCALLTYPE QueryInterface( 
        REFIID riid,
        void **ppvObject) = 0;
        
    virtual uint32_t STDMETHODCALLTYPE AddRef( void) = 0;
        
    virtual uint32_t STDMETHODCALLTYPE Release( void) = 0;

    template<class Q>
    int32_t
    STDMETHODCALLTYPE
    QueryInterface(Q** pp)
    {
        return QueryInterface(__uuidof(Q), (void **)pp);
    }
};

#endif // __IUnknown_INTERFACE_DEFINED__

#ifndef __IClassFactory_INTERFACE_DEFINED__
#define __IClassFactory_INTERFACE_DEFINED__

struct IClassFactory : public IUnknown
{
    virtual int32_t STDMETHODCALLTYPE CreateInstance( 
        IUnknown *pUnkOuter,
        REFIID riid,
        void **ppvObject) = 0;
    
    virtual int32_t STDMETHODCALLTYPE LockServer( 
        BOOL fLock) = 0;
};

#endif // __IClassFactory_INTERFACE_DEFINED__

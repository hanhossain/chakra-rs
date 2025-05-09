//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
//
//

//
// ===========================================================================
// File: palrt.h
//
// ===========================================================================

/*++


Abstract:

    Rotor runtime functions.  These are functions which are ordinarily
    implemented as part of the Win32 API set, but for Rotor, are
    implemented as a runtime library on top of the PAL.

Author:



Revision History:

--*/

#ifndef __PALRT_H__
#define __PALRT_H__

/******************* HRESULTs *********************************************/

#ifdef RC_INVOKED
#define _HRESULT_TYPEDEF_(_sc) _sc
#else // RC_INVOKED
#define _HRESULT_TYPEDEF_(_sc) ((HRESULT)_sc)
#endif // RC_INVOKED

#define S_OK                             _HRESULT_TYPEDEF_(0x00000000L)
#define S_FALSE                          _HRESULT_TYPEDEF_(0x00000001L)

#define E_NOTIMPL                        _HRESULT_TYPEDEF_(0x80004001L)
#define E_NOINTERFACE                    _HRESULT_TYPEDEF_(0x80004002L)
#define E_UNEXPECTED                     _HRESULT_TYPEDEF_(0x8000FFFFL)
#define E_OUTOFMEMORY                    _HRESULT_TYPEDEF_(0x8007000EL)
#define E_INVALIDARG                     _HRESULT_TYPEDEF_(0x80070057L)
#define E_POINTER                        _HRESULT_TYPEDEF_(0x80004003L)
#define E_HANDLE                         _HRESULT_TYPEDEF_(0x80070006L)
#define E_ABORT                          _HRESULT_TYPEDEF_(0x80004004L)
#define E_FAIL                           _HRESULT_TYPEDEF_(0x80004005L)
#define E_ACCESSDENIED                   _HRESULT_TYPEDEF_(0x80070005L)
#define E_PENDING                        _HRESULT_TYPEDEF_(0x8000000AL)

#define DISP_E_PARAMNOTFOUND             _HRESULT_TYPEDEF_(0x80020004L)
#define DISP_E_TYPEMISMATCH              _HRESULT_TYPEDEF_(0x80020005L)
#define DISP_E_BADVARTYPE                _HRESULT_TYPEDEF_(0x80020008L)
#define DISP_E_OVERFLOW                  _HRESULT_TYPEDEF_(0x8002000AL)
#define DISP_E_DIVBYZERO                 _HRESULT_TYPEDEF_(0x80020012L)

#define CO_E_CLASSSTRING                 _HRESULT_TYPEDEF_(0x800401F3L)

#define URL_E_INVALID_SYNTAX             _HRESULT_TYPEDEF_(0x80041001L)
#define MK_E_SYNTAX                      _HRESULT_TYPEDEF_(0x800401E4L)

#define STG_E_FILENOTFOUND               _HRESULT_TYPEDEF_(0x80030002L)
#define STG_E_PATHNOTFOUND               _HRESULT_TYPEDEF_(0x80030003L)
#define STG_E_WRITEFAULT                 _HRESULT_TYPEDEF_(0x8003001DL)
#define STG_E_FILEALREADYEXISTS          _HRESULT_TYPEDEF_(0x80030050L)

#define NTE_BAD_HASH                     _HRESULT_TYPEDEF_(0x80090002L)
#define NTE_BAD_KEY                      _HRESULT_TYPEDEF_(0x80090003L)

#define STD_CTL_SCODE(n) MAKE_SCODE(SEVERITY_ERROR, FACILITY_CONTROL, n)
/********************** errorrep.h ****************************************/

typedef enum tagEFaultRepRetVal
{
    frrvOk = 0,
    frrvOkManifest,
    frrvOkQueued,
    frrvErr,
    frrvErrNoDW,
    frrvErrTimeout,
    frrvLaunchDebugger,
    frrvOkHeadless,
    frrvErrAnotherInstance
} EFaultRepRetVal;

/**************************************************************************/

#ifndef RC_INVOKED

#include "pal.h"

#include <pal_assert.h>

#if defined(_DEBUG)
#define ROTOR_PAL_CTOR_TEST_BODY(TESTNAME)                              \
    class TESTNAME ## _CTOR_TEST {                                      \
    public:                                                             \
        class HelperClass {                                             \
        public:                                                         \
            HelperClass(const char *String) {                           \
                _ASSERTE (m_s == NULL);                                 \
                m_s = String;                                           \
            }                                                           \
                                                                        \
            void Validate (const char *String) {                        \
                _ASSERTE (m_s);                                         \
                _ASSERTE (m_s == String);                               \
                _ASSERTE (!strncmp (                                    \
                              m_s,                                      \
                              String,                                   \
                              1000));                                   \
            }                                                           \
                                                                        \
        private:                                                        \
            const char *m_s;                                            \
        };                                                              \
                                                                        \
        TESTNAME ## _CTOR_TEST() {                                      \
            _ASSERTE (m_This == NULL);                                  \
            m_This = this;                                              \
        }                                                               \
                                                                        \
        void Validate () {                                              \
            _ASSERTE (m_This == this);                                  \
            m_String.Validate(#TESTNAME "_CTOR_TEST");                  \
        }                                                               \
                                                                        \
    private:                                                            \
        void              *m_This;                                      \
        static HelperClass m_String;                                    \
    };                                                                  \
                                                                        \
    static TESTNAME ## _CTOR_TEST                                       \
      g_ ## TESTNAME ## _CTOR_TEST;                                     \
    TESTNAME ## _CTOR_TEST::HelperClass                                 \
      TESTNAME ## _CTOR_TEST::m_String(#TESTNAME "_CTOR_TEST");

#define ROTOR_PAL_CTOR_TEST_RUN(TESTNAME)                               \
    g_ ## TESTNAME ##_CTOR_TEST.Validate()

#else // DEBUG

#define ROTOR_PAL_CTOR_TEST_BODY(TESTNAME)
#define ROTOR_PAL_CTOR_TEST_RUN(TESTNAME)  do {} while (0)

#endif // DEBUG

#define NTAPI
#define WINAPI
#define CALLBACK
#define NTSYSAPI

#define _WINNT_

// C++ standard, 18.1.5 - offsetof requires a POD (plain old data) struct or
// union. Since offsetof is a macro, gcc doesn't actually check for improper
// use of offsetof, it keys off of the -> from NULL (which is also invalid for
// non-POD types by 18.1.5)
//
// As we have numerous examples of this behavior in our codebase,
// making an offsetof which doesn't use 0.

// PAL_safe_offsetof is a version of offsetof that protects against an
// overridden operator&

#if defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 5 || __GNUC__ > 3)
#define FIELD_OFFSET(type, field) __builtin_offsetof(type, field)
#ifndef offsetof
#define offsetof(type, field) __builtin_offsetof(type, field)
#endif
#define PAL_safe_offsetof(type, field) __builtin_offsetof(type, field)
#else
#define FIELD_OFFSET(type, field) (((int32_t)(ptrdiff_t)&(((type *)64)->field)) - 64)
#ifndef offsetof
#define offsetof(s,m)          ((size_t)((ptrdiff_t)&(((s *)64)->m)) - 64)
#endif
#define PAL_safe_offsetof(s,m) ((size_t)((ptrdiff_t)&(char&)(((s *)64)->m))-64)
#endif

#define CONTAINING_RECORD(address, type, field) \
    ((type *)((ptrdiff_t)(address) - FIELD_OFFSET(type, field)))

#define ARGUMENT_PRESENT(ArgumentPointer)    (\
    (char *)(ArgumentPointer) != (char *)(NULL) )

#define MAX_NATURAL_ALIGNMENT sizeof(uint64_t)

#define DECLARE_HANDLE(name) struct name##__ { int unused; }; typedef struct name##__ *name

#ifndef COM_NO_WINDOWS_H
#define COM_NO_WINDOWS_H
#endif

#define interface struct

#define STDMETHODCALLTYPE

#define STDMETHODIMP         HRESULT STDMETHODCALLTYPE
#define STDMETHODIMP_(type)  type STDMETHODCALLTYPE

#define STDMETHODIMPV        HRESULT
#define STDMETHODIMPV_(type) type

#define STDMETHOD(method)       virtual HRESULT STDMETHODCALLTYPE method
#define STDMETHOD_(type,method) virtual type STDMETHODCALLTYPE method

#define STDMETHODV(method)       virtual HRESULT method
#define STDMETHODV_(type,method) virtual type method

#define STDAPI               EXTERN_C HRESULT
#define STDAPI_(type)        EXTERN_C type

#define STDAPIV              EXTERN_C HRESULT
#define STDAPIV_(type)       EXTERN_C type

#define PURE                    = 0
#define THIS_
#define THIS                void

#ifndef _DECLSPEC_DEFINED_
#define _DECLSPEC_DEFINED_

#if defined(__GNUC__)
#define DECLSPEC_NOVTABLE
#define DECLSPEC_IMPORT
#define DECLSPEC_SELECTANY  __attribute__((weak))
#else
#define DECLSPEC_NOVTABLE
#define DECLSPEC_IMPORT
#define DECLSPEC_SELECTANY
#endif

#endif // !_DECLSPEC_DEFINED_

#define DECLARE_INTERFACE(iface)    interface DECLSPEC_NOVTABLE iface
#define DECLARE_INTERFACE_(iface, baseiface)    interface DECLSPEC_NOVTABLE iface : public baseiface

#ifdef __cplusplus
#define REFGUID const GUID &
#else
#define REFGUID const GUID *
#endif

EXTERN_C const GUID GUID_NULL;

typedef GUID *LPGUID;
typedef const GUID  *LPCGUID;

#ifdef __cplusplus
extern "C++" {
#if !defined _SYS_GUID_OPERATOR_EQ_ && !defined _NO_SYS_GUID_OPERATOR_EQ_
#define _SYS_GUID_OPERATOR_EQ_
inline int IsEqualGUID(REFGUID rguid1, REFGUID rguid2)
    { return !memcmp(&rguid1, &rguid2, sizeof(GUID)); }
inline int operator==(REFGUID guidOne, REFGUID guidOther)
    { return IsEqualGUID(guidOne,guidOther); }
inline int operator!=(REFGUID guidOne, REFGUID guidOther)
    { return !IsEqualGUID(guidOne,guidOther); }
#endif
};
#endif // __cplusplus

#define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
    EXTERN_C const GUID  name

typedef GUID IID;
#ifdef __cplusplus
#define REFIID const IID &
#else
#define REFIID const IID *
#endif
#define IID_NULL GUID_NULL
#define IsEqualIID(riid1, riid2) IsEqualGUID(riid1, riid2)

#define __IID_DEFINED__

typedef GUID CLSID;
#define CLSID_DEFINED
#ifdef __cplusplus
#define REFCLSID const CLSID &
#else
#define REFCLSID const CLSID *
#endif
#define CLSID_NULL GUID_NULL
#define IsEqualCLSID(rclsid1, rclsid2) IsEqualGUID(rclsid1, rclsid2)
typedef CLSID *LPCLSID;

typedef UINT_PTR WPARAM;
typedef ptrdiff_t LRESULT;

typedef int32_t SCODE;


typedef union _ULARGE_INTEGER {
    struct {
#if BIGENDIAN
        uint32_t HighPart;
        uint32_t LowPart;
#else
        uint32_t LowPart;
        uint32_t HighPart;
#endif
    }
#ifndef PAL_STDCPP_COMPAT
    u
#endif // PAL_STDCPP_COMPAT
     ;
    ULONGLONG QuadPart;
} ULARGE_INTEGER, *PULARGE_INTEGER;

/******************* HRESULT types ****************************************/

#define FACILITY_WINDOWS                 8
#define FACILITY_STORAGE                 3
#define FACILITY_WIN32                   7
#define FACILITY_CONTROL                 10
#define FACILITY_NULL                    0
#define FACILITY_ITF                     4
#define FACILITY_DISPATCH                2

#define NO_ERROR 0L

#define SEVERITY_SUCCESS    0
#define SEVERITY_ERROR      1

#define SUCCEEDED(Status) ((HRESULT)(Status) >= 0)
#define FAILED(Status) ((HRESULT)(Status)<0)
#define IS_ERROR(Status) ((uint32_t)(Status) >> 31 == SEVERITY_ERROR) // diff from win32
#define HRESULT_CODE(hr)    ((hr) & 0xFFFF)
#define SCODE_CODE(sc)      ((sc) & 0xFFFF)
#define HRESULT_FACILITY(hr)  (((hr) >> 16) & 0x1fff)
#define SCODE_FACILITY(sc)    (((sc) >> 16) & 0x1fff)
#define HRESULT_SEVERITY(hr)  (((hr) >> 31) & 0x1)
#define SCODE_SEVERITY(sc)    (((sc) >> 31) & 0x1)

// both macros diff from Win32
#define MAKE_HRESULT(sev,fac,code) \
    ((HRESULT) (((uint32_t)(sev)<<31) | ((uint32_t)(fac)<<16) | ((uint32_t)(code))) )
#define MAKE_SCODE(sev,fac,code) \
    ((SCODE) (((uint32_t)(sev)<<31) | ((uint32_t)(fac)<<16) | ((int32_t)(code))) )

#define FACILITY_NT_BIT                 0x10000000
#define HRESULT_FROM_WIN32(x) ((HRESULT)(x) <= 0 ? ((HRESULT)(x)) : ((HRESULT) (((x) & 0x0000FFFF) | (FACILITY_WIN32 << 16) | 0x80000000)))
#define __HRESULT_FROM_WIN32(x) HRESULT_FROM_WIN32(x)

#define HRESULT_FROM_NT(x)      ((HRESULT) ((x) | FACILITY_NT_BIT))

/******************* OLE, BSTR, VARIANT *************************/

STDAPI_(void *) CoTaskMemAlloc(SIZE_T cb);
STDAPI_(void *) CoTaskMemRealloc(void * pv, SIZE_T cb);
STDAPI_(void) CoTaskMemFree(void * pv);

typedef short VARIANT_BOOL;
#define VARIANT_TRUE ((VARIANT_BOOL)-1)
#define VARIANT_FALSE ((VARIANT_BOOL)0)

typedef WCHAR OLECHAR;
typedef OLECHAR* LPOLESTR;
typedef const OLECHAR* LPCOLESTR;

typedef WCHAR *BSTR;

STDAPI_(BSTR) SysAllocString(const OLECHAR*);
STDAPI_(BSTR) SysAllocStringLen(const OLECHAR*, uint32_t);
STDAPI_(BSTR) SysAllocStringByteLen(const char *, uint32_t);
STDAPI_(void) SysFreeString(BSTR);
STDAPI_(uint32_t) SysStringLen(BSTR);
STDAPI_(uint32_t) SysStringByteLen(BSTR);

typedef double DATE;

typedef union tagCY {
    struct {
#if BIGENDIAN
        int32_t    Hi;
        uint32_t   Lo;
#else
        uint32_t   Lo;
        int32_t    Hi;
#endif
    } u;
    LONGLONG int64;
} CY, *LPCY;

typedef CY CURRENCY;

typedef struct tagDEC {
    // Decimal.cs treats the first two shorts as one long
    // And they seriable the data so we need to little endian
    // seriliazation
    // The wReserved overlaps with Variant's vt member
#if BIGENDIAN
    union {
        struct {
            uint8_t sign;
            uint8_t scale;
        };
        unsigned short signscale;
    };
    unsigned short wReserved;
#else
    unsigned short wReserved;
    union {
        struct {
            uint8_t scale;
            uint8_t sign;
        };
        unsigned short signscale;
    };
#endif
    uint32_t Hi32;
    union {
        struct {
            uint32_t Lo32;
            uint32_t Mid32;
        };
        ULONGLONG Lo64;
    };
} DECIMAL, *LPDECIMAL;

typedef struct tagBLOB {
    uint32_t cbSize;
    uint8_t *pBlobData;
} BLOB, *LPBLOB;

interface IStream;
interface IRecordInfo;

typedef unsigned short VARTYPE;

enum VARENUM {
    VT_EMPTY    = 0,
    VT_NULL = 1,
    VT_I2   = 2,
    VT_I4   = 3,
    VT_R4   = 4,
    VT_R8   = 5,
    VT_CY   = 6,
    VT_DATE = 7,
    VT_BSTR = 8,
    VT_DISPATCH = 9,
    VT_ERROR    = 10,
    VT_BOOL = 11,
    VT_VARIANT  = 12,
    VT_UNKNOWN  = 13,
    VT_DECIMAL  = 14,
    VT_I1   = 16,
    VT_UI1  = 17,
    VT_UI2  = 18,
    VT_UI4  = 19,
    VT_I8   = 20,
    VT_UI8  = 21,
    VT_INT  = 22,
    VT_UINT = 23,
    VT_VOID = 24,
    VT_HRESULT  = 25,
    VT_PTR  = 26,
    VT_SAFEARRAY    = 27,
    VT_CARRAY   = 28,
    VT_USERDEFINED  = 29,
    VT_LPSTR    = 30,
    VT_LPWSTR   = 31,
    VT_RECORD   = 36,
    VT_INT_PTR	= 37,
    VT_UINT_PTR	= 38,

    VT_FILETIME        = 64,
    VT_BLOB            = 65,
    VT_STREAM          = 66,
    VT_STORAGE         = 67,
    VT_STREAMED_OBJECT = 68,
    VT_STORED_OBJECT   = 69,
    VT_BLOB_OBJECT     = 70,
    VT_CF              = 71,
    VT_CLSID           = 72,

    VT_VECTOR   = 0x1000,
    VT_ARRAY    = 0x2000,
    VT_BYREF    = 0x4000,
    VT_TYPEMASK = 0xfff,
};

typedef struct tagVARIANT VARIANT, *LPVARIANT;
typedef struct tagSAFEARRAY SAFEARRAY;

struct tagVARIANT
    {
    union
        {
        struct
            {
#if BIGENDIAN
            // We need to make sure vt overlaps with DECIMAL's wReserved.
            // See the DECIMAL type for details.
            uint16_t wReserved1;
            VARTYPE vt;
#else
            VARTYPE vt;
            uint16_t wReserved1;
#endif
            uint16_t wReserved2;
            uint16_t wReserved3;
            union
                {
                LONGLONG llVal;
                int32_t lVal;
                uint8_t bVal;
                short iVal;
                float fltVal;
                double dblVal;
                VARIANT_BOOL boolVal;
                SCODE scode;
                CY cyVal;
                DATE date;
                BSTR bstrVal;
                interface IUnknown *punkVal;
                interface IDispatch *pdispVal;
                SAFEARRAY *parray;
                uint8_t *pbVal;
                short *piVal;
                int32_t *plVal;
                LONGLONG *pllVal;
                float *pfltVal;
                double *pdblVal;
                VARIANT_BOOL *pboolVal;
                SCODE *pscode;
                CY *pcyVal;
                DATE *pdate;
                BSTR *pbstrVal;
                interface IUnknown **ppunkVal;
                VARIANT *pvarVal;
                void * byref;
                char cVal;
                unsigned short uiVal;
                uint32_t ulVal;
                ULONGLONG ullVal;
                int32_t intVal;
                uint32_t uintVal;
                DECIMAL *pdecVal;
                char *pcVal;
                unsigned short *puiVal;
                uint32_t *pulVal;
                ULONGLONG *pullVal;
                int32_t *pintVal;
                uint32_t *puintVal;
                struct __tagBRECORD
                    {
                    void * pvRecord;
                    interface IRecordInfo *pRecInfo;
                    } brecVal;
                } n3;
            } n2;
        DECIMAL decVal;
        } n1;
    };

typedef VARIANT VARIANTARG, *LPVARIANTARG;

STDAPI_(void) VariantInit(VARIANT * pvarg);
STDAPI_(HRESULT) VariantClear(VARIANT * pvarg);

#define V_VT(X)         ((X)->n1.n2.vt)
#define V_UNION(X, Y)   ((X)->n1.n2.n3.Y)
#define V_RECORDINFO(X) ((X)->n1.n2.n3.brecVal.pRecInfo)
#define V_RECORD(X)     ((X)->n1.n2.n3.brecVal.pvRecord)

#define V_UI1(X)         V_UNION(X, bVal)
#define V_UI1REF(X)      V_UNION(X, pbVal)
#define V_I2(X)          V_UNION(X, iVal)
#define V_I2REF(X)       V_UNION(X, piVal)
#define V_I4(X)          V_UNION(X, lVal)
#define V_I4REF(X)       V_UNION(X, plVal)
#define V_I8(X)          V_UNION(X, llVal)
#define V_I8REF(X)       V_UNION(X, pllVal)
#define V_R4(X)          V_UNION(X, fltVal)
#define V_R4REF(X)       V_UNION(X, pfltVal)
#define V_R8(X)          V_UNION(X, dblVal)
#define V_R8REF(X)       V_UNION(X, pdblVal)
#define V_I1(X)          V_UNION(X, cVal)
#define V_I1REF(X)       V_UNION(X, pcVal)
#define V_UI2(X)         V_UNION(X, uiVal)
#define V_UI2REF(X)      V_UNION(X, puiVal)
#define V_UI4(X)         V_UNION(X, ulVal)
#define V_UI4REF(X)      V_UNION(X, pulVal)
#define V_UI8(X)         V_UNION(X, ullVal)
#define V_UI8REF(X)      V_UNION(X, pullVal)
#define V_INT(X)         V_UNION(X, intVal)
#define V_INTREF(X)      V_UNION(X, pintVal)
#define V_UINT(X)        V_UNION(X, uintVal)
#define V_UINTREF(X)     V_UNION(X, puintVal)
#define V_ARRAY(X)       V_UNION(X, parray)

#define V_INT_PTR(X)        V_UNION(X, llVal)
#define V_UINT_PTR(X)       V_UNION(X, ullVal)
#define V_INT_PTRREF(X)     V_UNION(X, pllVal)
#define V_UINT_PTRREF(X)    V_UNION(X, pullVal)

#define V_CY(X)          V_UNION(X, cyVal)
#define V_CYREF(X)       V_UNION(X, pcyVal)
#define V_DATE(X)        V_UNION(X, date)
#define V_DATEREF(X)     V_UNION(X, pdate)
#define V_BSTR(X)        V_UNION(X, bstrVal)
#define V_BSTRREF(X)     V_UNION(X, pbstrVal)
#define V_UNKNOWN(X)     V_UNION(X, punkVal)
#define V_UNKNOWNREF(X)  V_UNION(X, ppunkVal)
#define V_VARIANTREF(X)  V_UNION(X, pvarVal)
#define V_DISPATCH(X)    V_UNION(X, pdispVal)
#define V_ERROR(X)       V_UNION(X, scode)
#define V_ERRORREF(X)    V_UNION(X, pscode)
#define V_BOOL(X)        V_UNION(X, boolVal)
#define V_BOOLREF(X)     V_UNION(X, pboolVal)
#define V_BYREF(X)       V_UNION(X, byref)

#define V_DECIMAL(X)     ((X)->n1.decVal)
#define V_DECIMALREF(X)    V_UNION(X, pdecVal)

#define V_ISBYREF(X)     (V_VT(X)&VT_BYREF)

STDAPI CreateStreamOnHGlobal(void * hGlobal, BOOL fDeleteOnRelease, interface IStream** ppstm);

STDAPI IIDFromString(LPOLESTR lpsz, IID* lpiid);
STDAPI_(int) StringFromGUID2(REFGUID rguid, LPOLESTR lpsz, int cchMax);

/******************* CRYPT **************************************/

#define PUBLICKEYBLOB           0x6

//
// Algorithm IDs and Flags
//
#define GET_ALG_CLASS(x)        (x & (7 << 13))
#define GET_ALG_TYPE(x)         (x & (15 << 9))
#define GET_ALG_SID(x)          (x & (511))

typedef unsigned int ALG_ID;

// Algorithm classes
#define ALG_CLASS_SIGNATURE     (1 << 13)
#define ALG_CLASS_HASH          (4 << 13)

// Algorithm types
#define ALG_TYPE_ANY            (0)

// Hash sub ids
#define ALG_SID_SHA1            4

// algorithm identifier definitions
#define CALG_SHA1               (ALG_CLASS_HASH | ALG_TYPE_ANY | ALG_SID_SHA1)

/******************* NLS ****************************************/

typedef
enum tagMIMECONTF {
    MIMECONTF_MAILNEWS  = 0x1,
    MIMECONTF_BROWSER   = 0x2,
    MIMECONTF_MINIMAL   = 0x4,
    MIMECONTF_IMPORT    = 0x8,
    MIMECONTF_SAVABLE_MAILNEWS  = 0x100,
    MIMECONTF_SAVABLE_BROWSER   = 0x200,
    MIMECONTF_EXPORT    = 0x400,
    MIMECONTF_PRIVCONVERTER = 0x10000,
    MIMECONTF_VALID = 0x20000,
    MIMECONTF_VALID_NLS = 0x40000,
    MIMECONTF_MIME_IE4  = 0x10000000,
    MIMECONTF_MIME_LATEST   = 0x20000000,
    MIMECONTF_MIME_REGISTRY = 0x40000000
    }   MIMECONTF;

#define LCMAP_LOWERCASE           0x00000100
#define LCMAP_UPPERCASE           0x00000200
#define LCMAP_SORTKEY             0x00000400
#define LCMAP_BYTEREV             0x00000800

#define LCMAP_HIRAGANA            0x00100000
#define LCMAP_KATAKANA            0x00200000
#define LCMAP_HALFWIDTH           0x00400000
#define LCMAP_FULLWIDTH           0x00800000

#define LCMAP_LINGUISTIC_CASING   0x01000000

// 8 characters for language
// 8 characters for region
// 64 characters for suffix (script)
// 2 characters for '-' separators
// 2 characters for prefix like "i-" or "x-"
// 1 null termination
#define LOCALE_NAME_MAX_LENGTH   85

#define LOCALE_SCOUNTRY           0x00000006
#define LOCALE_SENGCOUNTRY        0x00001002

#define LOCALE_SLANGUAGE          0x00000002
#define LOCALE_SENGLANGUAGE       0x00001001

#define LOCALE_SDATE              0x0000001D
#define LOCALE_STIME              0x0000001E

#define CSTR_LESS_THAN            1
#define CSTR_EQUAL                2
#define CSTR_GREATER_THAN         3

#define NORM_IGNORENONSPACE       0x00000002

#define WC_COMPOSITECHECK         0x00000000 // NOTE: diff from winnls.h

/******************* shlwapi ************************************/

// note: diff in NULL handing and calling convetion
#define StrCpyW                 PAL_wcscpy
#define StrCpyNW                lstrcpynW // note: can't be wcsncpy!
#define StrCatW                 PAL_wcscat
#define StrChrW                 (WCHAR*)PAL_wcschr
#define StrCmpW                 PAL_wcscmp
#define StrCmpIW                _wcsicmp
#define StrCmpNW                PAL_wcsncmp
#define StrCmpNIW               _wcsnicmp

STDAPI_(LPWSTR) StrNCatW(LPWSTR lpFront, LPCWSTR lpBack, int cchMax);
STDAPI_(int) StrToIntW(LPCWSTR lpSrc);
STDAPI_(LPWSTR) StrStrIW(LPCWSTR lpFirst, LPCWSTR lpSrch);
STDAPI_(LPWSTR) StrRChrW(LPCWSTR lpStart, LPCWSTR lpEnd, WCHAR wMatch);
STDAPI_(LPWSTR) StrCatBuffW(LPWSTR pszDest, LPCWSTR pszSrc, int cchDestBuffSize);

#define lstrcmpW                PAL_wcscmp
#define lstrcmpiW               _wcsicmp
#define wnsprintfW              _snwprintf // note: not 100% compatible (wsprintf should be subset of sprintf...)
#define wvnsprintfW             _vsnwprintf // note: not 100% compatible (wsprintf should be subset of sprintf...)

#ifdef UNICODE
#define StrCpy                  StrCpyW
#define StrCpyN                 StrCpyNW
#define StrCat                  StrCatW
#define StrNCat                 StrNCatW
#define StrChr                  StrChrW
#define StrCmp                  StrCmpW
#define StrCmpN                 StrCmpNW
#define StrCmpI                 StrCmpIW
#define StrCmpNI                StrCmpNIW

#define StrToInt                StrToIntW
#define StrStrI                 StrStrIW
#define StrRChr                 StrRChrW
#define StrCatBuff              StrCatBuffW

#define lstrcmp                 lstrcmpW
#define lstrcmpi                lstrcmpiW
#define wnsprintf               wnsprintfW
#endif


#ifdef __cplusplus
/*
  Safe CRT functions are not available (yet) on all platforms, so we use our own implementations from safecrt.h.
*/
#define _CRT_ALTERNATIVE_INLINES
#define _SAFECRT_NO_INCLUDES 1
#define _SAFECRT_USE_INLINES 1
#define _SAFECRT_SET_ERRNO 0
#define _SAFECRT_DEFINE_MBS_FUNCTIONS 0
#define _SAFECRT_DEFINE_TCS_MACROS 1
/*
#define _SAFECRT__ISMBBLEAD(_Character) 0
#define _SAFECRT__MBSDEC(_String, _Current) (_Current - 1)
*/
#include "safecrt.h"
#include "specstrings.h"

/*
The wrappers below are simple implementations that may not be as robust as complete functions in the Secure CRT library.
Remember to fix the errcode defintion in safecrt.h.
*/

#define _wcslwr_s _wcslwr_unsafe
#define _snwprintf_s _snwprintf_unsafe
#define _vsnwprintf_s _vsnwprintf_unsafe
#define _snprintf_s _snprintf_unsafe
#define _vsnprintf_s _vsnprintf_unsafe
#define swscanf_s swscanf
#define sscanf_s sscanf

#define _wfopen_s _wfopen_unsafe
#define fopen_s _fopen_unsafe

#define _strlwr_s _strlwr_unsafe

#define _vscprintf _vscprintf_unsafe
#define _vscwprintf _vscwprintf_unsafe
#define _scwprintf _scwprintf_unsafe

extern "C++" {

#include <safemath.h>

inline errno_t _wcslwr_unsafe(WCHAR *str, size_t sz)
{
    if (sz >= INT_MAX / sizeof(WCHAR))
        return 1;

    size_t fullSize = sz * sizeof(WCHAR);
    WCHAR *copy = (WCHAR *)malloc(fullSize);
    if(copy == nullptr)
        return 1;

    errno_t retCode = wcscpy_s(copy, sz, str);
    if(retCode) {
        free(copy);
        return 1;
    }

    _wcslwr(copy);
    wcscpy_s(str, sz, copy);
    free(copy);

    return 0;
}
inline errno_t _strlwr_unsafe(char *str, size_t sz)
{
    char *copy = (char *)malloc(sz);
    if(copy == nullptr)
        return 1;

    errno_t retCode = strcpy_s(copy, sz, str);
    if(retCode) {
        free(copy);
        return 1;
    }

    _strlwr(copy);
    strcpy_s(str, sz, copy);
    free(copy);

    return 0;
}

inline int _vscprintf_unsafe(const char *_Format, va_list _ArgList)
{
    int guess = 10;

    for (;;)
    {
        char *buf = (char *)malloc(guess * sizeof(char));
        if(buf == nullptr)
            return 0;

        int ret = _vsnprintf(buf, guess, _Format, _ArgList);
        free(buf);

        if ((ret != -1) && (ret < guess))
            return ret;

        guess *= 2;
    }
}

inline int _vscwprintf_unsafe(const WCHAR *_Format, va_list _ArgList)
{
    int guess = 256;

    for (;;)
    {
        WCHAR *buf = (WCHAR *)malloc(guess * sizeof(WCHAR));
        if (buf == nullptr)
            return 0;

        va_list apcopy;
        va_copy(apcopy, _ArgList);
        int ret = _vsnwprintf(buf, guess, _Format, apcopy);
        free(buf);
        va_end(apcopy);

        if ((ret != -1) && (ret < guess))
            return ret;

        guess *= 2;
    }
}

inline int _scwprintf_unsafe(const WCHAR *_Format, ...)
{
    int ret;
    va_list _ArgList;
    va_start(_ArgList, _Format);
    ret = _vscwprintf_unsafe(_Format, _ArgList);
    va_end(_ArgList);
    return ret;
}

inline int _vsnwprintf_unsafe(WCHAR *_Dst, size_t _SizeInWords, size_t _Count, const WCHAR *_Format, va_list _ArgList)
{
    if (_Count == _TRUNCATE) _Count = _SizeInWords;
    int ret = _vsnwprintf(_Dst, _Count, _Format, _ArgList);
    _Dst[_SizeInWords - 1] = L'\0';
    if (ret < 0 && errno == 0)
    {
        errno = ERANGE;
    }
    return ret;
}

inline int _snwprintf_unsafe(WCHAR *_Dst, size_t _SizeInWords, size_t _Count, const WCHAR *_Format, ...)
{
    int ret;
    va_list _ArgList;
    va_start(_ArgList, _Format);
    ret = _vsnwprintf_unsafe(_Dst, _SizeInWords, _Count, _Format, _ArgList);
    va_end(_ArgList);
    return ret;
}

inline int _vsnprintf_unsafe(char *_Dst, size_t _SizeInWords, size_t _Count, const char *_Format, va_list _ArgList)
{
    if (_Count == _TRUNCATE) _Count = _SizeInWords;
    int ret = _vsnprintf(_Dst, _Count, _Format, _ArgList);
    _Dst[_SizeInWords - 1] = L'\0';
    if (ret < 0 && errno == 0)
    {
        errno = ERANGE;
    }
    return ret;
}

inline int _snprintf_unsafe(char *_Dst, size_t _SizeInWords, size_t _Count, const char *_Format, ...)
{
    int ret;
    va_list _ArgList;
    va_start(_ArgList, _Format);
    ret = _vsnprintf_unsafe(_Dst, _SizeInWords, _Count, _Format, _ArgList);
    va_end(_ArgList);
    return ret;
}

inline errno_t _wfopen_unsafe(PAL_FILE * *ff, const WCHAR *fileName, const WCHAR *mode)
{
    PAL_FILE *result = _wfopen(fileName, mode);
    if(result == 0) {
        return 1;
    } else {
        *ff = result;
        return 0;
    }
}

inline errno_t _fopen_unsafe(PAL_FILE * *ff, const char *fileName, const char *mode)
{
  PAL_FILE *result = PAL_fopen(fileName, mode);
  if(result == 0) {
    return 1;
  } else {
    *ff = result;
    return 0;
  }
}

/* _itow_s */
_SAFECRT__EXTERN_C
errno_t _itow_s(int _Value, WCHAR *_Dst, size_t _SizeInWords, int _Radix);

#if defined(__cplusplus) && _SAFECRT_USE_CPP_OVERLOADS
template <size_t _SizeInWords>
inline
errno_t _itow_s(int _Value, WCHAR (&_Dst)[_SizeInWords], int _Radix)
{
    return _itow_s(_Value, _Dst, _SizeInWords, _Radix);
}
#endif

#if _SAFECRT_USE_INLINES

__inline
errno_t _itow_s(int _Value, WCHAR *_Dst, size_t _SizeInWords, int _Radix)
{
    /* validation section */
    _SAFECRT__VALIDATE_STRING(_Dst, _SizeInWords);

    /* TODO: do not write past buffer size */
    _itow(_Value, _Dst, _Radix);
    return 0;
}

#endif

_SAFECRT__EXTERN_C
errno_t _ltow_s(long _Value, WCHAR *_Dst, size_t _SizeInWords, int _Radix);

#if defined(__cplusplus) && _SAFECRT_USE_CPP_OVERLOADS
template <size_t _SizeInWords>
inline
errno_t _ltow_s(long _Value, WCHAR (&_Dst)[_SizeInWords], int _Radix)
{
    return _ltow_s(_Value, _Dst, _SizeInWords, _Radix);
}
#endif

#if _SAFECRT_USE_INLINES

__inline
errno_t _ltow_s(long _Value, WCHAR *_Dst, size_t _SizeInWords, int _Radix)
{
    /* validation section */
    _SAFECRT__VALIDATE_STRING(_Dst, _SizeInWords);

    /* TODO: do not write past buffer size */
    _ltow(_Value, _Dst, _Radix);
    return 0;
}

#endif

/* _i64tow_s */
_SAFECRT__EXTERN_C
errno_t _i64tow_s(__int64 _Value, WCHAR *_Dst, size_t _SizeInWords, int _Radix);

#if defined(__cplusplus) && _SAFECRT_USE_CPP_OVERLOADS
template <size_t _SizeInWords>
inline
errno_t _i64tow_s(__int64 _Value, WCHAR (&_Dst)[_SizeInWords], int _Radix)
{
    return _i64tow_s(_Value, _Dst, _SizeInWords, _Radix);
}
#endif

#if _SAFECRT_USE_INLINES

__inline
errno_t _i64tow_s(__int64 _Value, WCHAR *_Dst, size_t _SizeInWords, int _Radix)
{
    /* validation section */
    _SAFECRT__VALIDATE_STRING(_Dst, _SizeInWords);

    /* TODO: do not write past buffer size */
    _i64tow(_Value, _Dst, _Radix);
    return 0;
}

#endif

/* getenv_s */
/*
 * _ReturnValue indicates if the variable has been found and size needed
 */
_SAFECRT__EXTERN_C
errno_t getenv_s(size_t *_ReturnValue, char *_Dst, size_t _SizeInWords, const char *_Name);

#if defined(__cplusplus) && _SAFECRT_USE_CPP_OVERLOADS
template <size_t _SzInWords>
inline
errno_t getenv_s(size_t *_ReturnValue, char *_Dst, size_t _SizeInWords, const char *_Name)
{
    return getenv_s(_ReturnValue, _Dst, _SizeInWords, _Name);
}
#endif

#if _SAFECRT_USE_INLINES

__inline
errno_t getenv_s(size_t *_ReturnValue, char *_Dst, size_t _SizeInWords, const char *_Name)
{
    char *szFound;

    /* validation section */
    _SAFECRT__VALIDATE_STRING(_Dst, _SizeInWords);

    szFound = getenv(_Name);
    if (szFound == nullptr)
    {
        *_ReturnValue = 0;
        return 0;
    }
    *_ReturnValue = strlen(szFound) + 1;
    return strcpy_s(_Dst, _SizeInWords, szFound);
}

#endif

}
#endif /* __cplusplus */


STDAPI_(BOOL) PathAppendW(LPWSTR pszPath, LPCWSTR pszMore);
STDAPI_(int) PathCommonPrefixW(LPCWSTR pszFile1, LPCWSTR pszFile2, LPWSTR  pszPath);
LPWSTR PathFindFileNameW(LPCWSTR pPath);
STDAPI_(int) PathGetDriveNumberW(LPCWSTR lpsz);
STDAPI_(BOOL) PathIsRelativeW(LPCWSTR lpszPath);
STDAPI_(BOOL) PathIsUNCW(LPCWSTR pszPath);
STDAPI_(LPWSTR) PathAddBackslashW(LPWSTR lpszPath);
STDAPI_(LPWSTR) PathRemoveBackslashW(LPWSTR lpszPath);
STDAPI_(void) PathRemoveExtensionW(LPWSTR pszPath);
STDAPI_(LPWSTR) PathCombineW(LPWSTR lpszDest, LPCWSTR lpszDir, LPCWSTR lpszFile);
STDAPI_(BOOL) PathCanonicalizeW(LPWSTR lpszDst, LPCWSTR lpszSrc);
STDAPI_(BOOL) PathRelativePathToW(LPWSTR pszPath, LPCWSTR pszFrom, uint32_t dwAttrFrom, LPCWSTR pszTo, uint32_t dwAttrTo);
STDAPI_(BOOL) PathRenameExtensionW(LPWSTR pszPath, LPCWSTR pszExt);
STDAPI_(BOOL) PathRemoveFileSpecW(LPWSTR pFile);
STDAPI_(void) PathStripPathW (LPWSTR pszPath);

STDAPI PathCreateFromUrlW(LPCWSTR pszUrl, LPWSTR pszPath, uint32_t * pcchPath, uint32_t dwFlags);
STDAPI_(BOOL) PathIsURLW(LPCWSTR pszPath);


#define URL_UNESCAPE                    0x10000000
#define URL_ESCAPE_PERCENT              0x00001000

typedef enum {
    URLIS_FILEURL = 3,
} URLIS;

typedef enum {
    URL_PART_SCHEME     = 1,
    URL_PART_HOSTNAME   = 2,
} URL_PART;

STDAPI UrlCanonicalizeW(LPCWSTR pszUrl, LPWSTR pszCanonicalized, uint32_t * pcchCanonicalized, uint32_t dwFlags);
STDAPI UrlCombineW(LPCWSTR pszBase, LPCWSTR pszRelative, LPWSTR pszCombined, uint32_t * pcchCombined, uint32_t dwFlags);
STDAPI UrlEscapeW(LPCWSTR pszUrl, LPWSTR pszEscaped, uint32_t * pcchEscaped, uint32_t dwFlags);
STDAPI UrlUnescapeW(LPWSTR pszURL, LPWSTR pszUnescaped, uint32_t * pcchUnescaped, uint32_t dwFlags);
STDAPI_(BOOL) UrlIsW(LPCWSTR pszUrl, URLIS dwUrlIs);
STDAPI UrlGetPartW(LPCWSTR pszIn, LPWSTR pszOut, uint32_t * pcchOut, uint32_t dwPart, uint32_t dwFlags);

#ifdef UNICODE
#define PathAppend          PathAppendW
#define PathCommonPrefix    PathCommonPrefixW
#define PathFindFileName    PathFindFileNameW
#define PathIsRelative      PathIsRelativeW
#define PathGetDriveNumber  PathGetDriveNumberW
#define PathIsUNC           PathIsUNCW
#define PathAddBackslash    PathAddBackslashW
#define PathRemoveBackslash PathRemoveBackslashW
#define PathRemoveExtension PathRemoveExtensionW
#define PathCombine         PathCombineW
#define PathSkipRoot        PathSkipRootW
#define PathFindExtension   PathFindExtensionW
#define PathCanonicalize    PathCanonicalizeW
#define PathRelativePathTo  PathRelativePathToW
#define PathRemoveFileSpec  PathRemoveFileSpecW
#define PathRenameExtension PathRenameExtensionW
#define PathStripPath       PathStripPathW

#define PathCreateFromUrl   PathCreateFromUrlW
#define PathIsURL           PathIsURLW

#define UrlCanonicalize     UrlCanonicalizeW
#define UrlCombine          UrlCombineW
#define UrlEscape           UrlEscapeW
#define UrlUnescape         UrlUnescapeW
#define UrlIs               UrlIsW
#define UrlGetPart          UrlGetPartW

#endif // UNICODE

/******************* misc ***************************************/

#define __RPC__out
#define __RPC__in
#define __RPC__deref_out_opt
#define __RPC__in_opt
#define __RPC__inout_xcount(x)
#define __RPC__in_ecount_full(x)
#define __RPC__out_ecount_part(x, y)
#define __RPC__in_xcount(x)
#define __RPC__inout
#define __RPC__deref_out_ecount_full_opt(x)

typedef uint32_t OLE_COLOR;

#define PF_COMPARE_EXCHANGE_DOUBLE          2

typedef void (NTAPI * WAITORTIMERCALLBACKFUNC) (void *, BOOLEAN );

typedef HANDLE HWND;

#define IS_TEXT_UNICODE_SIGNATURE             0x0008
#define IS_TEXT_UNICODE_UNICODE_MASK          0x000F

BOOL IsTextUnicode(const void* lpv, int iSize, int32_t * lpiResult);

typedef struct _LIST_ENTRY {
   struct _LIST_ENTRY *Flink;
   struct _LIST_ENTRY *Blink;
} LIST_ENTRY, *PLIST_ENTRY;

typedef void (*WAITORTIMERCALLBACK)(void *, BOOLEAN);

// PORTABILITY_ASSERT and PORTABILITY_WARNING macros are meant to be used to
// mark places in the code that needs attention for portability. The usual
// usage pattern is:
//
// int get_scratch_register() {
// #if defined(_TARGET_X86_)
//     return eax;
// #elif defined(_TARGET_AMD64_)
//     return rax;
// #elif defined(_TARGET_ARM_)
//     return r0;
// #else
//     PORTABILITY_ASSERT("scratch register");
//     return 0;
// #endif
// }
//
// PORTABILITY_ASSERT is meant to be used inside functions/methods. It can
// introduce compile-time and/or run-time errors.
// PORTABILITY_WARNING is meant to be used outside functions/methods. It can
// introduce compile-time errors or warnings only.
//
// People starting new ports will first define these to just cause run-time
// errors. Once they fix all the places that need attention for portability,
// they can define PORTABILITY_ASSERT and PORTABILITY_WARNING to cause
// compile-time errors to make sure that they haven't missed anything.
//
// If it is reasonably possible all codepaths containing PORTABILITY_ASSERT
// should be compilable (e.g. functions should return NULL or something if
// they are expected to return a value).
//
// The message in these two macros should not contain any keywords like TODO
// or NYI. It should be just the brief description of the problem.

#if defined(_TARGET_X86_)
// Finished ports - compile-time errors
#define PORTABILITY_WARNING(message)    NEED_TO_PORT_THIS_ONE(NEED_TO_PORT_THIS_ONE)
#define PORTABILITY_ASSERT(message)     NEED_TO_PORT_THIS_ONE(NEED_TO_PORT_THIS_ONE)
#else
// Ports in progress - run-time asserts only
#define PORTABILITY_WARNING(message)
#define PORTABILITY_ASSERT(message)     _ASSERTE(false && message)
#endif

#define UNREFERENCED_PARAMETER(P)          (void)(P)

#define VALPTR(x) VAL64(x)
#define GET_UNALIGNED_PTR(x) GET_UNALIGNED_64(x)
#define GET_UNALIGNED_VALPTR(x) GET_UNALIGNED_VAL64(x)
#define SET_UNALIGNED_PTR(p,x) SET_UNALIGNED_64(p,x)
#define SET_UNALIGNED_VALPTR(p,x) SET_UNALIGNED_VAL64(p,x)

#ifdef _TARGET_AMD64_
#define RUNTIME_FUNCTION_INDIRECT 0x1
#endif

#define _ReturnAddress() __builtin_return_address(0)

#define DIRECTORY_SEPARATOR_CHAR_A '/'
#define DIRECTORY_SEPARATOR_CHAR_W W('/')
#define DIRECTORY_SEPARATOR_STR_W W("/")
#define PATH_SEPARATOR_CHAR_W W(':')
#define PATH_SEPARATOR_STR_W W(":")
#define VOLUME_SEPARATOR_CHAR_W W('/')

#ifndef IMAGE_IMPORT_DESC_FIELD
#define IMAGE_IMPORT_DESC_FIELD(img, f)     ((img).u.f)
#endif

#ifndef IMAGE_COR20_HEADER_FIELD
#define IMAGE_COR20_HEADER_FIELD(obj, f)    ((obj).f)
#endif

//
// JIT Debugging Info. This structure is defined to have constant size in
// both the emulated and native environment.
//

typedef struct _JIT_DEBUG_INFO {
    uint32_t dwSize;
    uint32_t dwProcessorArchitecture;
    uint32_t dwThreadID;
    uint32_t dwReserved0;
    ULONG64 lpExceptionAddress;
    ULONG64 lpExceptionRecord;
    ULONG64 lpContextRecord;
} JIT_DEBUG_INFO, *LPJIT_DEBUG_INFO;

typedef JIT_DEBUG_INFO JIT_DEBUG_INFO32, *LPJIT_DEBUG_INFO32;
typedef JIT_DEBUG_INFO JIT_DEBUG_INFO64, *LPJIT_DEBUG_INFO64;

/******************* resources ***************************************/

#define MAKEINTRESOURCEW(i) ((LPWSTR)((size_t)((uint16_t)(i))))
#define RT_RCDATA           MAKEINTRESOURCE(10)
#define RT_VERSION          MAKEINTRESOURCE(16)

/******************* SAFEARRAY ************************/

#define	FADF_VARIANT	( 0x800 )

typedef struct tagSAFEARRAYBOUND
    {
    uint32_t cElements;
    int32_t lLbound;
    } 	SAFEARRAYBOUND;

typedef struct tagSAFEARRAYBOUND *LPSAFEARRAYBOUND;

typedef struct tagSAFEARRAY
    {
    unsigned short cDims;
    unsigned short fFeatures;
    uint32_t cbElements;
    uint32_t cLocks;
    void * pvData;
    SAFEARRAYBOUND rgsabound[ 1 ];
    } 	SAFEARRAY;

typedef SAFEARRAY *LPSAFEARRAY;


STDAPI_(SAFEARRAY *) SafeArrayCreateVector(VARTYPE vt, int32_t lLbound, uint32_t cElements);
STDAPI_(uint32_t) SafeArrayGetDim(SAFEARRAY * psa);
STDAPI SafeArrayGetElement(SAFEARRAY * psa, int32_t * rgIndices, void * pv);
STDAPI SafeArrayGetLBound(SAFEARRAY * psa, uint32_t nDim, int32_t * plLbound);
STDAPI SafeArrayGetUBound(SAFEARRAY * psa, uint32_t nDim, int32_t * plUbound);
STDAPI SafeArrayGetVartype(SAFEARRAY * psa, VARTYPE * pvt);
STDAPI SafeArrayPutElement(SAFEARRAY * psa, int32_t * rgIndices, void * pv);
STDAPI SafeArrayDestroy(SAFEARRAY * psa);

EXTERN_C void * _stdcall _lfind(const void *, const void *, unsigned int *, unsigned int,
        int (*)(const void *, const void *));

interface IDispatch;
interface ITypeInfo;
interface ITypeLib;
interface IMoniker;

typedef void (WINAPI *LPOVERLAPPED_COMPLETION_ROUTINE)(
    uint32_t dwErrorCode,
    uint32_t dwNumberOfBytesTransfered,
    void * lpOverlapped);

//
// Debug APIs
//

typedef struct _EXCEPTION_DEBUG_INFO {
    EXCEPTION_RECORD ExceptionRecord;
    uint32_t dwFirstChance;
} EXCEPTION_DEBUG_INFO, *LPEXCEPTION_DEBUG_INFO;

typedef struct _CREATE_THREAD_DEBUG_INFO {
    HANDLE hThread;
    void * lpThreadLocalBase;
    LPTHREAD_START_ROUTINE lpStartAddress;
} CREATE_THREAD_DEBUG_INFO, *LPCREATE_THREAD_DEBUG_INFO;

typedef struct _CREATE_PROCESS_DEBUG_INFO {
    HANDLE hFile;
    HANDLE hProcess;
    HANDLE hThread;
    void * lpBaseOfImage;
    uint32_t dwDebugInfoFileOffset;
    uint32_t nDebugInfoSize;
    void * lpThreadLocalBase;
    LPTHREAD_START_ROUTINE lpStartAddress;
    void * lpImageName;
    uint16_t fUnicode;
} CREATE_PROCESS_DEBUG_INFO, *LPCREATE_PROCESS_DEBUG_INFO;

typedef struct _EXIT_THREAD_DEBUG_INFO {
    uint32_t dwExitCode;
} EXIT_THREAD_DEBUG_INFO, *LPEXIT_THREAD_DEBUG_INFO;

typedef struct _EXIT_PROCESS_DEBUG_INFO {
    uint32_t dwExitCode;
} EXIT_PROCESS_DEBUG_INFO, *LPEXIT_PROCESS_DEBUG_INFO;

typedef struct _LOAD_DLL_DEBUG_INFO {
    HANDLE hFile;
    void * lpBaseOfDll;
    uint32_t dwDebugInfoFileOffset;
    uint32_t nDebugInfoSize;
    void * lpImageName;
    uint16_t fUnicode;
} LOAD_DLL_DEBUG_INFO, *LPLOAD_DLL_DEBUG_INFO;

typedef struct _UNLOAD_DLL_DEBUG_INFO {
    void * lpBaseOfDll;
} UNLOAD_DLL_DEBUG_INFO, *LPUNLOAD_DLL_DEBUG_INFO;

typedef struct _OUTPUT_DEBUG_STRING_INFO {
    LPSTR lpDebugStringData;
    uint16_t fUnicode;
    uint16_t nDebugStringLength;
} OUTPUT_DEBUG_STRING_INFO, *LPOUTPUT_DEBUG_STRING_INFO;

typedef struct _RIP_INFO {
    uint32_t dwError;
    uint32_t dwType;
} RIP_INFO, *LPRIP_INFO;

typedef struct _DEBUG_EVENT {
    uint32_t dwDebugEventCode;
    uint32_t dwProcessId;
    uint32_t dwThreadId;
    union {
        EXCEPTION_DEBUG_INFO Exception;
        CREATE_THREAD_DEBUG_INFO CreateThread;
        CREATE_PROCESS_DEBUG_INFO CreateProcessInfo;
        EXIT_THREAD_DEBUG_INFO ExitThread;
        EXIT_PROCESS_DEBUG_INFO ExitProcess;
        LOAD_DLL_DEBUG_INFO LoadDll;
        UNLOAD_DLL_DEBUG_INFO UnloadDll;
        OUTPUT_DEBUG_STRING_INFO DebugString;
        RIP_INFO RipInfo;
    } u;
} DEBUG_EVENT, *LPDEBUG_EVENT;

//
// Define dynamic function table entry.
//

typedef
PRUNTIME_FUNCTION
GET_RUNTIME_FUNCTION_CALLBACK (
    DWORD64 ControlPc,
    void * Context
    );
typedef GET_RUNTIME_FUNCTION_CALLBACK *PGET_RUNTIME_FUNCTION_CALLBACK;

typedef
uint32_t
OUT_OF_PROCESS_FUNCTION_TABLE_CALLBACK (
    HANDLE Process,
    void * TableAddress,
    uint32_t * Entries,
    PRUNTIME_FUNCTION* Functions
    );
typedef OUT_OF_PROCESS_FUNCTION_TABLE_CALLBACK *POUT_OF_PROCESS_FUNCTION_TABLE_CALLBACK;

#define OUT_OF_PROCESS_FUNCTION_TABLE_CALLBACK_EXPORT_NAME \
    "OutOfProcessFunctionTableCallback"

#if defined(FEATURE_PAL_SXS)

// #if !defined(__APPLE__)
// typedef int32_t (*PEXCEPTION_ROUTINE)(
    // IN PEXCEPTION_POINTERS pExceptionPointers,
    // IN void * lpvParam);

// #define DISPATCHER_CONTEXT    void *

// #else // defined(__APPLE__)

//
// Define unwind history table structure.
//

#define UNWIND_HISTORY_TABLE_SIZE 12

typedef struct _UNWIND_HISTORY_TABLE_ENTRY {
    DWORD64 ImageBase;
    PRUNTIME_FUNCTION FunctionEntry;
} UNWIND_HISTORY_TABLE_ENTRY, *PUNWIND_HISTORY_TABLE_ENTRY;

typedef struct _UNWIND_HISTORY_TABLE {
    uint32_t Count;
    uint8_t  LocalHint;
    uint8_t  GlobalHint;
    uint8_t  Search;
    uint8_t  Once;
    DWORD64 LowAddress;
    DWORD64 HighAddress;
    UNWIND_HISTORY_TABLE_ENTRY Entry[UNWIND_HISTORY_TABLE_SIZE];
} UNWIND_HISTORY_TABLE, *PUNWIND_HISTORY_TABLE;

typedef
EXCEPTION_DISPOSITION
(*PEXCEPTION_ROUTINE) (
    PEXCEPTION_RECORD ExceptionRecord,
    ULONG64 EstablisherFrame,
    PCONTEXT ContextRecord,
    void * DispatcherContext
    );

#if defined(_ARM_)

typedef struct _DISPATCHER_CONTEXT {
    uint32_t ControlPc;
    uint32_t ImageBase;
    PRUNTIME_FUNCTION FunctionEntry;
    uint32_t EstablisherFrame;
    uint32_t TargetPc;
    PCONTEXT ContextRecord;
    PEXCEPTION_ROUTINE LanguageHandler;
    void * HandlerData;
    PUNWIND_HISTORY_TABLE HistoryTable;
    uint32_t ScopeIndex;
    BOOLEAN ControlPcIsUnwound;
    uint8_t *  NonVolatileRegisters;
    uint32_t Reserved;
} DISPATCHER_CONTEXT, *PDISPATCHER_CONTEXT;

#elif defined(_ARM64_)

typedef struct _DISPATCHER_CONTEXT {
    ULONG64 ControlPc;
    ULONG64 ImageBase;
    PRUNTIME_FUNCTION FunctionEntry;
    ULONG64 EstablisherFrame;
    ULONG64 TargetPc;
    PCONTEXT ContextRecord;
    PEXCEPTION_ROUTINE LanguageHandler;
    void * HandlerData;
    PUNWIND_HISTORY_TABLE HistoryTable;
    ULONG64 ScopeIndex;
    BOOLEAN ControlPcIsUnwound;
    uint8_t *  NonVolatileRegisters;
    ULONG64 Reserved;
} DISPATCHER_CONTEXT, *PDISPATCHER_CONTEXT;

#else

typedef struct _DISPATCHER_CONTEXT {
    ULONG64 ControlPc;
    ULONG64 ImageBase;
    PRUNTIME_FUNCTION FunctionEntry;
    ULONG64 EstablisherFrame;
    ULONG64 TargetIp;
    PCONTEXT ContextRecord;
    PEXCEPTION_ROUTINE LanguageHandler;
    void * HandlerData;
    PUNWIND_HISTORY_TABLE HistoryTable;
} DISPATCHER_CONTEXT, *PDISPATCHER_CONTEXT;

#endif

// #endif // !defined(__APPLE__)

typedef DISPATCHER_CONTEXT *PDISPATCHER_CONTEXT;

#define ExceptionContinueSearch     EXCEPTION_CONTINUE_SEARCH
#define ExceptionStackUnwind        EXCEPTION_EXECUTE_HANDLER
#define ExceptionContinueExecution  EXCEPTION_CONTINUE_EXECUTION

#endif // FEATURE_PAL_SXS

typedef struct _EXCEPTION_REGISTRATION_RECORD EXCEPTION_REGISTRATION_RECORD;
typedef EXCEPTION_REGISTRATION_RECORD *PEXCEPTION_REGISTRATION_RECORD;

typedef void * HKEY;
typedef void * PACL;
typedef void * LPBC;
typedef void * PSECURITY_DESCRIPTOR;

typedef struct _EXCEPTION_RECORD64 {
    uint32_t ExceptionCode;
    uint32_t ExceptionFlags;
    ULONG64 ExceptionRecord;
    ULONG64 ExceptionAddress;
    uint32_t NumberParameters;
    uint32_t __unusedAlignment;
    ULONG64 ExceptionInformation[EXCEPTION_MAXIMUM_PARAMETERS];
} EXCEPTION_RECORD64, *PEXCEPTION_RECORD64;

typedef int32_t (WINAPI *PTOP_LEVEL_EXCEPTION_FILTER)(
     struct _EXCEPTION_POINTERS *ExceptionInfo
    );
typedef PTOP_LEVEL_EXCEPTION_FILTER LPTOP_LEVEL_EXCEPTION_FILTER;

/******************* ntdef ************************************/

#ifndef ANYSIZE_ARRAY
#define ANYSIZE_ARRAY 1       // winnt
#endif

/******************* winnt ************************************/

typedef struct LIST_ENTRY32 {
    uint32_t Flink;
    uint32_t Blink;
} LIST_ENTRY32;
typedef LIST_ENTRY32 *PLIST_ENTRY32;

typedef struct LIST_ENTRY64 {
    ULONGLONG Flink;
    ULONGLONG Blink;
} LIST_ENTRY64;
typedef LIST_ENTRY64 *PLIST_ENTRY64;

/******************** PAL RT APIs *******************************/

typedef struct _HSATELLITE *HSATELLITE;

EXTERN_C HSATELLITE PAL_LoadSatelliteResourceW(LPCWSTR SatelliteResourceFileName);
EXTERN_C HSATELLITE PAL_LoadSatelliteResourceA(LPCSTR SatelliteResourceFileName);
EXTERN_C BOOL PAL_FreeSatelliteResource(HSATELLITE SatelliteResource);
EXTERN_C uint32_t PAL_LoadSatelliteStringW(HSATELLITE SatelliteResource,
             uint32_t uID,
             LPWSTR lpBuffer,
             uint32_t nBufferMax);
EXTERN_C uint32_t PAL_LoadSatelliteStringA(HSATELLITE SatelliteResource,
             uint32_t uID,
             LPSTR lpBuffer,
             uint32_t nBufferMax);

EXTERN_C HRESULT PAL_CoCreateInstance(REFCLSID   rclsid,
                             REFIID     riid,
                             void     **ppv);

// So we can have CoCreateInstance in most of the code base,
// instead of spreading around of if'def FEATURE_PALs for PAL_CoCreateInstance.
#define CoCreateInstance(rclsid, pUnkOuter, dwClsContext, riid, ppv) PAL_CoCreateInstance(rclsid, riid, ppv)

/************** verrsrc.h ************************************/

/* ----- VS_VERSION.dwFileFlags ----- */
#define VS_FF_DEBUG             0x00000001L
#define VS_FF_PRERELEASE        0x00000002L
#define VS_FF_PATCHED           0x00000004L
#define VS_FF_PRIVATEBUILD      0x00000008L
#define VS_FF_INFOINFERRED      0x00000010L
#define VS_FF_SPECIALBUILD      0x00000020L

/* ----- Types and structures ----- */
typedef struct tagVS_FIXEDFILEINFO
{
    uint32_t   dwSignature;            /* e.g. 0xfeef04bd */
    uint32_t   dwStrucVersion;         /* e.g. 0x00000042 = "0.42" */
    uint32_t   dwFileVersionMS;        /* e.g. 0x00030075 = "3.75" */
    uint32_t   dwFileVersionLS;        /* e.g. 0x00000031 = "0.31" */
    uint32_t   dwProductVersionMS;     /* e.g. 0x00030010 = "3.10" */
    uint32_t   dwProductVersionLS;     /* e.g. 0x00000031 = "0.31" */
    uint32_t   dwFileFlagsMask;        /* = 0x3F for version "0.42" */
    uint32_t   dwFileFlags;            /* e.g. VFF_DEBUG | VFF_PRERELEASE */
    uint32_t   dwFileOS;               /* e.g. VOS_DOS_WINDOWS16 */
    uint32_t   dwFileType;             /* e.g. VFT_DRIVER */
    uint32_t   dwFileSubtype;          /* e.g. VFT2_DRV_KEYBOARD */
    uint32_t   dwFileDateMS;           /* e.g. 0 */
    uint32_t   dwFileDateLS;           /* e.g. 0 */
} VS_FIXEDFILEINFO;

/************** Byte swapping & unaligned access ******************/

#include <pal_endian.h>

/******************** external includes *************************/

#include "ntimage.h"
#ifndef PAL_STDCPP_COMPAT
// #include "cpp/ccombstr.h"
// #include "cpp/cstring.h"
#endif // !PAL_STDCPP_COMPAT
  // #include "sscli_version.h"

#endif // RC_INVOKED

#endif // __PALRT_H__

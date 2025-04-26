//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Copyright (c) ChakraCore Project Contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

#ifndef __has_builtin
#define __has_builtin(x) 0
#endif

#define USING_PAL_STDLIB 1
#define STRSAFE_INLINE   1

#ifdef PAL_STDCPP_COMPAT
#include <wchar.h>
#include <math.h>
#include <time.h>
#if defined(_AMD64_)
#include <smmintrin.h>
#include <xmmintrin.h>
#endif // defined(_AMD64_)
#endif

#include "inc/pal.h"
#include "inc/rt/palrt.h"
#include "inc/rt/no_sal2.h"
#include "inc/rt/oaidl.h"

#define _u(s) u##s

typedef GUID UUID;
#define INIT_PRIORITY(x) __attribute__((init_priority(x)))

#ifdef PAL_STDCPP_COMPAT
#define __in
#define __out
#define FILE PAL_FILE
#endif

#if defined(_AMD64_)
// xplat-todo: verify below is correct
#include <cpuid.h>
inline int get_cpuid(int cpuInfo[4], int function_id)
{
    return __get_cpuid(
            static_cast<unsigned int>(function_id),
            reinterpret_cast<unsigned int*>(&cpuInfo[0]),
            reinterpret_cast<unsigned int*>(&cpuInfo[1]),
            reinterpret_cast<unsigned int*>(&cpuInfo[2]),
            reinterpret_cast<unsigned int*>(&cpuInfo[3]));
}
#elif defined(_ARM_)
inline int get_cpuid(int cpuInfo[4], int function_id)
{
    int empty[4] = {0};
    memcpy(cpuInfo, empty, sizeof(int) * 4);
    // xplat-todo: implement me!!
}
#endif

inline void DebugBreak()
{
    __builtin_trap();
}

#define EXIT_FAILURE 1
#define _BitScanForward BitScanForward
#define _BitScanForward64 BitScanForward64
#define _BitScanReverse BitScanReverse
#define _BitScanReverse64 BitScanReverse64
#define _bittest BitTest
#define _bittestandset BitTestAndSet
#define _interlockedbittestandset InterlockedBitTestAndSet

#define DbgRaiseAssertionFailure() __builtin_trap()

// These are not available in pal
#define fwprintf_s      fwprintf

// PAL LoadLibraryExW not supported
#define LOAD_LIBRARY_SEARCH_SYSTEM32     0
// winnt.h
#define FAST_FAIL_INVALID_ARG            5
// winerror.h
#define FACILITY_JSCRIPT                 2306
#define JSCRIPT_E_CANTEXECUTE            _HRESULT_TYPEDEF_(0x89020001L)
#define DISP_E_UNKNOWNINTERFACE          _HRESULT_TYPEDEF_(0x80020001L)
#define DISP_E_MEMBERNOTFOUND            _HRESULT_TYPEDEF_(0x80020003L)
#define DISP_E_UNKNOWNNAME               _HRESULT_TYPEDEF_(0x80020006L)
#define DISP_E_NONAMEDARGS               _HRESULT_TYPEDEF_(0x80020007L)
#define DISP_E_EXCEPTION                 _HRESULT_TYPEDEF_(0x80020009L)
#define DISP_E_BADINDEX                  _HRESULT_TYPEDEF_(0x8002000BL)
#define DISP_E_UNKNOWNLCID               _HRESULT_TYPEDEF_(0x8002000CL)
#define DISP_E_ARRAYISLOCKED             _HRESULT_TYPEDEF_(0x8002000DL)
#define DISP_E_BADPARAMCOUNT             _HRESULT_TYPEDEF_(0x8002000EL)
#define DISP_E_PARAMNOTOPTIONAL          _HRESULT_TYPEDEF_(0x8002000FL)
#define DISP_E_NOTACOLLECTION            _HRESULT_TYPEDEF_(0x80020011L)
#define TYPE_E_DLLFUNCTIONNOTFOUND       _HRESULT_TYPEDEF_(0x8002802FL)
#define TYPE_E_TYPEMISMATCH              _HRESULT_TYPEDEF_(0x80028CA0L)
#define TYPE_E_OUTOFBOUNDS               _HRESULT_TYPEDEF_(0x80028CA1L)
#define TYPE_E_IOERROR                   _HRESULT_TYPEDEF_(0x80028CA2L)
#define TYPE_E_CANTCREATETMPFILE         _HRESULT_TYPEDEF_(0x80028CA3L)
#define TYPE_E_CANTLOADLIBRARY           _HRESULT_TYPEDEF_(0x80029C4AL)
#define STG_E_TOOMANYOPENFILES           _HRESULT_TYPEDEF_(0x80030004L)
#define STG_E_ACCESSDENIED               _HRESULT_TYPEDEF_(0x80030005L)
#define STG_E_INSUFFICIENTMEMORY         _HRESULT_TYPEDEF_(0x80030008L)
#define STG_E_NOMOREFILES                _HRESULT_TYPEDEF_(0x80030012L)
#define STG_E_DISKISWRITEPROTECTED       _HRESULT_TYPEDEF_(0x80030013L)
#define STG_E_READFAULT                  _HRESULT_TYPEDEF_(0x8003001EL)
#define STG_E_SHAREVIOLATION             _HRESULT_TYPEDEF_(0x80030020L)
#define STG_E_LOCKVIOLATION              _HRESULT_TYPEDEF_(0x80030021L)
#define STG_E_MEDIUMFULL                 _HRESULT_TYPEDEF_(0x80030070L)
#define STG_E_INVALIDNAME                _HRESULT_TYPEDEF_(0x800300FCL)
#define STG_E_INUSE                      _HRESULT_TYPEDEF_(0x80030100L)
#define STG_E_NOTCURRENT                 _HRESULT_TYPEDEF_(0x80030101L)
#define STG_E_CANTSAVE                   _HRESULT_TYPEDEF_(0x80030103L)
#define REGDB_E_CLASSNOTREG              _HRESULT_TYPEDEF_(0x80040154L)
#define MK_E_UNAVAILABLE                 _HRESULT_TYPEDEF_(0x800401E3L)
#define MK_E_INVALIDEXTENSION            _HRESULT_TYPEDEF_(0x800401E6L)
#define MK_E_CANTOPENFILE                _HRESULT_TYPEDEF_(0x800401EAL)
#define CO_E_APPNOTFOUND                 _HRESULT_TYPEDEF_(0x800401F5L)
#define CO_E_APPDIDNTREG                 _HRESULT_TYPEDEF_(0x800401FEL)
#define GetScode(hr) ((SCODE) (hr))
// activscp.h
#define SCRIPT_E_RECORDED                _HRESULT_TYPEDEF_(0x86664004L)

#define GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS        (0x00000004)

typedef
enum tagBREAKPOINT_STATE
    {
        BREAKPOINT_DELETED  = 0,
        BREAKPOINT_DISABLED = 1,
        BREAKPOINT_ENABLED  = 2
    }   BREAKPOINT_STATE;

typedef
enum tagBREAKRESUME_ACTION
    {
        BREAKRESUMEACTION_ABORT     = 0,
        BREAKRESUMEACTION_CONTINUE  = ( BREAKRESUMEACTION_ABORT + 1 ) ,
        BREAKRESUMEACTION_STEP_INTO = ( BREAKRESUMEACTION_CONTINUE + 1 ) ,
        BREAKRESUMEACTION_STEP_OVER = ( BREAKRESUMEACTION_STEP_INTO + 1 ) ,
        BREAKRESUMEACTION_STEP_OUT  = ( BREAKRESUMEACTION_STEP_OVER + 1 ) ,
        BREAKRESUMEACTION_IGNORE    = ( BREAKRESUMEACTION_STEP_OUT + 1 ) ,
        BREAKRESUMEACTION_STEP_DOCUMENT = ( BREAKRESUMEACTION_IGNORE + 1 )
    }   BREAKRESUMEACTION;

enum tagDBGPROP_ATTRIB_FLAGS
{
    DBGPROP_ATTRIB_NO_ATTRIB= 0,
    DBGPROP_ATTRIB_VALUE_IS_INVALID= 0x8,
    DBGPROP_ATTRIB_VALUE_IS_EXPANDABLE= 0x10,
    DBGPROP_ATTRIB_VALUE_IS_FAKE= 0x20,
    DBGPROP_ATTRIB_VALUE_IS_METHOD= 0x100,
    DBGPROP_ATTRIB_VALUE_IS_EVENT= 0x200,
    DBGPROP_ATTRIB_VALUE_IS_RAW_STRING= 0x400,
    DBGPROP_ATTRIB_VALUE_READONLY= 0x800,
    DBGPROP_ATTRIB_ACCESS_PUBLIC= 0x1000,
    DBGPROP_ATTRIB_ACCESS_PRIVATE= 0x2000,
    DBGPROP_ATTRIB_ACCESS_PROTECTED= 0x4000,
    DBGPROP_ATTRIB_ACCESS_FINAL= 0x8000,
    DBGPROP_ATTRIB_STORAGE_GLOBAL= 0x10000,
    DBGPROP_ATTRIB_STORAGE_STATIC= 0x20000,
    DBGPROP_ATTRIB_STORAGE_FIELD= 0x40000,
    DBGPROP_ATTRIB_STORAGE_VIRTUAL= 0x80000,
    DBGPROP_ATTRIB_TYPE_IS_CONSTANT= 0x100000,
    DBGPROP_ATTRIB_TYPE_IS_SYNCHRONIZED= 0x200000,
    DBGPROP_ATTRIB_TYPE_IS_VOLATILE= 0x400000,
    DBGPROP_ATTRIB_HAS_EXTENDED_ATTRIBS= 0x800000,
    DBGPROP_ATTRIB_FRAME_INTRYBLOCK= 0x1000000,
    DBGPROP_ATTRIB_FRAME_INCATCHBLOCK= 0x2000000,
    DBGPROP_ATTRIB_FRAME_INFINALLYBLOCK= 0x4000000,
    DBGPROP_ATTRIB_VALUE_IS_RETURN_VALUE= 0x8000000
};
typedef uint32_t DBGPROP_ATTRIB_FLAGS;

enum tagDBGPROP_INFO_FLAgS
{
    DBGPROP_INFO_NAME= 0x1,
    DBGPROP_INFO_TYPE= 0x2,
    DBGPROP_INFO_VALUE= 0x4,
    DBGPROP_INFO_FULLNAME= 0x20,
    DBGPROP_INFO_ATTRIBUTES= 0x8,
    DBGPROP_INFO_DEBUGPROP= 0x10,
    DBGPROP_INFO_BEAUTIFY= 0x2000000,
    DBGPROP_INFO_CALLTOSTRING= 0x4000000,
    DBGPROP_INFO_AUTOEXPAND= 0x800000
};

typedef uint32_t DBGPROP_INFO_FLAGS;

#define DBGPROP_INFO_STANDARD ((((DBGPROP_INFO_NAME | DBGPROP_INFO_TYPE) | DBGPROP_INFO_VALUE) | DBGPROP_INFO_ATTRIBUTES))

#define DBGPROP_INFO_ALL ((((((DBGPROP_INFO_NAME | DBGPROP_INFO_TYPE ) | DBGPROP_INFO_VALUE) | DBGPROP_INFO_FULLNAME) | DBGPROP_INFO_ATTRIBUTES) | DBGPROP_INFO_DEBUGPROP))

// _countof
#if defined _M_X64 || defined _M_ARM || defined _M_ARM64
#define _UNALIGNED __unaligned
#else
#define _UNALIGNED
#endif

#ifdef __cplusplus
extern "C++"
{
  template <typename _CountofType, size_t _SizeOfArray>
  char(*__countof_helper(_UNALIGNED _CountofType(&_Array)[_SizeOfArray]))[_SizeOfArray];

#define __crt_countof(_Array) (sizeof(*__countof_helper(_Array)) + 0)
}
#else
#define __crt_countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))
#endif

#ifndef _countof
#define _countof __crt_countof
#endif
// _countof

#define ARRAYSIZE(A) _countof(A)

//
//  Singly linked list structure. Can be used as either a list head, or
//  as link words.
//
typedef struct _SINGLE_LIST_ENTRY {
  struct _SINGLE_LIST_ENTRY *Next;
} SINGLE_LIST_ENTRY, *PSINGLE_LIST_ENTRY;

#if defined(TARGET_64)

//
// The type SINGLE_LIST_ENTRY is not suitable for use with SLISTs.  For
// TARGET_64, an entry on an SLIST is required to be 16-byte aligned, while a
// SINGLE_LIST_ENTRY structure has only 8 byte alignment.
//
// Therefore, all SLIST code should use the SLIST_ENTRY type instead of the
// SINGLE_LIST_ENTRY type.
//

#pragma warning(push)
#pragma warning(disable:4324)   // structure padded due to align()

typedef struct DECLSPEC_ALIGN(16) _SLIST_ENTRY {
  struct _SLIST_ENTRY *Next;
} SLIST_ENTRY, *PSLIST_ENTRY;

#pragma warning(pop)

#else // defined(TARGET_64)

typedef struct _SINGLE_LIST_ENTRY SLIST_ENTRY, *PSLIST_ENTRY;

#endif // defined(TARGET_64)

#if defined(_AMD64_)

typedef union DECLSPEC_ALIGN(16) _SLIST_HEADER {
  struct {  // original struct
    ULONGLONG Alignment;
    ULONGLONG Region;
  } DUMMYSTRUCTNAME;
  struct {  // x64 16-byte header
    ULONGLONG Depth : 16;
    ULONGLONG Sequence : 48;
    ULONGLONG Reserved : 4;
    ULONGLONG NextEntry : 60; // last 4 bits are always 0's
  } HeaderX64;
} SLIST_HEADER, *PSLIST_HEADER;

#elif defined(_X86_)

typedef union _SLIST_HEADER {
  ULONGLONG Alignment;
  struct {
    SLIST_ENTRY Next;
    uint16_t   Depth;
    uint16_t   CpuId;
  } DUMMYSTRUCTNAME;
} SLIST_HEADER, *PSLIST_HEADER;

#elif defined(_ARM_)

typedef union _SLIST_HEADER {
  ULONGLONG Alignment;
  struct {
    SLIST_ENTRY Next;
    uint16_t   Depth;
    uint16_t   Reserved;
  } DUMMYSTRUCTNAME;
} SLIST_HEADER, *PSLIST_HEADER;

#elif defined(_ARM64_)

typedef union _SLIST_HEADER {
  ULONGLONG Alignment;
  struct {
    SLIST_ENTRY Next;
    uint16_t   Depth;
    uint16_t   Reserved;
  } DUMMYSTRUCTNAME;
} SLIST_HEADER, *PSLIST_HEADER;


#endif

void InitializeSListHead(  PSLIST_HEADER ListHead);
PSLIST_ENTRY InterlockedPushEntrySList(  PSLIST_HEADER ListHead,   PSLIST_ENTRY  ListEntry);
PSLIST_ENTRY InterlockedPopEntrySList(  PSLIST_HEADER ListHead);


template <class T>
inline T InterlockedExchangeAdd(
      T volatile *Addend,
     T Value)
{
    return __sync_fetch_and_add(Addend, Value);
}

template <class T>
inline T InterlockedExchangeSubtract(
      T volatile *Addend,
     T Value)
{
    return __sync_fetch_and_sub(Addend, Value);
}

template <class T>
inline T InterlockedIncrement(
      T volatile *Addend)
{
    return __sync_add_and_fetch(Addend, T(1));
}

template <class T>
inline T InterlockedDecrement(
      T volatile *Addend)
{
    return __sync_sub_and_fetch(Addend, T(1));
}

inline __int64 _abs64(__int64 n)
{
    return n < 0 ? -n : n;
}

int GetCurrentThreadStackLimits(size_t* lowLimit, size_t* highLimit);
bool IsAddressOnStack(size_t address);

errno_t rand_s(unsigned int* randomValue);

inline char16* wmemset(char16* wcs, char16 wc, size_t n)
{
    while (n)
    {
        wcs[--n] = wc;
    }
    return wcs;
}

inline errno_t wmemcpy_s(char16* dest, size_t destSize, const char16* src, size_t count)
{
    return memcpy_s(dest, sizeof(char16) * destSize, src, sizeof(char16) * count);
}

inline int _wunlink(const char16* filename)
{
    // WARN: does not set errno when fail
    return DeleteFile(filename) ? 0 : -1;
}

template <size_t size>
inline errno_t _wcserror_s(char16 (&buffer)[size], int errnum)
{
    const char* str = strerror(errnum);
    // WARN: does not return detail errno when fail
    return MultiByteToWideChar(CP_ACP, 0, str, -1, buffer, size) ? 0 : -1;
}

#define midl_user_allocate(size) \
    HeapAlloc(GetProcessHeap(), 0, (size))
#define midl_user_free(ptr) \
    if (ptr != NULL) { HeapFree(GetProcessHeap(), NULL, ptr); }

uint32_t CharLowerBuffW(const char16* lpsz, uint32_t  cchLength);
uint32_t CharUpperBuffW(const char16* lpsz, uint32_t  cchLength);

// TODO (hanhossain): replace with std::numeric_limits<uint32_t>::max()
#define MAXUINT32   ((uint32_t)~((uint32_t)0))
#define MAXINT32    ((int32_t)(MAXUINT32 >> 1))
#define BYTE_MAX    0xff
#define USHORT_MAX  0xffff

#ifdef UNICODE
#define StringCchPrintf  StringCchPrintfW
#endif



// Use intsafe.h for internal builds (currently missing some files with stdint.h)
#include <stdint.h>

// `typename QualifiedName` declarations outside of template code not supported before MSVC 2015 update 1
#define _TYPENAME typename

#define _NOEXCEPT_ noexcept

#if defined(__GNUC__) || defined(__clang__)
#define _ReturnAddress() __builtin_return_address(0)
#if !__has_builtin(_AddressOfReturnAddress)
inline void * _AddressOfReturnAddress()
{
    return (void*)((char*) __builtin_frame_address(0) + sizeof(void*));
}
#else
extern "C" void * _AddressOfReturnAddress(void);
#endif
#else
#error _AddressOfReturnAddress and _ReturnAddress not defined for this platform
#endif

// ----- START: Define strsafe related types and defines for non-VC++ compilers -----
// xplat-todo: figure out why strsafe.h includes stdio etc
// which prevents me from directly including PAL's strsafe.h
#ifdef __cplusplus
#define _STRSAFE_EXTERN_C    extern "C"
#else
#define _STRSAFE_EXTERN_C    extern
#endif

// If you do not want to use these functions inline (and instead want to link w/ strsafe.lib), then
// #define STRSAFE_LIB before including this header file.
#if defined(STRSAFE_LIB)
#define STRSAFEAPI  _STRSAFE_EXTERN_C HRESULT
#pragma comment(lib, "strsafe.lib")
#elif defined(STRSAFE_LIB_IMPL)
#define STRSAFEAPI  _STRSAFE_EXTERN_C HRESULT
#else
#define STRSAFEAPI  inline HRESULT
#define STRSAFE_INLINE
#endif

STRSAFEAPI StringCchPrintfW(WCHAR* pszDest, size_t cchDest, const WCHAR* pszFormat, ...);
STRSAFEAPI StringVPrintfWorkerW(WCHAR* pszDest, size_t cchDest, const WCHAR* pszFormat, va_list argList);

#define STRSAFE_MAX_CCH  2147483647 // max # of characters we support (same as INT_MAX)

// STRSAFE error return codes
//
#define STRSAFE_E_INSUFFICIENT_BUFFER       ((HRESULT)0x8007007AL)  // 0x7A = 122L = ERROR_INSUFFICIENT_BUFFER
#define STRSAFE_E_INVALID_PARAMETER         ((HRESULT)0x80070057L)  // 0x57 =  87L = ERROR_INVALID_PARAMETER
#define STRSAFE_E_END_OF_FILE               ((HRESULT)0x80070026L)  // 0x26 =  38L = ERROR_HANDLE_EOF
// ----- END: Define strsafe related types and defines for non-VC++ compilers -----

// Provide the definitions for non-windows platforms
STRSAFEAPI StringVPrintfWorkerW(WCHAR* pszDest, size_t cchDest, const WCHAR* pszFormat, va_list argList)
{
    HRESULT hr = S_OK;

    if (cchDest == 0)
    {
        // can not null terminate a zero-byte dest buffer
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        int iRet;
        size_t cchMax;

        // leave the last space for the null terminator
        cchMax = cchDest - 1;

        iRet = _vsnwprintf(pszDest, cchMax, pszFormat, argList);
        // ASSERT((iRet < 0) || (((size_t)iRet) <= cchMax));

        if ((iRet < 0) || (((size_t)iRet) > cchMax))
        {
            // need to null terminate the string
            pszDest += cchMax;
            *pszDest = _u('\0');

            // we have truncated pszDest
            hr = STRSAFE_E_INSUFFICIENT_BUFFER;
        }
        else if (((size_t)iRet) == cchMax)
        {
            // need to null terminate the string
            pszDest += cchMax;
            *pszDest = _u('\0');
        }
    }

    return hr;
}

STRSAFEAPI StringCchPrintfW(WCHAR* pszDest, size_t cchDest, const WCHAR* pszFormat, ...)
{
    HRESULT hr;

    if (cchDest > STRSAFE_MAX_CCH)
    {
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        va_list argList;

        va_start(argList, pszFormat);

        hr = StringVPrintfWorkerW(pszDest, cchDest, pszFormat, argList);

        va_end(argList);
    }

    return hr;
}

__inline
HRESULT ULongMult(uint32_t ulMultiplicand, uint32_t ulMultiplier, uint32_t* pulResult);

/* **** WARNING : finallyFunc is not allowed to raise exception *****
 * **** DO NOT ADD stack probe or memory allocations within the finallyFunc ****
 */
template <class TryFunc, class FinallyFunc>
void TryFinally(const TryFunc& tryFunc, const FinallyFunc& finallyFunc)
{
    class FinallyObject
    {
    public:
        FinallyObject(const FinallyFunc& finallyFunc) : finallyFunc(finallyFunc), abnormalTermination(true) {}
        ~FinallyObject() { finallyFunc(abnormalTermination); }

        void SetHasNoAbnormalTermination() { abnormalTermination = false; }
    private:
        const FinallyFunc& finallyFunc;
        bool abnormalTermination;
    } finallyObject(finallyFunc);

    tryFunc();
    finallyObject.SetHasNoAbnormalTermination();
}

#ifdef DISABLE_SEH
#define __TRY_FINALLY_BEGIN TryFinally([&]()
#define __FINALLY           , [&](bool /* hasException */)
#define __TRY_FINALLY_END   );
#else
#define __TRY_FINALLY_BEGIN __try
#define __FINALLY   __finally
#define __TRY_FINALLY_END
#endif

namespace PlatformAgnostic
{
    inline unsigned char _BitTestAndSet(int32_t *_BitBase, int _BitPos)
    {
#if defined(__clang__) && !defined(_ARM_) && !defined(_ARM64_)
        // Clang doesn't expand _bittestandset intrinic to bts, and it's implemention also doesn't work for _BitPos >= 32
        unsigned char retval = 0;
        asm(
            "bts %[_BitPos], %[_BitBase]\n\t"
            "setc %b[retval]\n\t"
            : [_BitBase] "+m" (*_BitBase), [retval] "+rm" (retval)
            : [_BitPos] "ri" (_BitPos)
            : "cc" // clobber condition code
        );
        return retval;
#else
        return _bittestandset(_BitBase, _BitPos);
#endif
    }

    inline unsigned char _BitTest(int32_t *_BitBase, int _BitPos)
    {
#if defined(__clang__) && !defined(_ARM_) && !defined(_ARM64_)
        // Clang doesn't expand _bittest intrinic to bt, and it's implemention also doesn't work for _BitPos >= 32
        unsigned char retval;
        asm(
            "bt %[_BitPos], %[_BitBase]\n\t"
            "setc %b[retval]\n\t"
            : [retval] "+rm" (retval)
            : [_BitPos] "ri" (_BitPos), [_BitBase] "m" (*_BitBase)
            : "cc" // clobber condition code
        );
        return retval;
#else
        return _bittest(_BitBase, _BitPos);
#endif
    }

    inline unsigned char _InterlockedBitTestAndSet(volatile int32_t *_BitBase, int _BitPos)
    {
#if defined(__clang__) && !defined(_ARM_) && !defined(_ARM64_)
        // Clang doesn't expand _interlockedbittestandset intrinic to lock bts, and it's implemention also doesn't work for _BitPos >= 32
        unsigned char retval;
        asm(
            "lock bts %[_BitPos], %[_BitBase]\n\t"
            "setc %b[retval]\n\t"
            : [_BitBase] "+m" (*_BitBase), [retval] "+rm" (retval)
            : [_BitPos] "ri" (_BitPos)
            : "cc" // clobber condition code
        );
        return retval;
#else
        return _interlockedbittestandset(_BitBase, _BitPos);
#endif
    }

    inline unsigned char _InterlockedBitTestAndReset(volatile int32_t *_BitBase, int _BitPos)
    {
#if defined(__clang__) && !defined(_ARM_) && !defined(_ARM64_)
        // Clang doesn't expand _interlockedbittestandset intrinic to lock btr, and it's implemention also doesn't work for _BitPos >= 32
        unsigned char retval;
        asm(
            "lock btr %[_BitPos], %[_BitBase]\n\t"
            "setc %b[retval]\n\t"
            : [_BitBase] "+m" (*_BitBase), [retval] "+rm" (retval)
            : [_BitPos] "ri" (_BitPos)
            : "cc" // clobber condition code
        );
        return retval;
#elif !defined(__ANDROID__)
        return _interlockedbittestandreset(_BitBase, (long)_BitPos);
#else
        // xplat-todo: Implement _interlockedbittestandreset for Android
        abort();
#endif
    }
};

#include "PlatformAgnostic/DateTime.h"
#include "PlatformAgnostic/Numbers.h"
#include "PlatformAgnostic/Arrays.h"
#include "PlatformAgnostic/SystemInfo.h"
#include "PlatformAgnostic/Thread.h"
#include "PlatformAgnostic/AssemblyCommon.h"
#include "PlatformAgnostic/Debugger.h"

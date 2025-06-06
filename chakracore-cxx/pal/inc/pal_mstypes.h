//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
//

/*++

// TODO (hanhossain): reduce usage of mstypes



--*/

////////////////////////////////////////////////////////////////////////
// Extensions to the usual posix header files
////////////////////////////////////////////////////////////////////////

#ifndef __PAL_MSTYPES_H__
#define __PAL_MSTYPES_H__

#define __MSTYPES_DEFINED

#include <stdint.h>
#include <stddef.h>

#ifdef  __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////
// calling convention stuff
////////////////////////////////////////////////////////////////////////


#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif // __cplusplus

#define CALLBACK

#if !defined(_declspec)
#define _declspec(e)  __declspec(e)
#endif

#if defined(_VAC_) && defined(__cplusplus)
#define __inline        inline
#endif

////////////////////////////////////////////////////////////////////////
// Type attribute stuff
////////////////////////////////////////////////////////////////////////

#ifdef UNICODE
#define __TEXT(x) L##x
#else
#define __TEXT(x) x
#endif
#define TEXT(x) __TEXT(x)

////////////////////////////////////////////////////////////////////////
// Some special values
////////////////////////////////////////////////////////////////////////

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

////////////////////////////////////////////////////////////////////////
// Misc. type helpers
////////////////////////////////////////////////////////////////////////

// GCC's way of declaring large integer constants
// If you define these in one step, without the _HELPER macros, you
// get extra whitespace when composing these with other concatenating macros.
#define I64_HELPER(x) x ## LL
#define I64(x)        I64_HELPER(x)

#define UI64_HELPER(x) x ## ULL
#define UI64(x)        UI64_HELPER(x)

////////////////////////////////////////////////////////////////////////
// Misc. types
////////////////////////////////////////////////////////////////////////

// A bunch of source files (e.g. most of the ndp tree) include pal.h
// but are written to be LLP64, not LP64.  (LP64 => long = 64 bits
// LLP64 => longs = 32 bits, long long = 64 bits)
//
// To handle this difference, we #define long to be int (and thus 32 bits) when
// compiling those files.  (See the bottom of this file or search for
// #define long to see where we do this.)
//
// But this fix is more complicated than it seems, because we also use the
// preprocessor to #define __int64 to long for LP64 architectures (__int64
// isn't a builtin in gcc).   We don't want __int64 to be an int (by cascading
// macro rules).  So we play this little trick below where we add
// __cppmungestrip before "long", which is what we're really #defining __int64
// to.  The preprocessor sees __cppmungestriplong as something different than
// long, so it doesn't replace it with int.  The during the cppmunge phase, we
// remove the __cppmungestrip part, leaving long for the compiler to see.
//
// Note that we can't just use a typedef to define __int64 as long before
// #defining long because typedefed types can't be signedness-agnostic (i.e.
// they must be either signed or unsigned) and we want to be able to use
// __int64 as though it were intrinsic

#define __int64     long

typedef __int64 LONGLONG;
typedef unsigned __int64 ULONGLONG;
typedef ULONGLONG DWORD64;
typedef DWORD64 *PDWORD64;
typedef LONGLONG *PLONG64;
typedef ULONGLONG *PULONG64;
typedef ULONGLONG *PULONGLONG;
typedef ULONGLONG DWORDLONG;

typedef int BOOL, *LPBOOL;
typedef uint8_t BOOLEAN;

typedef unsigned __int64 UINT64;

typedef unsigned __int64 ULONG64;
typedef signed __int64 LONG64;

#define _W64

#define _atoi64 (__int64)atoll

typedef __int64 INT_PTR;
typedef unsigned __int64 UINT_PTR;
typedef unsigned __int64 DWORD_PTR;

/* maximum signed 64 bit value */
#define LONG_PTR_MAX      I64(9223372036854775807)
/* maximum unsigned 64 bit value */

typedef size_t SIZE_T;

typedef ptrdiff_t LPARAM;

#define _PTRDIFF_T_DEFINED
#ifdef _MINGW_
// We need to define _PTRDIFF_T to make sure ptrdiff_t doesn't get defined
// again by system headers - but only for MinGW.
#define _PTRDIFF_T
#endif

#if !defined(__cplusplus)
typedef unsigned short char16_t;
#endif // __cplusplus

typedef char16_t char16;
typedef char16_t WCHAR;

// TODO (hanhossain): can probably remove this
#define _INTPTR_T_DEFINED
// TODO (hanhossain): can probably remove this
#define _UINTPTR_T_DEFINED

typedef uint32_t LCID;
typedef uint16_t LANGID;

typedef uint32_t LCTYPE;

typedef const WCHAR *LPCWCH;
typedef WCHAR *LPWSTR, *PWSTR;

typedef const WCHAR *LPCWSTR, *PCWSTR;

typedef char *PCHAR;
typedef char *LPSTR;
typedef const char *LPCSTR;

#ifdef UNICODE
typedef WCHAR TCHAR;
typedef WCHAR _TCHAR;
#else
typedef char TCHAR;
typedef char _TCHAR;
#endif
typedef TCHAR *LPTSTR;
typedef const TCHAR *LPCTSTR;

#define MAKEWORD(a, b)      ((uint16_t)(((uint8_t)((DWORD_PTR)(a) & 0xff)) | ((uint16_t)((uint8_t)((DWORD_PTR)(b) & 0xff))) << 8))
#define MAKELONG(a, b)      ((int32_t)(((uint16_t)((DWORD_PTR)(a) & 0xffff)) | ((uint32_t)((uint16_t)((DWORD_PTR)(b) & 0xffff))) << 16))
#define LOWORD(l)           ((uint16_t)((DWORD_PTR)(l) & 0xffff))
#define HIWORD(l)           ((uint16_t)((DWORD_PTR)(l) >> 16))
#define LOBYTE(w)           ((uint8_t)((DWORD_PTR)(w) & 0xff))
#define HIBYTE(w)           ((uint8_t)((DWORD_PTR)(w) >> 8))

typedef void *HANDLE;
typedef HANDLE HWND;
typedef struct __PAL_RemoteHandle__ { HANDLE h; } *RHANDLE;
typedef HANDLE *PHANDLE;
typedef HANDLE *LPHANDLE;
#define INVALID_HANDLE_VALUE ((void *)(-1))
#define INVALID_FILE_SIZE ((uint32_t)0xFFFFFFFF)
#define INVALID_FILE_ATTRIBUTES ((uint32_t) -1)
typedef HANDLE HMODULE;
typedef HANDLE HINSTANCE;
typedef HANDLE HGLOBAL;
typedef HANDLE HLOCAL;
typedef HANDLE HRSRC;

typedef int32_t HRESULT;
typedef int32_t NTSTATUS;

typedef union _LARGE_INTEGER {
    struct {
#if BIGENDIAN
        int32_t HighPart;
        uint32_t LowPart;
#else
        uint32_t LowPart;
        int32_t HighPart;
#endif
    };
    struct {
#if BIGENDIAN
        int32_t HighPart;
        uint32_t LowPart;
#else
        uint32_t LowPart;
        int32_t HighPart;
#endif
    } u;
    LONGLONG QuadPart;
} LARGE_INTEGER, *PLARGE_INTEGER;

#ifndef GUID_DEFINED
typedef struct _GUID {
    uint32_t   Data1;    // NOTE: diff from Win32, for LP64
    unsigned short  Data2;
    unsigned short  Data3;
    unsigned char   Data4[ 8 ];
} GUID;
typedef const GUID *LPCGUID;
#define GUID_DEFINED
#endif // !GUID_DEFINED

typedef struct _FILETIME {
    uint32_t dwLowDateTime;
    uint32_t dwHighDateTime;
} FILETIME, *PFILETIME, *LPFILETIME;

/* Code Page Default Values */
#define CP_ACP          0   /* default to ANSI code page */
#define CP_OEMCP        1   /* default to OEM code page */
#define CP_MACCP        2   /* default to MAC code page */
#define CP_THREAD_ACP   3   /* current thread's ANSI code page */
#define CP_WINUNICODE   1200
#define CP_UNICODE      1200 /* Unicode */
#define CP_UNICODESWAP  1201 /* Unicode Big-Endian */
#define CP_UTF7     65000   /* UTF-7 translation */
#define CP_UTF8     65001   /* UTF-8 translation */

typedef void * PSID;

#ifdef  __cplusplus
}
#endif

#endif // __PAL_MSTYPES_H__

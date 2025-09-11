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

#define __TEXT(x) L##x
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
// Misc. types
////////////////////////////////////////////////////////////////////////

typedef int BOOL, *LPBOOL;
typedef uint8_t BOOLEAN;

typedef unsigned long UINT64;

typedef unsigned long ULONG64;
typedef signed long LONG64;

#define _W64

#define _atoi64 (long)atoll

typedef long INT_PTR;
typedef unsigned long UINT_PTR;
typedef unsigned long DWORD_PTR;

#define _PTRDIFF_T_DEFINED
#ifdef _MINGW_
// We need to define _PTRDIFF_T to make sure ptrdiff_t doesn't get defined
// again by system headers - but only for MinGW.
#define _PTRDIFF_T
#endif

#if !defined(__cplusplus)
typedef unsigned short char16_t;
#endif // __cplusplus

// TODO (hanhossain): can probably remove this
#define _INTPTR_T_DEFINED
// TODO (hanhossain): can probably remove this
#define _UINTPTR_T_DEFINED

typedef uint32_t LCID;
typedef uint16_t LANGID;

typedef uint32_t LCTYPE;

typedef const char16_t *LPCWCH;
typedef char16_t *LPWSTR, *PWSTR;

typedef const char16_t *LPCWSTR, *PCWSTR;

typedef char *PCHAR;
typedef char *LPSTR;
typedef const char *LPCSTR;

typedef char16_t TCHAR;
typedef char16_t _TCHAR;
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
    long QuadPart;
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

#ifdef  __cplusplus
}
#endif

#endif // __PAL_MSTYPES_H__

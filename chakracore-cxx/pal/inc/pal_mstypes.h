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

// TODO (hanhossain): remove for int32_t
#define __int32     int
#define __int16     short int
#define __int8      char        // assumes char is signed


#ifndef PAL_STDCPP_COMPAT
// Defined in gnu's types.h. For non PAL_IMPLEMENTATION system
// includes are not included, so we need to define them.
// Types below are already defined by stdint.h on __APPLE__.

#if _WIN64
typedef long double LONG_DOUBLE;
#endif

#endif // !PAL_STDCPP_COMPAT

typedef __int64 LONGLONG;
typedef unsigned __int64 ULONGLONG;
typedef ULONGLONG DWORD64;
typedef DWORD64 *PDWORD64;
typedef LONGLONG *PLONG64;
typedef ULONGLONG *PULONG64;
typedef ULONGLONG *PULONGLONG;
typedef char *PSZ;
typedef ULONGLONG DWORDLONG;

typedef int BOOL, *PBOOL, *LPBOOL;
typedef int *PINT, *LPINT;
typedef uint8_t BOOLEAN, *PBOOLEAN;

typedef signed __int32 INT32, *PINT32;
typedef unsigned __int64 UINT64, *PUINT64;

typedef unsigned __int32 ULONG32, *PULONG32;
typedef signed __int32 LONG32, *PLONG32;
typedef unsigned __int64 ULONG64;
typedef signed __int64 LONG64;

#define _W64

#define _atoi64 (__int64)atoll

typedef __int64 INT_PTR, *PINT_PTR;
typedef unsigned __int64 UINT_PTR, *PUINT_PTR;
typedef __int64 *PLONG_PTR;
typedef unsigned __int64 *PULONG_PTR;
typedef unsigned __int64 DWORD_PTR, *PDWORD_PTR;

/* maximum signed 64 bit value */
#define LONG_PTR_MAX      I64(9223372036854775807)
/* maximum unsigned 64 bit value */
#define ULONG_PTR_MAX     UI64(0xffffffffffffffff)

#ifndef SIZE_MAX
#define SIZE_MAX _UI64_MAX
#endif

#define HandleToULong( h ) ((uint32_t)(size_t)(h) )
#define HandleToLong( h )  ((int32_t)(ptrdiff_t) (h) )
#define ULongToHandle( ul ) ((HANDLE)(size_t) (ul) )
#define LongToHandle( h )   ((HANDLE)(ptrdiff_t) (h) )
#define PtrToUlong( p ) ((uint32_t)(size_t) (p) )
#define PtrToLong( p )  ((int32_t)(ptrdiff_t) (p) )
#define PtrToUint( p ) ((uint32_t)(UINT_PTR) (p) )
#define PtrToInt( p )  ((int32_t)(INT_PTR) (p) )
#define PtrToUshort( p ) ((unsigned short)(size_t)(p) )
#define PtrToShort( p )  ((short)(ptrdiff_t)(p) )
#define IntToPtr( i )    ((void *)(INT_PTR)((int)(i)))
#define UIntToPtr( ui )  ((void *)(UINT_PTR)((unsigned int)(ui)))
#define LongToPtr( l )   ((void *)(ptrdiff_t)((long)(l)))
#define ULongToPtr( ul ) ((void *)(size_t)((unsigned long)(ul)))
#define ShortToPtr( s )  ((void *)(INT_PTR)((short)(s)))
#define UShortToPtr( us )  ((void *)(UINT_PTR)((unsigned short)(s)))

#define HandleToUlong(h)  HandleToULong(h)
#define UlongToHandle(ul) ULongToHandle(ul)
#define UlongToPtr(ul) ULongToPtr(ul)
#define UintToPtr(ui)  UIntToPtr(ui)

typedef size_t SIZE_T, *PSIZE_T;
typedef ptrdiff_t SSIZE_T, *PSSIZE_T;

#ifndef SIZE_T_MAX
#define SIZE_T_MAX ULONG_PTR_MAX
#endif // SIZE_T_MAX

#ifndef SSIZE_T_MAX
#define SSIZE_T_MAX LONG_PTR_MAX
#endif

#ifndef SSIZE_T_MIN
#define SSIZE_T_MIN I64(-9223372036854775808)
#endif

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

#ifdef PAL_STDCPP_COMPAT

#ifdef __APPLE__
static_assert(sizeof(unsigned long) == sizeof(void*), "This platform is not supported");
#else
typedef unsigned long int uintptr_t;
#endif

#else // !PAL_STDCPP_COMPAT

#if defined(__LINUX__)
typedef long int intptr_t;
typedef unsigned long int uintptr_t;
#else
typedef INT_PTR intptr_t;
typedef UINT_PTR uintptr_t;
#endif

#endif // PAL_STDCPP_COMPAT

#define _INTPTR_T_DEFINED
#define _UINTPTR_T_DEFINED

typedef uint32_t LCID;
typedef uint32_t * PLCID;
typedef uint16_t LANGID;

typedef uint32_t LCTYPE;

typedef WCHAR *PWCHAR;
typedef WCHAR *LPWCH, *PWCH;
typedef const WCHAR *LPCWCH, *PCWCH;
typedef WCHAR *NWPSTR;
typedef WCHAR *LPWSTR, *PWSTR;

typedef const WCHAR *LPCWSTR, *PCWSTR;

typedef char *PCHAR;
typedef char *LPCH, *PCH;
typedef const char *LPCCH, *PCCH;
typedef char *NPSTR;
typedef char *LPSTR, *PSTR;
typedef const char *LPCSTR, *PCSTR;

#ifdef UNICODE
typedef WCHAR TCHAR;
typedef WCHAR _TCHAR;
#else
typedef char TCHAR;
typedef char _TCHAR;
#endif
typedef TCHAR *PTCHAR;
typedef TCHAR *LPTSTR, *PTSTR;
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

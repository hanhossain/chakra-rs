//-------------------------------------------------------------------------------------------------------
// ChakraCore/Pal
// Contains portions (c) copyright Microsoft, portions copyright (c) the .NET Foundation and Contributors
// and edits (c) copyright the ChakraCore Contributors.
// See THIRD-PARTY-NOTICES.txt in the project root for .NET Foundation license
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
/*++

Module Name:

    pal.h

Abstract:

    Rotor Platform Adaptation Layer (PAL) header file.  This file
    defines all types and API calls required by the Rotor port of
    the Microsoft Common Language Runtime.

    Defines which control the behavior of this include file:
      UNICODE - define it to set the Ansi/Unicode neutral names to
                be the ...W names.  Otherwise the neutral names default
                to be the ...A names.
      PAL_IMPLEMENTATION - define it when implementing the PAL.  Otherwise
                leave it undefined when consuming the PAL.

    Note:  some fields in structs have been renamed from the original
    SDK documentation names, with _PAL_Undefined appended.  This leaves
    the structure layout identical to its Win32 version, but prevents
    PAL consumers from inadvertently referencing undefined fields.

    If you want to add a PAL_ wrapper function to a native function in
    here, you also need to edit palinternal.h and win32pal.h.



--*/

#ifndef __PAL_H__
#define __PAL_H__

#ifdef PAL_STDCPP_COMPAT
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#endif

#include <stdint.h>

#if !defined(static_assert)
#define static_assert _Static_assert
#endif

#if defined(__APPLE__)
#ifndef __IOS__
#include "TargetConditionals.h"
#if TARGET_IPHONE_SIMULATOR
#define __IOS__
#elif TARGET_OS_IPHONE
#define __IOS__
#elif TARGET_OS_MAC
// macOS
#endif
#endif // __IOS__ ?
#ifndef INCLUDE_PAL_INTERNAL_
namespace std {
    typedef decltype(nullptr) nullptr_t;
}
#endif
#endif // __APPLE__ ?

#ifdef __ANDROID__
#define S_IREAD   0000400
#define S_IWRITE  0000200
#define S_IEXEC   0000100

#ifndef CC_AND_TAG
#define CC_AND_TAG "chakracore-log"
#endif
#include <android/log.h>
#include <stdarg.h>
#define PRINT_LOG(...) \
    __android_log_print(ANDROID_LOG_INFO, CC_AND_TAG, __VA_ARGS__)
#define PRINT_ERROR(...) \
    __android_log_print(ANDROID_LOG_ERROR, CC_AND_TAG, __VA_ARGS__)
#else
typedef __builtin_va_list va_list;
#define PRINT_LOG(...) \
    fprintf(stdout, __VA_ARGS__)
#define PRINT_ERROR(...) \
    fprintf(stderr, __VA_ARGS__)
#endif

#ifdef  __cplusplus
extern "C" {
#endif

// This macro is used to standardize the wide character string literals between UNIX and Windows.
// Unix L"" is UTF32, and on windows it's UTF16.  Because of built-in assumptions on the size
// of string literals, it's important to match behaviour between Unix and Windows.  Unix will be defined
// as u"" (char16_t)
#define W(str)  u##str

// Undefine the QUOTE_MACRO_L helper and redefine it in terms of u.
// The reason that we do this is that quote macro is defined in ndp\common\inc,
// not inside of coreclr sources.

#define QUOTE_MACRO_L(x) QUOTE_MACRO_u(x)
#define QUOTE_MACRO_u_HELPER(x)     u###x
#define QUOTE_MACRO_u(x)            QUOTE_MACRO_u_HELPER(x)

#include <pal_error.h>
#include <pal_mstypes.h>

/******************* Processor-specific glue  *****************************/

#if defined(__i586__) && !defined(_M_IX86)
#define _M_IX86 500
#elif defined(__i486__) && !defined(_M_IX86)
#define _M_IX86 400
#elif defined(__i386__) && !defined(_M_IX86)
#define _M_IX86 300
#elif defined(__ppc__) && !defined(_M_PPC)
#define _M_PPC 100
#elif defined(_AIX) && defined(_POWER) && !defined(_M_PPC)
#define _M_PPC 100
#elif defined(__sparc__) && !defined(_M_SPARC)
#define _M_SPARC 100
#elif defined(__hppa__) && !defined(_M_PARISC)
#define _M_PARISC 100
#elif defined(__ia64__) && !defined(_M_IA64)
#define _M_IA64 64100
#elif defined(__x86_64__) && !defined(_M_AMD64)
#define _M_AMD64 100
#elif defined(__arm__) && !defined(_M_ARM)
#define _M_ARM 7
#elif defined(__aarch64__) && !defined(_M_ARM64)
#define _M_ARM64 1
#endif

#if defined(_M_IX86) && !defined(_X86_)
#define _X86_
#elif defined(_M_ALPHA) && !defined(_ALPHA_)
#define _ALPHA_
#elif defined(_M_PPC) && !defined(_PPC_)
#define _PPC_
#elif defined(_M_SPARC) && !defined(_SPARC_)
#define _SPARC_
#elif defined(_M_PARISC) && !defined(_PARISC_)
#define _PARISC_
#elif defined(_M_MRX000) && !defined(_MIPS_)
#define _MIPS_
#elif defined(_M_M68K) && !defined(_68K_)
#define _68K_
#elif defined(_M_IA64) && !defined(_IA64_)
#define _IA64_
#elif defined(_M_AMD64) && !defined(_AMD64_)
#define _AMD64_
#elif defined(_M_ARM) && !defined(_ARM_)
#define _ARM_
#elif defined(_M_ARM64) && !defined(_ARM64_)
#define _ARM64_
#endif

/******************* ABI-specific glue *******************************/

#if defined(_PPC_) || defined(_PPC64_) || defined(_SPARC_) || defined(_PARISC_) || defined(_IA64_)
#define BIGENDIAN 1
#endif

#ifdef __APPLE__
// Both PowerPC, i386 and x86_64 on Mac OS X use 16-byte alignment.
#define STACK_ALIGN_BITS             4
#define STACK_ALIGN_REQ             (1 << STACK_ALIGN_BITS)
#endif

#define MAX_PATH 260
#define _MAX_PATH 260
#define _MAX_DRIVE  3   /* max. length of drive component */
#define _MAX_DIR    256 /* max. length of path component */
#define _MAX_FNAME  256 /* max. length of file name component */
#define _MAX_EXT    256 /* max. length of extension component */

// In some Win32 APIs MAX_PATH is used for file names (even though 256 is the normal file system limit)
// use _MAX_PATH_FNAME to indicate these cases
#define MAX_PATH_FNAME MAX_PATH
#define MAX_LONGPATH   1024  /* max. length of full pathname */

#define MAXSHORT      0x7fff
#define MAXLONG       0x7fffffff
#define MAXCHAR       0x7f
#define MAXDWORD      0xffffffff

//  Sorting IDs.
//
//  Note that the named locale APIs (eg CompareStringExEx) are recommended.
//

#define LANG_CHINESE                     0x04
#define LANG_ENGLISH                     0x09
#define LANG_JAPANESE                    0x11
#define LANG_KOREAN                      0x12
#define LANG_THAI                        0x1e

/******************* Compiler-specific glue *******************************/

#define FEATURE_PAL_SXS 1
#define DECLSPEC_ALIGN(x)   __declspec(align(x))
#define __assume(x) (void)0
#define __annotation(x)
#define UNALIGNED

#ifndef FORCEINLINE
#define FORCEINLINE inline
#endif

#define PAL_GLOBAL __attribute__((init_priority(200)))
/******************* PAL-Specific Entrypoints *****************************/

#define IsDebuggerPresent PAL_IsDebuggerPresent

BOOL
PAL_IsDebuggerPresent();

// TODO (hanhossain): remove all the defines

#define MAXIMUM_SUSPEND_COUNT  MAXCHAR

#define CHAR_BIT      8

#define SCHAR_MIN   (-128)
#define SCHAR_MAX     127
#define UCHAR_MAX     0xff

#define SHRT_MIN    (-32768)
#define SHRT_MAX      32767
#define USHRT_MAX     0xffff

#define INT_MIN     (-2147483647 - 1)
#define INT_MAX       2147483647
#define UINT_MAX      0xffffffff

#define LONG_MIN    (-2147483647L - 1)
#define LONG_MAX      2147483647L
#define ULONG_MAX     0xffffffffUL

#define LONGLONG_MIN    (-9223372036854775807i64 - 1)
#define LONG64_MIN      (-9223372036854775807i64 - 1)
#define LONGLONG_MAX    9223372036854775807i64
#define LONG64_MAX      9223372036854775807i64
#define ULONGLONG_MAX   0xffffffffffffffffui64
#define DWORDLONG_MAX   0xffffffffffffffffui64
#define ULONG64_MAX     0xffffffffffffffffui64
#define DWORD64_MAX     0xffffffffffffffffui64

#define FLT_MAX 3.402823466e+38F
#define DBL_MAX 1.7976931348623157e+308

/* minimum signed 64 bit value */
#define _I64_MIN    (I64(-9223372036854775807) - 1)
/* maximum signed 64 bit value */
#define _I64_MAX      I64(9223372036854775807)
/* maximum unsigned 64 bit value */
#define _UI64_MAX     UI64(0xffffffffffffffff)

#define _I8_MAX   SCHAR_MAX
#define _I8_MIN   SCHAR_MIN
#define _I16_MAX  SHRT_MAX
#define _I16_MIN  SHRT_MIN
#define _I32_MAX  INT_MAX
#define _I32_MIN  INT_MIN
#define _UI8_MAX  UCHAR_MAX
#define _UI8_MIN  UCHAR_MIN
#define _UI16_MAX USHRT_MAX
#define _UI16_MIN USHRT_MIN
#define _UI32_MAX UINT_MAX
#define _UI32_MIN UINT_MIN

#ifdef PAL_STDCPP_COMPAT
#undef NULL
#endif

#ifndef NULL
#if defined(__cplusplus)
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

#if defined(PAL_STDCPP_COMPAT) && !defined(__cplusplus)
#define nullptr NULL
#endif // defined(PAL_STDCPP_COMPAT) && !defined(__cplusplus)

#ifndef PAL_STDCPP_COMPAT
typedef ULONG64   fpos_t;

typedef __int64 time_t;
#define _TIME_T_DEFINED
#endif // !PAL_STDCPP_COMPAT

#if ENABLE_DOWNLEVEL_FOR_NLS
#define MAKELCID(lgid, srtid)  ((uint32_t)((((uint32_t)((uint16_t  )(srtid))) << 16) |  \
                                         ((uint32_t)((uint16_t  )(lgid)))))
#define LANGIDFROMLCID(lcid)   ((uint16_t)(lcid))
#define SORTIDFROMLCID(lcid)   ((uint16_t)((((uint32_t)(lcid)) >> 16) & 0xf))

#define LANG_NEUTRAL                     0x00
#define LANG_INVARIANT                   0x7f
#define SUBLANG_NEUTRAL                  0x00    // language neutral
#define SUBLANG_DEFAULT                  0x01    // user default
#define SORT_DEFAULT                     0x0     // sorting default
#define SUBLANG_SYS_DEFAULT              0x02    // system default

#define MAKELANGID(p, s)       ((((uint16_t  )(s)) << 10) | (uint16_t  )(p))
#define PRIMARYLANGID(lgid)    ((uint16_t  )(lgid) & 0x3ff)
#define SUBLANGID(lgid)        ((uint16_t  )(lgid) >> 10)

#define LANG_SYSTEM_DEFAULT    (MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT))
#define LANG_USER_DEFAULT      (MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT))
#define LOCALE_SYSTEM_DEFAULT  (MAKELCID(LANG_SYSTEM_DEFAULT, SORT_DEFAULT))
#define LOCALE_USER_DEFAULT    (MAKELCID(LANG_USER_DEFAULT, SORT_DEFAULT))
#define LOCALE_NEUTRAL         (MAKELCID(MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), SORT_DEFAULT))
#define LOCALE_US_ENGLISH      (MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT))
#define LOCALE_INVARIANT       (MAKELCID(MAKELANGID(LANG_INVARIANT, SUBLANG_NEUTRAL), SORT_DEFAULT))

#define SUBLANG_ENGLISH_US               0x01
#define SUBLANG_CHINESE_TRADITIONAL      0x01    /* Chinese (Traditional) */

#endif // ENABLE_DOWNLEVEL_FOR_NLS


#define CT_CTYPE1                 0x00000001  /* ctype 1 information */
#define CT_CTYPE2                 0x00000002  /* ctype 2 information */
#define CT_CTYPE3                 0x00000004  /* ctype 3 information */
#define C1_UPPER                  0x0001      /* upper case */
#define C1_LOWER                  0x0002      /* lower case */
#define C1_DIGIT                  0x0004      /* decimal digits */
#define C1_SPACE                  0x0008      /* spacing characters */
#define C1_PUNCT                  0x0010      /* punctuation characters */
#define C1_CNTRL                  0x0020      /* control characters */
#define C1_BLANK                  0x0040      /* blank characters */
#define C1_XDIGIT                 0x0080      /* other digits */
#define C1_ALPHA                  0x0100      /* any linguistic character */
#define C2_LEFTTORIGHT            0x0001      /* left to right */
#define C2_RIGHTTOLEFT            0x0002      /* right to left */
#define C2_EUROPENUMBER           0x0003      /* European number, digit */
#define C2_EUROPESEPARATOR        0x0004      /* European numeric separator */
#define C2_EUROPETERMINATOR       0x0005      /* European numeric terminator */
#define C2_ARABICNUMBER           0x0006      /* Arabic number */
#define C2_COMMONSEPARATOR        0x0007      /* common numeric separator */
#define C2_BLOCKSEPARATOR         0x0008      /* block separator */
#define C2_SEGMENTSEPARATOR       0x0009      /* segment separator */
#define C2_WHITESPACE             0x000A      /* white space */
#define C2_OTHERNEUTRAL           0x000B      /* other neutrals */
#define C2_NOTAPPLICABLE          0x0000      /* no implicit directionality */
#define C3_NONSPACING             0x0001      /* nonspacing character */
#define C3_DIACRITIC              0x0002      /* diacritic mark */
#define C3_VOWELMARK              0x0004      /* vowel mark */
#define C3_SYMBOL                 0x0008      /* symbols */
#define C3_KATAKANA               0x0010      /* katakana character */
#define C3_HIRAGANA               0x0020      /* hiragana character */
#define C3_HALFWIDTH              0x0040      /* half width character */
#define C3_FULLWIDTH              0x0080      /* full width character */
#define C3_IDEOGRAPH              0x0100      /* ideographic character */
#define C3_KASHIDA                0x0200      /* Arabic kashida character */
#define C3_LEXICAL                0x0400      /* lexical character */
#define C3_ALPHA                  0x8000      /* any ling. char (C1_ALPHA) */
#define C3_NOTAPPLICABLE          0x0000      /* ctype 3 is not applicable */

#define DLL_PROCESS_ATTACH 1
#define DLL_THREAD_ATTACH  2
#define DLL_THREAD_DETACH  3
#define DLL_PROCESS_DETACH 0

typedef uint32_t (*PTHREAD_START_ROUTINE)(void * lpThreadParameter);
typedef PTHREAD_START_ROUTINE LPTHREAD_START_ROUTINE;

/******************* PAL-Specific Entrypoints *****************************/

int
PAL_InitializeChakraCore();

/// <summary>
/// This function shuts down PAL WITHOUT exiting the current process.
/// </summary>
void
PAL_Shutdown(
    void);

/// <summary>
/// This function shuts down PAL and exits the current process.
/// </summary>
void
PAL_Terminate(
    void);

/// <summary>
/// This function shuts down PAL and exits the current process with
/// the specified exit code.
/// </summary>
void
PAL_TerminateEx(
    int exitCode);

void
PAL_UnregisterModule(
     HINSTANCE hInstance);

BOOL
PAL_GetPALDirectoryW(
     LPWSTR lpDirectoryName,
     uint32_t cchDirectoryName);

BOOL
PAL_GetPALDirectoryA(
     LPSTR lpDirectoryName,
     uint32_t cchDirectoryName);

#ifdef UNICODE
#define PAL_GetPALDirectory PAL_GetPALDirectoryW
#else
#define PAL_GetPALDirectory PAL_GetPALDirectoryA
#endif

BOOL
PAL_Random(
     BOOL bStrong,
      void * lpBuffer,
     uint32_t dwLength);

uint32_t
PAL_CreateExecWatchpoint(
    HANDLE hThread,
    void * pvInstruction
    );

uint32_t
PAL_DeleteExecWatchpoint(
    HANDLE hThread,
    void * pvInstruction
    );

/******************* winuser.h Entrypoints *******************************/

LPSTR
CharNextA(
             LPCSTR lpsz);

LPSTR
CharNextExA(
         uint16_t CodePage,
         LPCSTR lpCurrentChar,
         uint32_t dwFlags);

#ifndef UNICODE
#define CharNext CharNextA
#define CharNextEx CharNextExA
#endif

extern int sprintf_s(char *_Dst, size_t _SizeInBytes, const char *_Format, ...);

typedef int errno_t;
extern errno_t _ultow_s(unsigned long inValue, WCHAR* outBuffer, size_t inDestBufferSize, int inRadix );
extern errno_t _ui64tow_s(unsigned long long inValue, WCHAR* outBuffer, size_t inDestBufferSize, int inRadix );

int
wsprintfA(
       LPSTR,
       LPCSTR,
      ...);

int
wsprintfW(
       LPWSTR,
       LPCWSTR,
      ...);

#ifdef UNICODE
#define wsprintf wsprintfW
#else
#define wsprintf wsprintfA
#endif

// From win32.h
#ifndef _CRTIMP
#ifdef __llvm__
#define _CRTIMP
#else // __llvm__
#define _CRTIMP __declspec(dllimport)
#endif // __llvm__
#endif // _CRTIMP

/******************* winbase.h Entrypoints and defines ************************/
BOOL
AreFileApisANSI(
        void);

typedef struct _SECURITY_ATTRIBUTES {
            uint32_t nLength;
            void * lpSecurityDescriptor;
            BOOL bInheritHandle;
} SECURITY_ATTRIBUTES, *PSECURITY_ATTRIBUTES, *LPSECURITY_ATTRIBUTES;

#define _SH_DENYWR      0x20    /* deny write mode */

#define FILE_READ_DATA            ( 0x0001 )    // file & pipe
#define FILE_APPEND_DATA          ( 0x0004 )    // file

#define GENERIC_READ               (0x80000000L)
#define GENERIC_WRITE              (0x40000000L)

#define FILE_SHARE_READ            0x00000001
#define FILE_SHARE_WRITE           0x00000002
#define FILE_SHARE_DELETE          0x00000004

#define CREATE_NEW                 1
#define CREATE_ALWAYS              2
#define OPEN_EXISTING              3
#define OPEN_ALWAYS                4
#define TRUNCATE_EXISTING          5

#define FILE_ATTRIBUTE_READONLY                 0x00000001
#define FILE_ATTRIBUTE_HIDDEN                   0x00000002
#define FILE_ATTRIBUTE_SYSTEM                   0x00000004
#define FILE_ATTRIBUTE_DIRECTORY                0x00000010
#define FILE_ATTRIBUTE_ARCHIVE                  0x00000020
#define FILE_ATTRIBUTE_DEVICE                   0x00000040
#define FILE_ATTRIBUTE_NORMAL                   0x00000080

#define FILE_FLAG_WRITE_THROUGH    0x80000000
#define FILE_FLAG_NO_BUFFERING     0x20000000
#define FILE_FLAG_RANDOM_ACCESS    0x10000000
#define FILE_FLAG_SEQUENTIAL_SCAN  0x08000000
#define FILE_FLAG_BACKUP_SEMANTICS 0x02000000

#define FILE_BEGIN                 0
#define FILE_CURRENT               1
#define FILE_END                   2

#define STILL_ACTIVE (0x00000103L)

#define INVALID_SET_FILE_POINTER   ((uint32_t)-1)

HANDLE
CreateFileA(
         LPCSTR lpFileName,
         uint32_t dwDesiredAccess,
         uint32_t dwShareMode,
         LPSECURITY_ATTRIBUTES lpSecurityAttributes,
         uint32_t dwCreationDisposition,
         uint32_t dwFlagsAndAttributes,
         HANDLE hTemplateFile);

HANDLE
CreateFileW(
         LPCWSTR lpFileName,
         uint32_t dwDesiredAccess,
         uint32_t dwShareMode,
         LPSECURITY_ATTRIBUTES lpSecurityAttributes,
         uint32_t dwCreationDisposition,
         uint32_t dwFlagsAndAttributes,
         HANDLE hTemplateFile);

#ifdef UNICODE
#define CreateFile CreateFileW
#else
#define CreateFile CreateFileA
#endif

BOOL
LockFile(
     HANDLE hFile,
     uint32_t dwFileOffsetLow,
     uint32_t dwFileOffsetHigh,
     uint32_t nNumberOfBytesToLockLow,
     uint32_t nNumberOfBytesToLockHigh
    );

BOOL
UnlockFile(
     HANDLE hFile,
     uint32_t dwFileOffsetLow,
     uint32_t dwFileOffsetHigh,
     uint32_t nNumberOfBytesToUnlockLow,
     uint32_t nNumberOfBytesToUnlockHigh
    );

uint32_t
SearchPathA(
     LPCSTR lpPath,
     LPCSTR lpFileName,
     LPCSTR lpExtension,
     uint32_t nBufferLength,
     LPSTR lpBuffer,
     LPSTR *lpFilePart
    );

uint32_t
SearchPathW(
     LPCWSTR lpPath,
     LPCWSTR lpFileName,
     LPCWSTR lpExtension,
     uint32_t nBufferLength,
     LPWSTR lpBuffer,
     LPWSTR *lpFilePart
    );
#ifdef UNICODE
#define SearchPath  SearchPathW
#else
#define SearchPath  SearchPathA
#endif // !UNICODE


BOOL
CopyFileA(
       LPCSTR lpExistingFileName,
       LPCSTR lpNewFileName,
       BOOL bFailIfExists);

BOOL
CopyFileW(
       LPCWSTR lpExistingFileName,
       LPCWSTR lpNewFileName,
       BOOL bFailIfExists);

#ifdef UNICODE
#define CopyFile CopyFileW
#else
#define CopyFile CopyFileA
#endif

BOOL
DeleteFileA(
         LPCSTR lpFileName);

BOOL
DeleteFileW(
         LPCWSTR lpFileName);

#ifdef UNICODE
#define DeleteFile DeleteFileW
#else
#define DeleteFile DeleteFileA
#endif


BOOL
MoveFileA(
      LPCSTR lpExistingFileName,
      LPCSTR lpNewFileName);

BOOL
MoveFileW(
      LPCWSTR lpExistingFileName,
      LPCWSTR lpNewFileName);

#ifdef UNICODE
#define MoveFile MoveFileW
#else
#define MoveFile MoveFileA
#endif

#define MOVEFILE_REPLACE_EXISTING      0x00000001
#define MOVEFILE_COPY_ALLOWED          0x00000002

BOOL
MoveFileExA(
         LPCSTR lpExistingFileName,
         LPCSTR lpNewFileName,
         uint32_t dwFlags);

BOOL
MoveFileExW(
         LPCWSTR lpExistingFileName,
         LPCWSTR lpNewFileName,
         uint32_t dwFlags);

#ifdef UNICODE
#define MoveFileEx MoveFileExW
#else
#define MoveFileEx MoveFileExA
#endif

BOOL
CreateDirectoryA(
          LPCSTR lpPathName,
          LPSECURITY_ATTRIBUTES lpSecurityAttributes);

BOOL
CreateDirectoryW(
          LPCWSTR lpPathName,
          LPSECURITY_ATTRIBUTES lpSecurityAttributes);

#ifdef UNICODE
#define CreateDirectory CreateDirectoryW
#else
#define CreateDirectory CreateDirectoryA
#endif

BOOL
RemoveDirectoryW(
          LPCWSTR lpPathName);

BOOL
RemoveDirectoryA(
          LPCSTR lpPathName);

#ifdef UNICODE
#define RemoveDirectory RemoveDirectoryW
#else
#define RemoveDirectory RemoveDirectoryA
#endif

typedef struct _BY_HANDLE_FILE_INFORMATION {
    uint32_t dwFileAttributes;
    FILETIME ftCreationTime;
    FILETIME ftLastAccessTime;
    FILETIME ftLastWriteTime;
    uint32_t dwVolumeSerialNumber;
    uint32_t nFileSizeHigh;
    uint32_t nFileSizeLow;
    uint32_t nNumberOfLinks;
    uint32_t nFileIndexHigh;
    uint32_t nFileIndexLow;
} BY_HANDLE_FILE_INFORMATION, *PBY_HANDLE_FILE_INFORMATION, *LPBY_HANDLE_FILE_INFORMATION;

typedef struct _WIN32_FIND_DATAA {
    uint32_t dwFileAttributes;
    FILETIME ftCreationTime;
    FILETIME ftLastAccessTime;
    FILETIME ftLastWriteTime;
    uint32_t nFileSizeHigh;
    uint32_t nFileSizeLow;
    uint32_t dwReserved0;
    uint32_t dwReserved1;
    char cFileName[ MAX_PATH_FNAME ];
    char cAlternateFileName[ 14 ];
} WIN32_FIND_DATAA, *PWIN32_FIND_DATAA, *LPWIN32_FIND_DATAA;

typedef struct _WIN32_FIND_DATAW {
    uint32_t dwFileAttributes;
    FILETIME ftCreationTime;
    FILETIME ftLastAccessTime;
    FILETIME ftLastWriteTime;
    uint32_t nFileSizeHigh;
    uint32_t nFileSizeLow;
    uint32_t dwReserved0;
    uint32_t dwReserved1;
    WCHAR cFileName[ MAX_PATH_FNAME ];
    WCHAR cAlternateFileName[ 14 ];
} WIN32_FIND_DATAW, *PWIN32_FIND_DATAW, *LPWIN32_FIND_DATAW;

#ifdef UNICODE
typedef WIN32_FIND_DATAW WIN32_FIND_DATA;
typedef PWIN32_FIND_DATAW PWIN32_FIND_DATA;
typedef LPWIN32_FIND_DATAW LPWIN32_FIND_DATA;
#else
typedef WIN32_FIND_DATAA WIN32_FIND_DATA;
typedef PWIN32_FIND_DATAA PWIN32_FIND_DATA;
typedef LPWIN32_FIND_DATAA LPWIN32_FIND_DATA;
#endif

HANDLE
FindFirstFileA(
            LPCSTR lpFileName,
            LPWIN32_FIND_DATAA lpFindFileData);

HANDLE
FindFirstFileW(
            LPCWSTR lpFileName,
            LPWIN32_FIND_DATAW lpFindFileData);

#ifdef UNICODE
#define FindFirstFile FindFirstFileW
#else
#define FindFirstFile FindFirstFileA
#endif

BOOL
FindNextFileA(
           HANDLE hFindFile,
           LPWIN32_FIND_DATAA lpFindFileData);

BOOL
FindNextFileW(
           HANDLE hFindFile,
           LPWIN32_FIND_DATAW lpFindFileData);

#ifdef UNICODE
#define FindNextFile FindNextFileW
#else
#define FindNextFile FindNextFileA
#endif

BOOL
FindClose(
        HANDLE hFindFile);

uint32_t
GetFileAttributesA(
            LPCSTR lpFileName);

uint32_t
GetFileAttributesW(
            LPCWSTR lpFileName);

#ifdef UNICODE
#define GetFileAttributes GetFileAttributesW
#else
#define GetFileAttributes GetFileAttributesA
#endif

typedef enum _GET_FILEEX_INFO_LEVELS {
  GetFileExInfoStandard
} GET_FILEEX_INFO_LEVELS;

typedef struct _WIN32_FILE_ATTRIBUTE_DATA {
    uint32_t      dwFileAttributes;
    FILETIME   ftCreationTime;
    FILETIME   ftLastAccessTime;
    FILETIME   ftLastWriteTime;
    uint32_t      nFileSizeHigh;
    uint32_t      nFileSizeLow;
} WIN32_FILE_ATTRIBUTE_DATA, *LPWIN32_FILE_ATTRIBUTE_DATA;

BOOL
GetFileAttributesExW(
              LPCWSTR lpFileName,
              GET_FILEEX_INFO_LEVELS fInfoLevelId,
              void * lpFileInformation);

#ifdef UNICODE
#define GetFileAttributesEx GetFileAttributesExW
#endif

BOOL
SetFileAttributesA(
            LPCSTR lpFileName,
            uint32_t dwFileAttributes);

BOOL
SetFileAttributesW(
            LPCWSTR lpFileName,
            uint32_t dwFileAttributes);

#ifdef UNICODE
#define SetFileAttributes SetFileAttributesW
#else
#define SetFileAttributes SetFileAttributesA
#endif

typedef void * LPOVERLAPPED;  // diff from winbase.h

BOOL
WriteFile(
       HANDLE hFile,
       const void * lpBuffer,
       uint32_t nNumberOfBytesToWrite,
       uint32_t * lpNumberOfBytesWritten,
       LPOVERLAPPED lpOverlapped);

BOOL
ReadFile(
      HANDLE hFile,
      void * lpBuffer,
      uint32_t nNumberOfBytesToRead,
      uint32_t * lpNumberOfBytesRead,
      LPOVERLAPPED lpOverlapped);

#define STD_INPUT_HANDLE         ((uint32_t)-10)
#define STD_OUTPUT_HANDLE        ((uint32_t)-11)
#define STD_ERROR_HANDLE         ((uint32_t)-12)

HANDLE
GetStdHandle(
          uint32_t nStdHandle);

BOOL
SetEndOfFile(
          HANDLE hFile);

uint32_t
SetFilePointer(
            HANDLE hFile,
            int32_t lDistanceToMove,
            int32_t * lpDistanceToMoveHigh,
            uint32_t dwMoveMethod);

BOOL
SetFilePointerEx(
            HANDLE hFile,
            LARGE_INTEGER liDistanceToMove,
            PLARGE_INTEGER lpNewFilePointer,
            uint32_t dwMoveMethod);

uint32_t
GetFileSize(
         HANDLE hFile,
         uint32_t * lpFileSizeHigh);

BOOL
GetFileSizeEx(
           HANDLE hFile,
          PLARGE_INTEGER lpFileSize);

BOOL
GetFileInformationByHandle(
         HANDLE hFile,
         BY_HANDLE_FILE_INFORMATION* lpFileInformation);

int32_t
CompareFileTime(
         const FILETIME *lpFileTime1,
         const FILETIME *lpFileTime2);

BOOL
SetFileTime(
         HANDLE hFile,
         const FILETIME *lpCreationTime,
         const FILETIME *lpLastAccessTime,
         const FILETIME *lpLastWriteTime);

BOOL
GetFileTime(
         HANDLE hFile,
         LPFILETIME lpCreationTime,
         LPFILETIME lpLastAccessTime,
         LPFILETIME lpLastWriteTime);

void
GetSystemTimeAsFileTime(
             LPFILETIME lpSystemTimeAsFileTime);

typedef struct _SYSTEMTIME {
    uint16_t wYear;
    uint16_t wMonth;
    uint16_t wDayOfWeek;
    uint16_t wDay;
    uint16_t wHour;
    uint16_t wMinute;
    uint16_t wSecond;
    uint16_t wMilliseconds;
} SYSTEMTIME, *PSYSTEMTIME, *LPSYSTEMTIME;

BOOL
FileTimeToSystemTime(
             const FILETIME *lpFileTime,
             LPSYSTEMTIME lpSystemTime);

BOOL
FileTimeToDosDateTime(
     const FILETIME *lpFileTime,
     uint16_t * lpFatDate,
     uint16_t * lpFatTime
    );



BOOL
FlushFileBuffers(
          HANDLE hFile);

#define FILE_TYPE_UNKNOWN         0x0000
#define FILE_TYPE_DISK            0x0001
#define FILE_TYPE_CHAR            0x0002
#define FILE_TYPE_PIPE            0x0003
#define FILE_TYPE_REMOTE          0x8000

uint32_t
GetFileType(
         HANDLE hFile);

uint32_t
GetConsoleCP(
         void);

uint32_t
GetConsoleOutputCP(
           void);

uint32_t
GetFullPathNameA(
          LPCSTR lpFileName,
          uint32_t nBufferLength,
          LPSTR lpBuffer,
          LPSTR *lpFilePart);

uint32_t
GetFullPathNameW(
          LPCWSTR lpFileName,
          uint32_t nBufferLength,
          LPWSTR lpBuffer,
          LPWSTR *lpFilePart);

#ifdef UNICODE
#define GetFullPathName GetFullPathNameW
#else
#define GetFullPathName GetFullPathNameA
#endif

uint32_t
GetLongPathNameW(
          LPCWSTR lpszShortPath,
                  LPWSTR lpszLongPath,
          uint32_t cchBuffer);

#ifdef UNICODE
#define GetLongPathName GetLongPathNameW
#endif

uint32_t
GetShortPathNameW(
          LPCWSTR lpszLongPath,
                  LPWSTR lpszShortPath,
          uint32_t cchBuffer);

#ifdef UNICODE
#define GetShortPathName GetShortPathNameW
#endif


uint32_t
GetTempFileNameA(
          LPCSTR lpPathName,
          LPCSTR lpPrefixString,
          uint32_t uUnique,
          LPSTR lpTempFileName);

uint32_t
GetTempFileNameW(
          LPCWSTR lpPathName,
          LPCWSTR lpPrefixString,
          uint32_t uUnique,
          LPWSTR lpTempFileName);

#ifdef UNICODE
#define GetTempFileName GetTempFileNameW
#else
#define GetTempFileName GetTempFileNameA
#endif

uint32_t
GetTempPathA(
          uint32_t nBufferLength,
          LPSTR lpBuffer);

uint32_t
GetTempPathW(
          uint32_t nBufferLength,
          LPWSTR lpBuffer);

#ifdef UNICODE
#define GetTempPath GetTempPathW
#else
#define GetTempPath GetTempPathA
#endif

uint32_t
GetCurrentDirectoryA(
              uint32_t nBufferLength,
              LPSTR lpBuffer);

uint32_t
GetCurrentDirectoryW(
              uint32_t nBufferLength,
              LPWSTR lpBuffer);

#ifdef UNICODE
#define GetCurrentDirectory GetCurrentDirectoryW
#else
#define GetCurrentDirectory GetCurrentDirectoryA
#endif

BOOL
SetCurrentDirectoryA(
             LPCSTR lpPathName);

BOOL
SetCurrentDirectoryW(
             LPCWSTR lpPathName);


#ifdef UNICODE
#define SetCurrentDirectory SetCurrentDirectoryW
#else
#define SetCurrentDirectory SetCurrentDirectoryA
#endif

BOOL
GetUserNameW(
     LPWSTR lpBuffer,      // address of name buffer
      uint32_t * nSize );   // address of size of name buffer

BOOL
GetComputerNameW(
     LPWSTR lpBuffer,     // address of name buffer
      uint32_t * nSize);   // address of size of name buffer

#ifdef UNICODE
#define GetUserName GetUserNameW
#define GetComputerName GetComputerNameW
#endif // UNICODE

HANDLE
CreateSemaphoreA(
          LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
          int32_t lInitialCount,
          int32_t lMaximumCount,
          LPCSTR lpName);

HANDLE
CreateSemaphoreExA(
          LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
          int32_t lInitialCount,
          int32_t lMaximumCount,
          LPCSTR lpName,
          /*_Reserved_*/  uint32_t dwFlags,
          uint32_t dwDesiredAccess);

HANDLE
CreateSemaphoreW(
          LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
          int32_t lInitialCount,
          int32_t lMaximumCount,
          LPCWSTR lpName);

HANDLE
CreateSemaphoreExW(
         LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
         int32_t lInitialCount,
         int32_t lMaximumCount,
         LPCWSTR lpName,
         /*_Reserved_*/  uint32_t dwFlags,
         uint32_t dwDesiredAccess);

HANDLE
OpenSemaphoreW(
     uint32_t dwDesiredAccess,
     BOOL bInheritHandle,
     LPCWSTR lpName);

#ifdef UNICODE
#define CreateSemaphore CreateSemaphoreW
#define CreateSemaphoreEx CreateSemaphoreExW
#else
#define CreateSemaphore CreateSemaphoreA
#define CreateSemaphoreEx CreateSemaphoreExA
#endif

BOOL
ReleaseSemaphore(
          HANDLE hSemaphore,
          int32_t lReleaseCount,
          int32_t * lpPreviousCount);

HANDLE
CreateEventA(
          LPSECURITY_ATTRIBUTES lpEventAttributes,
          BOOL bManualReset,
          BOOL bInitialState,
          LPCSTR lpName);

HANDLE
CreateEventW(
          LPSECURITY_ATTRIBUTES lpEventAttributes,
          BOOL bManualReset,
          BOOL bInitialState,
          LPCWSTR lpName);

#ifdef UNICODE
#define CreateEvent CreateEventW
#else
#define CreateEvent CreateEventA
#endif

BOOL
SetEvent(
      HANDLE hEvent);

BOOL
ResetEvent(
        HANDLE hEvent);

HANDLE
OpenEventW(
        uint32_t dwDesiredAccess,
        BOOL bInheritHandle,
        LPCWSTR lpName);

#ifdef UNICODE
#define OpenEvent OpenEventW
#endif

HANDLE
CreateMutexW(
     LPSECURITY_ATTRIBUTES lpMutexAttributes,
     BOOL bInitialOwner,
     LPCWSTR lpName);

HANDLE
CreateMutexA(
     LPSECURITY_ATTRIBUTES lpMutexAttributes,
     BOOL bInitialOwner,
     LPCSTR lpName);

#ifdef UNICODE
#define CreateMutex  CreateMutexW
#else
#define CreateMutex  CreateMutexA
#endif

HANDLE
OpenMutexW(
        uint32_t dwDesiredAccess,
        BOOL bInheritHandle,
        LPCWSTR lpName);

HANDLE
OpenMutexA(
        uint32_t dwDesiredAccess,
        BOOL bInheritHandle,
        LPCSTR lpName);

#ifdef UNICODE
#define OpenMutex  OpenMutexW
#else
#define OpenMutex  OpenMutexA
#endif // UNICODE

BOOL
ReleaseMutex(
     HANDLE hMutex);

uint32_t
GetCurrentProcessId(
            void);

HANDLE
GetCurrentProcess(
          void);

uint32_t
GetCurrentThreadId(
           void);

uint32_t
GetThreadId(
    HANDLE hThread);

// To work around multiply-defined symbols in the Carbon framework.
#define GetCurrentThread PAL_GetCurrentThread
HANDLE
GetCurrentThread(
         void);


#define STARTF_USESTDHANDLES       0x00000100

typedef struct _STARTUPINFOW {
    uint32_t cb;
    LPWSTR lpReserved_PAL_Undefined;
    LPWSTR lpDesktop_PAL_Undefined;
    LPWSTR lpTitle_PAL_Undefined;
    uint32_t dwX_PAL_Undefined;
    uint32_t dwY_PAL_Undefined;
    uint32_t dwXSize_PAL_Undefined;
    uint32_t dwYSize_PAL_Undefined;
    uint32_t dwXCountChars_PAL_Undefined;
    uint32_t dwYCountChars_PAL_Undefined;
    uint32_t dwFillAttribute_PAL_Undefined;
    uint32_t dwFlags;
    uint16_t wShowWindow_PAL_Undefined;
    uint16_t cbReserved2_PAL_Undefined;
    uint8_t * lpReserved2_PAL_Undefined;
    HANDLE hStdInput;
    HANDLE hStdOutput;
    HANDLE hStdError;
} STARTUPINFOW, *LPSTARTUPINFOW;

typedef struct _STARTUPINFOA {
    uint32_t cb;
    LPSTR lpReserved_PAL_Undefined;
    LPSTR lpDesktop_PAL_Undefined;
    LPSTR lpTitle_PAL_Undefined;
    uint32_t dwX_PAL_Undefined;
    uint32_t dwY_PAL_Undefined;
    uint32_t dwXSize_PAL_Undefined;
    uint32_t dwYSize_PAL_Undefined;
    uint32_t dwXCountChars_PAL_Undefined;
    uint32_t dwYCountChars_PAL_Undefined;
    uint32_t dwFillAttribute_PAL_Undefined;
    uint32_t dwFlags;
    uint16_t wShowWindow_PAL_Undefined;
    uint16_t cbReserved2_PAL_Undefined;
    uint8_t * lpReserved2_PAL_Undefined;
    HANDLE hStdInput;
    HANDLE hStdOutput;
    HANDLE hStdError;
} STARTUPINFOA, *LPSTARTUPINFOA;

#ifdef UNICODE
typedef STARTUPINFOW STARTUPINFO;
typedef LPSTARTUPINFOW LPSTARTUPINFO;
#else
typedef STARTUPINFOA STARTUPINFO;
typedef LPSTARTUPINFOW LPSTARTUPINFO;
#endif

#define CREATE_NEW_CONSOLE          0x00000010

#define NORMAL_PRIORITY_CLASS             0x00000020

typedef struct _PROCESS_INFORMATION {
    HANDLE hProcess;
    HANDLE hThread;
    uint32_t dwProcessId;
    uint32_t dwThreadId_PAL_Undefined;
} PROCESS_INFORMATION, *PPROCESS_INFORMATION, *LPPROCESS_INFORMATION;

BOOL
CreateProcessA(
            LPCSTR lpApplicationName,
            LPSTR lpCommandLine,
            LPSECURITY_ATTRIBUTES lpProcessAttributes,
            LPSECURITY_ATTRIBUTES lpThreadAttributes,
            BOOL bInheritHandles,
            uint32_t dwCreationFlags,
            void * lpEnvironment,
            LPCSTR lpCurrentDirectory,
            LPSTARTUPINFOA lpStartupInfo,
            LPPROCESS_INFORMATION lpProcessInformation);

BOOL
CreateProcessW(
            LPCWSTR lpApplicationName,
            LPWSTR lpCommandLine,
            LPSECURITY_ATTRIBUTES lpProcessAttributes,
            LPSECURITY_ATTRIBUTES lpThreadAttributes,
            BOOL bInheritHandles,
            uint32_t dwCreationFlags,
            void * lpEnvironment,
            LPCWSTR lpCurrentDirectory,
            LPSTARTUPINFOW lpStartupInfo,
            LPPROCESS_INFORMATION lpProcessInformation);

#ifdef UNICODE
#define CreateProcess CreateProcessW
#else
#define CreateProcess CreateProcessA
#endif

__attribute__((noreturn))
void
ExitProcess(
         uint32_t uExitCode);

BOOL
TerminateProcess(
          HANDLE hProcess,
          uint32_t uExitCode);

BOOL
GetExitCodeProcess(
            HANDLE hProcess,
            uint32_t * lpExitCode);

BOOL
GetProcessTimes(
         HANDLE hProcess,
         LPFILETIME lpCreationTime,
         LPFILETIME lpExitTime,
         LPFILETIME lpKernelTime,
         LPFILETIME lpUserTime);

#define MAXIMUM_WAIT_OBJECTS  64
#define WAIT_OBJECT_0 0
#define WAIT_ABANDONED   0x00000080
#define WAIT_ABANDONED_0 0x00000080
#define WAIT_TIMEOUT 258
#define WAIT_FAILED ((uint32_t)0xFFFFFFFF)

#define INFINITE 0xFFFFFFFF // Infinite timeout

uint32_t
WaitForSingleObject(
             HANDLE hHandle,
             uint32_t dwMilliseconds);

uint32_t
WaitForSingleObjectEx(
             HANDLE hHandle,
             uint32_t dwMilliseconds,
             BOOL bAlertable);

uint32_t
WaitForMultipleObjects(
                uint32_t nCount,
                const HANDLE *lpHandles,
                BOOL bWaitAll,
                uint32_t dwMilliseconds);

uint32_t
WaitForMultipleObjectsEx(
              uint32_t nCount,
              const HANDLE *lpHandles,
              BOOL bWaitAll,
              uint32_t dwMilliseconds,
              BOOL bAlertable);

RHANDLE
PAL_LocalHandleToRemote(
             HANDLE hLocal);

HANDLE
PAL_RemoteHandleToLocal(
             RHANDLE hRemote);


#define DUPLICATE_CLOSE_SOURCE      0x00000001
#define DUPLICATE_SAME_ACCESS       0x00000002

BOOL
DuplicateHandle(
         HANDLE hSourceProcessHandle,
         HANDLE hSourceHandle,
         HANDLE hTargetProcessHandle,
         LPHANDLE lpTargetHandle,
         uint32_t dwDesiredAccess,
         BOOL bInheritHandle,
         uint32_t dwOptions);

void
Sleep(
       uint32_t dwMilliseconds);

uint32_t
SleepEx(
     uint32_t dwMilliseconds,
     BOOL bAlertable);

BOOL
SwitchToThread(
    void);

#define DEBUG_PROCESS                     0x00000001
#define DEBUG_ONLY_THIS_PROCESS           0x00000002
#define CREATE_SUSPENDED                  0x00000004
#define STACK_SIZE_PARAM_IS_A_RESERVATION 0x00010000

HANDLE
CreateThread(
          LPSECURITY_ATTRIBUTES lpThreadAttributes,
          uint32_t dwStackSize,
          LPTHREAD_START_ROUTINE lpStartAddress,
          void * lpParameter,
          uint32_t dwCreationFlags,
          uint32_t * lpThreadId);

__attribute__((noreturn))
void
ExitThread(
        uint32_t dwExitCode);

BOOL
GetExitCodeThread(
            HANDLE hThread,
            uint32_t * lpExitCode);

uint32_t
ResumeThread(
          HANDLE hThread);

typedef void (*PAPCFUNC)(size_t dwParam);

uint32_t
QueueUserAPC(
          PAPCFUNC pfnAPC,
          HANDLE hThread,
          size_t dwData);

#ifdef _X86_

//
// ***********************************************************************************
//
// NOTE: These context definitions are replicated in ndp/clr/src/debug/inc/DbgTargetContext.h (for the
// purposes manipulating contexts from different platforms during remote debugging). Be sure to keep those
// definitions in sync if you make any changes here.
//
// ***********************************************************************************
//

#define SIZE_OF_80387_REGISTERS      80

#define CONTEXT_i386            0x00010000
#define CONTEXT_CONTROL         (CONTEXT_i386 | 0x00000001L) // SS:SP, CS:IP, FLAGS, BP
#define CONTEXT_INTEGER         (CONTEXT_i386 | 0x00000002L) // AX, BX, CX, DX, SI, DI
#define CONTEXT_SEGMENTS        (CONTEXT_i386 | 0x00000004L)
#define CONTEXT_FLOATING_POINT  (CONTEXT_i386 | 0x00000008L) // 387 state
#define CONTEXT_DEBUG_REGISTERS (CONTEXT_i386 | 0x00000010L)

#define CONTEXT_FULL (CONTEXT_CONTROL | CONTEXT_INTEGER | CONTEXT_SEGMENTS)
#define CONTEXT_EXTENDED_REGISTERS  (CONTEXT_i386 | 0x00000020L)
#define CONTEXT_ALL (CONTEXT_CONTROL | CONTEXT_INTEGER | CONTEXT_SEGMENTS | CONTEXT_FLOATING_POINT | CONTEXT_DEBUG_REGISTERS | CONTEXT_EXTENDED_REGISTERS)

#define MAXIMUM_SUPPORTED_EXTENSION     512

typedef struct _FLOATING_SAVE_AREA {
    uint32_t   ControlWord;
    uint32_t   StatusWord;
    uint32_t   TagWord;
    uint32_t   ErrorOffset;
    uint32_t   ErrorSelector;
    uint32_t   DataOffset;
    uint32_t   DataSelector;
    uint8_t    RegisterArea[SIZE_OF_80387_REGISTERS];
    uint32_t   Cr0NpxState;
} FLOATING_SAVE_AREA;

typedef FLOATING_SAVE_AREA *PFLOATING_SAVE_AREA;

typedef struct _CONTEXT {
    uint32_t ContextFlags;

    uint32_t   Dr0_PAL_Undefined;
    uint32_t   Dr1_PAL_Undefined;
    uint32_t   Dr2_PAL_Undefined;
    uint32_t   Dr3_PAL_Undefined;
    uint32_t   Dr6_PAL_Undefined;
    uint32_t   Dr7_PAL_Undefined;

    FLOATING_SAVE_AREA FloatSave;

    uint32_t   SegGs_PAL_Undefined;
    uint32_t   SegFs_PAL_Undefined;
    uint32_t   SegEs_PAL_Undefined;
    uint32_t   SegDs_PAL_Undefined;

    uint32_t   Edi;
    uint32_t   Esi;
    uint32_t   Ebx;
    uint32_t   Edx;
    uint32_t   Ecx;
    uint32_t   Eax;

    uint32_t   Ebp;
    uint32_t   Eip;
    uint32_t   SegCs;
    uint32_t   EFlags;
    uint32_t   Esp;
    uint32_t   SegSs;

    unsigned char   ExtendedRegisters[MAXIMUM_SUPPORTED_EXTENSION];

} CONTEXT, *PCONTEXT, *LPCONTEXT;

// To support saving and loading xmm register context we need to know the offset in the ExtendedRegisters
// section at which they are stored. This has been determined experimentally since I have found no
// documentation thus far but it corresponds to the offset we'd expect if a fxsave instruction was used to
// store the regular FP state along with the XMM registers at the start of the extended registers section.
// Technically the offset doesn't really matter if no code in the PAL or runtime knows what the offset should
// be either (as long as we're consistent across GetThreadContext() and SetThreadContext() and we don't
// support any other values in the ExtendedRegisters) but we might as well be as accurate as we can.
#define CONTEXT_EXREG_XMM_OFFSET 160

#elif defined(_PPC_)

//
// ***********************************************************************************
//
// NOTE: These context definitions are replicated in ndp/clr/src/debug/inc/DbgTargetContext.h (for the
// purposes manipulating contexts from different platforms during remote debugging). Be sure to keep those
// definitions in sync if you make any changes here.
//
// ***********************************************************************************
//

#define CONTEXT_CONTROL         0x00000001L
#define CONTEXT_FLOATING_POINT  0x00000002L
#define CONTEXT_INTEGER         0x00000004L

#define CONTEXT_FULL (CONTEXT_CONTROL | CONTEXT_FLOATING_POINT | CONTEXT_INTEGER)
#define CONTEXT_ALL CONTEXT_FULL

typedef struct _CONTEXT {

    //
    // This section is specified/returned if the ContextFlags word contains
    // the flag CONTEXT_FLOATING_POINT.
    //

    double Fpr0;                        // Floating registers 0..31
    double Fpr1;
    double Fpr2;
    double Fpr3;
    double Fpr4;
    double Fpr5;
    double Fpr6;
    double Fpr7;
    double Fpr8;
    double Fpr9;
    double Fpr10;
    double Fpr11;
    double Fpr12;
    double Fpr13;
    double Fpr14;
    double Fpr15;
    double Fpr16;
    double Fpr17;
    double Fpr18;
    double Fpr19;
    double Fpr20;
    double Fpr21;
    double Fpr22;
    double Fpr23;
    double Fpr24;
    double Fpr25;
    double Fpr26;
    double Fpr27;
    double Fpr28;
    double Fpr29;
    double Fpr30;
    double Fpr31;
    double Fpscr;                       // Floating point status/control reg

    //
    // This section is specified/returned if the ContextFlags word contains
    // the flag CONTEXT_INTEGER.
    //

    uint32_t Gpr0;                         // General registers 0..31
    uint32_t Gpr1;                         // StackPointer
    uint32_t Gpr2;
    uint32_t Gpr3;
    uint32_t Gpr4;
    uint32_t Gpr5;
    uint32_t Gpr6;
    uint32_t Gpr7;
    uint32_t Gpr8;
    uint32_t Gpr9;
    uint32_t Gpr10;
    uint32_t Gpr11;
    uint32_t Gpr12;
    uint32_t Gpr13;
    uint32_t Gpr14;
    uint32_t Gpr15;
    uint32_t Gpr16;
    uint32_t Gpr17;
    uint32_t Gpr18;
    uint32_t Gpr19;
    uint32_t Gpr20;
    uint32_t Gpr21;
    uint32_t Gpr22;
    uint32_t Gpr23;
    uint32_t Gpr24;
    uint32_t Gpr25;
    uint32_t Gpr26;
    uint32_t Gpr27;
    uint32_t Gpr28;
    uint32_t Gpr29;
    uint32_t Gpr30;
    uint32_t Gpr31;

    uint32_t Cr;                           // Condition register
    uint32_t Xer;                          // Fixed point exception register

    //
    // This section is specified/returned if the ContextFlags word contains
    // the flag CONTEXT_CONTROL.
    //

    uint32_t Msr;                          // Machine status register
    uint32_t Iar;                          // Instruction address register
    uint32_t Lr;                           // Link register
    uint32_t Ctr;                          // Count register

    //
    // The flags values within this flag control the contents of
    // a CONTEXT record.
    //
    // If the context record is used as an input parameter, then
    // for each portion of the context record controlled by a flag
    // whose value is set, it is assumed that that portion of the
    // context record contains valid context. If the context record
    // is being used to modify a thread's context, then only that
    // portion of the threads context will be modified.
    //
    // If the context record is used as an IN OUT parameter to capture
    // the context of a thread, then only those portions of the thread's
    // context corresponding to set flags will be returned.
    //
    // The context record is never used as an OUT only parameter.
    //

    uint32_t ContextFlags;

    uint32_t Fill[3];                      // Pad out to multiple of 16 bytes

    //
    // This section is specified/returned if CONTEXT_DEBUG_REGISTERS is
    // set in ContextFlags.  Note that CONTEXT_DEBUG_REGISTERS is NOT
    // included in CONTEXT_FULL.
    //
    uint32_t Dr0;                          // Breakpoint Register 1
    uint32_t Dr1;                          // Breakpoint Register 2
    uint32_t Dr2;                          // Breakpoint Register 3
    uint32_t Dr3;                          // Breakpoint Register 4
    uint32_t Dr4;                          // Breakpoint Register 5
    uint32_t Dr5;                          // Breakpoint Register 6
    uint32_t Dr6;                          // Debug Status Register
    uint32_t Dr7;                          // Debug Control Register

} CONTEXT, *PCONTEXT, *LPCONTEXT;

#elif defined(_SPARC_)

#define CONTEXT_CONTROL         0x00000001L
#define CONTEXT_FLOATING_POINT  0x00000002L
#define CONTEXT_INTEGER         0x00000004L

#define COUNT_FLOATING_REGISTER 32
#define COUNT_DOUBLE_REGISTER 16
#define CONTEXT_FULL (CONTEXT_CONTROL | CONTEXT_FLOATING_POINT | CONTEXT_INTEGER)
#define CONTEXT_ALL CONTEXT_FULL

typedef struct _CONTEXT {
    //
    // This section is specified/returned if the ContextFlags word contains
    // the flag CONTEXT_INTEGER.
    //
    uint32_t g0;
    uint32_t g1;
    uint32_t g2;
    uint32_t g3;
    uint32_t g4;
    uint32_t g5;
    uint32_t g6;
    uint32_t g7;
    uint32_t o0;
    uint32_t o1;
    uint32_t o2;
    uint32_t o3;
    uint32_t o4;
    uint32_t o5;
    uint32_t sp;
    uint32_t o7;
    uint32_t l0;
    uint32_t l1;
    uint32_t l2;
    uint32_t l3;
    uint32_t l4;
    uint32_t l5;
    uint32_t l6;
    uint32_t l7;
    uint32_t i0;
    uint32_t i1;
    uint32_t i2;
    uint32_t i3;
    uint32_t i4;
    uint32_t i5;
    uint32_t fp;
    uint32_t i7;

    uint32_t y;

    //
    // This section is specified/returned if the ContextFlags word contains
    // the flag CONTEXT_CONTROL.
    //
#if defined(__sparcv9)
    uint32_t ccr;
#else
    uint32_t psr;
#endif
    uint32_t pc;     // program counter
    uint32_t npc;    // next address to be executed

    uint32_t ContextFlags;

    //
    // This section is specified/returned if the ContextFlags word contains
    // the flag CONTEXT_FLOATING_POINT.
    //
    ULONGLONG fsr;
    union {
        float f[COUNT_FLOATING_REGISTER];
        double d[COUNT_DOUBLE_REGISTER];
        } fprs;

} CONTEXT, *PCONTEXT, *LPCONTEXT;

#elif defined(_PARISC_)

// ToDo: Get this correct for PARISC architecture
#define CONTEXT_CONTROL         0x00000001L
#define CONTEXT_FLOATING_POINT  0x00000002L
#define CONTEXT_INTEGER         0x00000004L

#define COUNT_FLOATING_REGISTER 32
#define COUNT_DOUBLE_REGISTER 16
#define CONTEXT_FULL (CONTEXT_CONTROL | CONTEXT_FLOATING_POINT | CONTEXT_INTEGER)
#define CONTEXT_ALL CONTEXT_FULL

typedef struct _CONTEXT {
    //
    // This section is specified/returned if the ContextFlags word contains
    // the flag CONTEXT_INTEGER.
    //
    uint32_t g0;
    uint32_t g1;
    uint32_t g2;
    uint32_t g3;
    uint32_t g4;
    uint32_t g5;
    uint32_t g6;
    uint32_t g7;
    uint32_t o0;
    uint32_t o1;
    uint32_t o2;
    uint32_t o3;
    uint32_t o4;
    uint32_t o5;
    uint32_t sp;
    uint32_t o7;
    uint32_t l0;
    uint32_t l1;
    uint32_t l2;
    uint32_t l3;
    uint32_t l4;
    uint32_t l5;
    uint32_t l6;
    uint32_t l7;
    uint32_t i0;
    uint32_t i1;
    uint32_t i2;
    uint32_t i3;
    uint32_t i4;
    uint32_t i5;
    uint32_t fp;
    uint32_t i7;

    uint32_t y;

    //
    // This section is specified/returned if the ContextFlags word contains
    // the flag CONTEXT_CONTROL.
    //
    uint32_t psr;
    uint32_t pc;     // program counter
    uint32_t npc;    // next address to be executed

    uint32_t ContextFlags;

    //
    // This section is specified/returned if the ContextFlags word contains
    // the flag CONTEXT_FLOATING_POINT.
    //
    ULONGLONG fsr;
    union {
        float f[COUNT_FLOATING_REGISTER];
        double d[COUNT_DOUBLE_REGISTER];
        } fprs;

} CONTEXT, *PCONTEXT, *LPCONTEXT;

#elif defined(_IA64_)

// copied from winnt.h
typedef struct _FLOAT128 {
    __int64 LowPart;
    __int64 HighPart;
} FLOAT128;

typedef FLOAT128 *PFLOAT128;

// begin_ntddk begin_nthal

//
// The following flags control the contents of the CONTEXT structure.
//

#if !defined(RC_INVOKED)

#define CONTEXT_IA64                    0x00080000

#define CONTEXT_CONTROL                 (CONTEXT_IA64 | 0x00000001L)
#define CONTEXT_LOWER_FLOATING_POINT    (CONTEXT_IA64 | 0x00000002L)
#define CONTEXT_HIGHER_FLOATING_POINT   (CONTEXT_IA64 | 0x00000004L)
#define CONTEXT_INTEGER                 (CONTEXT_IA64 | 0x00000008L)
#define CONTEXT_DEBUG                   (CONTEXT_IA64 | 0x00000010L)
#define CONTEXT_IA32_CONTROL            (CONTEXT_IA64 | 0x00000020L)  // Includes StIPSR


#define CONTEXT_FLOATING_POINT          (CONTEXT_LOWER_FLOATING_POINT | CONTEXT_HIGHER_FLOATING_POINT)
#define CONTEXT_FULL                    (CONTEXT_CONTROL | CONTEXT_FLOATING_POINT | CONTEXT_INTEGER | CONTEXT_IA32_CONTROL)
#define CONTEXT_ALL                     (CONTEXT_CONTROL | CONTEXT_FLOATING_POINT | CONTEXT_INTEGER | CONTEXT_DEBUG | CONTEXT_IA32_CONTROL)

#define CONTEXT_EXCEPTION_ACTIVE        0x8000000
#define CONTEXT_SERVICE_ACTIVE          0x10000000
#define CONTEXT_EXCEPTION_REQUEST       0x40000000
#define CONTEXT_EXCEPTION_REPORTING     0x80000000

#endif // !defined(RC_INVOKED)

//
// Context Frame
//
//  This frame has a several purposes: 1) it is used as an argument to
//  NtContinue, 2) it is used to construct a call frame for APC delivery,
//  3) it is used to construct a call frame for exception dispatching
//  in user mode, 4) it is used in the user level thread creation
//  routines, and 5) it is used to to pass thread state to debuggers.
//
//  N.B. Because this record is used as a call frame, it must be EXACTLY
//  a multiple of 16 bytes in length and aligned on a 16-byte boundary.
//

typedef struct _CONTEXT {

    //
    // The flags values within this flag control the contents of
    // a CONTEXT record.
    //
    // If the context record is used as an input parameter, then
    // for each portion of the context record controlled by a flag
    // whose value is set, it is assumed that that portion of the
    // context record contains valid context. If the context record
    // is being used to modify a thread's context, then only that
    // portion of the threads context will be modified.
    //
    // If the context record is used as an IN OUT parameter to capture
    // the context of a thread, then only those portions of the thread's
    // context corresponding to set flags will be returned.
    //
    // The context record is never used as an OUT only parameter.
    //

    uint32_t ContextFlags;
    uint32_t Fill1[3];         // for alignment of following on 16-byte boundary

    //
    // This section is specified/returned if the ContextFlags word contains
    // the flag CONTEXT_DEBUG.
    //
    // N.B. CONTEXT_DEBUG is *not* part of CONTEXT_FULL.
    //

    ULONGLONG DbI0;
    ULONGLONG DbI1;
    ULONGLONG DbI2;
    ULONGLONG DbI3;
    ULONGLONG DbI4;
    ULONGLONG DbI5;
    ULONGLONG DbI6;
    ULONGLONG DbI7;

    ULONGLONG DbD0;
    ULONGLONG DbD1;
    ULONGLONG DbD2;
    ULONGLONG DbD3;
    ULONGLONG DbD4;
    ULONGLONG DbD5;
    ULONGLONG DbD6;
    ULONGLONG DbD7;

    //
    // This section is specified/returned if the ContextFlags word contains
    // the flag CONTEXT_LOWER_FLOATING_POINT.
    //

    FLOAT128 FltS0;
    FLOAT128 FltS1;
    FLOAT128 FltS2;
    FLOAT128 FltS3;
    FLOAT128 FltT0;
    FLOAT128 FltT1;
    FLOAT128 FltT2;
    FLOAT128 FltT3;
    FLOAT128 FltT4;
    FLOAT128 FltT5;
    FLOAT128 FltT6;
    FLOAT128 FltT7;
    FLOAT128 FltT8;
    FLOAT128 FltT9;

    //
    // This section is specified/returned if the ContextFlags word contains
    // the flag CONTEXT_HIGHER_FLOATING_POINT.
    //

    FLOAT128 FltS4;
    FLOAT128 FltS5;
    FLOAT128 FltS6;
    FLOAT128 FltS7;
    FLOAT128 FltS8;
    FLOAT128 FltS9;
    FLOAT128 FltS10;
    FLOAT128 FltS11;
    FLOAT128 FltS12;
    FLOAT128 FltS13;
    FLOAT128 FltS14;
    FLOAT128 FltS15;
    FLOAT128 FltS16;
    FLOAT128 FltS17;
    FLOAT128 FltS18;
    FLOAT128 FltS19;

    FLOAT128 FltF32;
    FLOAT128 FltF33;
    FLOAT128 FltF34;
    FLOAT128 FltF35;
    FLOAT128 FltF36;
    FLOAT128 FltF37;
    FLOAT128 FltF38;
    FLOAT128 FltF39;

    FLOAT128 FltF40;
    FLOAT128 FltF41;
    FLOAT128 FltF42;
    FLOAT128 FltF43;
    FLOAT128 FltF44;
    FLOAT128 FltF45;
    FLOAT128 FltF46;
    FLOAT128 FltF47;
    FLOAT128 FltF48;
    FLOAT128 FltF49;

    FLOAT128 FltF50;
    FLOAT128 FltF51;
    FLOAT128 FltF52;
    FLOAT128 FltF53;
    FLOAT128 FltF54;
    FLOAT128 FltF55;
    FLOAT128 FltF56;
    FLOAT128 FltF57;
    FLOAT128 FltF58;
    FLOAT128 FltF59;

    FLOAT128 FltF60;
    FLOAT128 FltF61;
    FLOAT128 FltF62;
    FLOAT128 FltF63;
    FLOAT128 FltF64;
    FLOAT128 FltF65;
    FLOAT128 FltF66;
    FLOAT128 FltF67;
    FLOAT128 FltF68;
    FLOAT128 FltF69;

    FLOAT128 FltF70;
    FLOAT128 FltF71;
    FLOAT128 FltF72;
    FLOAT128 FltF73;
    FLOAT128 FltF74;
    FLOAT128 FltF75;
    FLOAT128 FltF76;
    FLOAT128 FltF77;
    FLOAT128 FltF78;
    FLOAT128 FltF79;

    FLOAT128 FltF80;
    FLOAT128 FltF81;
    FLOAT128 FltF82;
    FLOAT128 FltF83;
    FLOAT128 FltF84;
    FLOAT128 FltF85;
    FLOAT128 FltF86;
    FLOAT128 FltF87;
    FLOAT128 FltF88;
    FLOAT128 FltF89;

    FLOAT128 FltF90;
    FLOAT128 FltF91;
    FLOAT128 FltF92;
    FLOAT128 FltF93;
    FLOAT128 FltF94;
    FLOAT128 FltF95;
    FLOAT128 FltF96;
    FLOAT128 FltF97;
    FLOAT128 FltF98;
    FLOAT128 FltF99;

    FLOAT128 FltF100;
    FLOAT128 FltF101;
    FLOAT128 FltF102;
    FLOAT128 FltF103;
    FLOAT128 FltF104;
    FLOAT128 FltF105;
    FLOAT128 FltF106;
    FLOAT128 FltF107;
    FLOAT128 FltF108;
    FLOAT128 FltF109;

    FLOAT128 FltF110;
    FLOAT128 FltF111;
    FLOAT128 FltF112;
    FLOAT128 FltF113;
    FLOAT128 FltF114;
    FLOAT128 FltF115;
    FLOAT128 FltF116;
    FLOAT128 FltF117;
    FLOAT128 FltF118;
    FLOAT128 FltF119;

    FLOAT128 FltF120;
    FLOAT128 FltF121;
    FLOAT128 FltF122;
    FLOAT128 FltF123;
    FLOAT128 FltF124;
    FLOAT128 FltF125;
    FLOAT128 FltF126;
    FLOAT128 FltF127;

    //
    // This section is specified/returned if the ContextFlags word contains
    // the flag CONTEXT_LOWER_FLOATING_POINT | CONTEXT_HIGHER_FLOATING_POINT | CONTEXT_CONTROL.
    //

    ULONGLONG StFPSR;       //  FP status

    //
    // This section is specified/returned if the ContextFlags word contains
    // the flag CONTEXT_INTEGER.
    //
    // N.B. The registers gp, sp, rp are part of the control context
    //

    ULONGLONG IntGp;        //  r1, volatile
    ULONGLONG IntT0;        //  r2-r3, volatile
    ULONGLONG IntT1;        //
    ULONGLONG IntS0;        //  r4-r7, preserved
    ULONGLONG IntS1;
    ULONGLONG IntS2;
    ULONGLONG IntS3;
    ULONGLONG IntV0;        //  r8, volatile
    ULONGLONG IntT2;        //  r9-r11, volatile
    ULONGLONG IntT3;
    ULONGLONG IntT4;
    ULONGLONG IntSp;        //  stack pointer (r12), special
    ULONGLONG IntTeb;       //  teb (r13), special
    ULONGLONG IntT5;        //  r14-r31, volatile
    ULONGLONG IntT6;
    ULONGLONG IntT7;
    ULONGLONG IntT8;
    ULONGLONG IntT9;
    ULONGLONG IntT10;
    ULONGLONG IntT11;
    ULONGLONG IntT12;
    ULONGLONG IntT13;
    ULONGLONG IntT14;
    ULONGLONG IntT15;
    ULONGLONG IntT16;
    ULONGLONG IntT17;
    ULONGLONG IntT18;
    ULONGLONG IntT19;
    ULONGLONG IntT20;
    ULONGLONG IntT21;
    ULONGLONG IntT22;

    ULONGLONG IntNats;      //  Nat bits for r1-r31
                            //  r1-r31 in bits 1 thru 31.
    ULONGLONG Preds;        //  predicates, preserved

    ULONGLONG BrRp;         //  return pointer, b0, preserved
    ULONGLONG BrS0;         //  b1-b5, preserved
    ULONGLONG BrS1;
    ULONGLONG BrS2;
    ULONGLONG BrS3;
    ULONGLONG BrS4;
    ULONGLONG BrT0;         //  b6-b7, volatile
    ULONGLONG BrT1;

    //
    // This section is specified/returned if the ContextFlags word contains
    // the flag CONTEXT_CONTROL.
    //

    // Other application registers
    ULONGLONG ApUNAT;       //  User Nat collection register, preserved
    ULONGLONG ApLC;         //  Loop counter register, preserved
    ULONGLONG ApEC;         //  Epilog counter register, preserved
    ULONGLONG ApCCV;        //  CMPXCHG value register, volatile
    ULONGLONG ApDCR;        //  Default control register (TBD)

    // Register stack info
    ULONGLONG RsPFS;        //  Previous function state, preserved
    ULONGLONG RsBSP;        //  Backing store pointer, preserved
    ULONGLONG RsBSPSTORE;
    ULONGLONG RsRSC;        //  RSE configuration, volatile
    ULONGLONG RsRNAT;       //  RSE Nat collection register, preserved

    // Trap Status Information
    ULONGLONG StIPSR;       //  Interruption Processor Status
    ULONGLONG StIIP;        //  Interruption IP
    ULONGLONG StIFS;        //  Interruption Function State

    // iA32 related control registers
    ULONGLONG StFCR;        //  copy of Ar21
    ULONGLONG Eflag;        //  Eflag copy of Ar24
    ULONGLONG SegCSD;       //  iA32 CSDescriptor (Ar25)
    ULONGLONG SegSSD;       //  iA32 SSDescriptor (Ar26)
    ULONGLONG Cflag;        //  Cr0+Cr4 copy of Ar27
    ULONGLONG StFSR;        //  x86 FP status (copy of AR28)
    ULONGLONG StFIR;        //  x86 FP status (copy of AR29)
    ULONGLONG StFDR;        //  x86 FP status (copy of AR30)

      ULONGLONG UNUSEDPACK;   //  added to pack StFDR to 16-bytes

} CONTEXT, *PCONTEXT, *LPCONTEXT;
#elif defined(_AMD64_)
// copied from winnt.h

#define CONTEXT_AMD64   0x100000

#define CONTEXT_CONTROL (CONTEXT_AMD64 | 0x1L)
#define CONTEXT_INTEGER (CONTEXT_AMD64 | 0x2L)
#define CONTEXT_SEGMENTS (CONTEXT_AMD64 | 0x4L)
#define CONTEXT_FLOATING_POINT  (CONTEXT_AMD64 | 0x8L)
#define CONTEXT_DEBUG_REGISTERS (CONTEXT_AMD64 | 0x10L)

#define CONTEXT_FULL (CONTEXT_CONTROL | CONTEXT_INTEGER | CONTEXT_FLOATING_POINT)

#define CONTEXT_ALL (CONTEXT_CONTROL | CONTEXT_INTEGER | CONTEXT_SEGMENTS | CONTEXT_FLOATING_POINT | CONTEXT_DEBUG_REGISTERS)

#define CONTEXT_XSTATE (CONTEXT_AMD64 | 0x40L)

#define CONTEXT_EXCEPTION_ACTIVE 0x8000000
#define CONTEXT_SERVICE_ACTIVE 0x10000000
#define CONTEXT_EXCEPTION_REQUEST 0x40000000
#define CONTEXT_EXCEPTION_REPORTING 0x80000000

typedef struct _M128U {
    ULONGLONG Low;
    LONGLONG High;
} M128U, *PM128U;

// Same as _M128U but aligned to a 16-byte boundary
typedef DECLSPEC_ALIGN(16) M128U M128A;
typedef M128A *PM128A;

typedef struct _XMM_SAVE_AREA32 {
    uint16_t   ControlWord;
    uint16_t   StatusWord;
    uint8_t  TagWord;
    uint8_t  Reserved1;
    uint16_t   ErrorOpcode;
    uint32_t ErrorOffset;
    uint16_t   ErrorSelector;
    uint16_t   Reserved2;
    uint32_t DataOffset;
    uint16_t   DataSelector;
    uint16_t   Reserved3;
    uint32_t MxCsr;
    uint32_t MxCsr_Mask;
    M128A FloatRegisters[8];
    M128A XmmRegisters[16];
    uint8_t  Reserved4[96];
} XMM_SAVE_AREA32, *PXMM_SAVE_AREA32;

#define LEGACY_SAVE_AREA_LENGTH sizeof(XMM_SAVE_AREA32)

//
// Context Frame
//
//  This frame has a several purposes: 1) it is used as an argument to
//  NtContinue, 2) is is used to constuct a call frame for APC delivery,
//  and 3) it is used in the user level thread creation routines.
//
//
// The flags field within this record controls the contents of a CONTEXT
// record.
//
// If the context record is used as an input parameter, then for each
// portion of the context record controlled by a flag whose value is
// set, it is assumed that that portion of the context record contains
// valid context. If the context record is being used to modify a threads
// context, then only that portion of the threads context is modified.
//
// If the context record is used as an output parameter to capture the
// context of a thread, then only those portions of the thread's context
// corresponding to set flags will be returned.
//
// CONTEXT_CONTROL specifies SegSs, Rsp, SegCs, Rip, and EFlags.
//
// CONTEXT_INTEGER specifies Rax, Rcx, Rdx, Rbx, Rbp, Rsi, Rdi, and R8-R15.
//
// CONTEXT_SEGMENTS specifies SegDs, SegEs, SegFs, and SegGs.
//
// CONTEXT_DEBUG_REGISTERS specifies Dr0-Dr3 and Dr6-Dr7.
//
// CONTEXT_MMX_REGISTERS specifies the floating point and extended registers
//     Mm0/St0-Mm7/St7 and Xmm0-Xmm15).
//

typedef struct DECLSPEC_ALIGN(16) _CONTEXT {

    //
    // Register parameter home addresses.
    //
    // N.B. These fields are for convience - they could be used to extend the
    //      context record in the future.
    //

    DWORD64 P1Home;
    DWORD64 P2Home;
    DWORD64 P3Home;
    DWORD64 P4Home;
    DWORD64 P5Home;
    DWORD64 P6Home;

    //
    // Control flags.
    //

    uint32_t ContextFlags;
    uint32_t MxCsr;

    //
    // Segment Registers and processor flags.
    //

    uint16_t   SegCs;
    uint16_t   SegDs;
    uint16_t   SegEs;
    uint16_t   SegFs;
    uint16_t   SegGs;
    uint16_t   SegSs;
    uint32_t EFlags;

    //
    // Debug registers
    //

    DWORD64 Dr0;
    DWORD64 Dr1;
    DWORD64 Dr2;
    DWORD64 Dr3;
    DWORD64 Dr6;
    DWORD64 Dr7;

    //
    // Integer registers.
    //

    DWORD64 Rax;
    DWORD64 Rcx;
    DWORD64 Rdx;
    DWORD64 Rbx;
    DWORD64 Rsp;
    DWORD64 Rbp;
    DWORD64 Rsi;
    DWORD64 Rdi;
    DWORD64 R8;
    DWORD64 R9;
    DWORD64 R10;
    DWORD64 R11;
    DWORD64 R12;
    DWORD64 R13;
    DWORD64 R14;
    DWORD64 R15;

    //
    // Program counter.
    //

    DWORD64 Rip;

    //
    // Floating point state.
    //

    union {
        XMM_SAVE_AREA32 FltSave;
        struct {
            M128A Header[2];
            M128A Legacy[8];
            M128A Xmm0;
            M128A Xmm1;
            M128A Xmm2;
            M128A Xmm3;
            M128A Xmm4;
            M128A Xmm5;
            M128A Xmm6;
            M128A Xmm7;
            M128A Xmm8;
            M128A Xmm9;
            M128A Xmm10;
            M128A Xmm11;
            M128A Xmm12;
            M128A Xmm13;
            M128A Xmm14;
            M128A Xmm15;
        };
    };

    //
    // Vector registers.
    //

    M128A VectorRegister[26];
    DWORD64 VectorControl;

    //
    // Special debug control registers.
    //

    DWORD64 DebugControl;
    DWORD64 LastBranchToRip;
    DWORD64 LastBranchFromRip;
    DWORD64 LastExceptionToRip;
    DWORD64 LastExceptionFromRip;
} CONTEXT, *PCONTEXT, *LPCONTEXT;

//
// Nonvolatile context pointer record.
//

typedef struct _KNONVOLATILE_CONTEXT_POINTERS {
    union {
        PM128A FloatingContext[16];
        struct {
            PM128A Xmm0;
            PM128A Xmm1;
            PM128A Xmm2;
            PM128A Xmm3;
            PM128A Xmm4;
            PM128A Xmm5;
            PM128A Xmm6;
            PM128A Xmm7;
            PM128A Xmm8;
            PM128A Xmm9;
            PM128A Xmm10;
            PM128A Xmm11;
            PM128A Xmm12;
            PM128A Xmm13;
            PM128A Xmm14;
            PM128A Xmm15;
        } ;
    } ;

    union {
        PDWORD64 IntegerContext[16];
        struct {
            PDWORD64 Rax;
            PDWORD64 Rcx;
            PDWORD64 Rdx;
            PDWORD64 Rbx;
            PDWORD64 Rsp;
            PDWORD64 Rbp;
            PDWORD64 Rsi;
            PDWORD64 Rdi;
            PDWORD64 R8;
            PDWORD64 R9;
            PDWORD64 R10;
            PDWORD64 R11;
            PDWORD64 R12;
            PDWORD64 R13;
            PDWORD64 R14;
            PDWORD64 R15;
        } ;
    } ;

} KNONVOLATILE_CONTEXT_POINTERS, *PKNONVOLATILE_CONTEXT_POINTERS;

#elif defined(_ARM_)

#define CONTEXT_ARM   0x00200000L

// end_wx86

#define CONTEXT_CONTROL (CONTEXT_ARM | 0x1L)
#define CONTEXT_INTEGER (CONTEXT_ARM | 0x2L)
#define CONTEXT_FLOATING_POINT  (CONTEXT_ARM | 0x4L)
#define CONTEXT_DEBUG_REGISTERS (CONTEXT_ARM | 0x8L)

#define CONTEXT_FULL (CONTEXT_CONTROL | CONTEXT_INTEGER | CONTEXT_FLOATING_POINT)

#define CONTEXT_ALL (CONTEXT_CONTROL | CONTEXT_INTEGER | CONTEXT_FLOATING_POINT | CONTEXT_DEBUG_REGISTERS)

#define CONTEXT_EXCEPTION_ACTIVE 0x8000000L
#define CONTEXT_SERVICE_ACTIVE 0x10000000L
#define CONTEXT_EXCEPTION_REQUEST 0x40000000L
#define CONTEXT_EXCEPTION_REPORTING 0x80000000L

//
// This flag is set by the unwinder if it has unwound to a call
// site, and cleared whenever it unwinds through a trap frame.
// It is used by language-specific exception handlers to help
// differentiate exception scopes during dispatching.
//

#define CONTEXT_UNWOUND_TO_CALL 0x20000000

//
// Specify the number of breakpoints and watchpoints that the OS
// will track. Architecturally, ARM supports up to 16. In practice,
// however, almost no one implements more than 4 of each.
//

#define ARM_MAX_BREAKPOINTS     8
#define ARM_MAX_WATCHPOINTS     1

typedef struct _NEON128 {
    ULONGLONG Low;
    LONGLONG High;
} NEON128, *PNEON128;

//
// Context Frame
//
//  This frame has a several purposes: 1) it is used as an argument to
//  NtContinue, 2) it is used to constuct a call frame for APC delivery,
//  and 3) it is used in the user level thread creation routines.
//
//
// The flags field within this record controls the contents of a CONTEXT
// record.
//
// If the context record is used as an input parameter, then for each
// portion of the context record controlled by a flag whose value is
// set, it is assumed that that portion of the context record contains
// valid context. If the context record is being used to modify a threads
// context, then only that portion of the threads context is modified.
//
// If the context record is used as an output parameter to capture the
// context of a thread, then only those portions of the thread's context
// corresponding to set flags will be returned.
//
// CONTEXT_CONTROL specifies Sp, Lr, Pc, and Cpsr
//
// CONTEXT_INTEGER specifies R0-R12
//
// CONTEXT_FLOATING_POINT specifies Q0-Q15 / D0-D31 / S0-S31
//
// CONTEXT_DEBUG_REGISTERS specifies up to 16 of DBGBVR, DBGBCR, DBGWVR,
//      DBGWCR.
//

typedef struct DECLSPEC_ALIGN(8) _CONTEXT {

    //
    // Control flags.
    //

    uint32_t ContextFlags;

    //
    // Integer registers
    //

    uint32_t R0;
    uint32_t R1;
    uint32_t R2;
    uint32_t R3;
    uint32_t R4;
    uint32_t R5;
    uint32_t R6;
    uint32_t R7;
    uint32_t R8;
    uint32_t R9;
    uint32_t R10;
    uint32_t R11;
    uint32_t R12;

    //
    // Control Registers
    //

    uint32_t Sp;
    uint32_t Lr;
    uint32_t Pc;
    uint32_t Cpsr;

    //
    // Floating Point/NEON Registers
    //

    uint32_t Fpscr;
    uint32_t Padding;
    union {
        NEON128 Q[16];
        ULONGLONG D[32];
        uint32_t S[32];
    };

    //
    // Debug registers
    //

    uint32_t Bvr[ARM_MAX_BREAKPOINTS];
    uint32_t Bcr[ARM_MAX_BREAKPOINTS];
    uint32_t Wvr[ARM_MAX_WATCHPOINTS];
    uint32_t Wcr[ARM_MAX_WATCHPOINTS];

    uint32_t Padding2[2];

} CONTEXT, *PCONTEXT, *LPCONTEXT;

//
// Nonvolatile context pointer record.
//

typedef struct _KNONVOLATILE_CONTEXT_POINTERS {

    uint32_t * R4;
    uint32_t * R5;
    uint32_t * R6;
    uint32_t * R7;
    uint32_t * R8;
    uint32_t * R9;
    uint32_t * R10;
    uint32_t * R11;
    uint32_t * Lr;

    PULONGLONG D8;
    PULONGLONG D9;
    PULONGLONG D10;
    PULONGLONG D11;
    PULONGLONG D12;
    PULONGLONG D13;
    PULONGLONG D14;
    PULONGLONG D15;

} KNONVOLATILE_CONTEXT_POINTERS, *PKNONVOLATILE_CONTEXT_POINTERS;

typedef struct _IMAGE_ARM_RUNTIME_FUNCTION_ENTRY {
    uint32_t BeginAddress;
    union {
        uint32_t UnwindData;
        struct {
            uint32_t Flag : 2;
            uint32_t FunctionLength : 11;
            uint32_t Ret : 2;
            uint32_t H : 1;
            uint32_t Reg : 3;
            uint32_t R : 1;
            uint32_t L : 1;
            uint32_t C : 1;
            uint32_t StackAdjust : 10;
        };
    };
} IMAGE_ARM_RUNTIME_FUNCTION_ENTRY, * PIMAGE_ARM_RUNTIME_FUNCTION_ENTRY;

#elif defined(_ARM64_)

#define CONTEXT_ARM64   0x00400000L

#define CONTEXT_CONTROL (CONTEXT_ARM64 | 0x1L)
#define CONTEXT_INTEGER (CONTEXT_ARM64 | 0x2L)
#define CONTEXT_SEGMENTS (CONTEXT_ARM64 | 0x4L)
#define CONTEXT_FLOATING_POINT  (CONTEXT_ARM64 | 0x4L)
#define CONTEXT_DEBUG_REGISTERS (CONTEXT_ARM64 | 0x8L)

#define CONTEXT_FULL (CONTEXT_CONTROL | CONTEXT_INTEGER | CONTEXT_FLOATING_POINT)

#define CONTEXT_ALL (CONTEXT_CONTROL | CONTEXT_INTEGER | CONTEXT_FLOATING_POINT | CONTEXT_DEBUG_REGISTERS)

#define CONTEXT_EXCEPTION_ACTIVE 0x8000000L
#define CONTEXT_SERVICE_ACTIVE 0x10000000L
#define CONTEXT_EXCEPTION_REQUEST 0x40000000L
#define CONTEXT_EXCEPTION_REPORTING 0x80000000L

//
// This flag is set by the unwinder if it has unwound to a call
// site, and cleared whenever it unwinds through a trap frame.
// It is used by language-specific exception handlers to help
// differentiate exception scopes during dispatching.
//

#define CONTEXT_UNWOUND_TO_CALL 0x20000000

//
// Define initial Cpsr/Fpscr value
//

#define INITIAL_CPSR 0x10
#define INITIAL_FPSCR 0

// begin_ntoshvp

//
// Specify the number of breakpoints and watchpoints that the OS
// will track. Architecturally, ARM64 supports up to 16. In practice,
// however, almost no one implements more than 4 of each.
//

#define ARM64_MAX_BREAKPOINTS     8
#define ARM64_MAX_WATCHPOINTS     2

//
// Context Frame
//
//  This frame has a several purposes: 1) it is used as an argument to
//  NtContinue, 2) it is used to constuct a call frame for APC delivery,
//  and 3) it is used in the user level thread creation routines.
//
//
// The flags field within this record controls the contents of a CONTEXT
// record.
//
// If the context record is used as an input parameter, then for each
// portion of the context record controlled by a flag whose value is
// set, it is assumed that that portion of the context record contains
// valid context. If the context record is being used to modify a threads
// context, then only that portion of the threads context is modified.
//
// If the context record is used as an output parameter to capture the
// context of a thread, then only those portions of the thread's context
// corresponding to set flags will be returned.
//
// CONTEXT_CONTROL specifies Sp, Lr, Pc, and Cpsr
//
// CONTEXT_INTEGER specifies R0-R12
//
// CONTEXT_FLOATING_POINT specifies Q0-Q15 / D0-D31 / S0-S31
//
// CONTEXT_DEBUG_REGISTERS specifies up to 16 of DBGBVR, DBGBCR, DBGWVR,
//      DBGWCR.
//

typedef struct _NEON128 {
    ULONGLONG Low;
    LONGLONG High;
} NEON128, *PNEON128;

typedef struct DECLSPEC_ALIGN(16) _CONTEXT {

    //
    // Control flags.
    //

    /* +0x000 */ uint32_t ContextFlags;

    //
    // Integer registers
    //

    /* +0x004 */ uint32_t Cpsr;       // NZVF + DAIF + CurrentEL + SPSel
    /* +0x008 */ union {
                    struct {
                        DWORD64 X0;
                        DWORD64 X1;
                        DWORD64 X2;
                        DWORD64 X3;
                        DWORD64 X4;
                        DWORD64 X5;
                        DWORD64 X6;
                        DWORD64 X7;
                        DWORD64 X8;
                        DWORD64 X9;
                        DWORD64 X10;
                        DWORD64 X11;
                        DWORD64 X12;
                        DWORD64 X13;
                        DWORD64 X14;
                        DWORD64 X15;
                        DWORD64 X16;
                        DWORD64 X17;
                        DWORD64 X18;
                        DWORD64 X19;
                        DWORD64 X20;
                        DWORD64 X21;
                        DWORD64 X22;
                        DWORD64 X23;
                        DWORD64 X24;
                        DWORD64 X25;
                        DWORD64 X26;
                        DWORD64 X27;
                        DWORD64 X28;
                    };
                    DWORD64 X[29];
                };
    /* +0x0f0 */ DWORD64 Fp;
    /* +0x0f8 */ DWORD64 Lr;
    /* +0x100 */ DWORD64 Sp;
    /* +0x108 */ DWORD64 Pc;

    //
    // Floating Point/NEON Registers
    //

    /* +0x110 */ NEON128 V[32];
    /* +0x310 */ uint32_t Fpcr;
    /* +0x314 */ uint32_t Fpsr;

    //
    // Debug registers
    //

    /* +0x318 */ uint32_t Bcr[ARM64_MAX_BREAKPOINTS];
    /* +0x338 */ DWORD64 Bvr[ARM64_MAX_BREAKPOINTS];
    /* +0x378 */ uint32_t Wcr[ARM64_MAX_WATCHPOINTS];
    /* +0x380 */ DWORD64 Wvr[ARM64_MAX_WATCHPOINTS];
    /* +0x390 */

} CONTEXT, *PCONTEXT, *LPCONTEXT;

//
// Nonvolatile context pointer record.
//

typedef struct _KNONVOLATILE_CONTEXT_POINTERS {

    PDWORD64 X19;
    PDWORD64 X20;
    PDWORD64 X21;
    PDWORD64 X22;
    PDWORD64 X23;
    PDWORD64 X24;
    PDWORD64 X25;
    PDWORD64 X26;
    PDWORD64 X27;
    PDWORD64 X28;
    PDWORD64 Fp;
    PDWORD64 Lr;

    PDWORD64 D8;
    PDWORD64 D9;
    PDWORD64 D10;
    PDWORD64 D11;
    PDWORD64 D12;
    PDWORD64 D13;
    PDWORD64 D14;
    PDWORD64 D15;

} KNONVOLATILE_CONTEXT_POINTERS, *PKNONVOLATILE_CONTEXT_POINTERS;

#else
#error Unknown architecture for defining CONTEXT.
#endif


BOOL
GetThreadContext(
          HANDLE hThread,
           LPCONTEXT lpContext);

BOOL
SetThreadContext(
          HANDLE hThread,
          const CONTEXT *lpContext);

#define THREAD_BASE_PRIORITY_LOWRT    15
#define THREAD_BASE_PRIORITY_MAX      2
#define THREAD_BASE_PRIORITY_MIN      (-2)
#define THREAD_BASE_PRIORITY_IDLE     (-15)

#define THREAD_PRIORITY_LOWEST        THREAD_BASE_PRIORITY_MIN
#define THREAD_PRIORITY_BELOW_NORMAL  (THREAD_PRIORITY_LOWEST+1)
#define THREAD_PRIORITY_NORMAL        0
#define THREAD_PRIORITY_HIGHEST       THREAD_BASE_PRIORITY_MAX
#define THREAD_PRIORITY_ABOVE_NORMAL  (THREAD_PRIORITY_HIGHEST-1)
#define THREAD_PRIORITY_ERROR_RETURN  (MAXLONG)

#define THREAD_PRIORITY_TIME_CRITICAL THREAD_BASE_PRIORITY_LOWRT
#define THREAD_PRIORITY_IDLE          THREAD_BASE_PRIORITY_IDLE

int
GetThreadPriority(
           HANDLE hThread);

BOOL
SetThreadPriority(
           HANDLE hThread,
           int nPriority);

BOOL
GetThreadTimes(
         HANDLE hThread,
         LPFILETIME lpCreationTime,
         LPFILETIME lpExitTime,
         LPFILETIME lpKernelTime,
         LPFILETIME lpUserTime);

void *
PAL_GetStackBase();

void *
PAL_GetStackLimit();

uint32_t
PAL_GetLogicalCpuCountFromOS();

size_t
PAL_GetLogicalProcessorCacheSizeFromOS();

typedef BOOL (*ReadMemoryWordCallback)(SIZE_T address, SIZE_T *value);

#if defined(_AMD64_) || defined(_ARM_) || defined(_ARM64_)
BOOL PAL_VirtualUnwind(CONTEXT *context, KNONVOLATILE_CONTEXT_POINTERS *contextPointers);

BOOL PAL_VirtualUnwindOutOfProc(CONTEXT *context,
                                                 KNONVOLATILE_CONTEXT_POINTERS *contextPointers,
                                                 uint32_t pid,
                                                 ReadMemoryWordCallback readMemCallback);
#endif

#define GetLogicalProcessorCacheSizeFromOS PAL_GetLogicalProcessorCacheSizeFromOS

#if defined(__FreeBSD__) && defined(_X86_)
#define PAL_CS_NATIVE_DATA_SIZE 12
#elif defined(__FreeBSD__) && defined(__x86_64__)
#define PAL_CS_NATIVE_DATA_SIZE 24
#elif defined(__sun__)
#define PAL_CS_NATIVE_DATA_SIZE 48
#elif defined(__hpux__) && (defined(__hppa__) || defined (__ia64__))
#define PAL_CS_NATIVE_DATA_SIZE 148
#elif defined(_AIX)
#define PAL_CS_NATIVE_DATA_SIZE 100
#elif defined(__APPLE__) && defined(__i386__)
#define PAL_CS_NATIVE_DATA_SIZE 76
#elif defined(__APPLE__) && defined(__x86_64__)
#define PAL_CS_NATIVE_DATA_SIZE 120
#elif defined(__APPLE__) && defined(_ARM64_)
#define PAL_CS_NATIVE_DATA_SIZE 120
#elif defined(__LINUX__) && defined(__i386__)
#define PAL_CS_NATIVE_DATA_SIZE 56
#elif defined(__LINUX__) && defined(__x86_64__)
#define PAL_CS_NATIVE_DATA_SIZE 96
#elif defined(__ANDROID__) && defined(_ARM_)
#define PAL_CS_NATIVE_DATA_SIZE 12
#elif defined(__LINUX__) && defined(_ARM_)
#define PAL_CS_NATIVE_DATA_SIZE 80
#elif defined(__LINUX__) && defined(_ARM64_)
#define PAL_CS_NATIVE_DATA_SIZE 116
#else
#warning
#error  PAL_CS_NATIVE_DATA_SIZE is not defined for this architecture
#endif

//
typedef struct _CRITICAL_SECTION {
    void * DebugInfo;
    int32_t LockCount;
    int32_t RecursionCount;
    HANDLE OwningThread;
    HANDLE LockSemaphore;
    size_t SpinCount;

    BOOL bInternal;
    volatile uint32_t dwInitState;
    union CSNativeDataStorage
    {
        uint8_t rgNativeDataStorage[PAL_CS_NATIVE_DATA_SIZE];
        void * pvAlign; // make sure the storage is machine-pointer-size aligned
    } csnds;
} CRITICAL_SECTION, *PCRITICAL_SECTION, *LPCRITICAL_SECTION;

void EnterCriticalSection(  LPCRITICAL_SECTION lpCriticalSection);
void LeaveCriticalSection(  LPCRITICAL_SECTION lpCriticalSection);
void InitializeCriticalSection( LPCRITICAL_SECTION lpCriticalSection);
BOOL InitializeCriticalSectionEx(LPCRITICAL_SECTION lpCriticalSection, uint32_t dwSpinCount, uint32_t Flags);
BOOL InitializeCriticalSectionAndSpinCount(LPCRITICAL_SECTION lpCriticalSection, uint32_t dwSpinCount);
void DeleteCriticalSection(  LPCRITICAL_SECTION lpCriticalSection);
BOOL TryEnterCriticalSection(  LPCRITICAL_SECTION lpCriticalSection);

#define SEM_FAILCRITICALERRORS          0x0001
#define SEM_NOOPENFILEERRORBOX          0x8000

uint32_t
SetErrorMode(
          uint32_t uMode);

#define PAGE_NOACCESS                   0x01
#define PAGE_READONLY                   0x02
#define PAGE_READWRITE                  0x04
#define PAGE_WRITECOPY                  0x08
#define PAGE_EXECUTE                    0x10
#define PAGE_EXECUTE_READ               0x20
#define PAGE_EXECUTE_READWRITE          0x40
#define PAGE_EXECUTE_WRITECOPY          0x80
#define MEM_COMMIT                      0x1000
#define MEM_RESERVE                     0x2000
#define MEM_DECOMMIT                    0x4000
#define MEM_RELEASE                     0x8000
#define MEM_RESET                       0x80000
#define MEM_FREE                        0x10000
#define MEM_PRIVATE                     0x20000
#define MEM_MAPPED                      0x40000
#define MEM_TOP_DOWN                    0x100000
#define MEM_WRITE_WATCH                 0x200000
#define MEM_RESERVE_EXECUTABLE          0x40000000 // reserve memory using executable memory allocator

HANDLE
CreateFileMappingA(
            HANDLE hFile,
            LPSECURITY_ATTRIBUTES lpFileMappingAttributes,
            uint32_t flProtect,
            uint32_t dwMaximumSizeHigh,
            uint32_t dwMaximumSizeLow,
            LPCSTR lpName);

HANDLE
CreateFileMappingW(
            HANDLE hFile,
            LPSECURITY_ATTRIBUTES lpFileMappingAttributes,
            uint32_t flProtect,
            uint32_t dwMaxmimumSizeHigh,
            uint32_t dwMaximumSizeLow,
            LPCWSTR lpName);

#ifdef UNICODE
#define CreateFileMapping CreateFileMappingW
#else
#define CreateFileMapping CreateFileMappingA
#endif

#define SECTION_QUERY       0x0001
#define SECTION_MAP_WRITE   0x0002
#define SECTION_MAP_READ    0x0004
#define SECTION_ALL_ACCESS  (SECTION_MAP_READ | SECTION_MAP_WRITE) // diff from winnt.h

#define FILE_MAP_WRITE      SECTION_MAP_WRITE
#define FILE_MAP_READ       SECTION_MAP_READ
#define FILE_MAP_ALL_ACCESS SECTION_ALL_ACCESS
#define FILE_MAP_COPY       SECTION_QUERY

HANDLE
OpenFileMappingA(
          uint32_t dwDesiredAccess,
          BOOL bInheritHandle,
          LPCSTR lpName);

HANDLE
OpenFileMappingW(
          uint32_t dwDesiredAccess,
          BOOL bInheritHandle,
          LPCWSTR lpName);

#ifdef UNICODE
#define OpenFileMapping OpenFileMappingW
#else
#define OpenFileMapping OpenFileMappingA
#endif

void *
MapViewOfFile(
           HANDLE hFileMappingObject,
           uint32_t dwDesiredAccess,
           uint32_t dwFileOffsetHigh,
           uint32_t dwFileOffsetLow,
           SIZE_T dwNumberOfBytesToMap);

void *
MapViewOfFileEx(
           HANDLE hFileMappingObject,
           uint32_t dwDesiredAccess,
           uint32_t dwFileOffsetHigh,
           uint32_t dwFileOffsetLow,
           SIZE_T dwNumberOfBytesToMap,
           void * lpBaseAddress);

BOOL
FlushViewOfFile(
         void * lpBaseAddress,
         SIZE_T dwNumberOfBytesToFlush);

BOOL
UnmapViewOfFile(
         const void * lpBaseAddress);

HMODULE
LoadLibraryA(
         LPCSTR lpLibFileName);

HMODULE
LoadLibraryW(
         LPCWSTR lpLibFileName);

HMODULE
LoadLibraryExA(
         LPCSTR lpLibFileName,
         /*Reserved*/ HANDLE hFile,
         uint32_t dwFlags);

HMODULE
LoadLibraryExW(
         LPCWSTR lpLibFileName,
         /*Reserved*/ HANDLE hFile,
         uint32_t dwFlags);

void *
PAL_LoadLibraryDirect(
         LPCWSTR lpLibFileName);

HMODULE
PAL_RegisterLibraryDirect(
         void *dl_handle,
         LPCWSTR lpLibFileName);

/*++
Function:
  PAL_LOADLoadPEFile

Abstract
  Loads a PE file into memory.  Properly maps all of the sections in the PE file.  Returns a pointer to the
  loaded base.

Parameters:
    IN hFile    - The file to load

Return value:
    A valid base address if successful.
    0 if failure
--*/
void *
PAL_LOADLoadPEFile(HANDLE hFile);

/*++
    PAL_LOADUnloadPEFile

    Unload a PE file that was loaded by PAL_LOADLoadPEFile().

Parameters:
    IN ptr - the file pointer returned by PAL_LOADLoadPEFile()

Return value:
    TRUE - success
    FALSE - failure (incorrect ptr, etc.)
--*/
BOOL
PAL_LOADUnloadPEFile(void * ptr);

#ifdef UNICODE
#define LoadLibrary LoadLibraryW
#define LoadLibraryEx LoadLibraryExW
#else
#define LoadLibrary LoadLibraryA
#define LoadLibraryEx LoadLibraryExA
#endif

typedef INT_PTR (*FARPROC)();

FARPROC
GetProcAddress(
     HMODULE hModule,
     LPCSTR lpProcName);

BOOL
FreeLibrary(
      HMODULE hLibModule);

__attribute__((noreturn))
void
FreeLibraryAndExitThread(
     HMODULE hLibModule,
     uint32_t dwExitCode);

BOOL
DisableThreadLibraryCalls(
     HMODULE hLibModule);

uint32_t
GetModuleFileNameA(
     HMODULE hModule,
     LPSTR lpFileName,
     uint32_t nSize);

uint32_t
GetModuleFileNameW(
     HMODULE hModule,
     LPWSTR lpFileName,
     uint32_t nSize);

#ifdef UNICODE
#define GetModuleFileName GetModuleFileNameW
#else
#define GetModuleFileName GetModuleFileNameA
#endif

uint32_t
GetModuleFileNameExW(
     HANDLE hProcess,
     HMODULE hModule,
     LPWSTR lpFilename,
     uint32_t nSize
    );

#ifdef UNICODE
#define GetModuleFileNameEx GetModuleFileNameExW
#endif

HMODULE
GetModuleHandleW(
      LPCWSTR lpModuleName
);

#ifdef UNICODE
#define GetModuleHandle GetModuleHandleW
#endif

BOOL
GetModuleHandleExW(
     uint32_t dwFlags,
      LPCWSTR lpModuleName,
     HMODULE *phModule);

#ifdef UNICODE
#define GetModuleHandleEx GetModuleHandleExW
#endif

// Get base address of the module containing a given symbol
const void *
PAL_GetSymbolModuleBase(void *symbol);

void *
VirtualAlloc(
          void * lpAddress,
          SIZE_T dwSize,
          uint32_t flAllocationType,
          uint32_t flProtect);

void *
VirtualAllocEx(
          HANDLE hProcess,
          void * lpAddress,
          SIZE_T dwSize,
          uint32_t flAllocationType,
          uint32_t flProtect);

BOOL
VirtualFree(
         void * lpAddress,
         SIZE_T dwSize,
         uint32_t dwFreeType);


BOOL
VirtualFreeEx(
         HANDLE hProcess,
         void * lpAddress,
         SIZE_T dwSize,
         uint32_t dwFreeType);

BOOL
VirtualProtect(
            void * lpAddress,
            SIZE_T dwSize,
            uint32_t flNewProtect,
            uint32_t * lpflOldProtect);

BOOL
VirtualProtectEx(
            HANDLE hProcess,
            void * lpAddress,
            SIZE_T dwSize,
            uint32_t flNewProtect,
            uint32_t * lpflOldProtect);

typedef struct _MEMORYSTATUSEX {
  uint32_t     dwLength;
  uint32_t     dwMemoryLoad;
  DWORDLONG ullTotalPhys;
  DWORDLONG ullAvailPhys;
  DWORDLONG ullTotalPageFile;
  DWORDLONG ullAvailPageFile;
  DWORDLONG ullTotalVirtual;
  DWORDLONG ullAvailVirtual;
  DWORDLONG ullAvailExtendedVirtual;
} MEMORYSTATUSEX, *LPMEMORYSTATUSEX;

BOOL
GlobalMemoryStatusEx(
              LPMEMORYSTATUSEX lpBuffer);

typedef struct _MEMORY_BASIC_INFORMATION {
    void * BaseAddress;
    void * AllocationBase_PAL_Undefined;
    uint32_t AllocationProtect;
    SIZE_T RegionSize;
    uint32_t State;
    uint32_t Protect;
    uint32_t Type;
} MEMORY_BASIC_INFORMATION, *PMEMORY_BASIC_INFORMATION;

SIZE_T
VirtualQuery(
          const void * lpAddress,
          PMEMORY_BASIC_INFORMATION lpBuffer,
          SIZE_T dwLength);

SIZE_T
VirtualQueryEx(
          HANDLE hProcess,
          const void * lpAddress,
          PMEMORY_BASIC_INFORMATION lpBuffer,
          SIZE_T dwLength);

BOOL
ReadProcessMemory(
           HANDLE hProcess,
           const void * lpBaseAddress,
           void * lpBuffer,
           SIZE_T nSize,
           SIZE_T * lpNumberOfBytesRead);

void
RtlMoveMemory(
           void * Destination,
           const void *Source,
           SIZE_T Length);

void
RtlZeroMemory(
     void * Destination,
     SIZE_T Length);

#define MoveMemory memmove
#define CopyMemory memcpy
#define FillMemory(Destination,Length,Fill) memset((Destination),(Fill),(Length))
#define ZeroMemory(Destination,Length) memset((Destination),0,(Length))


HANDLE
GetProcessHeap(
           void);

#define HEAP_ZERO_MEMORY 0x00000008

HANDLE
HeapCreate(
	        uint32_t flOptions,
	        SIZE_T dwInitialSize,
	        SIZE_T dwMaximumSize);

void *
HeapAlloc(
       HANDLE hHeap,
       uint32_t dwFlags,
       SIZE_T dwBytes);

void *
HeapReAlloc(
     HANDLE hHeap,
     uint32_t dwFlags,
     void * lpMem,
     SIZE_T dwBytes
    );

BOOL
HeapFree(
      HANDLE hHeap,
      uint32_t dwFlags,
      void * lpMem);

typedef enum _HEAP_INFORMATION_CLASS {
    HeapCompatibilityInformation,
    HeapEnableTerminationOnCorruption
} HEAP_INFORMATION_CLASS;

BOOL
HeapSetInformation(
          HANDLE HeapHandle,
         HEAP_INFORMATION_CLASS HeapInformationClass,
         void * HeapInformation,
         SIZE_T HeapInformationLength);

#define LMEM_FIXED          0x0000
#define LMEM_MOVEABLE       0x0002
#define LMEM_ZEROINIT       0x0040
#define LPTR                (LMEM_FIXED | LMEM_ZEROINIT)

HLOCAL
LocalAlloc(
        uint32_t uFlags,
        SIZE_T uBytes);

HLOCAL
LocalReAlloc(
        HLOCAL hMem,
        SIZE_T uBytes,
        uint32_t   uFlags);

HLOCAL
LocalFree(
       HLOCAL hMem);

BOOL
FlushInstructionCache(
               HANDLE hProcess,
               const void * lpBaseAddress,
               SIZE_T dwSize);

#if ENABLE_DOWNLEVEL_FOR_NLS

BOOL
GetStringTypeExW(
          LCID Locale,
          uint32_t dwInfoType,
          LPCWSTR lpSrcStr,
          int cchSrc,
          uint16_t * lpCharType);

#ifdef UNICODE
#define GetStringTypeEx GetStringTypeExW
#endif

#endif // ENABLE_DOWNLEVEL_FOR_NLS


#define NORM_IGNORECASE           0x00000001  // ignore case
#define NORM_IGNOREWIDTH          0x00020000  // ignore width

#define NORM_LINGUISTIC_CASING    0x08000000  // use linguistic rules for casing

#ifdef __APPLE__
#define NORM_IGNORENONSPACE       0x00000002  // ignore nonspacing chars
#define NORM_IGNORESYMBOLS        0x00000004  // ignore symbols
#define NORM_IGNOREKANATYPE       0x00010000  // ignore kanatype
#define SORT_STRINGSORT           0x00001000  // use string sort method
#else // LINUX
// Flags with no value on a given platform are given 0 so that program logic can be unaltered (a|0==a)
#define NORM_IGNORENONSPACE       0x00000000  // ignore nonspacing chars
#define NORM_IGNORESYMBOLS        0x00000000  // ignore symbols
#define NORM_IGNOREKANATYPE       0x00000000  // ignore kanatype
#define SORT_STRINGSORT           0x00000000  // use string sort method
#endif // __APPLE__
// __APPLE__ and LINUX
// Flags with no value on a given platform are given 0 so that program logic can be unaltered (a|0==a)
#define LINGUISTIC_IGNOREDIACRITIC 0x00000000  // linguistically appropriate 'ignore case'
#define LINGUISTIC_IGNORECASE     0x00000000  // linguistically appropriate 'ignore nonspace'
#define SORT_DIGITSASNUMBERS      0x00000000  // Sort digits as numbers (ie: 2 comes before 10)


typedef struct nlsversioninfo {
  uint32_t     dwNLSVersionInfoSize;
  uint32_t     dwNLSVersion;
  uint32_t     dwDefinedVersion;
} NLSVERSIONINFO, *LPNLSVERSIONINFO;

#define CSTR_LESS_THAN     1
#define CSTR_EQUAL         2
#define CSTR_GREATER_THAN  3

#if ENABLE_DOWNLEVEL_FOR_NLS


int
CompareStringA(
     LCID     Locale,
     uint32_t    dwCmpFlags,
     LPCSTR   lpString1,
     int      cchCount1,
     LPCSTR   lpString2,
     int      cchCount2);

int
CompareStringW(
     LCID     Locale,
     uint32_t    dwCmpFlags,
     LPCWSTR  lpString1,
     int      cchCount1,
     LPCWSTR  lpString2,
     int      cchCount2);

#endif // ENABLE_DOWNLEVEL_FOR_NLS


int
CompareStringEx(
     LPCWSTR lpLocaleName,
     uint32_t    dwCmpFlags,
     LPCWSTR  lpString1,
     int      cchCount1,
     LPCWSTR  lpString2,
     int      cchCount2,
     LPNLSVERSIONINFO lpVersionInformation,
     void * lpReserved,
     LPARAM lParam);


#ifdef UNICODE
#define CompareString  CompareStringW
#endif

#define MAX_LEADBYTES         12
#define MAX_DEFAULTCHAR       2

uint32_t
GetACP(void);

typedef struct _cpinfo {
    uint32_t MaxCharSize;
    uint8_t DefaultChar[MAX_DEFAULTCHAR];
    uint8_t LeadByte[MAX_LEADBYTES];
} CPINFO, *LPCPINFO;

BOOL
GetCPInfo(
       uint32_t CodePage,
       LPCPINFO lpCPInfo);

BOOL
IsDBCSLeadByteEx(
          uint32_t CodePage,
          uint8_t TestChar);

BOOL
IsDBCSLeadByte(
         uint8_t TestChar);

BOOL
IsValidCodePage(
         uint32_t CodePage);


#define MB_PRECOMPOSED            0x00000001
#define MB_ERR_INVALID_CHARS      0x00000008

int
MultiByteToWideChar(
             uint32_t CodePage,
             uint32_t dwFlags,
             LPCSTR lpMultiByteStr,
             int cbMultiByte,
             LPWSTR lpWideCharStr,
             int cchWideChar);

#define WC_NO_BEST_FIT_CHARS      0x00000400

int
WideCharToMultiByte(
             uint32_t CodePage,
             uint32_t dwFlags,
             LPCWSTR lpWideCharStr,
             int cchWideChar,
             LPSTR lpMultiByteStr,
             int cbMultyByte,
             LPCSTR lpDefaultChar,
             LPBOOL lpUsedDefaultChar);

#if ENABLE_DOWNLEVEL_FOR_NLS

LANGID
GetSystemDefaultLangID(
               void);

LANGID
GetUserDefaultLangID(
             void);

BOOL
SetThreadLocale(
         LCID Locale);

LCID
GetThreadLocale(
        void);

#endif //ENABLE_DOWNLEVEL_FOR_NLS

//
//  Locale Types.
//
//  These types are used for the GetLocaleInfo NLS API routine.
//

#ifdef __APPLE__

//
//  The following LCTypes may be used in combination with any other LCTypes.
//
//    LOCALE_NOUSEROVERRIDE is also used in GetTimeFormat and
//    GetDateFormat.
//
//    LOCALE_RETURN_NUMBER will return the result from GetLocaleInfo as a
//    number instead of a string.  This flag is only valid for the LCTypes
//    beginning with LOCALE_I.
//
#define LOCALE_NOUSEROVERRIDE         0x80000000    /* do not use user overrides */
#define LOCALE_RETURN_NUMBER          0x20000000    /* return number instead of string */
#define LOCALE_RETURN_GENITIVE_NAMES  0x10000000   //Flag to return the Genitive forms of month names

#define LOCALE_SLOCALIZEDDISPLAYNAME  0x00000002   // localized name of locale, eg "German (Germany)" in UI language
#define LOCALE_SENGLISHDISPLAYNAME    0x00000072   // Display name (language + country usually) in English, eg "German (Germany)"
#define LOCALE_SNATIVEDISPLAYNAME     0x00000073   // Display name in native locale language, eg "Deutsch (Deutschland)

#define LOCALE_SLOCALIZEDLANGUAGENAME 0x0000006f   // Language Display Name for a language, eg "German" in UI language
#define LOCALE_SENGLISHLANGUAGENAME   0x00001001   // English name of language, eg "German"
#define LOCALE_SNATIVELANGUAGENAME    0x00000004   // native name of language, eg "Deutsch"

#define LOCALE_SLOCALIZEDCOUNTRYNAME  0x00000006   // localized name of country, eg "Germany" in UI language
#define LOCALE_SENGLISHCOUNTRYNAME    0x00001002   // English name of country, eg "Germany"
#define LOCALE_SNATIVECOUNTRYNAME     0x00000008   // native name of country, eg "Deutschland"

//
//  The following LCTypes are mutually exclusive in that they may NOT
//  be used in combination with each other.
//
#define LOCALE_ILANGUAGE              0x00000001    /* language id */
#define LOCALE_SLANGUAGE              0x00000002    /* localized name of language */
#define LOCALE_SENGLANGUAGE           0x00001001    /* English name of language */
#define LOCALE_SABBREVLANGNAME        0x00000003    /* abbreviated language name */
#define LOCALE_SNATIVELANGNAME        0x00000004    /* native name of language */
#define LOCALE_ICOUNTRY               0x00000005    /* country code */
#define LOCALE_SCOUNTRY               0x00000006    /* localized name of country */

#define LOCALE_SENGCOUNTRY            0x00001002    /* English name of country */
#define LOCALE_SABBREVCTRYNAME        0x00000007    /* abbreviated country name */
#define LOCALE_SNATIVECTRYNAME        0x00000008    /* native name of country */

#define LOCALE_SLIST                  0x0000000C    /* list item separator */
#define LOCALE_IMEASURE               0x0000000D    /* 0 = metric, 1 = US */

#define LOCALE_SDECIMAL               0x0000000E    /* decimal separator */
#define LOCALE_STHOUSAND              0x0000000F    /* thousand separator */
#define LOCALE_SGROUPING              0x00000010    /* digit grouping */
#define LOCALE_IDIGITS                0x00000011    /* number of fractional digits */
#define LOCALE_ILZERO                 0x00000012    /* leading zeros for decimal */
#define LOCALE_INEGNUMBER             0x00001010    /* negative number mode */
#define LOCALE_SNATIVEDIGITS          0x00000013    /* native ascii 0-9 */

#define LOCALE_SCURRENCY              0x00000014    /* local monetary symbol */
#define LOCALE_SINTLSYMBOL            0x00000015    /* intl monetary symbol */
#define LOCALE_SMONDECIMALSEP         0x00000016    /* monetary decimal separator */
#define LOCALE_SMONTHOUSANDSEP        0x00000017    /* monetary thousand separator */
#define LOCALE_SMONGROUPING           0x00000018    /* monetary grouping */
#define LOCALE_ICURRDIGITS            0x00000019    /* # local monetary digits */
#define LOCALE_IINTLCURRDIGITS        0x0000001A    /* # intl monetary digits */
#define LOCALE_ICURRENCY              0x0000001B    /* positive currency mode */
#define LOCALE_INEGCURR               0x0000001C    /* negative currency mode */

#define LOCALE_SSHORTDATE             0x0000001F    /* short date format string */
#define LOCALE_SLONGDATE              0x00000020    /* long date format string */
#define LOCALE_STIMEFORMAT            0x00001003    /* time format string */
#define LOCALE_S1159                  0x00000028    /* AM designator */
#define LOCALE_S2359                  0x00000029    /* PM designator */

#define LOCALE_ICALENDARTYPE          0x00001009    /* type of calendar specifier */
#define LOCALE_IFIRSTDAYOFWEEK        0x0000100C    /* first day of week specifier */
#define LOCALE_IFIRSTWEEKOFYEAR       0x0000100D    /* first week of year specifier */

#define LOCALE_SDAYNAME1              0x0000002A    /* long name for Monday */
#define LOCALE_SDAYNAME2              0x0000002B    /* long name for Tuesday */
#define LOCALE_SDAYNAME3              0x0000002C    /* long name for Wednesday */
#define LOCALE_SDAYNAME4              0x0000002D    /* long name for Thursday */
#define LOCALE_SDAYNAME5              0x0000002E    /* long name for Friday */
#define LOCALE_SDAYNAME6              0x0000002F    /* long name for Saturday */
#define LOCALE_SDAYNAME7              0x00000030    /* long name for Sunday */
#define LOCALE_SABBREVDAYNAME1        0x00000031    /* abbreviated name for Monday */
#define LOCALE_SABBREVDAYNAME2        0x00000032    /* abbreviated name for Tuesday */
#define LOCALE_SABBREVDAYNAME3        0x00000033    /* abbreviated name for Wednesday */
#define LOCALE_SABBREVDAYNAME4        0x00000034    /* abbreviated name for Thursday */
#define LOCALE_SABBREVDAYNAME5        0x00000035    /* abbreviated name for Friday */
#define LOCALE_SABBREVDAYNAME6        0x00000036    /* abbreviated name for Saturday */
#define LOCALE_SABBREVDAYNAME7        0x00000037    /* abbreviated name for Sunday */
#define LOCALE_SMONTHNAME1            0x00000038    /* long name for January */
#define LOCALE_SMONTHNAME2            0x00000039    /* long name for February */
#define LOCALE_SMONTHNAME3            0x0000003A    /* long name for March */
#define LOCALE_SMONTHNAME4            0x0000003B    /* long name for April */
#define LOCALE_SMONTHNAME5            0x0000003C    /* long name for May */
#define LOCALE_SMONTHNAME6            0x0000003D    /* long name for June */
#define LOCALE_SMONTHNAME7            0x0000003E    /* long name for July */
#define LOCALE_SMONTHNAME8            0x0000003F    /* long name for August */
#define LOCALE_SMONTHNAME9            0x00000040    /* long name for September */
#define LOCALE_SMONTHNAME10           0x00000041    /* long name for October */
#define LOCALE_SMONTHNAME11           0x00000042    /* long name for November */
#define LOCALE_SMONTHNAME12           0x00000043    /* long name for December */
#define LOCALE_SMONTHNAME13           0x0000100E    /* long name for 13th month (if exists) */
#define LOCALE_SABBREVMONTHNAME1      0x00000044    /* abbreviated name for January */
#define LOCALE_SABBREVMONTHNAME2      0x00000045    /* abbreviated name for February */
#define LOCALE_SABBREVMONTHNAME3      0x00000046    /* abbreviated name for March */
#define LOCALE_SABBREVMONTHNAME4      0x00000047    /* abbreviated name for April */
#define LOCALE_SABBREVMONTHNAME5      0x00000048    /* abbreviated name for May */
#define LOCALE_SABBREVMONTHNAME6      0x00000049    /* abbreviated name for June */
#define LOCALE_SABBREVMONTHNAME7      0x0000004A    /* abbreviated name for July */
#define LOCALE_SABBREVMONTHNAME8      0x0000004B    /* abbreviated name for August */
#define LOCALE_SABBREVMONTHNAME9      0x0000004C    /* abbreviated name for September */
#define LOCALE_SABBREVMONTHNAME10     0x0000004D    /* abbreviated name for October */
#define LOCALE_SABBREVMONTHNAME11     0x0000004E    /* abbreviated name for November */
#define LOCALE_SABBREVMONTHNAME12     0x0000004F    /* abbreviated name for December */
#define LOCALE_SABBREVMONTHNAME13     0x0000100F    /* abbreviated name for 13th month (if exists) */

#define LOCALE_SPOSITIVESIGN          0x00000050    /* positive sign */
#define LOCALE_SNEGATIVESIGN          0x00000051    /* negative sign */

#define LOCALE_FONTSIGNATURE          0x00000058    /* font signature */
#define LOCALE_SISO639LANGNAME        0x00000059    /* ISO abbreviated language name */
#define LOCALE_SISO3166CTRYNAME       0x0000005A    /* ISO abbreviated country name */

#define LOCALE_SENGCURRNAME           0x00001007    /* english name of currency */
#define LOCALE_SNATIVECURRNAME        0x00001008    /* native name of currency */
#define LOCALE_SYEARMONTH             0x00001006    /* year month format string */
#define LOCALE_IDIGITSUBSTITUTION     0x00001014    /* 0 = context, 1 = none, 2 = national */

#define LOCALE_SNAME                  0x0000005C    /* locale name <language>[-<Script>][-<REGION>[_<sort order>]] */
#define LOCALE_SDURATION              0x0000005d    /* time duration format */
#define LOCALE_SKEYBOARDSTOINSTALL    0x0000005e    /* (windows only) keyboards to install */
#define LOCALE_SSHORTESTDAYNAME1      0x00000060    /* Shortest day name for Monday */
#define LOCALE_SSHORTESTDAYNAME2      0x00000061    /* Shortest day name for Tuesday */
#define LOCALE_SSHORTESTDAYNAME3      0x00000062    /* Shortest day name for Wednesday */
#define LOCALE_SSHORTESTDAYNAME4      0x00000063    /* Shortest day name for Thursday */
#define LOCALE_SSHORTESTDAYNAME5      0x00000064    /* Shortest day name for Friday */
#define LOCALE_SSHORTESTDAYNAME6      0x00000065    /* Shortest day name for Saturday */
#define LOCALE_SSHORTESTDAYNAME7      0x00000066    /* Shortest day name for Sunday */
#define LOCALE_SISO639LANGNAME2       0x00000067    /* 3 character ISO abbreviated language name */
#define LOCALE_SISO3166CTRYNAME2      0x00000068    /* 3 character ISO country name */
#define LOCALE_SNAN                   0x00000069    /* Not a Number */
#define LOCALE_SPOSINFINITY           0x0000006a    /* + Infinity */
#define LOCALE_SNEGINFINITY           0x0000006b    /* - Infinity */
#define LOCALE_SSCRIPTS               0x0000006c    /* Typical scripts in the locale */
#define LOCALE_SPARENT                0x0000006d    /* Fallback name for resources */
#define LOCALE_SCONSOLEFALLBACKNAME   0x0000006e    /* Fallback name for within the console */
#define LOCALE_SLANGDISPLAYNAME       0x0000006f    /* Language Display Name for a language */
#define LOCALE_IREADINGLAYOUT         0x00000070   // Returns one of the following 4 reading layout values:
                                                   // 0 - Left to right (eg en-US)
                                                   // 1 - Right to left (eg arabic locales)
                                                   // 2 - Vertical top to bottom with columns to the left and also left to right (ja-JP locales)
                                                   // 3 - Vertical top to bottom with columns proceeding to the right
#define LOCALE_INEUTRAL               0x00000071   // Returns 0 for specific cultures, 1 for neutral cultures.
#define LOCALE_INEGATIVEPERCENT       0x00000074   // Returns 0-11 for the negative percent format
#define LOCALE_IPOSITIVEPERCENT       0x00000075   // Returns 0-3 for the positive percent formatIPOSITIVEPERCENT
#define LOCALE_SPERCENT               0x00000076   // Returns the percent symbol
#define LOCALE_SPERMILLE              0x00000077   // Returns the permille (U+2030) symbol
#define LOCALE_SMONTHDAY              0x00000078   // Returns the preferred month/day format
#define LOCALE_SSHORTTIME             0x00000079   // Returns the preferred short time format (ie: no seconds, just h:mm)
#define LOCALE_SOPENTYPELANGUAGETAG   0x0000007a   // Open type language tag, eg: "latn" or "dflt"
#define LOCALE_SSORTLOCALE            0x0000007b   // Name of locale to use for sorting/collation/casing behavior.

#define LCMAP_LINGUISTIC_CASING       0x01000000    /* Use linguistic casing */

#define CAL_RETURN_GENITIVE_NAMES       LOCALE_RETURN_GENITIVE_NAMES  // return genitive forms of month names

#define CAL_SSHORTESTDAYNAME1         0x00000031
#define CAL_SSHORTESTDAYNAME2         0x00000032
#define CAL_SSHORTESTDAYNAME3         0x00000033
#define CAL_SSHORTESTDAYNAME4         0x00000034
#define CAL_SSHORTESTDAYNAME5         0x00000035
#define CAL_SSHORTESTDAYNAME6         0x00000036
#define CAL_SSHORTESTDAYNAME7         0x00000037

#define CAL_SMONTHDAY                   0x00000038  // Month/day pattern (reserve for potential inclusion in a future version)
#define CAL_SERASTRING                  0x00000004  // era name for IYearOffsetRanges, eg A.D.
#define CAL_SABBREVERASTRING            0x00000039  // Abbreviated era string (eg: AD)

#define CAL_SSHORTDATE            0x00000005  /* short date format string */
#define CAL_SLONGDATE             0x00000006  /* long date format string */
#define CAL_SDAYNAME1             0x00000007  /* native name for Monday */
#define CAL_SDAYNAME2             0x00000008  /* native name for Tuesday */
#define CAL_SDAYNAME3             0x00000009  /* native name for Wednesday */
#define CAL_SDAYNAME4             0x0000000a  /* native name for Thursday */
#define CAL_SDAYNAME5             0x0000000b  /* native name for Friday */
#define CAL_SDAYNAME6             0x0000000c  /* native name for Saturday */
#define CAL_SDAYNAME7             0x0000000d  /* native name for Sunday */
#define CAL_SABBREVDAYNAME1       0x0000000e  /* abbreviated name for Monday */
#define CAL_SABBREVDAYNAME2       0x0000000f  /* abbreviated name for Tuesday */
#define CAL_SABBREVDAYNAME3       0x00000010  /* abbreviated name for Wednesday */
#define CAL_SABBREVDAYNAME4       0x00000011  /* abbreviated name for Thursday */
#define CAL_SABBREVDAYNAME5       0x00000012  /* abbreviated name for Friday */
#define CAL_SABBREVDAYNAME6       0x00000013  /* abbreviated name for Saturday */
#define CAL_SABBREVDAYNAME7       0x00000014  /* abbreviated name for Sunday */
#define CAL_SMONTHNAME1           0x00000015  /* native name for January */
#define CAL_SMONTHNAME2           0x00000016  /* native name for February */
#define CAL_SMONTHNAME3           0x00000017  /* native name for March */
#define CAL_SMONTHNAME4           0x00000018  /* native name for April */
#define CAL_SMONTHNAME5           0x00000019  /* native name for May */
#define CAL_SMONTHNAME6           0x0000001a  /* native name for June */
#define CAL_SMONTHNAME7           0x0000001b  /* native name for July */
#define CAL_SMONTHNAME8           0x0000001c  /* native name for August */
#define CAL_SMONTHNAME9           0x0000001d  /* native name for September */
#define CAL_SMONTHNAME10          0x0000001e  /* native name for October */
#define CAL_SMONTHNAME11          0x0000001f  /* native name for November */
#define CAL_SMONTHNAME12          0x00000020  /* native name for December */
#define CAL_SMONTHNAME13          0x00000021  /* native name for 13th month (if any) */
#define CAL_SABBREVMONTHNAME1     0x00000022  /* abbreviated name for January */
#define CAL_SABBREVMONTHNAME2     0x00000023  /* abbreviated name for February */
#define CAL_SABBREVMONTHNAME3     0x00000024  /* abbreviated name for March */
#define CAL_SABBREVMONTHNAME4     0x00000025  /* abbreviated name for April */
#define CAL_SABBREVMONTHNAME5     0x00000026  /* abbreviated name for May */
#define CAL_SABBREVMONTHNAME6     0x00000027  /* abbreviated name for June */
#define CAL_SABBREVMONTHNAME7     0x00000028  /* abbreviated name for July */
#define CAL_SABBREVMONTHNAME8     0x00000029  /* abbreviated name for August */
#define CAL_SABBREVMONTHNAME9     0x0000002a  /* abbreviated name for September */
#define CAL_SABBREVMONTHNAME10    0x0000002b  /* abbreviated name for October */
#define CAL_SABBREVMONTHNAME11    0x0000002c  /* abbreviated name for November */
#define CAL_SABBREVMONTHNAME12    0x0000002d  /* abbreviated name for December */
#define CAL_SABBREVMONTHNAME13    0x0000002e  /* abbreviated name for 13th month (if any) */
#define CAL_SYEARMONTH            0x0000002f  /* year month format string */


#else // __APPLE__

#define LOCALE_SDECIMAL               0x0000000E    /* decimal separator */
#define LOCALE_STHOUSAND              0x0000000F    /* thousand separator */
#define LOCALE_ILZERO                 0x00000012    /* leading zeros for decimal */
#define LOCALE_SCURRENCY              0x00000014    /* local monetary symbol */
#define LOCALE_SMONDECIMALSEP         0x00000016    /* monetary decimal separator */
#define LOCALE_SMONTHOUSANDSEP        0x00000017    /* monetary thousand separator */

#endif // __APPLE__


#if ENABLE_DOWNLEVEL_FOR_NLS

int
GetLocaleInfoW(
     LCID     Locale,
     LCTYPE   LCType,
     LPWSTR  lpLCData,
     int      cchData);

#endif // ENABLE_DOWNLEVEL_FOR_NLS

int
GetLocaleInfoEx(
     LPCWSTR  lpLocaleName,
     LCTYPE   LCType,
     LPWSTR  lpLCData,
     int      cchData);


int
CompareStringOrdinal(
     LPCWSTR lpString1,
	 int cchCount1,
	 LPCWSTR lpString2,
	 int cchCount2,
	 BOOL bIgnoreCase);

typedef struct _nlsversioninfoex {
  uint32_t  dwNLSVersionInfoSize;
  uint32_t  dwNLSVersion;
  uint32_t  dwDefinedVersion;
  uint32_t  dwEffectiveId;
  GUID  guidCustomVersion;
  } NLSVERSIONINFOEX, *LPNLSVERSIONINFOEX;

int
FindNLSStringEx(
     LPCWSTR lpLocaleName,
	 uint32_t dwFindNLSStringFlags,
	 LPCWSTR lpStringSource,
	 int cchSource,
     LPCWSTR lpStringValue,
	 int cchValue,
	 int32_t * pcchFound,
	 LPNLSVERSIONINFOEX lpVersionInformation,
	 void * lpReserved,
	 LPARAM lParam );

typedef enum {
    COMPARE_STRING = 0x0001,
} NLS_FUNCTION;

BOOL
IsNLSDefinedString(
     NLS_FUNCTION Function,
	 uint32_t dwFlags,
	 LPNLSVERSIONINFOEX lpVersionInfo,
	 LPCWSTR lpString,
	 int cchStr );


int
ResolveLocaleName(
     LPCWSTR lpNameToResolve,
         LPWSTR lpLocaleName,
         int cchLocaleName );

BOOL
GetThreadPreferredUILanguages(
     uint32_t  dwFlags,
     uint32_t *  pulNumLanguages,
     PWSTR  pwszLanguagesBuffer,
      uint32_t *  pcchLanguagesBuffer);


int
GetSystemDefaultLocaleName(
     LPWSTR lpLocaleName,
	 int cchLocaleName);

#ifdef UNICODE
#define GetLocaleInfo GetLocaleInfoW
#endif

#if ENABLE_DOWNLEVEL_FOR_NLS
LCID
GetUserDefaultLCID(
           void);
#endif


int
GetUserDefaultLocaleName(
            LPWSTR lpLocaleName,
            int cchLocaleName);


#define LCID_INSTALLED            0x00000001  // installed locale ids
#define LCID_SUPPORTED            0x00000002  // supported locale ids
#ifdef __APPLE__
#define LCID_ALTERNATE_SORTS      0x00000004  // alternate sort locale ids
#endif // __APPLE__

#if ENABLE_DOWNLEVEL_FOR_NLS
BOOL
IsValidLocale(
           LCID Locale,
           uint32_t dwFlags);
#endif // ENABLE_DOWNLEVEL_FOR_NLS


typedef uint32_t CALID;
typedef uint32_t CALTYPE;

#define CAL_ITWODIGITYEARMAX 0x00000030 // two digit year max
#define CAL_RETURN_NUMBER    0x20000000 // return number instead of string

#define CAL_GREGORIAN                 1 // Gregorian (localized) calendar
#define CAL_GREGORIAN_US              2 // Gregorian (U.S.) calendar
#define CAL_JAPAN                     3 // Japanese Emperor Era calendar
#define CAL_TAIWAN                    4 // Taiwan Era calendar
#define CAL_KOREA                     5 // Korean Tangun Era calendar
#define CAL_HIJRI                     6 // Hijri (Arabic Lunar) calendar
#define CAL_THAI                      7 // Thai calendar
#define CAL_HEBREW                    8 // Hebrew (Lunar) calendar
#define CAL_GREGORIAN_ME_FRENCH       9 // Gregorian Middle East French calendar
#define CAL_GREGORIAN_ARABIC         10 // Gregorian Arabic calendar
#define CAL_GREGORIAN_XLIT_ENGLISH   11 // Gregorian Transliterated English calendar
#define CAL_GREGORIAN_XLIT_FRENCH    12 // Gregorian Transliterated French calendar
#define CAL_JULIAN                   13

#if ENABLE_DOWNLEVEL_FOR_NLS
int
GetCalendarInfoW(
          LCID Locale,
          CALID Calendar,
          CALTYPE CalType,
          LPWSTR lpCalData,
          int cchData,
          uint32_t * lpValue);

#ifdef UNICODE
#define GetCalendarInfo GetCalendarInfoW
#endif

#endif // ENABLE_DOWNLEVEL_FOR_NLS


int
GetCalendarInfoEx(
          LPCWSTR lpLocaleName,
          CALID Calendar,
          LPCWSTR lpReserved,
          CALTYPE CalType,
          LPWSTR lpCalData,
          int cchData,
          uint32_t * lpValue);

#if ENABLE_DOWNLEVEL_FOR_NLS
typedef BOOL (CALLBACK* LOCALE_ENUMPROCW)(LPWSTR);

BOOL
EnumSystemLocalesW(
     LOCALE_ENUMPROCW lpLocaleEnumProc,
     uint32_t            dwFlags);
#endif //  ENABLE_DOWNLEVEL_FOR_NLS

#define DATE_SHORTDATE            0x00000001  // use short date picture
#define DATE_LONGDATE             0x00000002  // use long date picture
#define DATE_YEARMONTH            0x00000008  // use year month picture

typedef BOOL (CALLBACK* DATEFMT_ENUMPROCEXW)(LPWSTR, CALID);

#if ENABLE_DOWNLEVEL_FOR_NLS

BOOL
EnumDateFormatsExW(
     DATEFMT_ENUMPROCEXW lpDateFmtEnumProcEx,
     LCID                Locale,
     uint32_t               dwFlags);

#else // ENABLE_DOWNLEVEL_FOR_NLS

typedef BOOL (CALLBACK* DATEFMT_ENUMPROCEXEXW)(LPWSTR, CALID, LPARAM);

BOOL
EnumDateFormatsExEx(
     DATEFMT_ENUMPROCEXEXW lpDateFmtEnumProcEx,
     LPCWSTR          lpLocaleName,
     uint32_t               dwFlags,
     LPARAM      lParam);

#endif // ENABLE_DOWNLEVEL_FOR_NLS

typedef BOOL (CALLBACK* TIMEFMT_ENUMPROCW)(LPWSTR);

#if ENABLE_DOWNLEVEL_FOR_NLS

BOOL
EnumTimeFormatsW(
     TIMEFMT_ENUMPROCW lpTimeFmtEnumProc,
     LCID              Locale,
     uint32_t             dwFlags);

#else // ENABLE_DOWNLEVEL_FOR_NLS

typedef BOOL (CALLBACK* TIMEFMT_ENUMPROCEXW)(LPWSTR, LPARAM);

BOOL
EnumTimeFormatsEx(
     TIMEFMT_ENUMPROCEXW lpTimeFmtEnumProc,
     LPCWSTR          lpLocaleName,
     uint32_t             dwFlags,
     LPARAM    lParam);

#endif // ENABLE_DOWNLEVEL_FOR_NLS

#define ENUM_ALL_CALENDARS        0xffffffff  // enumerate all calendars
#define CAL_ICALINTVALUE          0x00000001  // calendar type
#define CAL_NOUSEROVERRIDE        LOCALE_NOUSEROVERRIDE  // do not use user overrides
#define CAL_SCALNAME              0x00000002  // native name of calendar

typedef BOOL (CALLBACK* CALINFO_ENUMPROCEXW)(LPWSTR,CALID);

#if ENABLE_DOWNLEVEL_FOR_NLS

BOOL
EnumCalendarInfoExW(
     CALINFO_ENUMPROCEXW lpCalInfoEnumProc,
     LCID              Locale,
     CALID             Calendar,
     CALTYPE           CalType);

#else // ENABLE_DOWNLEVEL_FOR_NLS

typedef BOOL (CALLBACK* CALINFO_ENUMPROCEXEXW)(LPWSTR, CALID, LPWSTR, LPARAM);

BOOL
EnumCalendarInfoExEx(
     CALINFO_ENUMPROCEXEXW lpCalInfoEnumProc,
     LPCWSTR          lpLocaleName,
     CALID             Calendar,
     LPCWSTR           lpReserved,
     CALTYPE           CalType,
     LPARAM        lParam);

#endif // ENABLE_DOWNLEVEL_FOR_NLS

#define LCMAP_LOWERCASE  0x00000100
#define LCMAP_UPPERCASE  0x00000200

#if ENABLE_DOWNLEVEL_FOR_NLS

int
LCMapStringW(
     LCID    Locale,
     uint32_t   dwMapFlags,
     LPCWSTR lpSrcStr,
     int     cchSrc,
     LPWSTR lpDestStr,
     int     cchDest);

#ifdef UNICODE
#define LCMapString LCMapStringW
#endif


#endif // ENABLE_DOWNLEVEL_FOR_NLS


int
LCMapStringEx(
     LPCWSTR    lpLocaleName,
     uint32_t   dwMapFlags,
     LPCWSTR lpSrcStr,
     int     cchSrc,
     LPWSTR lpDestStr,
     int     cchDest,
     LPNLSVERSIONINFO lpVersionInformation,
     void * lpReserved,
     LPARAM lParam );

int
PAL_LCMapCharW(
     LPCWSTR    lpLocaleName,
     uint32_t   dwMapFlags,
     WCHAR   srcChar,
     WCHAR  *destChar,
    LPNLSVERSIONINFO lpVersionInformation,
    void * lpReserved,
    LPARAM lParam );

int
PAL_NormalizeStringExW(
     LPCWSTR    lpLocaleName,
     uint32_t   dwMapFlags,
     LPCWSTR lpSrcStr,
     int     cchSrc,
     LPWSTR lpDestStr,
     int     cchDest);

int
PAL_ParseDateW(
     LPCWSTR   lpLocaleName,
     LPCWSTR   lpFormat,
     LPCWSTR   lpString,
     LPSYSTEMTIME lpTime);

int
PAL_GetCalendar(
     LPCWSTR   lpLocaleName,
     CALID*   pCalendar);

#define GEOID_NOT_AVAILABLE -1

// "a number", might represent different types
typedef struct PALNUMBER__* PALNUMBER;

// return NULL on OOM
PALNUMBER PAL_DoubleToNumber(double);
PALNUMBER PAL_Int64ToNumber(int64_t);
PALNUMBER PAL_UInt64ToNumber(UINT64);
PALNUMBER PAL_IntToNumber(int);
PALNUMBER PAL_UIntToNumber(unsigned int);

void PAL_ReleaseNumber(PALNUMBER);


// return string length if Buffer is NULL or the result fits in cchBuffer, otherwise -1
int PAL_FormatScientific(LPCWSTR sLocale, LPWSTR pBuffer, SIZE_T cchBuffer, PALNUMBER number, int nMinDigits, int nMaxDigits,
                                                                      LPCWSTR sExponent, LPCWSTR sNumberDecimal, LPCWSTR sPositive, LPCWSTR sNegative, LPCWSTR sZero);

int PAL_FormatCurrency(LPCWSTR sLocale, LPWSTR pBuffer, SIZE_T cchBuffer, PALNUMBER number, int nMinDigits, int nMaxDigits, int iNegativeFormat, int iPositiveFormat,
                      int iPrimaryGroup, int iSecondaryGroup, LPCWSTR sCurrencyDecimal, LPCWSTR sCurrencyGroup, LPCWSTR sNegative, LPCWSTR sCurrency, LPCWSTR sZero);

int PAL_FormatPercent(LPCWSTR sLocale, LPWSTR pBuffer, SIZE_T cchBuffer, PALNUMBER number,  int nMinDigits, int nMaxDigits,int iNegativeFormat, int iPositiveFormat,
                      int iPrimaryGroup, int iSecondaryGroup, LPCWSTR sPercentDecimal, LPCWSTR sPercentGroup, LPCWSTR sNegative, LPCWSTR sPercent, LPCWSTR sZero);

int PAL_FormatDecimal(LPCWSTR sLocale, LPWSTR pBuffer, SIZE_T cchBuffer, PALNUMBER number, int nMinDigits, int nMaxDigits, int iNegativeFormat,
                                    int iPrimaryGroup, int iSecondaryGroup,  LPCWSTR sDecimal, LPCWSTR sGroup, LPCWSTR sNegative, LPCWSTR sZero);


#define DATE_USE_ALT_CALENDAR 0x00000004

#if ENABLE_DOWNLEVEL_FOR_NLS

int
GetDateFormatW(
            LCID Locale,
            uint32_t dwFlags,
            const SYSTEMTIME *lpDate,
            LPCWSTR lpFormat,
            LPWSTR lpDateStr,
            int cchDate);

#else

int
GetDateFormatEx(
            LPCWSTR Locale,
            uint32_t dwFlags,
            const SYSTEMTIME *lpDate,
            LPCWSTR lpFormat,
            LPWSTR lpDateStr,
            int cchDate,
            LPCWSTR lpCalendar);


#endif // ENABLE_DOWNLEVEL_FOR_NLS

int
GetDateFormatEx(
            LPCWSTR lpLocaleName,
            uint32_t dwFlags,
            const SYSTEMTIME *lpDate,
            LPCWSTR lpFormat,
            LPWSTR lpDateStr,
            int cchDate,
           LPCWSTR lpCalendar);


#ifdef UNICODE
#define GetDateFormat GetDateFormatW
#endif


int
PAL_GetResourceString(
         LPCSTR lpDomain,
         LPCSTR lpResourceStr,
         LPWSTR lpWideCharStr,
         int cchWideChar);

BOOL
PAL_BindResources( LPCSTR lpDomain);

#define EXCEPTION_NONCONTINUABLE 0x1
#define EXCEPTION_UNWINDING 0x2

#ifdef FEATURE_PAL_SXS

#define EXCEPTION_EXIT_UNWIND 0x4       // Exit unwind is in progress (not used by PAL SEH)
#define EXCEPTION_NESTED_CALL 0x10      // Nested exception handler call
#define EXCEPTION_TARGET_UNWIND 0x20    // Target unwind in progress
#define EXCEPTION_COLLIDED_UNWIND 0x40  // Collided exception handler call
#define EXCEPTION_SKIP_VEH 0x200

#define EXCEPTION_UNWIND (EXCEPTION_UNWINDING | EXCEPTION_EXIT_UNWIND | \
                          EXCEPTION_TARGET_UNWIND | EXCEPTION_COLLIDED_UNWIND)

#define IS_DISPATCHING(Flag) ((Flag & EXCEPTION_UNWIND) == 0)
#define IS_UNWINDING(Flag) ((Flag & EXCEPTION_UNWIND) != 0)
#define IS_TARGET_UNWIND(Flag) (Flag & EXCEPTION_TARGET_UNWIND)

#endif // FEATURE_PAL_SXS

#define EXCEPTION_IS_SIGNAL 0x100

#define EXCEPTION_MAXIMUM_PARAMETERS 15

// Index in the ExceptionInformation array where we will keep the reference
// to the native exception that needs to be deleted when dispatching
// exception in managed code.
#define NATIVE_EXCEPTION_ASYNC_SLOT (EXCEPTION_MAXIMUM_PARAMETERS-1)

typedef struct _EXCEPTION_RECORD {
    uint32_t ExceptionCode;
    uint32_t ExceptionFlags;
    struct _EXCEPTION_RECORD *ExceptionRecord;
    void * ExceptionAddress;
    uint32_t NumberParameters;
    size_t ExceptionInformation[EXCEPTION_MAXIMUM_PARAMETERS];
} EXCEPTION_RECORD, *PEXCEPTION_RECORD;

typedef struct _EXCEPTION_POINTERS {
    PEXCEPTION_RECORD ExceptionRecord;
    PCONTEXT ContextRecord;
} EXCEPTION_POINTERS, *PEXCEPTION_POINTERS, *LPEXCEPTION_POINTERS;

#ifdef FEATURE_PAL_SXS

typedef int32_t EXCEPTION_DISPOSITION;

enum {
    ExceptionContinueExecution,
    ExceptionContinueSearch,
    ExceptionNestedException,
    ExceptionCollidedUnwind,
};

#endif // FEATURE_PAL_SXS

//
// A function table entry is generated for each frame function.
//
#ifdef _M_ARM64
typedef struct _RUNTIME_FUNCTION {
    uint32_t BeginAddress;
    union {
        uint32_t UnwindData;
        struct {
            uint32_t Flag           : 2;
            uint32_t FunctionLength : 11;
            uint32_t RegF           : 3;
            uint32_t RegI           : 4;
            uint32_t H              : 1;
            uint32_t CR             : 2;
            uint32_t FrameSize      : 9;
        };
    };
} RUNTIME_FUNCTION, *PRUNTIME_FUNCTION;
#else
typedef struct _RUNTIME_FUNCTION {
    uint32_t BeginAddress;
    uint32_t EndAddress;
    uint32_t UnwindData;
} RUNTIME_FUNCTION, *PRUNTIME_FUNCTION;
#endif

BOOL
WriteProcessMemory( HANDLE hProcess,
                    void * lpBaseAddress,
                    const void * lpBuffer,
                    SIZE_T nSize,
                    SIZE_T * lpNumberOfBytesWritten);

#define STANDARD_RIGHTS_REQUIRED  (0x000F0000L)
#define SYNCHRONIZE               (0x00100000L)
#define READ_CONTROL              (0x00020000L)

#define EVENT_MODIFY_STATE        (0x0002)
#define EVENT_ALL_ACCESS          (STANDARD_RIGHTS_REQUIRED | SYNCHRONIZE | \
                                   0x3)

#define MUTANT_QUERY_STATE        (0x0001)
#define MUTANT_ALL_ACCESS         (STANDARD_RIGHTS_REQUIRED | SYNCHRONIZE | \
                                   MUTANT_QUERY_STATE)
#define MUTEX_ALL_ACCESS          MUTANT_ALL_ACCESS

#define SEMAPHORE_MODIFY_STATE    (0x0002)
#define SEMAPHORE_ALL_ACCESS      (STANDARD_RIGHTS_REQUIRED | SYNCHRONIZE | \
                                   0x3)

#define PROCESS_TERMINATE         (0x0001)
#define PROCESS_CREATE_THREAD     (0x0002)
#define PROCESS_SET_SESSIONID     (0x0004)
#define PROCESS_VM_OPERATION      (0x0008)
#define PROCESS_VM_READ           (0x0010)
#define PROCESS_VM_WRITE          (0x0020)
#define PROCESS_DUP_HANDLE        (0x0040)
#define PROCESS_CREATE_PROCESS    (0x0080)
#define PROCESS_SET_QUOTA         (0x0100)
#define PROCESS_SET_INFORMATION   (0x0200)
#define PROCESS_QUERY_INFORMATION (0x0400)
#define PROCESS_SUSPEND_RESUME    (0x0800)
#define PROCESS_ALL_ACCESS        (STANDARD_RIGHTS_REQUIRED | SYNCHRONIZE | \
                                   0xFFF)

HANDLE
OpenProcess(
     uint32_t dwDesiredAccess, /* PROCESS_DUP_HANDLE or PROCESS_ALL_ACCESS */
     BOOL bInheritHandle,
     uint32_t dwProcessId
    );

BOOL
EnumProcessModules(
     HANDLE hProcess,
     HMODULE *lphModule,
     uint32_t cb,
     uint32_t * lpcbNeeded
    );

void
OutputDebugStringA(
     LPCSTR lpOutputString);

void
OutputDebugStringW(
     LPCWSTR lpOutputStrig);

#ifdef UNICODE
#define OutputDebugString OutputDebugStringW
#else
#define OutputDebugString OutputDebugStringA
#endif

void
DebugBreak(
       void);

LPWSTR
lstrcatW(
       LPWSTR lpString1,
      LPCWSTR lpString2);

#ifdef UNICODE
#define lstrcat lstrcatW
#endif

LPWSTR
lstrcpyW(
      LPWSTR lpString1,
      LPCWSTR lpString2);

#ifdef UNICODE
#define lstrcpy lstrcpyW
#endif

int
lstrlenA(
      LPCSTR lpString);

int
lstrlenW(
      LPCWSTR lpString);

#ifdef UNICODE
#define lstrlen lstrlenW
#else
#define lstrlen lstrlenA
#endif

LPWSTR
lstrcpynW(
       LPWSTR lpString1,
       LPCWSTR lpString2,
       int iMaxLength);

#ifdef UNICODE
#define lstrcpyn lstrcpynW
#endif


uint32_t
GetEnvironmentVariableA(
             LPCSTR lpName,
             LPSTR lpBuffer,
             uint32_t nSize);

uint32_t
GetEnvironmentVariableW(
             LPCWSTR lpName,
             LPWSTR lpBuffer,
             uint32_t nSize);

#ifdef UNICODE
#define GetEnvironmentVariable GetEnvironmentVariableW
#else
#define GetEnvironmentVariable GetEnvironmentVariableA
#endif

BOOL
SetEnvironmentVariableA(
             LPCSTR lpName,
             LPCSTR lpValue);

BOOL
SetEnvironmentVariableW(
             LPCWSTR lpName,
             LPCWSTR lpValue);

#ifdef UNICODE
#define SetEnvironmentVariable SetEnvironmentVariableW
#else
#define SetEnvironmentVariable SetEnvironmentVariableA
#endif

LPSTR
GetEnvironmentStringsA(
               void);

LPWSTR
GetEnvironmentStringsW(
               void);

#ifdef UNICODE
#define GetEnvironmentStrings GetEnvironmentStringsW
#else
#define GetEnvironmentStrings GetEnvironmentStringsA
#endif

BOOL
FreeEnvironmentStringsA(
             LPSTR);

BOOL
FreeEnvironmentStringsW(
             LPWSTR);

#ifdef UNICODE
#define FreeEnvironmentStrings FreeEnvironmentStringsW
#else
#define FreeEnvironmentStrings FreeEnvironmentStringsA
#endif

BOOL
CloseHandle(
          HANDLE hObject);

void
RaiseException(
            uint32_t dwExceptionCode,
            uint32_t dwExceptionFlags,
            uint32_t nNumberOfArguments,
            const size_t *lpArguments);

#ifdef FEATURE_PAL_SXS
__attribute__((noreturn))
void
PAL_RaiseException(
            PEXCEPTION_POINTERS ExceptionPointers);
#endif // FEATURE_PAL_SXS

uint32_t
GetTickCount(
         void);

ULONGLONG
GetTickCount64();

BOOL
QueryPerformanceCounter(
     LARGE_INTEGER *lpPerformanceCount
    );

BOOL
QueryPerformanceFrequency(
     LARGE_INTEGER *lpFrequency
    );

BOOL
QueryThreadCycleTime(
     HANDLE ThreadHandle,
     PULONG64 CycleTime);

#ifndef FEATURE_PAL_SXS

typedef int32_t (*PTOP_LEVEL_EXCEPTION_FILTER)(
                           struct _EXCEPTION_POINTERS *ExceptionInfo);
typedef PTOP_LEVEL_EXCEPTION_FILTER LPTOP_LEVEL_EXCEPTION_FILTER;

LPTOP_LEVEL_EXCEPTION_FILTER
SetUnhandledExceptionFilter(
                 LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter);

int32_t
UnhandledExceptionFilter(
                 struct _EXCEPTION_POINTERS *ExceptionInfo);

LPEXCEPTION_POINTERS
GetExceptionInformation();

#else // FEATURE_PAL_SXS

typedef EXCEPTION_DISPOSITION (*PVECTORED_EXCEPTION_HANDLER)(
                           struct _EXCEPTION_POINTERS *ExceptionPointers);

#endif // FEATURE_PAL_SXS

// Define BitScanForward64 and BitScanForward
// Per MSDN, BitScanForward64 will search the mask data from LSB to MSB for a set bit.
// If one is found, its bit position is returned in the out uint32_t * argument and 1 is returned.
// Otherwise, 0 is returned.
//
// On GCC, the equivalent function is __builtin_ffsl. It returns 1+index of the least
// significant set bit, or 0 if if mask is zero.
//
// The same is true for BitScanForward, except that the GCC function is __builtin_ffs.
EXTERN_C
inline
unsigned char
BitScanForward(
      uint32_t * Index,
     uint32_t qwMask)
{
    unsigned char bRet = FALSE;
    static_assert(sizeof(qwMask) <= sizeof(int),
                  "use correct __builtin_ffs??? variant");
    int iIndex = __builtin_ffs(qwMask);
    if (iIndex != 0)
    {
        // Set the Index after deducting unity
        *Index = (uint32_t)(iIndex - 1);
        bRet = TRUE;
    }

    return bRet;
}

EXTERN_C
inline
unsigned char
BitScanForward64(
      uint32_t * Index,
     UINT64 qwMask)
{
    unsigned char bRet = FALSE;
    static_assert(sizeof(qwMask) <= sizeof(long long),
                  "use correct __builtin_ffs??? variant");
    int iIndex = __builtin_ffsll(qwMask);
    if (iIndex != 0)
    {
        // Set the Index after deducting unity
        *Index = (uint32_t)(iIndex - 1);
        bRet = TRUE;
    }

    return bRet;
}

// xplat-todo: review this implementation
EXTERN_C
inline
unsigned char
BitScanReverse(
      uint32_t * Index,
     uint32_t qwMask)
{
    unsigned char bRet = FALSE;
    if (qwMask != 0)
    {
        static_assert(sizeof(qwMask) <= sizeof(unsigned int),
                      "use correct __builtin_clz??? variant");
        int countLeadingZero = __builtin_clz(qwMask);
        *Index = (uint32_t)(sizeof(qwMask) * 8 - 1 - countLeadingZero);
        bRet = TRUE;
    }

    return bRet;
}

// xplat-todo: review this implementation
EXTERN_C
inline
unsigned char
BitScanReverse64(
      uint32_t * Index,
     UINT64 qwMask)
{
    unsigned char bRet = FALSE;
    if (qwMask != 0)
    {
        static_assert(sizeof(qwMask) <= sizeof(unsigned long long),
                      "use correct __builtin_clz??? variant");
        int countLeadingZero = __builtin_clzll(qwMask);
        *Index = (uint32_t)(sizeof(qwMask) * 8 - 1 - countLeadingZero);
        bRet = TRUE;
    }

    return bRet;
}

/*++
Function:
InterlockedIncrement

The InterlockedIncrement function increments (increases by one) the
value of the specified variable and checks the resulting value. The
function prevents more than one thread from using the same variable
simultaneously.

Parameters

lpAddend
[in/out] Pointer to the variable to increment.

Return Values

The return value is the resulting incremented value.

--*/
EXTERN_C
inline
int32_t
InterlockedIncrement(
      int32_t volatile *lpAddend)
{
    return __sync_add_and_fetch(lpAddend, (int32_t)1);
}

EXTERN_C
inline
LONGLONG
InterlockedIncrement16(
      short volatile *lpAddend)
{
    return __sync_add_and_fetch(lpAddend, (short)1);
}

EXTERN_C
inline
LONGLONG
InterlockedIncrement64(
      LONGLONG volatile *lpAddend)
{
    return __sync_add_and_fetch(lpAddend, (LONGLONG)1);
}

/*++
Function:
InterlockedDecrement

The InterlockedDecrement function decrements (decreases by one) the
value of the specified variable and checks the resulting value. The
function prevents more than one thread from using the same variable
simultaneously.

Parameters

lpAddend
[in/out] Pointer to the variable to decrement.

Return Values

The return value is the resulting decremented value.

--*/
EXTERN_C
inline
int32_t
InterlockedDecrement(
      int32_t volatile *lpAddend)
{
    return __sync_sub_and_fetch(lpAddend, (int32_t)1);
}

EXTERN_C
inline
LONGLONG
InterlockedDecrement64(
      LONGLONG volatile *lpAddend)
{
    return __sync_sub_and_fetch(lpAddend, (LONGLONG)1);
}

/*++
Function:
InterlockedExchange

The InterlockedExchange function atomically exchanges a pair of
values. The function prevents more than one thread from using the same
variable simultaneously.

Parameters

Target
[in/out] Pointer to the value to exchange. The function sets
this variable to Value, and returns its prior value.
Value
[in] Specifies a new value for the variable pointed to by Target.

Return Values

The function returns the initial value pointed to by Target.

--*/
EXTERN_C
inline
char
InterlockedExchange8(
      char volatile *Target,
     char Value)
{
    return __sync_swap(Target, Value);
}

EXTERN_C
inline
short
InterlockedExchange16(
      short volatile *Target,
     short Value)
{
    return __sync_swap(Target, Value);
}

EXTERN_C
inline
int32_t
InterlockedExchange(
      int32_t volatile *Target,
     int32_t Value)
{
    return __sync_swap(Target, Value);
}

EXTERN_C
inline
LONGLONG
InterlockedExchange64(
      LONGLONG volatile *Target,
     LONGLONG Value)
{
    return __sync_swap(Target, Value);
}

/*++
Function:
InterlockedCompareExchange

The InterlockedCompareExchange function performs an atomic comparison
of the specified values and exchanges the values, based on the outcome
of the comparison. The function prevents more than one thread from
using the same variable simultaneously.

If you are exchanging pointer values, this function has been
superseded by the InterlockedCompareExchangePointer function.

Parameters

Destination     [in/out] Specifies the address of the destination value. The sign is ignored.
Exchange        [in]     Specifies the exchange value. The sign is ignored.
Comperand       [in]     Specifies the value to compare to Destination. The sign is ignored.

Return Values

The return value is the initial value of the destination.

--*/
EXTERN_C
inline
char
InterlockedCompareExchange8(
      char volatile *Destination,
     char Exchange,
     char Comperand)
{
    return __sync_val_compare_and_swap(
        Destination, /* The pointer to a variable whose value is to be compared with. */
        Comperand, /* The value to be compared */
        Exchange /* The value to be stored */);
}

EXTERN_C
inline
short
InterlockedCompareExchange16(
      short volatile *Destination,
     short Exchange,
     short Comperand)
{
    return __sync_val_compare_and_swap(
        Destination, /* The pointer to a variable whose value is to be compared with. */
        Comperand, /* The value to be compared */
        Exchange /* The value to be stored */);
}

EXTERN_C
inline
int32_t
InterlockedCompareExchange(
      int32_t volatile *Destination,
     int32_t Exchange,
     int32_t Comperand)
{
    return __sync_val_compare_and_swap(
        Destination, /* The pointer to a variable whose value is to be compared with. */
        Comperand, /* The value to be compared */
        Exchange /* The value to be stored */);
}

EXTERN_C
inline
int32_t
InterlockedCompareExchangeAcquire(
      int32_t volatile *Destination,
     int32_t Exchange,
     int32_t Comperand)
{
    // TODO: implement the version with only the acquire semantics
    return __sync_val_compare_and_swap(
        Destination, /* The pointer to a variable whose value is to be compared with. */
        Comperand, /* The value to be compared */
        Exchange /* The value to be stored */);
}

EXTERN_C
inline
int32_t
InterlockedCompareExchangeRelease(
      int32_t volatile *Destination,
     int32_t Exchange,
     int32_t Comperand)
{
    // TODO: implement the version with only the release semantics
    return __sync_val_compare_and_swap(
        Destination, /* The pointer to a variable whose value is to be compared with. */
        Comperand, /* The value to be compared */
        Exchange /* The value to be stored */);
}

// See the 32-bit variant in interlock2.s
EXTERN_C
inline
LONGLONG
InterlockedCompareExchange64(
      LONGLONG volatile *Destination,
     LONGLONG Exchange,
     LONGLONG Comperand)
{
    return __sync_val_compare_and_swap(
        Destination, /* The pointer to a variable whose value is to be compared with. */
        Comperand, /* The value to be compared */
        Exchange /* The value to be stored */);
}

/*++
Function:
InterlockedExchangeAdd

The InterlockedExchangeAdd function atomically adds the value of 'Value'
to the variable that 'Addend' points to.

Parameters

lpAddend
[in/out] Pointer to the variable to to added.

Return Values

The return value is the original value that 'Addend' pointed to.

--*/
EXTERN_C
inline
char
InterlockedExchangeAdd8(
      char volatile *Addend,
     char Value)
{
    return __sync_fetch_and_add(Addend, Value);
}

EXTERN_C
inline
short
InterlockedExchangeAdd16(
      short volatile *Addend,
     short Value)
{
    return __sync_fetch_and_add(Addend, Value);
}

EXTERN_C
inline
int32_t
InterlockedExchangeAdd(
      int32_t volatile *Addend,
     int32_t Value)
{
    return __sync_fetch_and_add(Addend, Value);
}

EXTERN_C
inline
int32_t
InterlockedAdd(
      int32_t volatile *Addend,
     int32_t Value)
{
    return InterlockedExchangeAdd(Addend, Value) + Value;
}

EXTERN_C
inline
LONGLONG
InterlockedExchangeAdd64(
      LONGLONG volatile *Addend,
     LONGLONG Value)
{
    return __sync_fetch_and_add(Addend, Value);
}

EXTERN_C
inline
char
InterlockedAnd8(
      char volatile *Destination,
     char Value)
{
    return __sync_fetch_and_and(Destination, Value);
}

EXTERN_C
inline
short
InterlockedAnd16(
      short volatile *Destination,
     short Value)
{
    return __sync_fetch_and_and(Destination, Value);
}

EXTERN_C
inline
int32_t
InterlockedAnd(
      int32_t volatile *Destination,
     int32_t Value)
{
    return __sync_fetch_and_and(Destination, Value);
}

EXTERN_C
inline
LONGLONG
InterlockedAnd64(
      LONGLONG volatile *Destination,
     LONGLONG Value)
{
    return __sync_fetch_and_and(Destination, Value);
}

EXTERN_C
inline
char
InterlockedOr8(
      char volatile *Destination,
     char Value)
{
    return __sync_fetch_and_or(Destination, Value);
}

EXTERN_C
inline
short
InterlockedOr16(
      short volatile *Destination,
     short Value)
{
    return __sync_fetch_and_or(Destination, Value);
}

EXTERN_C
inline
int32_t
InterlockedOr(
      int32_t volatile *Destination,
     int32_t Value)
{
    return __sync_fetch_and_or(Destination, Value);
}

EXTERN_C
inline
LONGLONG
InterlockedOr64(
      LONGLONG volatile *Destination,
     LONGLONG Value)
{
    return __sync_fetch_and_or(Destination, Value);
}

EXTERN_C
inline
char
InterlockedXor8(
      char volatile *Destination,
     char Value)
{
    return __sync_fetch_and_xor(Destination, Value);
}

EXTERN_C
inline
short
InterlockedXor16(
      short volatile *Destination,
     short Value)
{
    return __sync_fetch_and_xor(Destination, Value);
}

EXTERN_C
inline
int32_t
InterlockedXor(
      int32_t volatile *Destination,
     int32_t Value)
{
    return __sync_fetch_and_xor(Destination, Value);
}

EXTERN_C
inline
LONGLONG
InterlockedXor64(
      LONGLONG volatile *Destination,
     LONGLONG Value)
{
    return __sync_fetch_and_xor(Destination, Value);
}

#define BITS_IN_BYTE 8
#define BITS_IN_LONG (sizeof(int32_t) * BITS_IN_BYTE)

EXTERN_C
inline
unsigned char
InterlockedBitTestAndReset(
      int32_t volatile *Base,
     int32_t Bit)
{
    // The BitTestAndReset family of functions allow for arbitrary bit
    // indices- so a bit index can be greater than the number of bits in
    // int32_t. We need to account for this in all BitTest/BitTestAndSet
    // related functions.
    volatile int32_t* longToTest = Base + (Bit / BITS_IN_LONG);
    int32_t bitToTest  = Bit % BITS_IN_LONG;
    return (InterlockedAnd(longToTest, ~(1 << bitToTest)) & (1 << bitToTest)) != 0;
}

EXTERN_C
inline
unsigned char
InterlockedBitTestAndSet(
      int32_t volatile *Base,
     int32_t Bit)
{
    volatile int32_t* longToTest = Base + (Bit / BITS_IN_LONG);
    int32_t bitToTest  = Bit % BITS_IN_LONG;
    return (InterlockedOr(longToTest, (1 << bitToTest)) & (1 << bitToTest)) != 0;
}

EXTERN_C
inline
unsigned char
BitTest(
     int32_t *Base,
     int32_t Bit)
{
    int32_t* longToTest = Base + (Bit / BITS_IN_LONG);
    int32_t bitToTest  = Bit % BITS_IN_LONG;
    return ((*longToTest) & (1 << bitToTest)) != 0;
}

EXTERN_C
inline
unsigned char
BitTestAndSet(
      int32_t *Base,
     int32_t Bit)
{
    // xplat-todo: Benchmark whether this is better than
    // using the inline assembler to generate a bts instruction
    int32_t* longToTest = Base + (Bit / BITS_IN_LONG);
    int32_t bitToTest  = Bit % BITS_IN_LONG;

    // Save whether the bit was set or not. Then, unconditionally set the
    // bit. Return whether the bit was set or not.
    unsigned char wasBitSet = (((*longToTest) & (1 << bitToTest)) != 0);
    *longToTest = *longToTest | (1 << bitToTest);
    return wasBitSet;
}

#define InterlockedExchangePointer(Target, Value) \
    ((void *)InterlockedExchange64((PLONG64)(Target), (LONGLONG)(Value)))

#define InterlockedCompareExchangePointer(Destination, ExChange, Comperand) \
    ((void *)InterlockedCompareExchange64((PLONG64)(Destination), (LONGLONG)(ExChange), (LONGLONG)(Comperand)))

/*++
Function:
MemoryBarrier

The MemoryBarrier function creates a full memory barrier.

--*/
EXTERN_C
inline
void
MemoryBarrier(
    void)
{
    __sync_synchronize();
}

void
YieldProcessor(
    void);

uint32_t
GetCurrentProcessorNumber();

/*++
Function:
PAL_HasGetCurrentProcessorNumber

Checks if GetCurrentProcessorNumber is available in the current environment

--*/
BOOL
PAL_HasGetCurrentProcessorNumber();

#define FORMAT_MESSAGE_ALLOCATE_BUFFER 0x00000100
#define FORMAT_MESSAGE_IGNORE_INSERTS  0x00000200
#define FORMAT_MESSAGE_FROM_STRING     0x00000400
#define FORMAT_MESSAGE_FROM_SYSTEM     0x00001000
#define FORMAT_MESSAGE_ARGUMENT_ARRAY  0x00002000
#define FORMAT_MESSAGE_MAX_WIDTH_MASK  0x000000FF

uint32_t
FormatMessageW(
            uint32_t dwFlags,
            const void * lpSource,
            uint32_t dwMessageId,
            uint32_t dwLanguageId,
            LPWSTR lpBffer,
            uint32_t nSize,
            va_list *Arguments);

#ifdef UNICODE
#define FormatMessage FormatMessageW
#endif


uint32_t
GetLastError(
         void);

void
SetLastError(
          uint32_t dwErrCode);

LPWSTR
GetCommandLineW(
        void);

#ifdef UNICODE
#define GetCommandLine GetCommandLineW
#endif

void
RtlRestoreContext(
   PCONTEXT ContextRecord,
   PEXCEPTION_RECORD ExceptionRecord
);

void
RtlCaptureContext(
   PCONTEXT ContextRecord
);

typedef void (*PAL_ActivationFunction)(CONTEXT *context);
typedef BOOL (*PAL_SafeActivationCheckFunction)(SIZE_T ip, BOOL checkingCurrentThread);

void
PAL_SetActivationFunction(
     PAL_ActivationFunction pActivationFunction,
     PAL_SafeActivationCheckFunction pSafeActivationCheckFunction);

#define VER_PLATFORM_WIN32_WINDOWS        1
#define VER_PLATFORM_WIN32_NT        2
#define VER_PLATFORM_UNIX            10
#define VER_PLATFORM_MACOSX          11

typedef struct _OSVERSIONINFOA {
    uint32_t dwOSVersionInfoSize;
    uint32_t dwMajorVersion;
    uint32_t dwMinorVersion;
    uint32_t dwBuildNumber;
    uint32_t dwPlatformId;
    char szCSDVersion[ 128 ];
} OSVERSIONINFOA, *POSVERSIONINFOA, *LPOSVERSIONINFOA;

typedef struct _OSVERSIONINFOW {
    uint32_t dwOSVersionInfoSize;
    uint32_t dwMajorVersion;
    uint32_t dwMinorVersion;
    uint32_t dwBuildNumber;
    uint32_t dwPlatformId;
    WCHAR szCSDVersion[ 128 ];
} OSVERSIONINFOW, *POSVERSIONINFOW, *LPOSVERSIONINFOW;

#ifdef UNICODE
typedef OSVERSIONINFOW OSVERSIONINFO;
typedef POSVERSIONINFOW POSVERSIONINFO;
typedef LPOSVERSIONINFOW LPOSVERSIONINFO;
#else
typedef OSVERSIONINFOA OSVERSIONINFO;
typedef POSVERSIONINFOA POSVERSIONINFO;
typedef LPOSVERSIONINFOA LPOSVERSIONINFO;
#endif

typedef struct _OSVERSIONINFOEXA {
    uint32_t dwOSVersionInfoSize;
    uint32_t dwMajorVersion;
    uint32_t dwMinorVersion;
    uint32_t dwBuildNumber;
    uint32_t dwPlatformId;
    char szCSDVersion[ 128 ];
    uint16_t  wServicePackMajor;
    uint16_t  wServicePackMinor;
    uint16_t  wSuiteMask;
    uint8_t  wProductType;
    uint8_t  wReserved;
} OSVERSIONINFOEXA, *POSVERSIONINFOEXA, *LPOSVERSIONINFOEXA;

typedef struct _OSVERSIONINFOEXW {
    uint32_t dwOSVersionInfoSize;
    uint32_t dwMajorVersion;
    uint32_t dwMinorVersion;
    uint32_t dwBuildNumber;
    uint32_t dwPlatformId;
    WCHAR szCSDVersion[ 128 ];
    uint16_t  wServicePackMajor;
    uint16_t  wServicePackMinor;
    uint16_t  wSuiteMask;
    uint8_t  wProductType;
    uint8_t  wReserved;
} OSVERSIONINFOEXW, *POSVERSIONINFOEXW, *LPOSVERSIONINFOEXW;

#ifdef UNICODE
typedef OSVERSIONINFOEXW OSVERSIONINFOEX;
typedef POSVERSIONINFOEXW POSVERSIONINFOEX;
typedef LPOSVERSIONINFOEXW LPOSVERSIONINFOEX;
#else
typedef OSVERSIONINFOEXA OSVERSIONINFOEX;
typedef POSVERSIONINFOEXA POSVERSIONINFOEX;
typedef LPOSVERSIONINFOEXA LPOSVERSIONINFOEX;
#endif

BOOL
GetVersionExA(
            LPOSVERSIONINFOA lpVersionInformation);

BOOL
GetVersionExW(
            LPOSVERSIONINFOW lpVersionInformation);

#ifdef UNICODE
#define GetVersionEx GetVersionExW
#else
#define GetVersionEx GetVersionExA
#endif

#define IMAGE_FILE_MACHINE_I386              0x014c
#define IMAGE_FILE_MACHINE_ARM64             0xAA64  // ARM64 Little-Endian

typedef struct _SYSTEM_INFO {
    uint16_t wProcessorArchitecture_PAL_Undefined;
    uint16_t wReserved_PAL_Undefined; // NOTE: diff from winbase.h - no obsolete dwOemId union
    uint32_t dwPageSize;
    void * lpMinimumApplicationAddress;
    void * lpMaximumApplicationAddress;
    DWORD_PTR dwActiveProcessorMask_PAL_Undefined;
    uint32_t dwNumberOfProcessors;
    uint32_t dwProcessorType_PAL_Undefined;
    uint32_t dwAllocationGranularity;
    uint16_t wProcessorLevel_PAL_Undefined;
    uint16_t wProcessorRevision_PAL_Undefined;
} SYSTEM_INFO, *LPSYSTEM_INFO;

void
GetSystemInfo(
           LPSYSTEM_INFO lpSystemInfo);

BOOL
GetDiskFreeSpaceW(
          LPCWSTR lpDirectoryName,
          uint32_t * lpSectorsPerCluster,
          uint32_t * lpBytesPerSector,
          uint32_t * lpNumberOfFreeClusters,
          uint32_t * lpTotalNumberOfClusters);

#ifdef UNICODE
#define GetDiskFreeSpace GetDiskFreeSpaceW
#endif

BOOL
CreatePipe(
     PHANDLE hReadPipe,
     PHANDLE hWritePipe,
     LPSECURITY_ATTRIBUTES lpPipeAttributes,
     uint32_t nSize
    );

BOOL
DeregisterEventSource (
     HANDLE hEventLog
    );

HANDLE
RegisterEventSourceA (
      LPCSTR lpUNCServerName,
         LPCSTR lpSourceName
    );
HANDLE
RegisterEventSourceW (
      LPCWSTR lpUNCServerName,
         LPCWSTR lpSourceName
    );
#ifdef UNICODE
#define RegisterEventSource  RegisterEventSourceW
#else
#define RegisterEventSource  RegisterEventSourceA
#endif // !UNICODE

//
// The types of events that can be logged.
//
#define EVENTLOG_SUCCESS                0x0000
#define EVENTLOG_ERROR_TYPE             0x0001
#define EVENTLOG_WARNING_TYPE           0x0002
#define EVENTLOG_INFORMATION_TYPE       0x0004
#define EVENTLOG_AUDIT_SUCCESS          0x0008
#define EVENTLOG_AUDIT_FAILURE          0x0010

BOOL
ReportEventA (
         HANDLE     hEventLog,
         uint16_t       wType,
         uint16_t       wCategory,
         uint32_t      dwEventID,
      PSID       lpUserSid,
         uint16_t       wNumStrings,
         uint32_t      dwDataSize,
      LPCSTR *lpStrings,
      void * lpRawData
    );
BOOL
ReportEventW (
         HANDLE     hEventLog,
         uint16_t       wType,
         uint16_t       wCategory,
         uint32_t      dwEventID,
      PSID       lpUserSid,
         uint16_t       wNumStrings,
         uint32_t      dwDataSize,
      LPCWSTR *lpStrings,
      void * lpRawData
    );
#ifdef UNICODE
#define ReportEvent  ReportEventW
#else
#define ReportEvent  ReportEventA
#endif // !UNICODE

HRESULT
CoCreateGuid( GUID * pguid);

/******************* C Runtime Entrypoints *******************************/

/* Some C runtime functions needs to be reimplemented by the PAL.
   To avoid name collisions, those functions have been renamed using
   defines */
#if !defined(PAL_STDCPP_COMPAT) || defined(USING_PAL_STDLIB)
#define exit          PAL_exit
#define vprintf       PAL_vprintf
#define wprintf       PAL_wprintf
#define sprintf       PAL_sprintf
#define swprintf      PAL_swprintf
#define sscanf        PAL_sscanf
#define wcsspn        PAL_wcsspn
#define wcstod        PAL_wcstod
#define wcstol        PAL_wcstol
#define wcstoll       PAL_wcstoll
#define wcstoul       PAL_wcstoul
#define wcscat        PAL_wcscat
#define wcscpy        PAL_wcscpy
#define wcslen        PAL_wcslen
#define wmemcmp       PAL_wmemcmp
#define wcsncmp       PAL_wcsncmp
#define wcschr        PAL_wcschr
#define wcsrchr       PAL_wcsrchr
#define wcsstr        PAL_wcsstr
#define swscanf       PAL_swscanf
#define wcspbrk       PAL_wcspbrk
#define wcscmp        PAL_wcscmp
#define wcsncat       PAL_wcsncat
#define wcsncpy       PAL_wcsncpy
#define wcstok        PAL_wcstok
#define wcscspn       PAL_wcscspn
#define iswprint      PAL_iswprint
#define iswalpha      PAL_iswalpha
#define iswdigit      PAL_iswdigit
#define iswspace      PAL_iswspace
#define iswupper      PAL_iswupper
#define iswxdigit     PAL_iswxdigit
#define towlower      PAL_towlower
#define towupper      PAL_towupper
#define vsprintf      PAL_vsprintf
#define vswprintf     PAL_vswprintf
#define fopen         PAL_fopen
#define strtok        PAL_strtok
#define strtoul       PAL_strtoul
#define fprintf       PAL_fprintf
#define fwprintf      PAL_fwprintf
#define vfprintf      PAL_vfprintf
#define vfwprintf     PAL_vfwprintf
#define ctime         PAL_ctime
#define rand          PAL_rand
#define getenv        PAL_getenv
#define fgets         PAL_fgets
#define fgetws        PAL_fgetws
#define fputc         PAL_fputc
#define putchar       PAL_putchar
#define bsearch       PAL_bsearch
#define ferror        PAL_ferror
#define fread         PAL_fread
#define fwrite        PAL_fwrite
#define feof          PAL_feof
#define ftell         PAL_ftell
#define fclose        PAL_fclose
#define setbuf        PAL_setbuf
#define fflush        PAL_fflush
#define fputs         PAL_fputs
#define fseek         PAL_fseek
#define fgetpos       PAL_fgetpos
#define fsetpos       PAL_fsetpos
#define getc          PAL_getc
#define fgetc         PAL_getc // not a typo
#define ungetc        PAL_ungetc
#define setvbuf       PAL_setvbuf
#define atol          PAL_atol
#define memmove       memmove_xplat
#define mkstemp       PAL_mkstemp
#define rename        PAL_rename
#define unlink        PAL_unlink
#define _strdup       PAL__strdup
#define _getcwd       PAL__getcwd
#define _open         PAL__open
#define _close        PAL__close
#define _wcstoui64    PAL__wcstoui64
#define _flushall     PAL__flushall

#endif // !PAL_STDCPP_COMPAT

#ifndef _CONST_RETURN
#ifdef  __cplusplus
#define _CONST_RETURN  const
#define _CRT_CONST_CORRECT_OVERLOADS
#else
#define _CONST_RETURN
#endif
#endif

/* For backwards compatibility */
#define _WConst_return _CONST_RETURN

#define EOF     (-1)

typedef int errno_t;

#ifndef PAL_STDCPP_COMPAT

typedef struct {
    int quot;
    int rem;
} div_t;

div_t div(int numer, int denom);

void * memcpy(void *, const void *, size_t);
int    memcmp(const void *, const void *, size_t);
void * memset(void *, int, size_t);
void * memchr(const void *, int, size_t);

size_t strlen(const char *);
int strcmp(const char*, const char *);
int strncmp(const char*, const char *, size_t);
int _strnicmp(const char *, const char *, size_t);
char * strcat(char *, const char *);
char * strncat(char *, const char *, size_t);
char * strcpy(char *, const char *);
char * strncpy(char *, const char *, size_t);
char * strchr(const char *, int);
char * strrchr(const char *, int);
char * strpbrk(const char *, const char *);
char * strstr(const char *, const char *);
char * strtok(char *, const char *);
size_t strspn(const char *, const char *);
size_t  strcspn(const char *, const char *);
int sprintf(char *, const char *, ...);
int vsprintf(char *, const char *, va_list);
int sscanf(const char *, const char *, ...);
int atoi(const char *);
int32_t atol(const char *);
//long long int atoll(const char *) __THROW;
uint32_t strtoul(const char *, char **, int);
double atof(const char *);
double strtod(const char *, char **);
int isprint(int);
int isspace(int);
int isalpha(int);
int isalnum(int);
int isdigit(int);
int isxdigit(int);
int isupper(int);
int islower(int);
int tolower(int);
int toupper(int);

#endif // PAL_STDCPP_COMPAT

errno_t memcpy_s(void *, size_t, const void *, size_t);
errno_t memmove_s(void *, size_t, const void *, size_t);
void * memmove_xplat(void *, const void *, size_t);
char * _strlwr(char *);
int _stricmp(const char *, const char *);
int _snprintf(char *, size_t, const char *, ...);
char * _gcvt_s(char *, int, double, int);
char * _ecvt(double, int, int *, int *);
int __iscsym(int);
size_t _mbslen(const unsigned char *);
unsigned char * _mbsinc(const unsigned char *);
unsigned char * _mbsninc(const unsigned char *, size_t);
unsigned char * _mbsdec(const unsigned char *, const unsigned char *);
int _wcsicmp(const WCHAR *, const WCHAR*);
int _wcsnicmp(const WCHAR *, const WCHAR *, size_t);
int _vsnprintf(char *, size_t, const char *, va_list);
int _vsnwprintf(WCHAR *, size_t, const WCHAR *, va_list);
WCHAR * _itow(int, WCHAR *, int);
WCHAR * _ltow(long, WCHAR *, int);

size_t PAL_wcslen(const WCHAR *);
int PAL_wcscmp(const WCHAR*, const WCHAR*);
int PAL_wmemcmp(const WCHAR *, const WCHAR *, size_t);
int PAL_wcsncmp(const WCHAR *, const WCHAR *, size_t);
WCHAR * PAL_wcscat(WCHAR *, const WCHAR *);
WCHAR * PAL_wcsncat(WCHAR *, const WCHAR *, size_t);
WCHAR * PAL_wcscpy(WCHAR *, const WCHAR *);
WCHAR * PAL_wcsncpy(WCHAR *, const WCHAR *, size_t);
const WCHAR * PAL_wcschr(const WCHAR *, WCHAR);
const WCHAR * PAL_wcsrchr(const WCHAR *, WCHAR);
WCHAR _WConst_return * PAL_wcspbrk(const WCHAR *, const WCHAR *);
WCHAR _WConst_return * PAL_wcsstr(const WCHAR *, const WCHAR *);
WCHAR * PAL_wcstok(WCHAR *, const WCHAR *);
size_t PAL_wcscspn(const WCHAR *, const WCHAR *);
int PAL_swprintf(WCHAR *, const WCHAR *, ...);
int PAL_vswprintf(WCHAR *, const WCHAR *, va_list);
int _snwprintf(WCHAR *, size_t, const WCHAR *, ...);
int PAL_swscanf(const WCHAR *, const WCHAR *, ...);
int32_t PAL_wcstol(const WCHAR *, WCHAR **, int);
uint32_t PAL_wcstoul(const WCHAR *, WCHAR **, int);
LONGLONG PAL_wcstoll(const WCHAR *, WCHAR **, int);
size_t PAL_wcsspn (const WCHAR *, const WCHAR *);
double PAL_wcstod(const WCHAR *, WCHAR **);
int PAL_iswalpha(WCHAR);
int PAL_iswprint(WCHAR);
int PAL_iswupper(WCHAR);
int PAL_iswspace(WCHAR);
int PAL_iswdigit(WCHAR);
int PAL_iswxdigit(WCHAR);
WCHAR PAL_towlower(WCHAR);
WCHAR PAL_towupper(WCHAR);

WCHAR * _wcslwr(WCHAR *);
ULONGLONG _wcstoui64(const WCHAR *, WCHAR **, int);
WCHAR * _i64tow(__int64, WCHAR *, int);
WCHAR * _ui64tow(unsigned __int64, WCHAR *, int);
int _wtoi(const WCHAR *);

#ifdef __cplusplus
extern "C++" {
inline WCHAR *PAL_wcschr(WCHAR *_S, WCHAR _C)
        {return ((WCHAR *)PAL_wcschr((const WCHAR *)_S, _C)); }
inline WCHAR *PAL_wcsrchr(WCHAR *_S, WCHAR _C)
        {return ((WCHAR *)PAL_wcsrchr((const WCHAR *)_S, _C)); }
inline WCHAR *PAL_wcspbrk(WCHAR *_S, const WCHAR *_P)
        {return ((WCHAR *)PAL_wcspbrk((const WCHAR *)_S, _P)); }
inline WCHAR *PAL_wcsstr(WCHAR *_S, const WCHAR *_P)
        {return ((WCHAR *)PAL_wcsstr((const WCHAR *)_S, _P)); }
}
#endif

#ifndef __has_builtin
#define __has_builtin(x) 0
#endif

#if !__has_builtin(_rotl)
/*++
Function:
_rotl

See MSDN doc.
--*/
EXTERN_C
inline
unsigned int _rotl(unsigned int value, int shift)
{
    unsigned int retval = 0;

    shift &= 0x1f;
    retval = (value << shift) | (value >> (sizeof(int) * CHAR_BIT - shift));
    return retval;
}
#endif

// On 64 bit unix, make the long an int.
#define _lrotl _rotl

#if !__has_builtin(_rotl64)
/*++
Function:
_rotl64

See MSDN doc.
--*/
EXTERN_C
inline
unsigned long long _rotl64(unsigned long long value, int shift)
{
    unsigned long long retval = 0;

    shift &= 0x3f;
    retval = (value << shift) | (value >> (sizeof(unsigned long long) * CHAR_BIT - shift));
    return retval;
}
#endif

#if !__has_builtin(_rotr)
/*++
Function:
_rotr

See MSDN doc.
--*/
EXTERN_C
inline
unsigned int _rotr(unsigned int value, int shift)
{
    unsigned int retval;

    shift &= 0x1f;
    retval = (value >> shift) | (value << (sizeof(int) * CHAR_BIT - shift));
    return retval;
}
#endif

#if !__has_builtin(_rotr64)
/*++
Function:
_rotr64

See MSDN doc.
--*/
EXTERN_C
inline
unsigned long long _rotr64(unsigned long long value, int shift)
{
    unsigned long long retval;

    shift &= 0x3f;
    retval = (value >> shift) | (value << (sizeof(unsigned long long) * CHAR_BIT - shift));
    return retval;
}
#endif

int _finite(double);
int _isnan(double);
double _copysign(double, double);
float _copysignf(float, float);

#if !defined(PAL_STDCPP_COMPAT) || defined(USING_PAL_STDLIB)

#ifdef PLATFORM_ACCEPTS_ABS_OVERLOAD
#ifdef __cplusplus
extern "C++" {

inline __int64 abs(__int64 _X) {
    return llabs(_X);
}

}
#endif
#endif

#ifdef INCLUDE_PAL_INTERNAL_
/* FIXME remove
 * PAL wrappers around memory management functions, only used inside PAL */
void * PAL_malloc(size_t);
void   PAL_free(void *);
void * PAL_realloc(void *, size_t);
#endif
char * _strdup(const char *);

#define _alloca alloca

#if defined(__GNUC__)
// we set -fno-builtin on the command line. This requires that if
// we use alloca, with either have to call __builtin_alloca, or
// ensure that the alloca call doesn't happen in code which is
// modifying the stack (such as "memset (alloca(x), y, z)"

#define alloca  __builtin_alloca
#endif // __GNUC__

#endif // !PAL_STDCPP_COMPAT

__attribute__((noreturn)) void exit(int);

void * bsearch(const void *, const void *, size_t, size_t,
int (*)(const void *, const void *));

void _splitpath(const char *, char *, char *, char *, char *);
void _wsplitpath(const WCHAR *, WCHAR *, WCHAR *, WCHAR *, WCHAR *);
void _makepath(char *, const char *, const char *, const char *, const char *);
void _wmakepath(WCHAR *, const WCHAR *, const WCHAR *, const WCHAR *, const WCHAR *);
char * _fullpath(char *, const char *, size_t);

#ifndef PAL_STDCPP_COMPAT
time_t time(time_t *);

struct tm {
        int tm_sec;     /* seconds after the minute - [0,59] */
        int tm_min;     /* minutes after the hour - [0,59] */
        int tm_hour;    /* hours since midnight - [0,23] */
        int tm_mday;    /* day of the month - [1,31] */
        int tm_mon;     /* months since January - [0,11] */
        int tm_year;    /* years since 1900 */
        int tm_wday;    /* days since Sunday - [0,6] */
        int tm_yday;    /* days since January 1 - [0,365] */
        int tm_isdst;   /* daylight savings time flag */
        };

char * ctime(const time_t *);
#endif // !PAL_STDCPP_COMPAT

int _open_osfhandle(INT_PTR, int);
int _close(int);
int _flushall();

#ifdef PAL_STDCPP_COMPAT

struct _PAL_FILE;
typedef struct _PAL_FILE PAL_FILE;

#else // PAL_STDCPP_COMPAT

struct _FILE;
typedef struct _FILE FILE;
typedef struct _FILE PAL_FILE;

#define SEEK_SET    0
#define SEEK_CUR    1
#define SEEK_END    2

/* Locale categories */
#define LC_ALL          0
#define LC_COLLATE      1
#define LC_CTYPE        2
#define LC_MONETARY     3
#define LC_NUMERIC      4
#define LC_TIME         5

#define _IOFBF  0       /* setvbuf should set fully buffered */
#define _IOLBF  1       /* setvbuf should set line buffered */
#define _IONBF  2       /* setvbuf should set unbuffered */

#endif // PAL_STDCPP_COMPAT

int PAL_fclose(PAL_FILE *);
void PAL_setbuf(PAL_FILE *, char*);
int PAL_fflush(PAL_FILE *);
size_t PAL_fwrite(const void *, size_t, size_t, PAL_FILE *);
size_t PAL_fread(void *, size_t, size_t, PAL_FILE *);
char * PAL_fgets(char *, int, PAL_FILE *);
int PAL_fputs(const char *, PAL_FILE *);
int PAL_fputc(int c, PAL_FILE *stream);
int PAL_putchar(int c);
int PAL_fprintf(PAL_FILE *, const char *, ...);
int PAL_vfprintf(PAL_FILE *, const char *, va_list);
int PAL_fseek(PAL_FILE *, int32_t, int);
int PAL_fgetpos(PAL_FILE *, fpos_t *);
int PAL_fsetpos(PAL_FILE *, const fpos_t *);
int32_t PAL_ftell(PAL_FILE *);
int PAL_feof(PAL_FILE *);
int PAL_ferror(PAL_FILE *);
PAL_FILE * PAL_fopen(const char *, const char *);
char * PAL_strtok(char *, const char *);
int PAL_getc(PAL_FILE *stream);
int PAL_fgetc(PAL_FILE *stream);
int PAL_ungetc(int c, PAL_FILE *stream);
int PAL_setvbuf(PAL_FILE *stream, char *, int, size_t);
WCHAR * PAL_fgetws(WCHAR *, int, PAL_FILE *);
int PAL_fwprintf(PAL_FILE *, const WCHAR *, ...);
int PAL_vfwprintf(PAL_FILE *, const WCHAR *, va_list);
int PAL_wprintf(const WCHAR*, ...);

int _getw(PAL_FILE *);
int _putw(int, PAL_FILE *);
PAL_FILE * _fdopen(int, const char *);
PAL_FILE * _wfopen(const WCHAR *, const WCHAR *);
PAL_FILE * _wfsopen(const WCHAR *, const WCHAR *, int);

/* Maximum value that can be returned by the rand function. */

#ifndef PAL_STDCPP_COMPAT
#define RAND_MAX 0x7fff
#endif // !PAL_STDCPP_COMPAT

int rand(void);
void srand(unsigned int);
errno_t rand_s(unsigned int*);
int vprintf(const char *, va_list);

#define PAL_get_caller 0

PAL_FILE * PAL_get_stdout(int caller);
PAL_FILE * PAL_get_stdin(int caller);
PAL_FILE * PAL_get_stderr(int caller);
int * PAL_errno(int caller);

#if defined(PAL_STDCPP_COMPAT) && !defined(USING_PAL_STDLIB)
#define PAL_stdout (PAL_get_stdout(PAL_get_caller))
#define PAL_stdin  (PAL_get_stdin(PAL_get_caller))
#define PAL_stderr (PAL_get_stderr(PAL_get_caller))
#define PAL_errno   (*PAL_errno(PAL_get_caller))
#else // PAL_STDCPP_COMPAT
#define stdout (PAL_get_stdout(PAL_get_caller))
#define stdin  (PAL_get_stdin(PAL_get_caller))
#define stderr (PAL_get_stderr(PAL_get_caller))
#define errno  (*PAL_errno(PAL_get_caller))
#endif // PAL_STDCPP_COMPAT

char * getenv(const char *);
int _putenv(const char *);

#define ERANGE          34

/******************* PAL-specific I/O completion port *****************/

typedef struct _PAL_IOCP_CPU_INFORMATION {
    union {
        FILETIME ftLastRecordedIdleTime;
        FILETIME ftLastRecordedCurrentTime;
    } LastRecordedTime;
    FILETIME ftLastRecordedKernelTime;
    FILETIME ftLastRecordedUserTime;
} PAL_IOCP_CPU_INFORMATION;

int32_t
PAL_GetCPUBusyTime(
      PAL_IOCP_CPU_INFORMATION *lpPrevCPUInfo);

/****************PAL Perf functions for PInvoke*********************/
#if PAL_PERF
void
PAL_EnableProcessProfile(void);

void
PAL_DisableProcessProfile(void);

BOOL
PAL_IsProcessProfileEnabled(void);

int64_t
PAL_GetCpuTickCount(void);
#endif // PAL_PERF

/******************* PAL side-by-side support  ************************/

#ifdef FEATURE_PAL_SXS
//
// Some versions of the PAL support several PALs side-by-side
// in the process.  To avoid those PALs interfering with one
// another, they need to be told by clients when they are active
// and when they are not.
//

// To avoid performance problems incurred by swapping thread
// exception ports every time we leave the PAL, there's also
// the concept of entering/leaving the PAL at its top boundary
// (entering down/leaving up) or at the bottom boundary
// (leaving down/entering up).

typedef enum _PAL_Boundary {
    PAL_BoundaryTop,            // closer to main()
    PAL_BoundaryBottom,         // closer to execution
    PAL_BoundaryEH,             // out-of-band during EH

    PAL_BoundaryMax = PAL_BoundaryEH
} PAL_Boundary;

// This function needs to be called on a thread when it enters
// a region of code that depends on this instance of the PAL
// in the process, and the current thread may or may not be
// known to the PAL.  This function can fail (for something else
// than an internal error) if this is the first time that the
// current thread entered this PAL.  Note that PAL_Initialize
// implies a call to this function.  Does not modify LastError.
uint32_t
PAL_Enter(PAL_Boundary boundary);

// Returns TRUE if we this thread has already entered the PAL,
// returns FALSE if we have not entered the PAL.
BOOL
PAL_HasEntered();

// Equivalent to PAL_Enter(PAL_BoundaryTop) and is for stub
// code generation use.
uint32_t
PAL_EnterTop();

// This function needs to be called on a thread when it enters
// a region of code that depends on this instance of the PAL
// in the process, and the current thread is already known to
// the PAL.  Does not modify LastError.
void
PAL_Reenter(PAL_Boundary boundary);

// This function needs to be called on a thread when it leaves
// a region of code that depends on this instance of the PAL
// in the process.  Does not modify LastError.
void
PAL_Leave(PAL_Boundary boundary);

// This function is equivalent to PAL_Leave(PAL_BoundaryBottom)
// and is available to limit the creation of stub code.
void
PAL_LeaveBottom();

// This function is equivalent to PAL_Leave(PAL_BoundaryTop)
// and is available to limit the creation of stub code.
void
PAL_LeaveTop();

#ifdef  __cplusplus
//
// A holder to enter the PAL for a specific region of code.
// Previously, we must have been executing outside the PAL
// (unless fEnter is set to FALSE).
//
class PAL_EnterHolder
{
private:
    BOOL m_fEntered;
    uint32_t m_palError;
public:
    PAL_EnterHolder(BOOL fEnter = TRUE) : m_palError(ERROR_SUCCESS)
    {
        if (fEnter)
        {
            m_palError = PAL_Enter(PAL_BoundaryTop);
            m_fEntered = m_palError == ERROR_SUCCESS;
        }
        else
        {
            m_fEntered = FALSE;
        }
    }

    ~PAL_EnterHolder()
    {
        if (m_fEntered)
        {
            PAL_Leave(PAL_BoundaryTop);
        }
    }

    uint32_t GetError()
    {
        return m_palError;
    }

    void SuppressRelease()
    {
        // Used to avoid calling PAL_Leave() when
        // another code path will explicitly do so.
        m_fEntered = FALSE;
    }
};

class PAL_LeaveHolder
{
public:
    PAL_LeaveHolder()
    {
        PAL_Leave(PAL_BoundaryBottom);
    }

    ~PAL_LeaveHolder()
    {
        PAL_Reenter(PAL_BoundaryBottom);
    }
};
#endif // __cplusplus

#else // FEATURE_PAL_SXS

#define PAL_Enter(boundary) ERROR_SUCCESS
#define PAL_Reenter(boundary)
#define PAL_Leave(boundary)

#ifdef __cplusplus
class PAL_EnterHolder {
public:
    // using constructor to suppress the "unused variable" warnings
    PAL_EnterHolder() {}
};
class PAL_LeaveHolder {
public:
    // using constructor to suppress the "unused variable" warnings
    PAL_LeaveHolder() {}
};
#endif // __cplusplus

#endif // FEATURE_PAL_SXS

#define EXCEPTION_CONTINUE_SEARCH   0
#define EXCEPTION_EXECUTE_HANDLER   1
#define EXCEPTION_CONTINUE_EXECUTION -1

// Platform-specific library naming
//
#ifdef __APPLE__
#define MAKEDLLNAME_W(name) u"lib" name u".dylib"
#define MAKEDLLNAME_A(name)  "lib" name  ".dylib"
#elif defined(_AIX)
#define MAKEDLLNAME_W(name) L"lib" name L".a"
#define MAKEDLLNAME_A(name)  "lib" name  ".a"
#elif defined(__hppa__) || defined(_IA64_)
#define MAKEDLLNAME_W(name) L"lib" name L".sl"
#define MAKEDLLNAME_A(name)  "lib" name  ".sl"
#else
#define MAKEDLLNAME_W(name) u"lib" name u".so"
#define MAKEDLLNAME_A(name)  "lib" name  ".so"
#endif

#ifdef UNICODE
#define MAKEDLLNAME(x) MAKEDLLNAME_W(x)
#else
#define MAKEDLLNAME(x) MAKEDLLNAME_A(x)
#endif

#define PAL_SHLIB_PREFIX "lib"

#if __APPLE__
#define PAL_SHLIB_SUFFIX ".dylib"
#elif _AIX
#define PAL_SHLIB_SUFFIX ".a"
#elif _HPUX_
#define PAL_SHLIB_SUFFIX ".sl"
#else
#define PAL_SHLIB_SUFFIX ".so"
#endif

#define DBG_EXCEPTION_HANDLED            ((uint32_t   )0x00010001L)
#define DBG_CONTINUE                     ((uint32_t   )0x00010002L)
#define DBG_EXCEPTION_NOT_HANDLED        ((uint32_t   )0x80010001L)

#define DBG_TERMINATE_THREAD             ((uint32_t   )0x40010003L)
#define DBG_TERMINATE_PROCESS            ((uint32_t   )0x40010004L)
#define DBG_CONTROL_C                    ((uint32_t   )0x40010005L)
#define DBG_RIPEXCEPTION                 ((uint32_t   )0x40010007L)
#define DBG_CONTROL_BREAK                ((uint32_t   )0x40010008L)
#define DBG_COMMAND_EXCEPTION            ((uint32_t   )0x40010009L)

#define STATUS_USER_APC                  ((uint32_t   )0x000000C0L)
#define STATUS_GUARD_PAGE_VIOLATION      ((uint32_t   )0x80000001L)
#define STATUS_DATATYPE_MISALIGNMENT     ((uint32_t   )0x80000002L)
#define STATUS_BREAKPOINT                ((uint32_t   )0x80000003L)
#define STATUS_SINGLE_STEP               ((uint32_t   )0x80000004L)
#define STATUS_LONGJUMP                  ((uint32_t   )0x80000026L)
#define STATUS_UNWIND_CONSOLIDATE        ((uint32_t   )0x80000029L)
#define STATUS_ACCESS_VIOLATION          ((uint32_t   )0xC0000005L)
#define STATUS_IN_PAGE_ERROR             ((uint32_t   )0xC0000006L)
#define STATUS_INVALID_HANDLE            ((uint32_t   )0xC0000008L)
#define STATUS_NO_MEMORY                 ((uint32_t   )0xC0000017L)
#define STATUS_ILLEGAL_INSTRUCTION       ((uint32_t   )0xC000001DL)
#define STATUS_NONCONTINUABLE_EXCEPTION  ((uint32_t   )0xC0000025L)
#define STATUS_INVALID_DISPOSITION       ((uint32_t   )0xC0000026L)
#define STATUS_ARRAY_BOUNDS_EXCEEDED     ((uint32_t   )0xC000008CL)
#define STATUS_FLOAT_DENORMAL_OPERAND    ((uint32_t   )0xC000008DL)
#define STATUS_FLOAT_DIVIDE_BY_ZERO      ((uint32_t   )0xC000008EL)
#define STATUS_FLOAT_INEXACT_RESULT      ((uint32_t   )0xC000008FL)
#define STATUS_FLOAT_INVALID_OPERATION   ((uint32_t   )0xC0000090L)
#define STATUS_FLOAT_OVERFLOW            ((uint32_t   )0xC0000091L)
#define STATUS_FLOAT_STACK_CHECK         ((uint32_t   )0xC0000092L)
#define STATUS_FLOAT_UNDERFLOW           ((uint32_t   )0xC0000093L)
#define STATUS_INTEGER_DIVIDE_BY_ZERO    ((uint32_t   )0xC0000094L)
#define STATUS_INTEGER_OVERFLOW          ((uint32_t   )0xC0000095L)
#define STATUS_PRIVILEGED_INSTRUCTION    ((uint32_t   )0xC0000096L)
#define STATUS_STACK_OVERFLOW            ((uint32_t   )0xC00000FDL)
#define STATUS_CONTROL_C_EXIT            ((uint32_t   )0xC000013AL)

#define WAIT_IO_COMPLETION                  STATUS_USER_APC

#define EXCEPTION_ACCESS_VIOLATION          STATUS_ACCESS_VIOLATION
#define EXCEPTION_DATATYPE_MISALIGNMENT     STATUS_DATATYPE_MISALIGNMENT
#define EXCEPTION_BREAKPOINT                STATUS_BREAKPOINT
#define EXCEPTION_SINGLE_STEP               STATUS_SINGLE_STEP
#define EXCEPTION_ARRAY_BOUNDS_EXCEEDED     STATUS_ARRAY_BOUNDS_EXCEEDED
#define EXCEPTION_FLT_DENORMAL_OPERAND      STATUS_FLOAT_DENORMAL_OPERAND
#define EXCEPTION_FLT_DIVIDE_BY_ZERO        STATUS_FLOAT_DIVIDE_BY_ZERO
#define EXCEPTION_FLT_INEXACT_RESULT        STATUS_FLOAT_INEXACT_RESULT
#define EXCEPTION_FLT_INVALID_OPERATION     STATUS_FLOAT_INVALID_OPERATION
#define EXCEPTION_FLT_OVERFLOW              STATUS_FLOAT_OVERFLOW
#define EXCEPTION_FLT_STACK_CHECK           STATUS_FLOAT_STACK_CHECK
#define EXCEPTION_FLT_UNDERFLOW             STATUS_FLOAT_UNDERFLOW
#define EXCEPTION_INT_DIVIDE_BY_ZERO        STATUS_INTEGER_DIVIDE_BY_ZERO
#define EXCEPTION_INT_OVERFLOW              STATUS_INTEGER_OVERFLOW
#define EXCEPTION_PRIV_INSTRUCTION          STATUS_PRIVILEGED_INSTRUCTION
#define EXCEPTION_IN_PAGE_ERROR             STATUS_IN_PAGE_ERROR
#define EXCEPTION_ILLEGAL_INSTRUCTION       STATUS_ILLEGAL_INSTRUCTION
#define EXCEPTION_NONCONTINUABLE_EXCEPTION  STATUS_NONCONTINUABLE_EXCEPTION
#define EXCEPTION_STACK_OVERFLOW            STATUS_STACK_OVERFLOW
#define EXCEPTION_INVALID_DISPOSITION       STATUS_INVALID_DISPOSITION
#define EXCEPTION_GUARD_PAGE                STATUS_GUARD_PAGE_VIOLATION
#define EXCEPTION_INVALID_HANDLE            STATUS_INVALID_HANDLE

#define CONTROL_C_EXIT                      STATUS_CONTROL_C_EXIT

/*  These are from the <FCNTL.H> file in windows.
    They are needed for _open_osfhandle.*/
#define _O_RDONLY   0x0000
#define _O_APPEND   0x0008
#define _O_TEXT     0x4000
#define _O_BINARY   0x8000

size_t GetCurrentSP();

// xplat-todo: implement me
#define IsProcessorFeaturePresent(x) false

#if defined(_ARM_)
#define _ARM_BARRIER_SY 0xF
#define _InstructionSynchronizationBarrier() __isb(_ARM_BARRIER_SY)
#endif

#ifndef MAXUINT16
#define MAXUINT16 ((unsigned short)-1)
#endif

#ifndef MAXUINT8
#define MAXUINT8 ((unsigned char)-1)
#endif

#ifdef  __cplusplus
}

#include "cclock.hpp"
#endif

#endif // __PAL_H__

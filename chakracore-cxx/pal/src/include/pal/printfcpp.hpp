//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
//

/*++



Module Name:

    pal/printfcpp.hpp

Abstract:
    Declarations for suspension safe memory allocation functions



--*/

#ifndef _PRINTFCPP_HPP
#define _PRINTFCPP_HPP

#ifdef __cplusplus
#include "pal/threadinfo.hpp"
#endif

#include <stdarg.h>

#ifndef __ANDROID__
typedef __builtin_va_list va_list;
#else
typedef __va_list va_list;
#endif

#ifdef __cplusplus

extern "C"
{
    int
    PAL__vsnprintf(
        LPSTR Buffer,
        size_t Count,
        LPCSTR Format,
        va_list ap);

    int
    PAL__wvsnprintf(
        LPWSTR Buffer,
        size_t Count,
        LPCWSTR Format,
        va_list ap);

    int
    PAL_vfprintf(
        PAL_FILE *stream,
        const char *format,
        va_list ap);

    int
    PAL_vfwprintf(
        PAL_FILE *stream,
        const char16_t *format,
        va_list ap);
}

namespace CorUnix
{
    int
    InternalVfprintf(
        CPalThread *pthrCurrent,
        PAL_FILE *stream,
        const char *format,
        va_list ap);

    int
    InternalWvsnprintf(
        CPalThread *pthrCurrent,
        LPWSTR Buffer,
        size_t Count,
        LPCWSTR Format,
        va_list ap);

    int
    InternalVsnprintf(
        CPalThread *pthrCurrent,
        LPSTR Buffer,
        size_t Count,
        LPCSTR Format,
        va_list ap);

    int
    InternalVfwprintf(
        CPalThread *pthrCurrent,
        PAL_FILE *stream,
        const char16_t *format,
        va_list ap);

}
#else // __cplusplus

    int
    PAL__vsnprintf(
        LPSTR Buffer,
        size_t Count,
        LPCSTR Format,
        va_list ap);

    int
    PAL__wvsnprintf(
        LPWSTR Buffer,
        size_t Count,
        LPCWSTR Format,
        va_list ap);

    int
    PAL_vfprintf(
        PAL_FILE *stream,
        const char *format,
        va_list ap);

    int
    PAL_vfwprintf(
        PAL_FILE *stream,
        const char16_t *format,
        va_list ap);

#endif // __cplusplus

#endif // _PRINTFCPP_HPP

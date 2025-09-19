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

typedef __builtin_va_list va_list;

#ifdef __cplusplus

extern "C"
{
    int
    PAL__vsnprintf(
        char* Buffer,
        size_t Count,
        LPCSTR Format,
        va_list ap);

    int
    PAL__wvsnprintf(
        char16_t* Buffer,
        size_t Count,
        const char16_t* Format,
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
        char16_t* Buffer,
        size_t Count,
        const char16_t* Format,
        va_list ap);

    int
    InternalVsnprintf(
        CPalThread *pthrCurrent,
        char* Buffer,
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
        char* Buffer,
        size_t Count,
        LPCSTR Format,
        va_list ap);

    int
    PAL__wvsnprintf(
        char16_t* Buffer,
        size_t Count,
        const char16_t* Format,
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

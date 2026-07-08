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

#include "pal/threadinfo.hpp"

#include <stdarg.h>

typedef __builtin_va_list va_list;

extern "C"
{
int
    PAL__wvsnprintf(
        char16_t* Buffer,
        size_t Count,
        const char16_t* Format,
        va_list ap);

    int
    PAL_vfwprintf(
        FILE *stream,
        const char16_t *format,
        va_list ap);
}

namespace CorUnix
{
}

#endif // _PRINTFCPP_HPP

//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
//

/*++



Module Name:

    printf.c

Abstract:

    Implementation of the printf family functions.

Revision History:



--*/

#include <string>
#include "pal/palinternal.h"
#include "pal/dbgmsg.h"
#include "pal/cruntime.h"
#include "pal/thread.hpp"
#include "pal/threadsusp.hpp"
#include "pal/printfcpp.hpp"
#include "chakra/Logger.h"
#include <format>

/* <stdarg.h> needs to be included after "palinternal.h" to avoid name
   collision for va_start and va_end */
#include <cstdio>
#include <stdarg.h>
#include <errno.h>

SET_DEFAULT_DEBUG_CHANNEL(CRT);

/*******************************************************************************
Function:
  PAL_printf_arg_remover

Parameters:
  ap
    - pointer to the va_list from which to remove arguments
  Width
    - the width of the current format operation
  Precision
    - the precision of the current format option
  Type
    - the type of the argument for the current format option
  Prefix
    - the prefix for the current format option
*******************************************************************************/
__attribute__((no_instrument_function))
void PAL_printf_arg_remover(va_list *ap, int32_t Width, int32_t Precision, int32_t Type, int32_t Prefix)
{
    /* remove arg and precision if needed */
    if (PRECISION_STAR == Precision ||
        PRECISION_INVALID == Precision)
    {
        (void)va_arg(*ap, int);
    }
    if (WIDTH_STAR == Width ||
        WIDTH_INVALID == Width)
    {
        (void)va_arg(*ap, int);
    }
    if (Type == PFF_TYPE_FLOAT)
    {
        (void)va_arg(*ap, double);
    }
    else if (Type == PFF_TYPE_INT && Prefix == PFF_PREFIX_LONGLONG)
    {
        (void)va_arg(*ap, int64_t);
    }
    else if (Type == PFF_TYPE_INT || Type == PFF_TYPE_CHAR)
    {
        (void)va_arg(*ap, int);
    }
    else
    {
        (void)va_arg(*ap, void *);
    }
}

/*++
Function:
  fwprintf

See MSDN doc.
--*/
__attribute__((no_instrument_function))
int
PAL_wprintf(
     const char16_t *format,
     ...)
{
    int32_t Length;
    va_list ap;

    va_start(ap, format);
    Length = PAL_vwprintf(format, ap);
    va_end(ap);

    LOGEXIT("PAL_wprintf returns int %d\n", Length);
    return Length;
}

/*++
Function:
  _vsnwprintf

See MSDN doc.
--*/
__attribute__((no_instrument_function))
int
_vsnwprintf(char16_t *buffer,
            size_t count,
            const char16_t *format,
            va_list argptr)
{
    int32_t Length;

    Length = PAL__wvsnprintf(buffer, count, format, argptr);

    LOGEXIT("_vsnwprintf returns int %d\n", Length);

    return Length;
}

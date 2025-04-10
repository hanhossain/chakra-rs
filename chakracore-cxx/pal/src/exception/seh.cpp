//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#include "pal/thread.hpp"
#include "pal/handleapi.hpp"

void
RaiseException( uint32_t dwExceptionCode,
                uint32_t dwExceptionFlags,
                uint32_t nNumberOfArguments,
                const size_t *lpArguments)
{
    if (dwExceptionCode == DBG_TERMINATE_PROCESS || dwExceptionFlags & EXCEPTION_NONCONTINUABLE)
    {
        abort();
    }
}

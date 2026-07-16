//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
//

/*++



Module Name:

    local.c

Abstract:

    Implementation of local memory management functions.

Revision History:



--*/

#include "pal/dbgmsg.h"
#include "pal/palinternal.h"


SET_DEFAULT_DEBUG_CHANNEL(MEM);

extern "C" void *CoTaskMemAlloc(size_t cb)
{
    void *ptr = malloc(cb);
    if (ptr != nullptr)
    {
        memset(ptr, 0, cb);
    }
    return ptr;
}

extern "C" void CoTaskMemFree(void *pv)
{
    if (pv)
    {
        free(pv);
    }
}

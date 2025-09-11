//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information. 
//

/*++



Module Name:

    malloc.cpp

Abstract:

    Implementation of suspension safe memory allocation functions.

Revision History:



--*/

#include "pal/corunix.hpp"
#include "pal/thread.hpp"
#include "pal/malloc.hpp"
#include "pal/dbgmsg.h"

#include <string.h>

SET_DEFAULT_DEBUG_CHANNEL(CRT);

using namespace CorUnix;

void *
PAL_realloc(
    void* pvMemblock,
    size_t szSize
    )
{
    return realloc(pvMemblock, szSize);
}

//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
//

/*++



Module Name:

    heap.c

Abstract:

    Implementation of heap memory management functions.

Revision History:



--*/

#include "pal/palinternal.h"
#include "pal/dbgmsg.h"
#include "pal/handlemgr.hpp"
#include "pal/corunix.hpp"
#include <errno.h>
using namespace CorUnix;

SET_DEFAULT_DEBUG_CHANNEL(MEM);

// In safemath.h, Template SafeInt uses macro _ASSERTE, which need to use variable
// defdbgchan defined by SET_DEFAULT_DEBUG_CHANNEL. Therefore, the include statement
// should be placed after the SET_DEFAULT_DEBUG_CHANNEL(MEM)
// #include <safemath.h>

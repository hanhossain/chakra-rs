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
#include <safemath.h>

#ifndef __APPLE__
#define DUMMY_HEAP 0x01020304
#endif // __APPLE__

#ifdef __APPLE__
#define CACHE_HEAP_ZONE
#endif // __APPLE__

#ifdef CACHE_HEAP_ZONE
/* This is a kludge.
 *
 * We need to know whether an instruction pointer fault is in our executable
 * heap, but the intersection between the HeapX functions on Windows and the
 * malloc_zone functions on Mac OS X are somewhat at odds and we'd have to
 * implement an unnecessarily complicated HeapWalk. Instead, we cache the only
 * "heap" we create, knowing it's the executable heap, and use that instead
 * with the much simpler malloc_zone_from_ptr.
 */
extern malloc_zone_t *s_pExecutableHeap;
malloc_zone_t *s_pExecutableHeap = NULL;
#endif // CACHE_HEAP_ZONE

/*++
Function:
  HeapCreate

See MSDN doc.
--*/
HANDLE
HeapCreate(
	        uint32_t flOptions,
	        size_t dwInitialSize,
	        size_t dwMaximumSize)
{
    HANDLE ret = INVALID_HANDLE_VALUE;
    ENTRY("HeapCreate(flOptions=%#x, dwInitialSize=%u, dwMaximumSize=%u)\n",
        flOptions, dwInitialSize, dwMaximumSize);
#ifdef __APPLE__
    if ((flOptions & 0x40005) != 0)
    {
        ERROR("Invalid flOptions\n");
        SetLastError(ERROR_INVALID_PARAMETER);
    }
    else if (flOptions != 0)
    {
        ERROR("No support for flOptions\n");
        SetLastError(ERROR_INVALID_PARAMETER);
    }
    else if (dwMaximumSize)
    {
        ERROR("Zone implementation does not support a max size\n");
        SetLastError(ERROR_INVALID_PARAMETER);
    }
    else
    {
        ret = (HANDLE)malloc_create_zone(dwInitialSize, 0 /* flags */);
    }

#else // __APPLE__
    ret = (HANDLE)DUMMY_HEAP;
#endif // __APPLE__

    LOGEXIT("HeapCreate returning HANDLE %p\n", ret);
    return ret;
}

#ifdef DEBUG
__attribute__((no_instrument_function, noinline))
static bool PAL_Initialize_Check_Once()
{
    int error = PAL_InitializeChakraCore();
    return error == ERROR_SUCCESS;
}
#endif

/*++
Function:
  GetProcessHeap

See MSDN doc.
--*/
HANDLE
GetProcessHeap(
	       void)
{
    HANDLE ret;
#ifdef DEBUG
    static bool was_pal_initialized = PAL_Initialize_Check_Once();
    _ASSERTE(was_pal_initialized);
#endif

    ENTRY("GetProcessHeap()\n");

#ifdef __APPLE__
#if HEAP_HANDLES_ARE_REAL_HANDLES
#error
#else
    malloc_zone_t *pZone = malloc_default_zone();
    ret = (HANDLE)pZone;
#ifdef CACHE_HEAP_ZONE
    if (s_pExecutableHeap == NULL)
    {
        s_pExecutableHeap = pZone;
        TRACE("s_pExecutableHeap is %p (process heap).\n", s_pExecutableHeap);
    }
#endif // CACHE_HEAP_ZONE
#endif // HEAP_HANDLES_ARE_REAL_HANDLES
#else
    ret = (HANDLE) DUMMY_HEAP;
#endif

    LOGEXIT("GetProcessHeap returning HANDLE %p\n", ret);
    return ret;
}
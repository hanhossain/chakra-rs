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
  RtlMoveMemory

See MSDN doc.
--*/
void
RtlMoveMemory(
           void * Destination,
           const void *Source,
           SIZE_T Length)
{
    PERF_ENTRY(RtlMoveMemory);
    ENTRY("RtlMoveMemory(Destination:%p, Source:%p, Length:%d)\n",
          Destination, Source, Length);

    memmove(Destination, Source, Length);

    LOGEXIT("RtlMoveMemory returning\n");
    PERF_EXIT(RtlMoveMemory);
}

/*++
Function:
  RtlZeroMemory

See MSDN doc.
--*/
void
RtlZeroMemory(
    void * Destination,
    SIZE_T Length
)
{
    PERF_ENTRY(RtlZeroMemory);
    ENTRY("RtlZeroMemory(Destination:%p, Length:%x)\n", Destination, Length);

    memset(Destination, 0, Length);

    LOGEXIT("RtlZeroMemory returning.\n");
    PERF_EXIT(RtlZeroMemory);
}

/*++
Function:
  HeapCreate

See MSDN doc.
--*/
HANDLE
HeapCreate(
	        uint32_t flOptions,
	        SIZE_T dwInitialSize,
	        SIZE_T dwMaximumSize)
{
    HANDLE ret = INVALID_HANDLE_VALUE;
    PERF_ENTRY(HeapCreate);
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
    PERF_EXIT(HeapCreate);
    return ret;
}


__attribute__((no_instrument_function, noinline))
static bool PAL_Initialize_Check_Once()
{
    int error = PAL_InitializeChakraCore();
    return error == ERROR_SUCCESS;
}

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

    PERF_ENTRY(GetProcessHeap);
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
    PERF_EXIT(GetProcessHeap);
    return ret;
}

/*++
Function:
  HeapAlloc

Abstract
  Implemented as wrapper over malloc

See MSDN doc.
--*/
void *
HeapAlloc(
     HANDLE hHeap,
     uint32_t dwFlags,
     SIZE_T numberOfBytes)
{
    uint8_t *pMem;

    PERF_ENTRY(HeapAlloc);
    ENTRY("HeapAlloc (hHeap=%p, dwFlags=%#x, numberOfBytes=%u)\n",
          hHeap, dwFlags, numberOfBytes);

#ifdef __APPLE__
    if (hHeap == NULL)
#else // __APPLE__
    if (hHeap != (HANDLE) DUMMY_HEAP)
#endif // __APPLE__ else
    {
        ERROR("Invalid heap handle\n");
        SetLastError(ERROR_INVALID_PARAMETER);
        LOGEXIT("HeapAlloc returning NULL\n");
        PERF_EXIT(HeapAlloc);
        return NULL;
    }

    if ((dwFlags != 0) && (dwFlags != HEAP_ZERO_MEMORY))
    {
        ASSERT("Invalid parameter dwFlags=%#x\n", dwFlags);
        SetLastError(ERROR_INVALID_PARAMETER);
        LOGEXIT("HeapAlloc returning NULL\n");
        PERF_EXIT(HeapAlloc);
        return NULL;
    }

#ifdef __APPLE__
    // This is patterned off of InternalMalloc in malloc.cpp.
    {
        pMem = (uint8_t *)malloc_zone_malloc((malloc_zone_t *)hHeap, numberOfBytes);
    }
#else // __APPLE__
    pMem = (uint8_t *) PAL_malloc(numberOfBytes);
#endif // __APPLE__ else

    if (pMem == NULL)
    {
        ERROR("Not enough memory\n");
        SetLastError(ERROR_NOT_ENOUGH_MEMORY);
        LOGEXIT("HeapAlloc returning NULL\n");
        PERF_EXIT(HeapAlloc);
        return NULL;
    }

    /* If the HEAP_ZERO_MEMORY flag is set initialize to zero */
    if (dwFlags == HEAP_ZERO_MEMORY)
    {
        memset(pMem, 0, numberOfBytes);
    }

    LOGEXIT("HeapAlloc returning LPVOID %p\n", pMem);
    PERF_EXIT(HeapAlloc);
    return (pMem);
}


/*++
Function:
  HeapFree

Abstract
  Implemented as wrapper over free

See MSDN doc.
--*/
BOOL
HeapFree(
     HANDLE hHeap,
     uint32_t dwFlags,
     void * lpMem)
{
    BOOL bRetVal = FALSE;

    PERF_ENTRY(HeapFree);
    ENTRY("HeapFree (hHeap=%p, dwFlags = %#x, lpMem=%p)\n",
          hHeap, dwFlags, lpMem);

#ifdef __APPLE__
    if (hHeap == NULL)
#else // __APPLE__
    if (hHeap != (HANDLE) DUMMY_HEAP)
#endif // __APPLE__ else
    {
        ERROR("Invalid heap handle\n");
        SetLastError(ERROR_INVALID_PARAMETER);
        goto done;
    }

    if (dwFlags != 0)
    {
        ASSERT("Invalid parameter dwFlags=%#x\n", dwFlags);
        SetLastError(ERROR_INVALID_PARAMETER);
        goto done;
    }

    if (!lpMem)
    {
        bRetVal = TRUE;
        goto done;
    }

    bRetVal = TRUE;
#ifdef __APPLE__
    // This is patterned off of InternalFree in malloc.cpp.
    {
        malloc_zone_free((malloc_zone_t *)hHeap, lpMem);
    }
#else // __APPLE__
    PAL_free (lpMem);
#endif // __APPLE__ else

done:
    LOGEXIT( "HeapFree returning BOOL %d\n", bRetVal );
    PERF_EXIT(HeapFree);
    return bRetVal;
}


/*++
Function:
  HeapReAlloc

Abstract
  Implemented as wrapper over realloc

See MSDN doc.
--*/
void *
HeapReAlloc(
     HANDLE hHeap,
     uint32_t dwFlags,
     void * lpmem,
     SIZE_T numberOfBytes)
{
    uint8_t *pMem = NULL;

    PERF_ENTRY(HeapReAlloc);
    ENTRY("HeapReAlloc (hHeap=%p, dwFlags=%#x, lpmem=%p, numberOfBytes=%u)\n",
          hHeap, dwFlags, lpmem, numberOfBytes);

#ifdef __APPLE__
    if (hHeap == NULL)
#else // __APPLE__
    if (hHeap != (HANDLE)DUMMY_HEAP)
#endif // __APPLE__ else
    {
        ASSERT("Invalid heap handle\n");
        SetLastError(ERROR_INVALID_HANDLE);
        goto done;
    }

    if ((dwFlags != 0))
    {
        ASSERT("Invalid parameter dwFlags=%#x\n", dwFlags);
        SetLastError(ERROR_INVALID_PARAMETER);
        goto done;
    }

    if (lpmem == NULL)
    {
        WARN("NULL memory pointer to realloc. Do not do anything.\n");
        /* set LastError back to zero. this appears to be an undocumented
        behavior in Windows, in doesn't cost much to match it */
        SetLastError(0);
        goto done;
    }

    if(numberOfBytes == 0)
    {
        // PAL's realloc behaves like free for a requested size of zero bytes. Force a nonzero size to get a valid pointer.
        numberOfBytes = 1;
    }

#ifdef __APPLE__
    // This is patterned off of InternalRealloc in malloc.cpp.
    {
        pMem = (uint8_t *) malloc_zone_realloc((malloc_zone_t *)hHeap, lpmem, numberOfBytes);
    }
#else // __APPLE__
    pMem = (uint8_t *) PAL_realloc(lpmem, numberOfBytes);
#endif // __APPLE__ else

    if (pMem == NULL)
    {
        ERROR("Not enough memory\n");
        SetLastError(ERROR_NOT_ENOUGH_MEMORY);
        goto done;
    }

done:
    LOGEXIT("HeapReAlloc returns LPVOID %p\n", pMem);
    PERF_EXIT(HeapReAlloc);
    return pMem;
}

BOOL
HeapSetInformation(
          HANDLE HeapHandle,
         HEAP_INFORMATION_CLASS HeapInformationClass,
         void * HeapInformation,
         SIZE_T HeapInformationLength)
{
    return TRUE;
}

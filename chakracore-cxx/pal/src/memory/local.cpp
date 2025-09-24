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

#include "pal/palinternal.h"
#include "pal/dbgmsg.h"


SET_DEFAULT_DEBUG_CHANNEL(MEM);

static
int
AllocFlagsToHeapAllocFlags (  uint32_t  AllocFlags,
                        uint32_t * pHeapallocFlags)
{
    int success = 1;
    uint32_t newFlags = 0, flags = AllocFlags;
    if (flags & LMEM_ZEROINIT) {
        newFlags |= HEAP_ZERO_MEMORY;
        flags &= ~LMEM_ZEROINIT;
    }
    if (flags != 0) {
        ASSERT("Invalid parameter AllocFlags=0x%x\n", AllocFlags);
        SetLastError(ERROR_INVALID_PARAMETER);
        success = 0;
    }
    if (success) {
        *pHeapallocFlags = newFlags;
    }
    return success;
}

/*++
Function:
  LocalFree

See MSDN doc.
--*/
HLOCAL
LocalFree(
	   HLOCAL hMem)
{
    BOOL bRetVal = FALSE;
    ENTRY("LocalFree (hmem=%p)\n", hMem);

    if ( hMem )
    {
        bRetVal = HeapFree( GetProcessHeap(), 0, hMem );
    }
    else
    {
        bRetVal = TRUE;
    }
    
    LOGEXIT( "LocalFree returning %p.\n", bRetVal == TRUE ? (HLOCAL)NULL : hMem );
    return bRetVal == TRUE ? (HLOCAL)NULL : hMem;
}

extern "C" void * CoTaskMemAlloc( size_t cb)
{
    return HeapAlloc(GetProcessHeap(), 0, cb);
}

extern "C" void * CoTaskMemRealloc( void * pv,  size_t cb)
{
    return HeapReAlloc(GetProcessHeap(), 0, pv, cb);
}

extern "C" void CoTaskMemFree( void * pv)
{
    HeapFree(GetProcessHeap(), 0, pv);
}

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

//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information. 
//

/*++



Module Name:

    common.c

Abstract:

    Implementation of the common mapping functions.



--*/

#include "pal/palinternal.h"
#include "pal/dbgmsg.h"

#include "common.h"

#include <sys/mman.h>

SET_DEFAULT_DEBUG_CHANNEL(VIRTUAL);

/*****
 *
 * W32toUnixAccessControl( uint32_t ) - Maps Win32 to Unix memory access controls .
 *
 */
int32_t W32toUnixAccessControl(  uint32_t flProtect )
{
    int32_t MemAccessControl = 0;

    switch ( flProtect & 0xff )
    {
    case PAGE_READONLY :
        MemAccessControl = PROT_READ;
        break;
    case PAGE_READWRITE :
        MemAccessControl = PROT_READ | PROT_WRITE;
        break;
    case PAGE_EXECUTE_READWRITE:
        MemAccessControl = PROT_EXEC | PROT_READ | PROT_WRITE;
        break;
    case PAGE_EXECUTE :
        MemAccessControl = PROT_EXEC | PROT_READ; // WinAPI PAGE_EXECUTE also implies readable
        break;
    case PAGE_EXECUTE_READ :
        MemAccessControl = PROT_EXEC | PROT_READ;
        break;
    case PAGE_NOACCESS :
        MemAccessControl = PROT_NONE;
        break;
    
    default:
        MemAccessControl = 0;
        ERROR( "Incorrect or no protection flags specified.\n" );
        break;
    }
    return MemAccessControl;
}

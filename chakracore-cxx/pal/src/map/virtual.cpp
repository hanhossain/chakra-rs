//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
//

/*++



Module Name:

    virtual.c

Abstract:

    Implementation of virtual memory management functions.



--*/

#include "pal/thread.hpp"
#include "pal/cs.hpp"
#include "pal/malloc.hpp"
#include "pal/file.hpp"
#include "pal/dbgmsg.h"
#include "pal/virtual.h"
#include "pal/map.h"
#include "pal/init.h"
#include "common.h"

#include <sys/types.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#if HAVE_VM_ALLOCATE
#include <mach/vm_map.h>
#include <mach/mach_init.h>
#endif // HAVE_VM_ALLOCATE

using namespace CorUnix;

SET_DEFAULT_DEBUG_CHANNEL(VIRTUAL);

CRITICAL_SECTION virtual_critsec PAL_GLOBAL;

#ifdef DEBUG
Volatile<int> attempt1 = 0;
Volatile<int> attempt2 = 0;
#endif

#if MMAP_IGNORES_HINT
typedef struct FREE_BLOCK {
    char *startBoundary;
    SIZE_T memSize;
    struct FREE_BLOCK *next;
} FREE_BLOCK;
#endif  // MMAP_IGNORES_HINT

// The first node in our list of allocated blocks.
static PCMI pVirtualMemoryLastFound;
static PCMI pVirtualMemory;

#if MMAP_IGNORES_HINT
// The first node in our list of freed blocks.
static FREE_BLOCK *pFreeMemory PAL_GLOBAL;

// The amount of memory that we'll try to reserve on our file.
// Currently 1GB.
static const int BACKING_FILE_SIZE = 1024 * 1024 * 1024;

static void *VIRTUALReserveFromBackingFile(UINT_PTR addr, size_t length);
static BOOL VIRTUALAddToFreeList(const PCMI pMemoryToBeReleased);

// The base address of the pages mapped onto our backing file.
static void *gBackingBaseAddress = MAP_FAILED;

// Separate the subset of the feature for experiments
#define RESERVE_FROM_BACKING_FILE 1
#else
// static const void *gBackingBaseAddress = MAP_FAILED;
// #define RESERVE_FROM_BACKING_FILE 1
#endif

#if RESERVE_FROM_BACKING_FILE
static BOOL VIRTUALGetBackingFile(CPalThread * pthrCurrent);

// The file that we're using to back our pages.
static int gBackingFile PAL_GLOBAL = -1;
#endif // RESERVE_FROM_BACKING_FILE

/* We need MAP_ANON. However on some platforms like HP-UX, it is defined as MAP_ANONYMOUS */
#if !defined(MAP_ANON) && defined(MAP_ANONYMOUS)
#define MAP_ANON MAP_ANONYMOUS
#endif

/*++
Function:
    ReserveVirtualMemory()

    Helper function that is used by Virtual* APIs and ExecutableMemoryAllocator
    to reserve virtual memory from the OS.

--*/
static void * ReserveVirtualMemory(
                CPalThread *pthrCurrent, /* Currently executing thread */
                void * lpAddress,        /* Region to reserve or commit */
                SIZE_T dwSize);          /* Size of Region */

/*++
Function:
    VIRTUALInitialize()

    Initializes this section's critical section.

Return value:
    TRUE  if initialization succeeded
    FALSE otherwise.

--*/
extern "C"
BOOL
VIRTUALInitialize( void )
{
    TRACE( "Initializing the Virtual Critical Sections. \n" );

    InternalInitializeCriticalSection(&virtual_critsec);

    pVirtualMemory = NULL;
    pVirtualMemoryLastFound = NULL;

    return TRUE;
}

/***
 *
 * VIRTUALCleanup()
 *      Deletes this section's critical section.
 *
 */
extern "C"
void VIRTUALCleanup()
{
    PCMI pEntry;
    PCMI pTempEntry;
#if MMAP_IGNORES_HINT
    FREE_BLOCK *pFreeBlock;
    FREE_BLOCK *pTempFreeBlock;
#endif  // MMAP_IGNORES_HINT
    CPalThread * pthrCurrent = InternalGetCurrentThread();

    InternalEnterCriticalSection(pthrCurrent, &virtual_critsec);

    // Clean up the allocated memory.
    pEntry = pVirtualMemory;
    while ( pEntry )
    {
        WARN( "The memory at %d was not freed through a call to VirtualFree.\n",
              pEntry->startBoundary );
        InternalFree(pEntry->pAllocState);
        InternalFree(pEntry->pProtectionState );
#if MMAP_DOESNOT_ALLOW_REMAP
        InternalFree(pEntry->pDirtyPages );
#endif
        pTempEntry = pEntry;
        pEntry = pEntry->pNext;
        InternalFree(pTempEntry );
    }
    pVirtualMemory = NULL;
    pVirtualMemoryLastFound = NULL;

#if MMAP_IGNORES_HINT
    // Clean up the free list.
    pFreeBlock = pFreeMemory;
    while (pFreeBlock != NULL)
    {
        // Ignore errors from munmap. There's nothing we'd really want to
        // do about them.
        munmap(pFreeBlock->startBoundary, pFreeBlock->memSize);
        pTempFreeBlock = pFreeBlock;
        pFreeBlock = pFreeBlock->next;
        InternalFree(pTempFreeBlock);
    }
    pFreeMemory = NULL;
    gBackingBaseAddress = MAP_FAILED;
#endif  // MMAP_IGNORES_HINT

#if RESERVE_FROM_BACKING_FILE
    if (gBackingFile != -1)
    {
        close(gBackingFile);
        gBackingFile = -1;
    }
#endif  // RESERVE_FROM_BACKING_FILE

    InternalLeaveCriticalSection(pthrCurrent, &virtual_critsec);

    TRACE( "Deleting the Virtual Critical Sections. \n" );
    DeleteCriticalSection( &virtual_critsec );
}

/***
 *
 *  VIRTUALContainsInvalidProtectionFlags()
 *          Returns TRUE if an invalid flag is specified. FALSE otherwise.
 */
static BOOL VIRTUALContainsInvalidProtectionFlags(  uint32_t flProtect )
{
    if ( ( flProtect & ~( PAGE_NOACCESS | PAGE_READONLY |
                          PAGE_READWRITE | PAGE_EXECUTE | PAGE_EXECUTE_READ |
                          PAGE_EXECUTE_READWRITE ) ) != 0 )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


/****
 *
 * VIRTUALIsPageCommitted
 *
 *  SIZE_T nBitToRetrieve - Which page to check.
 *
 *  Returns TRUE if committed, FALSE otherwise.
 *
 */
static BOOL VIRTUALIsPageCommitted( SIZE_T nBitToRetrieve, const PCMI pInformation )
{
    SIZE_T nByteOffset = 0;
    uint32_t nBitOffset = 0;
    uint32_t byteMask = 0;

    if ( !pInformation )
    {
        ERROR( "pInformation was NULL!\n" );
        return FALSE;
    }

    nByteOffset = nBitToRetrieve / CHAR_BIT;
    nBitOffset = nBitToRetrieve % CHAR_BIT;

    byteMask = 1 << nBitOffset;

    if ( pInformation->pAllocState[ nByteOffset ] & byteMask )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

#if MMAP_DOESNOT_ALLOW_REMAP
/****
 *
 * VIRTUALIsPageDirty
 *
 *  SIZE_T nBitToRetrieve - Which page to check.
 *
 *  Returns TRUE if the page needs to be cleared if re-committed,
 *  FALSE otherwise.
 *
 */
static BOOL VIRTUALIsPageDirty( SIZE_T nBitToRetrieve, const PCMI pInformation )
{
    SIZE_T nByteOffset = 0;
    uint32_t nBitOffset = 0;
    uint32_t byteMask = 0;

    if ( !pInformation )
    {
        ERROR( "pInformation was NULL!\n" );
        return FALSE;
    }

    nByteOffset = nBitToRetrieve / CHAR_BIT;
    nBitOffset = nBitToRetrieve % CHAR_BIT;

    byteMask = 1 << nBitOffset;

    if ( pInformation->pDirtyPages[ nByteOffset ] & byteMask )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
#endif // MMAP_DOESNOT_ALLOW_REMAP

/*********
 *
 *  VIRTUALGetAllocationType
 *
 *      IN SIZE_T Index - The page within the range to retrieve
 *                      the state for.
 *
 *      IN pInformation - The virtual memory object.
 *
 */
static int32_t VIRTUALGetAllocationType( SIZE_T Index, const PCMI pInformation )
{
    if ( VIRTUALIsPageCommitted( Index, pInformation ) )
    {
        return MEM_COMMIT;
    }
    else
    {
        return MEM_RESERVE;
    }
}

/****
 *
 * VIRTUALSetPageBits
 *
 *  IN uint32_t nStatus - Bit set / reset [0: reset, any other value: set].
 *  IN SIZE_T nStartingBit - The bit to set.
 *
 *  IN SIZE_T nNumberOfBits - The range of bits to set.
 *  IN uint8_t* pBitArray - A pointer the array to be manipulated.
 *
 *  Returns TRUE on success, FALSE otherwise.
 *  Turn on/off memory staus bits.
 *
 */
static BOOL VIRTUALSetPageBits ( uint32_t nStatus, SIZE_T nStartingBit,
                                 SIZE_T nNumberOfBits, uint8_t * pBitArray )
{
    /* byte masks for optimized modification of partial bytes (changing less
       than 8 bits in a single byte). note that bits are treated in little
       endian order : value 1 is bit 0; value 128 is bit 7. in the binary
       representations below, bit 0 is on the right */

    /* start masks : for modifying bits >= n while preserving bits < n.
       example : if nStartignBit%8 is 3, then bits 0, 1, 2 remain unchanged
       while bits 3..7 are changed; startmasks[3] can be used for this.  */
    static const uint8_t startmasks[8] = {
      0xff, /* start at 0 : 1111 1111 */
      0xfe, /* start at 1 : 1111 1110 */
      0xfc, /* start at 2 : 1111 1100 */
      0xf8, /* start at 3 : 1111 1000 */
      0xf0, /* start at 4 : 1111 0000 */
      0xe0, /* start at 5 : 1110 0000 */
      0xc0, /* start at 6 : 1100 0000 */
    0x80  /* start at 7 : 1000 0000 */
    };

    /* end masks : for modifying bits <= n while preserving bits > n.
       example : if the last bit to change is 5, then bits 6 & 7 stay unchanged
       while bits 1..5 are changed; endmasks[5] can be used for this.  */
    static const uint8_t endmasks[8] = {
      0x01, /* end at 0 : 0000 0001 */
      0x03, /* end at 1 : 0000 0011 */
      0x07, /* end at 2 : 0000 0111 */
      0x0f, /* end at 3 : 0000 1111 */
      0x1f, /* end at 4 : 0001 1111 */
      0x3f, /* end at 5 : 0011 1111 */
      0x7f, /* end at 6 : 0111 1111 */
      0xff  /* end at 7 : 1111 1111 */
    };
    /* last example : if only the middle of a byte must be changed, both start
       and end masks can be combined (bitwise AND) to obtain the correct mask.
       if we want to change bits 2 to 4 :
       startmasks[2] : 0xfc   1111 1100  (change 2,3,4,5,6,7)
       endmasks[4]:    0x1f   0001 1111  (change 0,1,2,3,4)
       bitwise AND :   0x1c   0001 1100  (change 2,3,4)
    */

    uint8_t byte_mask;
    SIZE_T nLastBit;
    SIZE_T nFirstByte;
    SIZE_T nLastByte;
    SIZE_T nFullBytes;

    TRACE( "VIRTUALSetPageBits( nStatus = %d, nStartingBit = %d, "
           "nNumberOfBits = %d, pBitArray = 0x%p )\n",
           nStatus, nStartingBit, nNumberOfBits, pBitArray );

    if ( 0 == nNumberOfBits )
    {
        ERROR( "nNumberOfBits was 0!\n" );
        return FALSE;
    }

    nLastBit = nStartingBit+nNumberOfBits-1;
    nFirstByte = nStartingBit / 8;
    nLastByte = nLastBit / 8;

    /* handle partial first byte (if any) */
    if(0 != (nStartingBit % 8))
    {
        byte_mask = startmasks[nStartingBit % 8];

        /* if 1st byte is the only changing byte, combine endmask to preserve
           trailing bits (see 3rd example above) */
        if( nLastByte == nFirstByte)
        {
            byte_mask &= endmasks[nLastBit % 8];
        }

        /* byte_mask contains 1 for bits to change, 0 for bits to leave alone */
        if(0 == nStatus)
        {
            /* bits to change must be set to 0 : invert byte_mask (giving 0 for
               bits to change), use bitwise AND */
            pBitArray[nFirstByte] &= ~byte_mask;
        }
        else
        {
            /* bits to change must be set to 1 : use bitwise OR */
            pBitArray[nFirstByte] |= byte_mask;
        }

        /* stop right away if only 1 byte is being modified */
        if(nLastByte == nFirstByte)
        {
            return TRUE;
        }

        /* we're done with the 1st byte; skip over it */
        nFirstByte++;
    }

    /* number of bytes to change, excluding the last byte (handled separately)*/
    nFullBytes = nLastByte - nFirstByte;

    if(0 != nFullBytes)
    {
        // Turn off/on dirty bits
        memset( &(pBitArray[nFirstByte]), (0 == nStatus) ? 0 : 0xFF, nFullBytes );
    }

    /* handle last (possibly partial) byte */
    byte_mask = endmasks[nLastBit % 8];

    /* byte_mask contains 1 for bits to change, 0 for bits to leave alone */
    if(0 == nStatus)
    {
        /* bits to change must be set to 0 : invert byte_mask (giving 0 for
           bits to change), use bitwise AND */
        pBitArray[nLastByte] &= ~byte_mask;
    }
    else
    {
        /* bits to change must be set to 1 : use bitwise OR */
        pBitArray[nLastByte] |= byte_mask;
    }

    return TRUE;
}

/****
 *
 * VIRTUALSetAllocState
 *
 *  IN uint32_t nAction - Which action to perform.
 *  IN SIZE_T nStartingBit - The bit to set.
 *
 *  IN SIZE_T nNumberOfBits - The range of bits to set.
 *  IN PCMI pStateArray - A pointer the array to be manipulated.
 *
 *  Returns TRUE on success, FALSE otherwise.
 *  Turn bit on to indicate committed, turn bit off to indicate reserved.
 *
 */
static BOOL VIRTUALSetAllocState( uint32_t nAction, SIZE_T nStartingBit,
                           SIZE_T nNumberOfBits, const PCMI pInformation )
{
    TRACE( "VIRTUALSetAllocState( nAction = %d, nStartingBit = %d, "
           "nNumberOfBits = %d, pStateArray = 0x%p )\n",
           nAction, nStartingBit, nNumberOfBits, pInformation );

    if ( !pInformation )
    {
        ERROR( "pInformation was invalid!\n" );
        return FALSE;
    }

    return VIRTUALSetPageBits((MEM_COMMIT == nAction) ? 1 : 0, nStartingBit,
                              nNumberOfBits, pInformation->pAllocState);
}

#if MMAP_DOESNOT_ALLOW_REMAP
/****
 *
 * VIRTUALSetDirtyPages
 *
 *  IN uint32_t nStatus - 0: memory clean, any other value: memory is dirty
 *  IN SIZE_T nStartingBit - The bit to set.
 *
 *  IN SIZE_T nNumberOfBits - The range of bits to set.
 *  IN PCMI pStateArray - A pointer the array to be manipulated.
 *
 *  Returns TRUE on success, FALSE otherwise.
 *  Turns bit(s) on/off bit to indicate dirty page(s)
 *
 */
static BOOL VIRTUALSetDirtyPages( uint32_t nStatus, SIZE_T nStartingBit,
                           SIZE_T nNumberOfBits, const PCMI pInformation )
{
    TRACE( "VIRTUALSetDirtyPages( nStatus = %d, nStartingBit = %d, "
           "nNumberOfBits = %d, pStateArray = 0x%p )\n",
           nStatus, nStartingBit, nNumberOfBits, pInformation );

    if ( !pInformation )
    {
        ERROR( "pInformation was invalid!\n" );
        return FALSE;
    }

    return VIRTUALSetPageBits(nStatus, nStartingBit,
                              nNumberOfBits, pInformation->pDirtyPages);
}
#endif // MMAP_DOESNOT_ALLOW_REMAP


/****
 *
 * VIRTUALFindRegionInformation( )
 *
 *          IN UINT_PTR address - The address to look for.
 *
 *          Returns the PCMI if found, NULL otherwise.
 */
static PCMI VIRTUALFindRegionInformation(  UINT_PTR address )
{
    PCMI pEntry = NULL;

    TRACE( "VIRTUALFindRegionInformation( %#x )\n", address );

    pEntry = pVirtualMemory;

    if (pVirtualMemoryLastFound && pVirtualMemoryLastFound->startBoundary <= address)
    {
        pEntry = pVirtualMemoryLastFound;
        if (pEntry->startBoundary == address)
        {
            return pEntry;
        }
    }
    else
    {
        pEntry = pVirtualMemory;
    }

    while( pEntry )
    {
        if ( pEntry->startBoundary > address )
        {
            /* Gone past the possible location in the list. */
            pEntry = NULL;
            break;
        }
        if ( pEntry->startBoundary + pEntry->memSize > address )
        {
            break;
        }

        pEntry = pEntry->pNext;
    }

    if (pEntry) pVirtualMemoryLastFound = pEntry;
    return pEntry;
}

/*++
Function :
    VIRTUALOwnedRegion

    Returns whether the space in question is owned the VIRTUAL system.

--*/
BOOL VIRTUALOwnedRegion(  UINT_PTR address )
{
    PCMI pEntry = NULL;
    CPalThread * pthrCurrent = InternalGetCurrentThread();

    InternalEnterCriticalSection(pthrCurrent, &virtual_critsec);
    pEntry = VIRTUALFindRegionInformation( address );
    InternalLeaveCriticalSection(pthrCurrent, &virtual_critsec);

    return pEntry != NULL;
}

/*++
Function :

    VIRTUALReleaseMemory

    Removes a PCMI entry from the list.

    Returns true on success. FALSE otherwise.
--*/
static BOOL VIRTUALReleaseMemory( PCMI pMemoryToBeReleased )
{
    BOOL bRetVal = TRUE;

    if ( !pMemoryToBeReleased )
    {
        ASSERT( "Invalid pointer.\n" );
        return FALSE;
    }

    if ( pMemoryToBeReleased == pVirtualMemory )
    {
        /* This is either the first entry, or the only entry. */
        pVirtualMemory = pMemoryToBeReleased->pNext;
        if ( pMemoryToBeReleased->pNext )
        {
            pMemoryToBeReleased->pNext->pLast = NULL;
        }
    }
    else /* Could be anywhere in the list. */
    {
        /* Delete the entry from the linked list. */
        if ( pMemoryToBeReleased->pLast )
        {
            pMemoryToBeReleased->pLast->pNext = pMemoryToBeReleased->pNext;
        }

        if ( pMemoryToBeReleased->pNext )
        {
            pMemoryToBeReleased->pNext->pLast = pMemoryToBeReleased->pLast;
        }
    }

    if (pVirtualMemoryLastFound->startBoundary >= pMemoryToBeReleased->startBoundary)
    {
        pVirtualMemoryLastFound = NULL;
    }

#if MMAP_IGNORES_HINT
    // We've removed the block from our allocated list. Add it to the
    // free list.
    bRetVal = VIRTUALAddToFreeList(pMemoryToBeReleased);
#endif  // MMAP_IGNORES_HINT

    InternalFree( pMemoryToBeReleased->pAllocState );
    pMemoryToBeReleased->pAllocState = NULL;

    InternalFree( pMemoryToBeReleased->pProtectionState );
    pMemoryToBeReleased->pProtectionState = NULL;

#if MMAP_DOESNOT_ALLOW_REMAP
    InternalFree( pMemoryToBeReleased->pDirtyPages );
    pMemoryToBeReleased->pDirtyPages = NULL;
#endif // MMAP_DOESNOT_ALLOW_REMAP

    InternalFree( pMemoryToBeReleased );
    pMemoryToBeReleased = NULL;

    return bRetVal;
}

/****
 *  VIRTUALConvertWinFlags() -
 *          Converts win32 protection flags to
 *          internal VIRTUAL flags.
 *
 */
static uint8_t VIRTUALConvertWinFlags(  uint32_t flProtect )
{
    uint8_t MemAccessControl = 0;

    switch ( flProtect & 0xff )
    {
    case PAGE_NOACCESS :
        MemAccessControl = VIRTUAL_NOACCESS;
        break;
    case PAGE_READONLY :
        MemAccessControl = VIRTUAL_READONLY;
        break;
    case PAGE_READWRITE :
        MemAccessControl = VIRTUAL_READWRITE;
        break;
    case PAGE_EXECUTE :
        MemAccessControl = VIRTUAL_EXECUTE;
        break;
    case PAGE_EXECUTE_READ :
        MemAccessControl = VIRTUAL_EXECUTE_READ;
        break;
    case PAGE_EXECUTE_READWRITE:
        MemAccessControl = VIRTUAL_EXECUTE_READWRITE;
        break;

    default :
        MemAccessControl = 0;
        ERROR( "Incorrect or no protection flags specified.\n" );
        break;
    }
    return MemAccessControl;
}
/****
 *  VIRTUALConvertVirtualFlags() -
 *              Converts internal virtual protection
 *              flags to their win32 counterparts.
 */
static uint32_t VIRTUALConvertVirtualFlags(  uint8_t VirtualProtect )
{
    uint32_t MemAccessControl = 0;

    if ( VirtualProtect == VIRTUAL_READONLY )
    {
        MemAccessControl = PAGE_READONLY;
    }
    else if ( VirtualProtect == VIRTUAL_READWRITE )
    {
        MemAccessControl = PAGE_READWRITE;
    }
    else if ( VirtualProtect == VIRTUAL_EXECUTE_READWRITE )
    {
        MemAccessControl = PAGE_EXECUTE_READWRITE;
    }
    else if ( VirtualProtect == VIRTUAL_EXECUTE_READ )
    {
        MemAccessControl = PAGE_EXECUTE_READ;
    }
    else if ( VirtualProtect == VIRTUAL_EXECUTE )
    {
        MemAccessControl = PAGE_EXECUTE;
    }
    else if ( VirtualProtect == VIRTUAL_NOACCESS )
    {
        MemAccessControl = PAGE_NOACCESS;
    }

    else
    {
        MemAccessControl = 0;
        ERROR( "Incorrect or no protection flags specified.\n" );
    }
    return MemAccessControl;
}

/****
 *  VIRTUALStoreAllocationInfo()
 *
 *      Stores the allocation information in the linked list.
 *      NOTE: The caller must own the critical section.
 */
static BOOL VIRTUALStoreAllocationInfo(
             UINT_PTR startBoundary,      /* Start of the region. */
             SIZE_T memSize,            /* Size of the region. */
             uint32_t flAllocationType,  /* Allocation Types. */
             uint32_t flProtection )     /* Protections flags on the memory. */
{
    PCMI pNewEntry       = NULL;
    PCMI pMemInfo        = NULL;
    BOOL bRetVal         = TRUE;
    SIZE_T nBufferSize   = 0;

    if ( ( memSize & VIRTUAL_PAGE_MASK ) != 0 )
    {
        ERROR( "The memory size was not in multiples of the page size. \n" );
        bRetVal =  FALSE;
        goto done;
    }

    if ( !(pNewEntry = ( PCMI )InternalMalloc( sizeof( *pNewEntry )) ) )
    {
        ERROR( "Unable to allocate memory for the structure.\n");
        bRetVal =  FALSE;
        goto done;
    }

    pNewEntry->startBoundary    = startBoundary;
    pNewEntry->memSize          = memSize;
    pNewEntry->allocationType   = flAllocationType;
    pNewEntry->accessProtection = flProtection;

    nBufferSize = memSize / VIRTUAL_PAGE_SIZE / CHAR_BIT;
    if ( ( memSize / VIRTUAL_PAGE_SIZE ) % CHAR_BIT != 0 )
    {
        nBufferSize++;
    }

    pNewEntry->pAllocState      = (uint8_t*)InternalMalloc( nBufferSize  );
    pNewEntry->pProtectionState = (uint8_t*)InternalMalloc( (memSize / VIRTUAL_PAGE_SIZE)  );
#if MMAP_DOESNOT_ALLOW_REMAP
    pNewEntry->pDirtyPages  = (uint8_t*)InternalMalloc( nBufferSize );
#endif //

    if ( pNewEntry->pAllocState && pNewEntry->pProtectionState
#if MMAP_DOESNOT_ALLOW_REMAP
        && pNewEntry->pDirtyPages
#endif // MMAP_DOESNOT_ALLOW_REMAP
      )
    {
        /* Set the intial allocation state, and initial allocation protection. */
#if MMAP_DOESNOT_ALLOW_REMAP
        memset (pNewEntry->pDirtyPages, 0, nBufferSize);
#endif // MMAP_DOESNOT_ALLOW_REMAP
        VIRTUALSetAllocState( MEM_RESERVE, 0, nBufferSize * CHAR_BIT, pNewEntry );
        memset( pNewEntry->pProtectionState,
            VIRTUALConvertWinFlags( flProtection ),
            memSize / VIRTUAL_PAGE_SIZE );
    }
    else
    {
        ERROR( "Unable to allocate memory for the structure.\n");
        bRetVal =  FALSE;

#if MMAP_DOESNOT_ALLOW_REMAP
        if (pNewEntry->pDirtyPages) InternalFree( pNewEntry->pDirtyPages );
        pNewEntry->pDirtyPages = NULL;
#endif //

        if (pNewEntry->pProtectionState) InternalFree( pNewEntry->pProtectionState );
        pNewEntry->pProtectionState = NULL;

        if (pNewEntry->pAllocState) InternalFree( pNewEntry->pAllocState );
        pNewEntry->pAllocState = NULL;

        InternalFree( pNewEntry );
        pNewEntry = NULL;

        goto done;
    }

    pMemInfo = pVirtualMemory;

    if ( pMemInfo && pMemInfo->startBoundary < startBoundary )
    {
        /* Look for the correct insert point */
        TRACE( "Looking for the correct insert location.\n");
        while ( pMemInfo->pNext && ( pMemInfo->pNext->startBoundary < startBoundary ) )
        {
            pMemInfo = pMemInfo->pNext;
        }

        pNewEntry->pNext = pMemInfo->pNext;
        pNewEntry->pLast = pMemInfo;

        if ( pNewEntry->pNext )
        {
            pNewEntry->pNext->pLast = pNewEntry;
        }

        pMemInfo->pNext = pNewEntry;
    }
    else
    {
        TRACE( "Inserting a new element into the linked list\n" );
        /* This is the first entry in the list. */
        pNewEntry->pNext = pMemInfo;
        pNewEntry->pLast = NULL;

        if ( pNewEntry->pNext )
        {
            pNewEntry->pNext->pLast = pNewEntry;
        }

        pVirtualMemory = pNewEntry ;
    }
done:
    TRACE( "Exiting StoreAllocationInformation. \n" );
    return bRetVal;
}

/****
 *  VIRTUALUpdateAllocationInfo()
 *
 *      Updates the allocation information in the linked list for an existing region.
 *      NOTE: The caller must own the critical section virtual_critsec.
 */
static BOOL VIRTUALUpdateAllocationInfo(
     PCMI pExistingEntry,         /* Existing entry in the virtual memory regions list that we are currently managing. */
     UINT_PTR startBoundary,      /* The new starting address of the region. */
     SIZE_T memSize)              /* The new size of the region. */
{
    BOOL bRetVal = TRUE;
    SIZE_T nBufferSize = 0;

    if ((memSize & VIRTUAL_PAGE_MASK) != 0)
    {
        ERROR("The memory size was not in multiples of the page size. \n");
        bRetVal = FALSE;
        goto done;
    }

    if (!pExistingEntry)
    {
        ERROR("Existing region not provided.\n");
        bRetVal = FALSE;
        goto done;
    }

    pExistingEntry->startBoundary = startBoundary;
    pExistingEntry->memSize = memSize;

    nBufferSize = memSize / VIRTUAL_PAGE_SIZE / CHAR_BIT;
    if ((memSize / VIRTUAL_PAGE_SIZE) % CHAR_BIT != 0)
    {
        nBufferSize++;
    }

    // Cleaup previous structures as we will need to reinitialize these using the new size of the region.
    {
#if MMAP_DOESNOT_ALLOW_REMAP
        if (pExistingEntry->pDirtyPages) InternalFree(pExistingEntry->pDirtyPages);
        pExistingEntry->pDirtyPages = NULL;
#endif //  MMAP_DOESNOT_ALLOW_REMAP

        if (pExistingEntry->pProtectionState) InternalFree(pExistingEntry->pProtectionState);
        pExistingEntry->pProtectionState = NULL;

        if (pExistingEntry->pAllocState) InternalFree(pExistingEntry->pAllocState);
        pExistingEntry->pAllocState = NULL;
    }

    pExistingEntry->pAllocState = (uint8_t*)InternalMalloc(nBufferSize);
    pExistingEntry->pProtectionState = (uint8_t*)InternalMalloc((memSize / VIRTUAL_PAGE_SIZE));
#if MMAP_DOESNOT_ALLOW_REMAP
    pExistingEntry->pDirtyPages = (uint8_t*)InternalMalloc(nBufferSize);
#endif //  MMAP_DOESNOT_ALLOW_REMAP

    if (pExistingEntry->pAllocState && pExistingEntry->pProtectionState
#if MMAP_DOESNOT_ALLOW_REMAP
        && pExistingEntry->pDirtyPages
#endif // MMAP_DOESNOT_ALLOW_REMAP
        )
    {
        /* Set the intial allocation state, and initial allocation protection. */
#if MMAP_DOESNOT_ALLOW_REMAP
        memset(pExistingEntry->pDirtyPages, 0, nBufferSize);
#endif // pExistingEntry
        VIRTUALSetAllocState(MEM_RESERVE, 0, nBufferSize * CHAR_BIT, pExistingEntry);
        memset(pExistingEntry->pProtectionState,
            VIRTUALConvertWinFlags(pExistingEntry->accessProtection),
            memSize / VIRTUAL_PAGE_SIZE);
    }
    else
    {
        ERROR("Unable to allocate memory for the structure.\n");
        bRetVal = FALSE;

#if MMAP_DOESNOT_ALLOW_REMAP
        if (pExistingEntry->pDirtyPages) InternalFree(pExistingEntry->pDirtyPages);
        pExistingEntry->pDirtyPages = NULL;
#endif //

        if (pExistingEntry->pProtectionState) InternalFree(pExistingEntry->pProtectionState);
        pExistingEntry->pProtectionState = NULL;

        if (pExistingEntry->pAllocState) InternalFree(pExistingEntry->pAllocState);
        pExistingEntry->pAllocState = NULL;

        goto done;
    }

done:
    return bRetVal;
}

/******
 *
 *  VIRTUALReserveMemory() - Helper function that actually reserves the memory.
 *
 *      NOTE: I call SetLastError in here, because many different error states
 *              exists, and that would be very complicated to work around.
 *
 */
static void * VIRTUALReserveMemory(
                 CPalThread *pthrCurrent, /* Currently executing thread */
                 void * lpAddress,        /* Region to reserve or commit */
                 SIZE_T dwSize,           /* Size of Region */
                 uint32_t flAllocationType,  /* Type of allocation */
                 uint32_t flProtect)         /* Type of access protection */
{
    void * pRetVal      = NULL;
    UINT_PTR StartBoundary;
    SIZE_T MemSize;

    TRACE( "Reserving the memory now..\n");

    // First, figure out where we're trying to reserve the memory and
    // how much we need. On most systems, requests to mmap must be
    // page-aligned and at multiples of the page size.
    StartBoundary = (UINT_PTR)lpAddress & ~BOUNDARY_64K;
    /* Add the sizes, and round down to the nearest page boundary. */
    MemSize = ( ((UINT_PTR)lpAddress + dwSize + VIRTUAL_PAGE_MASK) & ~VIRTUAL_PAGE_MASK ) -
               StartBoundary;

    if ((flAllocationType & MEM_RESERVE_EXECUTABLE) != 0)
    {
        fprintf(stderr, "MEM_RESERVE_EXECUTABLE is not supported!");
        abort();
    }

    InternalEnterCriticalSection(pthrCurrent, &virtual_critsec);

    if (pRetVal == NULL)
    {
        // Try to reserve memory from the OS
        pRetVal = ReserveVirtualMemory(pthrCurrent, (void *)StartBoundary, MemSize);
    }

    if (pRetVal != NULL)
    {
#if !MMAP_IGNORES_HINT
        if ( !lpAddress )
        {
#endif  // MMAP_IGNORES_HINT
            /* Compute the real values instead of the null values. */
            StartBoundary = (UINT_PTR)pRetVal & ~VIRTUAL_PAGE_MASK;

            MemSize = ( ((UINT_PTR)pRetVal + dwSize + VIRTUAL_PAGE_MASK) & ~VIRTUAL_PAGE_MASK ) -
                      StartBoundary;
#if !MMAP_IGNORES_HINT
        }
#endif  // MMAP_IGNORES_HINT
        if ( !VIRTUALStoreAllocationInfo( StartBoundary, MemSize,
                                   flAllocationType, flProtect ) )
        {
            ASSERT( "Unable to store the structure in the list.\n");
            pthrCurrent->SetLastError( ERROR_INTERNAL_ERROR );
            munmap( pRetVal, MemSize );
            pRetVal = NULL;
        }
    }

    InternalLeaveCriticalSection(pthrCurrent, &virtual_critsec);
    return pRetVal;
}

/******
 *
 *  ReserveVirtualMemory() - Helper function that is used by Virtual* APIs
 *  and ExecutableMemoryAllocator to reserve virtual memory from the OS.
 *
 */
static void * ReserveVirtualMemory(
                 CPalThread *pthrCurrent, /* Currently executing thread */
                 void * lpAddress,        /* Region to reserve or commit */
                 SIZE_T dwSize)           /* Size of Region */
{
    void * pRetVal = NULL;
#if !defined(__APPLE__) && !defined(vm_address_t)
#define vm_address_t UINT_PTR
#endif
    vm_address_t StartBoundary = (vm_address_t)lpAddress;
    SIZE_T MemSize = dwSize;
#if HAVE_VM_ALLOCATE
    int result;
#endif  // HAVE_VM_ALLOCATE

    TRACE( "Reserving the memory now..\n");

#if MMAP_IGNORES_HINT
    pRetVal = VIRTUALReserveFromBackingFile(StartBoundary, MemSize);
#else   // MMAP_IGNORES_HINT
    // Most platforms will only commit the memory if it is dirtied,
    // so this should not consume too much swap space.
    int mmapFlags = 0;
    int mmapFile = -1;
    off_t mmapOffset = 0;

#if HAVE_VM_ALLOCATE
    // Allocate with vm_allocate first, then map at the fixed address.
    result = vm_allocate(mach_task_self(), &StartBoundary, MemSize,
                          ((void *) StartBoundary != NULL) ? FALSE : TRUE);
    if (result != KERN_SUCCESS) {
        ERROR("vm_allocate failed to allocated the requested region!\n");
        pthrCurrent->SetLastError(ERROR_INVALID_ADDRESS);
        pRetVal = NULL;
        goto done;
    }
    mmapFlags |= MAP_FIXED;
#endif // HAVE_VM_ALLOCATE

#if RESERVE_FROM_BACKING_FILE
    mmapFile = gBackingFile;
    mmapOffset = (char *) StartBoundary - (char *) gBackingBaseAddress;
    mmapFlags |= MAP_PRIVATE;
#else // RESERVE_FROM_BACKING_FILE
    mmapFlags |= MAP_ANON | MAP_PRIVATE;
#endif // RESERVE_FROM_BACKING_FILE

    pRetVal = mmap((void *) StartBoundary, MemSize, PROT_NONE,
                   mmapFlags, mmapFile, mmapOffset);

    /* Check to see if the region is what we asked for. */
    if (pRetVal != MAP_FAILED && lpAddress != NULL &&
        StartBoundary != (UINT_PTR) pRetVal)
    {
        ERROR("We did not get the region we asked for!\n");
        pthrCurrent->SetLastError(ERROR_INVALID_ADDRESS);
        munmap(pRetVal, MemSize);
        pRetVal = NULL;
        goto done;
    }
#endif  // MMAP_IGNORES_HINT

    if ( pRetVal != MAP_FAILED)
    {
#if MMAP_ANON_IGNORES_PROTECTION
        if (mprotect(pRetVal, MemSize, PROT_NONE) != 0)
        {
            ERROR("mprotect failed to protect the region!\n");
            pthrCurrent->SetLastError(ERROR_INVALID_ADDRESS);
            munmap(pRetVal, MemSize);
            pRetVal = NULL;
            goto done;
        }
#endif  // MMAP_ANON_IGNORES_PROTECTION
    }
    else
    {
        ERROR( "Failed due to insufficient memory.\n" );
#if HAVE_VM_ALLOCATE
        vm_deallocate(mach_task_self(), StartBoundary, MemSize);
#endif  // HAVE_VM_ALLOCATE
        pthrCurrent->SetLastError( ERROR_NOT_ENOUGH_MEMORY );
        pRetVal = NULL;
        goto done;
    }

done:
    return pRetVal;
}

/******
 *
 *  VIRTUALCommitMemory() - Helper function that actually commits the memory.
 *
 *      NOTE: I call SetLastError in here, because many different error states
 *              exists, and that would be very complicated to work around.
 *
 */
static void * VIRTUALCommitMemory(
                 CPalThread *pthrCurrent, /* Currently executing thread */
                 void * lpAddress,        /* Region to reserve or commit */
                 SIZE_T dwSize,           /* Size of Region */
                 uint32_t flAllocationType,  /* Type of allocation */
                 uint32_t flProtect)         /* Type of access protection */
{
    UINT_PTR StartBoundary      = 0;
    SIZE_T MemSize              = 0;
    PCMI pInformation           = 0;
    void * pRetVal              = NULL;
    BOOL IsLocallyReserved      = FALSE;
    SIZE_T totalPages;
    int32_t allocationType, curAllocationType;
    int32_t protectionState, curProtectionState;
    SIZE_T initialRunStart;
    SIZE_T runStart;
    SIZE_T runLength;
    SIZE_T index;
    int32_t nProtect;
    int32_t vProtect;

    if ( lpAddress )
    {
        StartBoundary = (UINT_PTR)lpAddress & ~VIRTUAL_PAGE_MASK;
        /* Add the sizes, and round down to the nearest page boundary. */
        MemSize = ( ((UINT_PTR)lpAddress + dwSize + VIRTUAL_PAGE_MASK) & ~VIRTUAL_PAGE_MASK ) -
                  StartBoundary;
    }
    else
    {
        MemSize = ( dwSize + VIRTUAL_PAGE_MASK ) & ~VIRTUAL_PAGE_MASK;
    }

    /* See if we have already reserved this memory. */
    pInformation = VIRTUALFindRegionInformation( StartBoundary );

    if ( !pInformation )
    {
        /* According to the new MSDN docs, if MEM_COMMIT is specified,
        and the memory is not reserved, you reserve and then commit.
        */
        void * pReservedMemory =
                VIRTUALReserveMemory( pthrCurrent, lpAddress, dwSize,
                                      flAllocationType, flProtect );

        TRACE( "Reserve and commit the memory!\n " );

        if ( pReservedMemory )
        {
            /* Re-align the addresses and try again to find the memory. */
            StartBoundary = (UINT_PTR)pReservedMemory & ~VIRTUAL_PAGE_MASK;
            MemSize = ( ((UINT_PTR)pReservedMemory + dwSize + VIRTUAL_PAGE_MASK)
                        & ~VIRTUAL_PAGE_MASK ) - StartBoundary;

            pInformation = VIRTUALFindRegionInformation( StartBoundary );

            if ( !pInformation )
            {
                ASSERT( "Unable to locate the region information.\n" );
                pthrCurrent->SetLastError( ERROR_INTERNAL_ERROR );
                pRetVal = NULL;
                goto done;
            }
            IsLocallyReserved = TRUE;
        }
        else
        {
            ERROR( "Unable to reserve the memory.\n" );
            /* Don't set last error here, it will already be set. */
            pRetVal = NULL;
            goto done;
        }
    }

    TRACE( "Committing the memory now..\n");

    // Pages that aren't already committed need to be committed. Pages that
    // are committed don't need to be committed, but they might need to have
    // their permissions changed.
    // To get this right, we find runs of pages with similar states and
    // permissions. If a run is not committed, we commit it and then set
    // its permissions. If a run is committed but has different permissions
    // from what we're trying to set, we set its permissions. Finally,
    // if a run is already committed and has the right permissions,
    // we don't need to do anything to it.

    totalPages = MemSize / VIRTUAL_PAGE_SIZE;
    runStart = (StartBoundary - pInformation->startBoundary) /
                VIRTUAL_PAGE_SIZE;   // Page index
    initialRunStart = runStart;
    allocationType = VIRTUALGetAllocationType(runStart, pInformation);
    protectionState = pInformation->pProtectionState[runStart];
    curAllocationType = allocationType;
    curProtectionState = protectionState;
    runLength = 1;
    nProtect = W32toUnixAccessControl(flProtect);
    vProtect = VIRTUALConvertWinFlags(flProtect);

    if (totalPages > pInformation->memSize / VIRTUAL_PAGE_SIZE - runStart)
    {
        ERROR("Trying to commit beyond the end of the region!\n");
        goto error;
    }

    while(runStart < initialRunStart + totalPages)
    {
        // Find the next run of pages
        for(index = runStart + 1; index < initialRunStart + totalPages;
            index++)
        {
            curAllocationType = VIRTUALGetAllocationType(index, pInformation);
            curProtectionState = pInformation->pProtectionState[index];
            if (curAllocationType != allocationType ||
                curProtectionState != protectionState)
            {
                break;
            }
            runLength++;
        }

        StartBoundary = pInformation->startBoundary + runStart * VIRTUAL_PAGE_SIZE;
        MemSize = runLength * VIRTUAL_PAGE_SIZE;
        if (allocationType != MEM_COMMIT)
        {
            // Commit the pages
            void * pRet = MAP_FAILED;
#if MMAP_DOESNOT_ALLOW_REMAP
            if (mprotect((void *) StartBoundary, MemSize, PROT_WRITE | PROT_READ) == 0)
                pRet = (void *)StartBoundary;
#else // MMAP_DOESNOT_ALLOW_REMAP
            pRet = mmap((void *) StartBoundary, MemSize, PROT_WRITE | PROT_READ,
                     MAP_ANON | MAP_FIXED | MAP_PRIVATE, -1, 0);
#endif // MMAP_DOESNOT_ALLOW_REMAP
            if (pRet != MAP_FAILED)
            {
#if MMAP_DOESNOT_ALLOW_REMAP
                SIZE_T i;
                char *temp = (char *) StartBoundary;
                for(i = 0; i < runLength; i++)
                {

                    if (VIRTUALIsPageDirty(runStart + i, pInformation))
                    {
                        // This page is being recommitted after being decommitted,
                        // therefore the memory needs to be cleared
                        memset (temp, 0, VIRTUAL_PAGE_SIZE);
                    }

                    temp += VIRTUAL_PAGE_SIZE;
                }
#endif // MMAP_DOESNOT_ALLOW_REMAP
            }
            else
            {
                ERROR("mmap() failed! Error(%d)=%s\n", errno, strerror(errno));
                goto error;
            }
            VIRTUALSetAllocState(MEM_COMMIT, runStart, runLength, pInformation);
#if MMAP_DOESNOT_ALLOW_REMAP
            VIRTUALSetDirtyPages (0, runStart, runLength, pInformation);
#endif // MMAP_DOESNOT_ALLOW_REMAP

            if (nProtect == (PROT_WRITE | PROT_READ))
            {
                // Handle this case specially so we don't bother
                // mprotect'ing the region.
                memset(pInformation->pProtectionState + runStart,
                       vProtect, runLength);
            }
            protectionState = VIRTUAL_READWRITE;
        }
        if (protectionState != vProtect)
        {
            // Change permissions.
            if (mprotect((void *) StartBoundary, MemSize, nProtect) != -1)
            {
                memset(pInformation->pProtectionState + runStart,
                       vProtect, runLength);
            }
            else
            {
                ERROR("mprotect() failed! Error(%d)=%s\n",
                      errno, strerror(errno));
                goto error;
            }
        }

        runStart = index;
        runLength = 1;
        allocationType = curAllocationType;
        protectionState = curProtectionState;
    }
    pRetVal = (void *) (pInformation->startBoundary +
                        initialRunStart * VIRTUAL_PAGE_SIZE);
    goto done;

error:
    if ( flAllocationType & MEM_RESERVE || IsLocallyReserved )
    {
#if (MMAP_IGNORES_HINT && !MMAP_DOESNOT_ALLOW_REMAP)
        mmap(pRetVal, MemSize, PROT_NONE, MAP_FIXED | MAP_PRIVATE,
             gBackingFile, (char *) pRetVal - (char *) gBackingBaseAddress);
#else   // MMAP_IGNORES_HINT && !MMAP_DOESNOT_ALLOW_REMAP
        munmap( pRetVal, MemSize );
#endif  // MMAP_IGNORES_HINT && !MMAP_DOESNOT_ALLOW_REMAP
        if ( VIRTUALReleaseMemory( pInformation ) == FALSE )
        {
            ASSERT( "Unable to remove the PCMI entry from the list.\n" );
            pthrCurrent->SetLastError( ERROR_INTERNAL_ERROR );
            pRetVal = NULL;
            goto done;
        }
        pInformation = NULL;
        pRetVal = NULL;
    }

done:

    return pRetVal;
}

#if MMAP_IGNORES_HINT
/*++
Function:
    VIRTUALReserveFromBackingFile

    Locates a reserved but unallocated block of memory in the free list.

    If addr is not zero, this will only find a block that starts at addr
    and is at least large enough to hold the requested size.

    If addr is zero, this finds the first block of memory in the free list
    of the right size.

    Once the block is located, it is split if necessary to allocate only
    the requested size. The function then calls mmap() with MAP_FIXED to
    map the located block at its address on an anonymous fd.

    This function requires that length be a multiple of the page size. If
    length is not a multiple of the page size, subsequently allocated blocks
    may be allocated on addresses that are not page-size-aligned, which is
    invalid.

    Returns the base address of the mapped block, or MAP_FAILED if no
    suitable block exists or mapping fails.
--*/
static void *VIRTUALReserveFromBackingFile(UINT_PTR addr, size_t length)
{
    FREE_BLOCK *block;
    FREE_BLOCK *prev;
    FREE_BLOCK *temp;
    char *returnAddress;

    block = NULL;
    prev = NULL;
    for(temp = pFreeMemory; temp != NULL; temp = temp->next)
    {
        if (addr != 0)
        {
            if (addr < (UINT_PTR) temp->startBoundary)
            {
                // Not up to a block containing addr yet.
                prev = temp;
                continue;
            }
        }
        if ((addr == 0 && temp->memSize >= length) ||
            (addr >= (UINT_PTR) temp->startBoundary &&
             addr + length <= (UINT_PTR) temp->startBoundary + temp->memSize))
        {
            block = temp;
            break;
        }
        prev = temp;
    }
    if (block == NULL)
    {
        // No acceptable page exists.
        return MAP_FAILED;
    }

    // Grab the return address before we adjust the free list.
    if (addr == 0)
    {
        returnAddress = block->startBoundary;
    }
    else
    {
        returnAddress = (char *) addr;
    }

    // Adjust the free list to account for the block we're returning.
    if (block->memSize == length && returnAddress == block->startBoundary)
    {
        // We're going to remove this free block altogether.
        if (prev == NULL)
        {
            // block is the first in our free list.
            pFreeMemory = block->next;
        }
        else
        {
            prev->next = block->next;
        }
        InternalFree(block);
    }
    else
    {
        // We have to divide this block. Map in the new block.
        if (returnAddress == block->startBoundary)
        {
            // The address is right at the beginning of the block.
            // We can make the block smaller.
            block->memSize -= length;
            block->startBoundary += length;
        }
        else if (returnAddress + length ==
                 block->startBoundary + block->memSize)
        {
            // The allocation is at the end of the block. Make the
            // block smaller from the end.
            block->memSize -= length;
        }
        else
        {
            // Splitting the block. We'll need a new block for the free list.
            temp = (FREE_BLOCK *) InternalMalloc(sizeof(FREE_BLOCK));
            if (temp == NULL)
            {
                ERROR("Failed to allocate memory for a new free block!");
                return MAP_FAILED;
            }
            temp->startBoundary = returnAddress + length;
            temp->memSize = (block->startBoundary + block->memSize) -
                            (returnAddress + length);
            temp->next = block->next;
            block->memSize -= (length + temp->memSize);
            block->next = temp;
        }
    }
    return returnAddress;
}

/*++
Function:
    VIRTUALAddToFreeList

    Adds the given block to our free list. Coalesces the list if necessary.
    The block should already have been mapped back onto the backing file.

    Returns TRUE if the block was added to the free list.
--*/
static BOOL VIRTUALAddToFreeList(const PCMI pMemoryToBeReleased)
{
    FREE_BLOCK *temp;
    FREE_BLOCK *lastBlock;
    FREE_BLOCK *newBlock;
    BOOL coalesced;

    lastBlock = NULL;
    for(temp = pFreeMemory; temp != NULL; temp = temp->next)
    {
        if ((UINT_PTR) temp->startBoundary > pMemoryToBeReleased->startBoundary)
        {
            // This check isn't necessary unless the PAL is fundamentally
            // broken elsewhere.
            if (pMemoryToBeReleased->startBoundary +
                pMemoryToBeReleased->memSize > (UINT_PTR) temp->startBoundary)
            {
                ASSERT("Free and allocated memory blocks overlap!");
                return FALSE;
            }
            break;
        }
        lastBlock = temp;
    }

    // Check to see if we're going to coalesce blocks before we
    // allocate anything.
    coalesced = FALSE;

    // First, are we coalescing with the next block?
    if (temp != NULL)
    {
        if ((UINT_PTR) temp->startBoundary == pMemoryToBeReleased->startBoundary +
            pMemoryToBeReleased->memSize)
        {
            temp->startBoundary = (char *) pMemoryToBeReleased->startBoundary;
            temp->memSize += pMemoryToBeReleased->memSize;
            coalesced = TRUE;
        }
    }

    // Are we coalescing with the previous block? If so, check to see
    // if we can free one of the blocks.
    if (lastBlock != NULL)
    {
        if ((UINT_PTR) lastBlock->startBoundary + lastBlock->memSize ==
            pMemoryToBeReleased->startBoundary)
        {
            if (lastBlock->next != NULL &&
                lastBlock->startBoundary + lastBlock->memSize ==
                lastBlock->next->startBoundary)
            {
                lastBlock->memSize += lastBlock->next->memSize;
                temp = lastBlock->next;
                lastBlock->next = lastBlock->next->next;
                InternalFree(temp);
            }
            else
            {
                lastBlock->memSize += pMemoryToBeReleased->memSize;
            }
            coalesced = TRUE;
        }
    }

    // If we coalesced anything, we're done.
    if (coalesced)
    {
        return TRUE;
    }

    // At this point we know we're not coalescing anything and we need
    // a new block.
    newBlock = (FREE_BLOCK *) InternalMalloc(sizeof(FREE_BLOCK));
    if (newBlock == NULL)
    {
        ERROR("Failed to allocate memory for a new free block!");
        return FALSE;
    }
    newBlock->startBoundary = (char *) pMemoryToBeReleased->startBoundary;
    newBlock->memSize = pMemoryToBeReleased->memSize;
    if (lastBlock == NULL)
    {
        newBlock->next = temp;
        pFreeMemory = newBlock;
    }
    else
    {
        newBlock->next = lastBlock->next;
        lastBlock->next = newBlock;
    }
    return TRUE;
}
#endif  // MMAP_IGNORES_HINT

#if RESERVE_FROM_BACKING_FILE
/*++
Function:
    VIRTUALGetBackingFile

    Ensures that we have a set of pages that correspond to a backing file.
    We use the PAL as the backing file merely because we're pretty confident
    it exists.

    When the backing file hasn't been created, we create it, mmap pages
    onto it, and create the free list.

    Returns TRUE if we could locate our backing file, open it, mmap
    pages onto it, and create the free list. Does nothing if we already
    have a mapping.
--*/
static BOOL VIRTUALGetBackingFile(CPalThread *pthrCurrent)
{
    BOOL result = FALSE;
    char palName[MAX_PATH_FNAME];

    InternalEnterCriticalSection(pthrCurrent, &virtual_critsec);

    if (gBackingFile != -1)
    {
        result = TRUE;
        goto done;
    }

#if MMAP_IGNORES_HINT
    if (pFreeMemory != NULL)
    {
        // Sanity check. Our free list should always be NULL if we
        // haven't allocated our pages.
        ASSERT("Free list is unexpectedly non-NULL without a backing file!");
        goto done;
    }
#endif

    if (!(PALGetLibRotorPalName(palName, MAX_PATH_FNAME)))
    {
        ASSERT("Surprisingly, LibRotorPal can't be found!");
        goto done;
    }
    gBackingFile = InternalOpen(palName, O_RDONLY);
    if (gBackingFile == -1)
    {
        ASSERT("Failed to open %s as a backing file: errno=%d\n",
                palName, errno);
        goto done;
    }

#if MMAP_IGNORES_HINT
    gBackingBaseAddress = mmap(0, BACKING_FILE_SIZE, PROT_NONE,
                                MAP_PRIVATE, gBackingFile, 0);
    if (gBackingBaseAddress == MAP_FAILED)
    {
        ERROR("Failed to map onto the backing file: errno=%d\n", errno);
        // Hmph. This is bad.
        close(gBackingFile);
        gBackingFile = -1;
        goto done;
    }

    // Create our free list.
    pFreeMemory = (FREE_BLOCK *) InternalMalloc(sizeof(FREE_BLOCK));
    if (pFreeMemory == NULL)
    {
        // Not good.
        ERROR("Failed to allocate memory for the free list!");
        munmap(gBackingBaseAddress, BACKING_FILE_SIZE);
        close(gBackingFile);
        gBackingBaseAddress = (void *) -1;
        gBackingFile = -1;
        goto done;
    }
    pFreeMemory->startBoundary = (char*)gBackingBaseAddress;
    pFreeMemory->memSize = BACKING_FILE_SIZE;
    pFreeMemory->next = NULL;
    result = TRUE;
#endif // MMAP_IGNORES_HINT

done:
    InternalLeaveCriticalSection(pthrCurrent, &virtual_critsec);
    return result;
}
#endif // RESERVE_FROM_BACKING_FILE

void *
VirtualAllocEx(
          HANDLE hProcess,
          void * lpAddress,       /* Region to reserve or commit */
          SIZE_T dwSize,          /* Size of Region */
          uint32_t flAllocationType, /* Type of allocation */
          uint32_t flProtect)        /* Type of access protection */
{
    return VirtualAlloc(lpAddress, dwSize, flAllocationType, flProtect);
}

__attribute__((no_instrument_function, noinline))
static bool PAL_Initialize_Check_Once()
{
    int error = PAL_InitializeChakraCore();
    return error == ERROR_SUCCESS;
}
/*++
Function:
  VirtualAlloc

Note:
  MEM_TOP_DOWN, MEM_PHYSICAL, MEM_WRITE_WATCH are not supported.
  Unsupported flags are ignored.

  Page size on i386 is set to 4k.

See MSDN doc.
--*/
void *
VirtualAlloc_(
          void * lpAddress,       /* Region to reserve or commit */
          SIZE_T dwSize,          /* Size of Region */
          uint32_t flAllocationType, /* Type of allocation */
          uint32_t flProtect)        /* Type of access protection */
{
#ifdef DEBUG
    static bool was_pal_initialized = PAL_Initialize_Check_Once();
    _ASSERTE(was_pal_initialized);
#endif

    void *  pRetVal       = NULL;
    CPalThread *pthrCurrent;

    PERF_ENTRY(VirtualAlloc);
    ENTRY("VirtualAlloc(lpAddress=%p, dwSize=%u, flAllocationType=%#x, \
          flProtect=%#x)\n", lpAddress, dwSize, flAllocationType, flProtect);

    pthrCurrent = InternalGetCurrentThread();

    if ( ( flAllocationType & MEM_WRITE_WATCH )  != 0 )
    {
        pthrCurrent->SetLastError( ERROR_INVALID_PARAMETER );
        goto done;
    }

    /* Test for un-supported flags. */
    if ( ( flAllocationType & ~( MEM_COMMIT | MEM_RESERVE | MEM_TOP_DOWN | MEM_RESERVE_EXECUTABLE ) ) != 0 )
    {
        ASSERT( "flAllocationType can be one, or any combination of MEM_COMMIT, \
               MEM_RESERVE, MEM_TOP_DOWN, or MEM_RESERVE_EXECUTABLE.\n" );
        pthrCurrent->SetLastError( ERROR_INVALID_PARAMETER );
        goto done;
    }
    if ( VIRTUALContainsInvalidProtectionFlags( flProtect ) )
    {
        ASSERT( "flProtect can be one of PAGE_READONLY, PAGE_READWRITE, or \
               PAGE_EXECUTE_READWRITE || PAGE_NOACCESS. \n" );

        pthrCurrent->SetLastError( ERROR_INVALID_PARAMETER );
        goto done;
    }
    if ( flAllocationType & MEM_TOP_DOWN )
    {
        WARN( "Ignoring the allocation flag MEM_TOP_DOWN.\n" );
    }

#if RESERVE_FROM_BACKING_FILE
    // Make sure we have memory to map before we try to use it.
    VIRTUALGetBackingFile(pthrCurrent);
#endif  // RESERVE_FROM_BACKING_FILE

    if ( flAllocationType & MEM_RESERVE )
    {
        InternalEnterCriticalSection(pthrCurrent, &virtual_critsec);
        pRetVal = VIRTUALReserveMemory( pthrCurrent, lpAddress, dwSize, flAllocationType, flProtect );
        InternalLeaveCriticalSection(pthrCurrent, &virtual_critsec);

        if ( !pRetVal )
        {
            /* Error messages are already displayed, just leave. */
            goto done;
        }
    }

    if ( flAllocationType & MEM_COMMIT )
    {
        InternalEnterCriticalSection(pthrCurrent, &virtual_critsec);
        if ( pRetVal != NULL )
        {
            /* We are reserving and committing. */
            pRetVal = VIRTUALCommitMemory( pthrCurrent, pRetVal, dwSize,
                                    flAllocationType, flProtect );
        }
        else
        {
            /* Just a commit. */
            pRetVal = VIRTUALCommitMemory( pthrCurrent, lpAddress, dwSize,
                                    flAllocationType, flProtect );
        }
        InternalLeaveCriticalSection(pthrCurrent, &virtual_critsec);
    }

done:
    LOGEXIT("VirtualAlloc returning %p\n ", pRetVal  );
    PERF_EXIT(VirtualAlloc);
    return pRetVal;
}

/*++
Function:
  VirtualFreeEnclosing_ This method tries to free memory enclosing a 64K aligned region an already RESERVED larger region.
  This is to be used specifically when we attempt to get a 64K aligned address on new VirtualAlloc allocations.

--*/
BOOL
VirtualFreeEnclosing_(
     void * lpRegionStartAddress,         /* Starting address of the original region. */
     SIZE_T dwSize,                       /* Size of the requested region i.e. the intended size of the VirtualAlloc call.*/
     SIZE_T dwAlignmentSize,              /* The intended alignment of the returned address. This is also the size of the extra memory reserved i.e. 64KB in our case whenw e try a 64K alignment. */
     void * lpActualAlignedStartAddress)  /* Actual starting address that will be returned for the new allocation. */
{
    BOOL bRetVal = TRUE;

#ifdef DEBUG
    _ASSERTE(lpActualAlignedStartAddress >= lpRegionStartAddress);
    _ASSERTE(lpActualAlignedStartAddress < (char*)lpRegionStartAddress + dwSize + dwAlignmentSize);
#endif

    char * beforeRegionStart = (char *) lpRegionStartAddress;
    size_t beforeRegionSize = (char *) lpActualAlignedStartAddress - beforeRegionStart;
    char * afterRegionStart = (char *) lpActualAlignedStartAddress + dwSize;
    size_t afterRegionSize = dwAlignmentSize - beforeRegionSize;

    bool beforeRegionFreed = false;
    bool afterRegionFreed = (afterRegionSize == 0);

#ifdef DEBUG
    _ASSERTE(dwSize + dwAlignmentSize == beforeRegionSize + dwSize + afterRegionSize);

    SIZE_T alignmentDiff = ((size_t)lpRegionStartAddress % dwAlignmentSize);
    size_t beforeRegionSize2 = dwAlignmentSize - alignmentDiff;
    _ASSERTE(beforeRegionSize == beforeRegionSize2);
#endif

    CPalThread *pthrCurrent;
    pthrCurrent = InternalGetCurrentThread();

    if (dwSize == 0)
    {
        ERROR("dwSize must be non-zero when releasing enclosing memory region.\n");
        pthrCurrent->SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }

    InternalEnterCriticalSection(pthrCurrent, &virtual_critsec);

    PCMI pMemoryToBeReleased =
        VIRTUALFindRegionInformation((UINT_PTR)lpRegionStartAddress);

    if (!pMemoryToBeReleased)
    {
        ERROR("lpRegionStartAddress must be the base address returned by VirtualAlloc.\n");
        pthrCurrent->SetLastError(ERROR_INVALID_ADDRESS);
        bRetVal = FALSE;
        goto VirtualFreeEnclosingExit;
    }

    TRACE("Releasing the following memory %d to %d.\n", beforeRegionStart, beforeRegionSize);

#if (MMAP_IGNORES_HINT && !MMAP_DOESNOT_ALLOW_REMAP)
    if (mmap((void *)beforeRegionStart, beforeRegionSize, PROT_NONE, MAP_FIXED | MAP_PRIVATE, gBackingFile,
        (char *)beforeRegionStart - (char *)gBackingBaseAddress) != MAP_FAILED)
#else   // MMAP_IGNORES_HINT && !MMAP_DOESNOT_ALLOW_REMAP
    if (munmap((void *)beforeRegionStart, beforeRegionSize) == 0)
#endif  // MMAP_IGNORES_HINT && !MMAP_DOESNOT_ALLOW_REMAP
    {
        beforeRegionFreed = true;
    }
    else
    {
#if MMAP_IGNORES_HINT
        ASSERT("Unable to remap the memory onto the backing file; "
            "error is %d.\n", errno);
#else   // MMAP_IGNORES_HINT
        ASSERT("Unable to unmap the memory, munmap() returned "
            "an abnormal value.\n");
#endif  // MMAP_IGNORES_HINT
        pthrCurrent->SetLastError(ERROR_INTERNAL_ERROR);
        bRetVal = FALSE;
        goto VirtualFreeEnclosingExit;
    }

    if (!afterRegionFreed)
    {
        TRACE("Releasing the following memory %d to %d.\n", afterRegionStart, afterRegionSize);
#if (MMAP_IGNORES_HINT && !MMAP_DOESNOT_ALLOW_REMAP)
        if (mmap((void *)afterRegionStart, afterRegionSize, PROT_NONE, MAP_FIXED | MAP_PRIVATE, gBackingFile,
            (char *)afterRegionStart - (char *)gBackingBaseAddress) != MAP_FAILED)
#else   // MMAP_IGNORES_HINT && !MMAP_DOESNOT_ALLOW_REMAP
        if (munmap((void *)afterRegionStart, afterRegionSize) == 0)
#endif  // MMAP_IGNORES_HINT && !MMAP_DOESNOT_ALLOW_REMAP
        {
            afterRegionFreed = true;
        }
        else
        {
#if MMAP_IGNORES_HINT
            ASSERT("Unable to remap the memory onto the backing file; "
                "error is %d.\n", errno);
#else   // MMAP_IGNORES_HINT
            ASSERT("Unable to unmap the memory, munmap() returned "
                "an abnormal value.\n");
#endif  // MMAP_IGNORES_HINT
            pthrCurrent->SetLastError(ERROR_INTERNAL_ERROR);
            bRetVal = FALSE;
            goto VirtualFreeEnclosingExit;
        }
    }

    if (beforeRegionFreed && afterRegionFreed)
    {
        bRetVal = VIRTUALUpdateAllocationInfo(pMemoryToBeReleased, (UINT_PTR)lpActualAlignedStartAddress, dwSize);
        goto VirtualFreeEnclosingExit;
    }

VirtualFreeEnclosingExit:
    InternalLeaveCriticalSection(pthrCurrent, &virtual_critsec);
    return bRetVal;
}

#define KB64 (64 * 1024)
#define MB64 (KB64 * 1024)

void *
VirtualAlloc(
          void * lpAddress,       /* Region to reserve or commit */
          SIZE_T dwSize,          /* Size of Region */
          uint32_t flAllocationType, /* Type of allocation */
          uint32_t flProtect)        /* Type of access protection */
{
    if (lpAddress)
    {
        return VirtualAlloc_(lpAddress, dwSize, flAllocationType, flProtect);
    }

    bool reserve = (flAllocationType & MEM_RESERVE) == MEM_RESERVE;
    bool commit  = (flAllocationType & MEM_COMMIT)  == MEM_COMMIT;

    if (reserve || commit)
    {
        char *address = (char*) VirtualAlloc_(nullptr, dwSize, MEM_RESERVE, flProtect);
        if (!address)
        {
            return nullptr;
        }

        if (reserve)
        {
            flAllocationType &= ~MEM_RESERVE;
        }

        SIZE_T diff = ((size_t)address % KB64);
        if (diff != 0)
        {
            // Free the previously allocated address as it's not 64K aligned.
            VirtualFree(address, 0, MEM_RELEASE);

            // looks like ``pushed new address + dwSize`` is not available
            // try on a bigger surface
            address = (char*)VirtualAlloc_(nullptr, dwSize + KB64, MEM_RESERVE, flProtect);
            if (!address)
            {
                // This is an actual OOM.
                return nullptr;
            }

            diff = ((size_t)address % KB64);
            char * addr64 = address + (KB64 - diff);

            // Free the regions enclosing the 64K aligned region we intend to use.
            if (VirtualFreeEnclosing_(address, dwSize, KB64, addr64) == 0)
            {
                ASSERT("Unable to unmap the enclosing memory.\n");
                return nullptr;
            }

            address = addr64;
#ifdef DEBUG
            InterlockedIncrement(&attempt2);
#endif
        }
#ifdef DEBUG
        else
        {
            InterlockedIncrement(&attempt1);
        }
#endif

#ifdef DEBUG
        TRACE("VirtualAlloc 64K alignment attempts: %d : %d \n", attempt1.RawValue(), attempt2.RawValue());
#endif
        if (flAllocationType == 0) return address;
        lpAddress = address;
    }

    return VirtualAlloc_(lpAddress, dwSize, flAllocationType, flProtect);
}

#undef KB64
#undef MB64

BOOL
VirtualFreeEx(
         HANDLE hProcess,
         void * lpAddress,    /* Address of region. */
         SIZE_T dwSize,       /* Size of region. */
         uint32_t dwFreeType )   /* Operation type. */
{
    return VirtualFree(lpAddress, dwSize, dwFreeType);
}

/*++
Function:
  VirtualFree

See MSDN doc.
--*/
BOOL
VirtualFree(
         void * lpAddress,    /* Address of region. */
         SIZE_T dwSize,       /* Size of region. */
         uint32_t dwFreeType )   /* Operation type. */
{
    BOOL bRetVal = TRUE;
    CPalThread *pthrCurrent;

    PERF_ENTRY(VirtualFree);
    ENTRY("VirtualFree(lpAddress=%p, dwSize=%u, dwFreeType=%#x)\n",
          lpAddress, dwSize, dwFreeType);

    pthrCurrent = InternalGetCurrentThread();
    InternalEnterCriticalSection(pthrCurrent, &virtual_critsec);

    /* Sanity Checks. */
    if ( !lpAddress )
    {
        ERROR( "lpAddress cannot be NULL. You must specify the base address of\
               regions to be de-committed. \n" );
        pthrCurrent->SetLastError( ERROR_INVALID_ADDRESS );
        bRetVal = FALSE;
        goto VirtualFreeExit;
    }

    if ( !( dwFreeType & MEM_RELEASE ) && !(dwFreeType & MEM_DECOMMIT ) )
    {
        ERROR( "dwFreeType must contain one of the following: \
               MEM_RELEASE or MEM_DECOMMIT\n" );
        pthrCurrent->SetLastError( ERROR_INVALID_PARAMETER );
        bRetVal = FALSE;
        goto VirtualFreeExit;
    }
    /* You cannot release and decommit in one call.*/
    if ( dwFreeType & MEM_RELEASE && dwFreeType & MEM_DECOMMIT )
    {
        ERROR( "MEM_RELEASE cannot be combined with MEM_DECOMMIT.\n" );
        bRetVal = FALSE;
        goto VirtualFreeExit;
    }

    if ( dwFreeType & MEM_DECOMMIT )
    {
        UINT_PTR StartBoundary  = 0;
        SIZE_T MemSize        = 0;

        if ( dwSize == 0 )
        {
            ERROR( "dwSize cannot be 0. \n" );
            pthrCurrent->SetLastError( ERROR_INVALID_PARAMETER );
            bRetVal = FALSE;
            goto VirtualFreeExit;
        }
        /*
         * A two byte range straddling 2 pages caues both pages to be either
         * released or decommitted. So round the dwSize up to the next page
         * boundary and round the lpAddress down to the next page boundary.
         */
        MemSize = (((UINT_PTR)(dwSize) + ((UINT_PTR)(lpAddress) & VIRTUAL_PAGE_MASK)
                    + VIRTUAL_PAGE_MASK) & ~VIRTUAL_PAGE_MASK);

        StartBoundary = (UINT_PTR)lpAddress & ~VIRTUAL_PAGE_MASK;

        PCMI pUnCommittedMem;
        pUnCommittedMem = VIRTUALFindRegionInformation( StartBoundary );
        if (!pUnCommittedMem)
        {
            ASSERT( "Unable to locate the region information.\n" );
            pthrCurrent->SetLastError( ERROR_INTERNAL_ERROR );
            bRetVal = FALSE;
            goto VirtualFreeExit;
        }

        TRACE( "Un-committing the following page(s) %d to %d.\n",
               StartBoundary, MemSize );

#if MMAP_DOESNOT_ALLOW_REMAP
        // if no double mapping is supported,
        // just mprotect the memory with no access
        if (mprotect((void *)StartBoundary, MemSize, PROT_NONE) == 0)
#else // MMAP_DOESNOT_ALLOW_REMAP
        // Explicitly calling mmap instead of mprotect here makes it
        // that much more clear to the operating system that we no
        // longer need these pages.
#if RESERVE_FROM_BACKING_FILE
        if ( mmap( (void *)StartBoundary, MemSize, PROT_NONE,
                   MAP_FIXED | MAP_PRIVATE, gBackingFile,
                   (char *) StartBoundary - (char *) gBackingBaseAddress ) !=
             MAP_FAILED )
#else   // RESERVE_FROM_BACKING_FILE
        if ( mmap( (void *)StartBoundary, MemSize, PROT_NONE,
                   MAP_FIXED | MAP_ANON | MAP_PRIVATE, -1, 0 ) != MAP_FAILED )
#endif  // RESERVE_FROM_BACKING_FILE
#endif // MMAP_DOESNOT_ALLOW_REMAP
        {
#if (MMAP_ANON_IGNORES_PROTECTION && !MMAP_DOESNOT_ALLOW_REMAP)
            if (mprotect((void *) StartBoundary, MemSize, PROT_NONE) != 0)
            {
                ASSERT("mprotect failed to protect the region!\n");
                pthrCurrent->SetLastError(ERROR_INTERNAL_ERROR);
                munmap((void *) StartBoundary, MemSize);
                bRetVal = FALSE;
                goto VirtualFreeExit;
            }
#endif  // MMAP_ANON_IGNORES_PROTECTION && !MMAP_DOESNOT_ALLOW_REMAP

            SIZE_T index = 0;
            SIZE_T nNumOfPagesToChange = 0;

            /* We can now commit this memory by calling VirtualAlloc().*/
            index = (StartBoundary - pUnCommittedMem->startBoundary) / VIRTUAL_PAGE_SIZE;

            nNumOfPagesToChange = MemSize / VIRTUAL_PAGE_SIZE;
            VIRTUALSetAllocState( MEM_RESERVE, index,
                                  nNumOfPagesToChange, pUnCommittedMem );
#if MMAP_DOESNOT_ALLOW_REMAP
            VIRTUALSetDirtyPages( 1, index,
                                  nNumOfPagesToChange, pUnCommittedMem );
#endif // MMAP_DOESNOT_ALLOW_REMAP

            goto VirtualFreeExit;
        }
        else
        {
            ASSERT( "mmap() returned an abnormal value.\n" );
            bRetVal = FALSE;
            pthrCurrent->SetLastError( ERROR_INTERNAL_ERROR );
            goto VirtualFreeExit;
        }
    }

    if ( dwFreeType & MEM_RELEASE )
    {
        PCMI pMemoryToBeReleased =
            VIRTUALFindRegionInformation( (UINT_PTR)lpAddress );

        if ( !pMemoryToBeReleased )
        {
            ERROR( "lpAddress must be the base address returned by VirtualAlloc.\n" );
            pthrCurrent->SetLastError( ERROR_INVALID_ADDRESS );
            bRetVal = FALSE;
            goto VirtualFreeExit;
        }
        if ( dwSize != 0 )
        {
            ERROR( "dwSize must be 0 if you are releasing the memory.\n" );
            pthrCurrent->SetLastError( ERROR_INVALID_PARAMETER );
            bRetVal = FALSE;
            goto VirtualFreeExit;
        }

        TRACE( "Releasing the following memory %d to %d.\n",
               pMemoryToBeReleased->startBoundary, pMemoryToBeReleased->memSize );

#if (MMAP_IGNORES_HINT && !MMAP_DOESNOT_ALLOW_REMAP)
        if (mmap((void *) pMemoryToBeReleased->startBoundary,
                 pMemoryToBeReleased->memSize, PROT_NONE,
                 MAP_FIXED | MAP_PRIVATE, gBackingFile,
                 (char *) pMemoryToBeReleased->startBoundary -
                 (char *) gBackingBaseAddress) != MAP_FAILED)
#else   // MMAP_IGNORES_HINT && !MMAP_DOESNOT_ALLOW_REMAP
        if ( munmap( (void *)pMemoryToBeReleased->startBoundary,
                     pMemoryToBeReleased->memSize ) == 0 )
#endif  // MMAP_IGNORES_HINT && !MMAP_DOESNOT_ALLOW_REMAP
        {
            if ( VIRTUALReleaseMemory( pMemoryToBeReleased ) == FALSE )
            {
                ASSERT( "Unable to remove the PCMI entry from the list.\n" );
                pthrCurrent->SetLastError( ERROR_INTERNAL_ERROR );
                bRetVal = FALSE;
                goto VirtualFreeExit;
            }
            pMemoryToBeReleased = NULL;
        }
        else
        {
#if MMAP_IGNORES_HINT
            ASSERT("Unable to remap the memory onto the backing file; "
                   "error is %d.\n", errno);
#else   // MMAP_IGNORES_HINT
            ASSERT( "Unable to unmap the memory, munmap() returned "
                   "an abnormal value.\n" );
#endif  // MMAP_IGNORES_HINT
            pthrCurrent->SetLastError( ERROR_INTERNAL_ERROR );
            bRetVal = FALSE;
            goto VirtualFreeExit;
        }
    }

VirtualFreeExit:
    InternalLeaveCriticalSection(pthrCurrent, &virtual_critsec);
    LOGEXIT( "VirtualFree returning %s.\n", bRetVal == TRUE ? "TRUE" : "FALSE" );
    PERF_EXIT(VirtualFree);
    return bRetVal;
}

BOOL
VirtualProtectEx(
            HANDLE hProcess,
            void * lpAddress,
            SIZE_T dwSize,
            uint32_t flNewProtect,
            uint32_t * lpflOldProtect)
{
    return VirtualProtect(lpAddress, dwSize, flNewProtect, lpflOldProtect);
}

/*++
Function:
  VirtualProtect

See MSDN doc.
--*/
BOOL
VirtualProtect(
            void * lpAddress,
            SIZE_T dwSize,
            uint32_t flNewProtect,
            uint32_t * lpflOldProtect)
{
    BOOL     bRetVal = FALSE;
    PCMI     pEntry = NULL;
    SIZE_T   MemSize = 0;
    UINT_PTR StartBoundary = 0;
    SIZE_T   Index = 0;
    SIZE_T   NumberOfPagesToChange = 0;
    SIZE_T   OffSet = 0;
    CPalThread * pthrCurrent;

    PERF_ENTRY(VirtualProtect);
    ENTRY("VirtualProtect(lpAddress=%p, dwSize=%u, flNewProtect=%#x, "
          "flOldProtect=%p)\n",
          lpAddress, dwSize, flNewProtect, lpflOldProtect);

    pthrCurrent = InternalGetCurrentThread();
    InternalEnterCriticalSection(pthrCurrent, &virtual_critsec);

    StartBoundary = (UINT_PTR)lpAddress & ~VIRTUAL_PAGE_MASK;
    MemSize = (((UINT_PTR)(dwSize) + ((UINT_PTR)(lpAddress) & VIRTUAL_PAGE_MASK)
                + VIRTUAL_PAGE_MASK) & ~VIRTUAL_PAGE_MASK);

#if DEBUG
    if ( VIRTUALContainsInvalidProtectionFlags( flNewProtect ) )
    {
        ASSERT( "flProtect can be one of PAGE_NOACCESS, PAGE_READONLY, "
               "PAGE_READWRITE, PAGE_EXECUTE, PAGE_EXECUTE_READ "
               ", or PAGE_EXECUTE_READWRITE. \n" );
        SetLastError( ERROR_INVALID_PARAMETER );
        goto ExitVirtualProtect;
    }

    if ( !lpflOldProtect)
    {
        ERROR( "lpflOldProtect was invalid.\n" );
        SetLastError( ERROR_NOACCESS );
        goto ExitVirtualProtect;
    }
#endif

    pEntry = VIRTUALFindRegionInformation( StartBoundary );
    if ( NULL != pEntry )
    {
        /* See if the pages are committed. */
        Index = OffSet = StartBoundary - pEntry->startBoundary == 0 ?
             0 : ( StartBoundary - pEntry->startBoundary ) / VIRTUAL_PAGE_SIZE;
        NumberOfPagesToChange = MemSize / VIRTUAL_PAGE_SIZE;

        TRACE( "Number of pages to check %d, starting page %d \n",
               NumberOfPagesToChange, Index );

        for ( ; Index < NumberOfPagesToChange; Index++  )
        {
            if ( !VIRTUALIsPageCommitted( Index, pEntry ) )
            {
                ERROR( "You can only change the protection attributes"
                       " on committed memory.\n" );
                SetLastError( ERROR_INVALID_ADDRESS );
                goto ExitVirtualProtect;
            }
        }
    }

    if ( 0 == mprotect( (void *)StartBoundary, MemSize,
                   W32toUnixAccessControl( flNewProtect ) ) )
    {
        /* Reset the access protection. */
        TRACE( "Number of pages to change %d, starting page %d \n",
               NumberOfPagesToChange, OffSet );
        /*
         * Set the old protection flags. We only use the first flag, so
         * if there were several regions with each with different flags only the
         * first region's protection flag will be returned.
         */
        if ( pEntry )
        {
            *lpflOldProtect =
                VIRTUALConvertVirtualFlags( pEntry->pProtectionState[ OffSet ] );

            memset( pEntry->pProtectionState + OffSet,
                    VIRTUALConvertWinFlags( flNewProtect ),
                    NumberOfPagesToChange );
        }
        else
        {
            *lpflOldProtect = PAGE_EXECUTE_READWRITE;
        }
        bRetVal = TRUE;
    }
    else
    {
        ERROR( "%s\n", strerror( errno ) );
        if ( errno == EINVAL )
        {
            SetLastError( ERROR_INVALID_ADDRESS );
        }
        else if ( errno == EACCES )
        {
            SetLastError( ERROR_INVALID_ACCESS );
        }
    }
ExitVirtualProtect:
    InternalLeaveCriticalSection(pthrCurrent, &virtual_critsec);

    LOGEXIT( "VirtualProtect returning %s.\n", bRetVal == TRUE ? "TRUE" : "FALSE" );
    PERF_EXIT(VirtualProtect);
    return bRetVal;
}

#if HAVE_VM_ALLOCATE
//---------------------------------------------------------------------------------------
//
// Convert a vm_prot_t flag on the Mach kernel to the corresponding memory protection on Windows.
//
// Arguments:
//    protection - Mach protection to be converted
//
// Return Value:
//    Return the corresponding memory protection on Windows (e.g. PAGE_READ_WRITE, etc.)
//

static uint32_t VirtualMapMachProtectToWinProtect(vm_prot_t protection)
{
    if (protection & VM_PROT_READ)
    {
        if (protection & VM_PROT_WRITE)
        {
            if (protection & VM_PROT_EXECUTE)
            {
                return PAGE_EXECUTE_READWRITE;
            }
            else
            {
                return PAGE_READWRITE;
            }
        }
        else
        {
            if (protection & VM_PROT_EXECUTE)
            {
                return PAGE_EXECUTE_READ;
            }
            else
            {
                return PAGE_READONLY;
            }
        }
    }
    else
    {
        if (protection & VM_PROT_WRITE)
        {
            if (protection & VM_PROT_EXECUTE)
            {
                return PAGE_EXECUTE_WRITECOPY;
            }
            else
            {
                return PAGE_WRITECOPY;
            }
        }
        else
        {
            if (protection & VM_PROT_EXECUTE)
            {
                return PAGE_EXECUTE;
            }
            else
            {
                return PAGE_NOACCESS;
            }
        }
    }
}

static void VM_ALLOCATE_VirtualQuery(const void * lpAddress, PMEMORY_BASIC_INFORMATION lpBuffer)
{
    kern_return_t MachRet;
    vm_address_t vm_address;
    vm_size_t vm_size;
    vm_region_flavor_t vm_flavor;
    mach_msg_type_number_t infoCnt;
    mach_port_t object_name;
    vm_region_basic_info_data_64_t info;
    infoCnt = VM_REGION_BASIC_INFO_COUNT_64;
    vm_flavor = VM_REGION_BASIC_INFO_64;

    vm_address = (vm_address_t)lpAddress;
    MachRet = vm_region_64(
                        mach_task_self(),
                        &vm_address,
                        &vm_size,
                        vm_flavor,
                        (vm_region_info_t)&info,
                        &infoCnt,
                        &object_name);
    if (MachRet != KERN_SUCCESS) {
        return;
    }

    if (vm_address > (vm_address_t)lpAddress) {
        /* lpAddress was pointing into a free region */
        lpBuffer->State = MEM_FREE;
        return;
    }

    lpBuffer->BaseAddress = (void *)vm_address;

    // We don't actually have any information on the Mach kernel which maps to AllocationProtect.
    lpBuffer->AllocationProtect = VM_PROT_NONE;

    lpBuffer->RegionSize = (SIZE_T)vm_size;

    if (info.reserved)
    {
        lpBuffer->State = MEM_RESERVE;
    }
    else
    {
        lpBuffer->State = MEM_COMMIT;
    }

    lpBuffer->Protect = VirtualMapMachProtectToWinProtect(info.protection);

    /* Note that if a mapped region and a private region are adjacent, this
        will return MEM_PRIVATE but the region size will span
        both the mapped and private regions. */
    if (!info.shared)
    {
        lpBuffer->Type = MEM_PRIVATE;
    }
    else
    {
        // What should this be?  It's either MEM_MAPPED or MEM_IMAGE, but without an image list,
        // we can't determine which one it is.
        lpBuffer->Type = MEM_MAPPED;
    }
}
#endif // HAVE_VM_ALLOCATE

SIZE_T
VirtualQueryEx(
     HANDLE hProcess,
     const void * lpAddress,
     PMEMORY_BASIC_INFORMATION lpBuffer,
     SIZE_T dwLength)
{
    return VirtualQuery(lpAddress, lpBuffer, dwLength);
}

/*++
Function:
  VirtualQuery

See MSDN doc.
--*/
SIZE_T
VirtualQuery(
          const void * lpAddress,
          PMEMORY_BASIC_INFORMATION lpBuffer,
          SIZE_T dwLength)
{
    PCMI     pEntry = NULL;
    UINT_PTR StartBoundary = 0;
    CPalThread * pthrCurrent;

    PERF_ENTRY(VirtualQuery);
    ENTRY("VirtualQuery(lpAddress=%p, lpBuffer=%p, dwLength=%u)\n",
          lpAddress, lpBuffer, dwLength);

    pthrCurrent = InternalGetCurrentThread();
    InternalEnterCriticalSection(pthrCurrent, &virtual_critsec);

    if ( !lpBuffer)
    {
        ERROR( "lpBuffer has to be a valid pointer.\n" );
        pthrCurrent->SetLastError( ERROR_NOACCESS );
        goto ExitVirtualQuery;
    }
    if ( dwLength < sizeof( *lpBuffer ) )
    {
        ERROR( "dwLength cannot be smaller then the size of *lpBuffer.\n" );
        pthrCurrent->SetLastError( ERROR_BAD_LENGTH );
        goto ExitVirtualQuery;
    }

    StartBoundary = (UINT_PTR)lpAddress & ~VIRTUAL_PAGE_MASK;

#if MMAP_IGNORES_HINT
    // Make sure we have memory to map before we try to query it.
    VIRTUALGetBackingFile(pthrCurrent);

    // If we're suballocating, claim that any memory that isn't in our
    // suballocated block is already allocated. This keeps callers from
    // using these results to try to allocate those blocks and failing.
    if (StartBoundary < (UINT_PTR) gBackingBaseAddress ||
        StartBoundary >= (UINT_PTR) gBackingBaseAddress + BACKING_FILE_SIZE)
    {
        if (StartBoundary < (UINT_PTR) gBackingBaseAddress)
        {
            lpBuffer->RegionSize = (UINT_PTR) gBackingBaseAddress - StartBoundary;
        }
        else
        {
            lpBuffer->RegionSize = -StartBoundary;
        }
        lpBuffer->BaseAddress = (void *) StartBoundary;
        lpBuffer->State = MEM_COMMIT;
        lpBuffer->Type = MEM_MAPPED;
        lpBuffer->AllocationProtect = 0;
        lpBuffer->Protect = 0;
        goto ExitVirtualQuery;
    }
#endif  // MMAP_IGNORES_HINT

    /* Find the entry. */
    pEntry = VIRTUALFindRegionInformation( StartBoundary );

    if ( !pEntry )
    {
        /* Can't find a match, or no list present. */
        /* Next, looking for this region in file maps */
        if (!MAPGetRegionInfo((void *)StartBoundary, lpBuffer))
        {
            // When all else fails, call vm_region() if it's available.

            // Initialize the State to be MEM_FREE, in which case AllocationBase, AllocationProtect,
            // Protect, and Type are all undefined.
            lpBuffer->BaseAddress = (void *)StartBoundary;
            lpBuffer->RegionSize = 0;
            lpBuffer->State = MEM_FREE;
#if HAVE_VM_ALLOCATE
            VM_ALLOCATE_VirtualQuery(lpAddress, lpBuffer);
#endif
        }
    }
    else
    {
        /* Starting page. */
        SIZE_T Index = ( StartBoundary - pEntry->startBoundary ) / VIRTUAL_PAGE_SIZE;

        /* Attributes to check for. */
        uint8_t AccessProtection = pEntry->pProtectionState[ Index ];
        int32_t AllocationType = VIRTUALGetAllocationType( Index, pEntry );
        SIZE_T RegionSize = 0;

        TRACE( "Index = %d, Number of Pages = %d. \n",
               Index, pEntry->memSize / VIRTUAL_PAGE_SIZE );

        while ( Index < pEntry->memSize / VIRTUAL_PAGE_SIZE &&
                VIRTUALGetAllocationType( Index, pEntry ) == AllocationType &&
                pEntry->pProtectionState[ Index ] == AccessProtection )
        {
            RegionSize += VIRTUAL_PAGE_SIZE;
            Index++;
        }

        TRACE( "RegionSize = %d.\n", RegionSize );

        /* Fill the structure.*/
        lpBuffer->AllocationProtect = pEntry->accessProtection;
        lpBuffer->BaseAddress = (void *)StartBoundary;

        lpBuffer->Protect = AllocationType == MEM_COMMIT ?
            VIRTUALConvertVirtualFlags( AccessProtection ) : 0;

        lpBuffer->RegionSize = RegionSize;
        lpBuffer->State =
            ( AllocationType == MEM_COMMIT ? MEM_COMMIT : MEM_RESERVE );
        WARN( "Ignoring lpBuffer->Type. \n" );
    }

ExitVirtualQuery:

    InternalLeaveCriticalSection(pthrCurrent, &virtual_critsec);

    LOGEXIT( "VirtualQuery returning %d.\n", sizeof( *lpBuffer ) );
    PERF_EXIT(VirtualQuery);
    return sizeof( *lpBuffer );
}

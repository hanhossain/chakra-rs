//-------------------------------------------------------------------------------------------------------
// ChakraCore/Pal
// Contains portions (c) copyright Microsoft, portions copyright (c) the .NET Foundation and Contributors
// and edits (c) copyright the ChakraCore Contributors.
// See THIRD-PARTY-NOTICES.txt in the project root for .NET Foundation license
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

/*++



Module Name:

    include/pal/virtual.h

Abstract:
    Header file for virtual memory management.



--*/

#ifndef _PAL_VIRTUAL_H_
#define _PAL_VIRTUAL_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef struct _CMI {

    struct _CMI * pNext;        /* Link to the next entry. */
    struct _CMI * pLast;        /* Link to the previous entry. */

    UINT_PTR   startBoundary;   /* Starting location of the region. */
    SIZE_T   memSize;         /* Size of the entire region.. */

    uint32_t  accessProtection;    /* Initial allocation access protection. */
    uint32_t  allocationType;      /* Initial allocation type. */

    uint8_t * pAllocState;         /* Individual allocation type tracking for each */
                                /* page in the region. */

    uint8_t * pProtectionState;    /* Individual allocation type tracking for each */
                                /* page in the region. */
#if MMAP_DOESNOT_ALLOW_REMAP
    uint8_t * pDirtyPages;         /* Pages that need to be cleared if re-committed */
#endif // MMAP_DOESNOT_ALLOW_REMAP

}CMI, * PCMI;

enum VIRTUAL_CONSTANTS
{
    /* Allocation type. */
    VIRTUAL_COMMIT_ALL_BITS     = 0xFF,
    VIRTUAL_RESERVE_ALL_BITS    = 0x0,

    /* Protection Type. */
    VIRTUAL_READONLY,
    VIRTUAL_READWRITE,
    VIRTUAL_EXECUTE_READWRITE,
    VIRTUAL_NOACCESS,
    VIRTUAL_EXECUTE,
    VIRTUAL_EXECUTE_READ,

    /* Page manipulation constants. */
#if defined(__APPLE__) && defined(_ARM64_)
    VIRTUAL_PAGE_SIZE       = 0x4000,
#elif __sparc__
    VIRTUAL_PAGE_SIZE       = 0x2000,
#else   // __sparc__
    VIRTUAL_PAGE_SIZE       = 0x1000,
#endif  // __sparc__
    VIRTUAL_PAGE_MASK       = VIRTUAL_PAGE_SIZE - 1,
    BOUNDARY_64K    = 0xffff
};

/*++
Function :
    VIRTUALInitialize

    Initialize the critical sections.

Return value:
    TRUE  if initialization succeeded
    FALSE otherwise.
--*/
BOOL VIRTUALInitialize( void );

/*++
Function :
    VIRTUALCleanup

    Deletes the critical sections.

--*/
void VIRTUALCleanup( void );

/*++
Function :
    VIRTUALOwnedRegion

    Returns whether the space in question is owned the VIRTUAL system.

--*/
BOOL VIRTUALOwnedRegion(  UINT_PTR address );


#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* _PAL_VIRTUAL_H_ */

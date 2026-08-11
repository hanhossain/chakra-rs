//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information. 
//

/*++



Module Name:

    map.cpp

Abstract:

    Implementation of file mapping API.



--*/


#include "pal/palinternal.h"
#include "pal/dbgmsg.h"
#include "pal/init.h"
#include "pal/critsect.h"
#include "pal/virtual.h"
#include "common.h"
#include "pal/map.hpp"
#include "pal/thread.hpp"
#include "pal/file.hpp"
#include "chakra/Logger.h"
#include <new>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include <format>

using namespace CorUnix;

SET_DEFAULT_DEBUG_CHANNEL(VIRTUAL);

//
// The mapping critical section guards access to the list
// of currently mapped views. If a thread needs to access
// both this critical section and the data for an object
// it must acquire the object data first. That is, a thread
// cannot acquire any other locks after taking hold of
// this critical section.
//

// TODO (hanhossain): remove more-pal-file
CRITICAL_SECTION mapping_critsec __attribute__((init_priority(200)));
LIST_ENTRY MappedViewList __attribute__((init_priority(200)));

#if !defined(__linux__)
/* We need MAP_ANON. However on some platforms like HP-UX, it is defined as MAP_ANONYMOUS */
#if !defined(MAP_ANON) && defined(MAP_ANONYMOUS)
#define MAP_ANON MAP_ANONYMOUS
#endif
#endif

void
FileMappingCleanupRoutine(
    CPalThread *pThread,
    IPalObject *pObjectToCleanup,
    bool fShutdown,
    bool fCleanupSharedState
    );

CObjectType CorUnix::otFileMapping __attribute__((init_priority(200)))(
                otiFileMapping,
                FileMappingCleanupRoutine,
                sizeof(CFileMappingImmutableData),
                sizeof(CFileMappingProcessLocalData),
                CObjectType::UnwaitableObject,
                CObjectType::SignalingNotApplicable,
                CObjectType::ThreadReleaseNotApplicable,
                CObjectType::OwnershipNotApplicable
                );

CAllowedObjectTypes aotFileMapping __attribute__((init_priority(200))) (otiFileMapping);

void
FileMappingCleanupRoutine(
    CPalThread *pThread,
    IPalObject *pObjectToCleanup,
    bool fShutdown,
    bool fCleanupSharedState
    )
{
    PAL_ERROR palError = NO_ERROR;
    CFileMappingImmutableData *pImmutableData = NULL;
    CFileMappingProcessLocalData *pLocalData = NULL;
    IDataLock *pLocalDataLock = NULL;

    if (TRUE == fCleanupSharedState)
    {
        //
        // If we created a temporary file to back this mapping we need
        // to unlink it now
        //
        
        palError = pObjectToCleanup->GetImmutableData(
            reinterpret_cast<void**>(&pImmutableData)
            );

        if (NO_ERROR != palError)
        {
            chakra::Logger::error("Unable to obtain immutable data for object to be reclaimed");
            return;
        }
    }

    if (FALSE == fShutdown)
    {
        //
        // We only need to close the object's descriptor if we're not
        // shutting down
        //
        
        palError = pObjectToCleanup->GetProcessLocalData(
            pThread,
            WriteLock,
            &pLocalDataLock,
            reinterpret_cast<void**>(&pLocalData)
            );

        if (NO_ERROR != palError)
        {
            chakra::Logger::error("Unable to obtain process local data for object to be reclaimed");
            return;
        }

        if (-1 != pLocalData->UnixFd)
        {
            close(pLocalData->UnixFd);
            pLocalData->UnixFd = -1;
        }

        pLocalDataLock->ReleaseLock(pThread);
    }

    //
    // Why don't we need to deal with any views that may have been created
    // from this mapping? If the process is shutting down then there's nothing
    // that we need to take care of, as the OS will remove the underlying
    // mappings when the process goes away. If we're not shutting down then
    // there's no way for a view to exist against this mapping, since each
    // view holds a reference against the mapping object.
    //
}

/*++
Function :
    MAPInitialize

    Initialize the critical sections.

Return value:
    TRUE if initialization succeeded
    FALSE otherwise
--*/
BOOL
MAPInitialize( void )
{
    TRACE( "Initialising the critical section.\n" );

    InternalInitializeCriticalSection(&mapping_critsec);

    InitializeListHead(&MappedViewList);

    return TRUE;
}

/*++
Function :
    MAPConvertAccessToProtect

    Converts the FILE_MAP_READ type flags to PAGE_READONLY flags.
    Currently, this function only deals with the access flags recognized as valid
    by MAPContainsInvalidFlags().

--*/
static uint32_t MAPConvertAccessToProtect(uint32_t flAccess)
{
    if (flAccess == FILE_MAP_ALL_ACCESS)
    {
        return PAGE_READWRITE;
    }
    else if ((flAccess == FILE_MAP_COPY) || (flAccess == FILE_MAP_WRITE))
    {
        return PAGE_WRITECOPY;
    }
    else if (flAccess == FILE_MAP_READ)
    {
        return PAGE_READONLY;
    }
    else if (flAccess == 0)
    {
        return PAGE_NOACCESS;
    }

    chakra::Logger::error("Unknown flag for flAccess.\n");
    return static_cast<uint32_t>(-1);
}

// returns TRUE if we have information about the specified address
BOOL MAPGetRegionInfo(void * lpAddress,
                      PMEMORY_BASIC_INFORMATION lpBuffer)
{
    BOOL fFound = FALSE;
    CPalThread * pThread = InternalGetCurrentThread();
    
    InternalEnterCriticalSection(pThread, &mapping_critsec);

    for(LIST_ENTRY *pLink = MappedViewList.Flink;
        pLink != &MappedViewList;
        pLink = pLink->Flink)
    {
        uint32_t MappedSize;
        void * real_map_addr;
        size_t real_map_sz;
        PMAPPED_VIEW_LIST pView = CONTAINING_RECORD(pLink, MAPPED_VIEW_LIST, Link);

        real_map_addr = pView->lpAddress;
        real_map_sz = pView->NumberOfBytesToMap;

        MappedSize = ((real_map_sz-1) & ~VIRTUAL_PAGE_MASK) + VIRTUAL_PAGE_SIZE; 
        if ( real_map_addr <= lpAddress && 
             reinterpret_cast<void*>(reinterpret_cast<unsigned long>(real_map_addr) + MappedSize) > lpAddress )
        {
            if (lpBuffer)
            {
                size_t regionSize = MappedSize + reinterpret_cast<unsigned long>(real_map_addr) -
                       (reinterpret_cast<unsigned long>(lpAddress) & ~VIRTUAL_PAGE_MASK);

                lpBuffer->BaseAddress = lpAddress;
                lpBuffer->AllocationProtect = 0;
                lpBuffer->RegionSize = regionSize;
                lpBuffer->State = MEM_COMMIT;
                lpBuffer->Protect = MAPConvertAccessToProtect(pView->dwDesiredAccess);
                lpBuffer->Type = MEM_MAPPED;
            }

            fFound = TRUE;
            break;
        }
    }

    InternalLeaveCriticalSection(pThread, &mapping_critsec);
    
    return fFound;
}
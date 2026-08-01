//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information. 
//

/*++



Module Name:

    shmfilelockmgr.cpp

Abstract:
    Shared memory based file lock manager



--*/

#include "pal/thread.hpp"
#include <new>
#include "pal/dbgmsg.h"
#include "shmfilelockmgr.hpp"
#include "chakra/Logger.h"

using namespace CorUnix;

SET_DEFAULT_DEBUG_CHANNEL(FILE);

PAL_ERROR
FILEGetSHMFileLocks(
    const char * filename,
    SHMPTR *pshmFileLocks,
    BOOL noCreate
    );

/* return TRUE if LockToTest region is behind lockRgn, FALSE otherwise */
#define IS_LOCK_BEFORE(LockToTest, lockRgn) \
    (((LockToTest)->lockRgnStart + (LockToTest)->nbBytesLocked) <= \
                                                        (lockRgn)->lockRgnStart)

/* return TRUE if LockToTest region intersect with lockRgn, FALSE otherwise */
#define IS_LOCK_INTERSECT(LockToTest, lockRgn) \
    (!IS_LOCK_BEFORE(LockToTest, lockRgn) && !IS_LOCK_BEFORE(lockRgn, LockToTest))

/* return TRUE if LockToTest region and lockRgn have the same file pointer and 
   the same process Id, FALSE otherwise */
#define IS_LOCK_HAVE_SAME_OWNER(LockToTest, lockRgn) \
    (((LockToTest)->pvControllerInstance == (lockRgn)->pvControllerInstance) && \
     ((LockToTest)->processId == (lockRgn)->processId))

/* return TRUE if LockToTest region and lockRgn represent the same lock, 
   FALSE otherwise*/
#define IS_LOCK_EQUAL(LockToTest, lockRgn) \
        (((LockToTest)->processId == (lockRgn)->processId)           && \
         ((LockToTest)->pvControllerInstance == (lockRgn)->pvControllerInstance)   && \
         ((LockToTest)->lockRgnStart == (lockRgn)->lockRgnStart)     && \
         ((LockToTest)->nbBytesLocked == (lockRgn)->nbBytesLocked)   && \
         ((LockToTest)->lockType == (lockRgn)->lockType))


PAL_ERROR
FILEGetSHMFileLocks(
    const char * filename,
    SHMPTR *pshmFileLocks,
    BOOL noCreate
    )
{
    PAL_ERROR palError = NO_ERROR;
    SHMPTR shmPtrRet = 0;
    SHMFILELOCKS *filelocksPtr, *nextFilelocksPtr;
    char *unix_filename;

    SHMLock();

    shmPtrRet = SHMGetInfo(SIID_FILE_LOCKS);

    while(shmPtrRet != 0)
    {        
        if ( (SHMPTR_TO_TYPED_PTR_BOOL(SHMFILELOCKS, filelocksPtr, shmPtrRet) == FALSE) ||
             (SHMPTR_TO_TYPED_PTR_BOOL(char, unix_filename, filelocksPtr->unix_filename) == FALSE))
        {
            chakra::Logger::error("Unable to get pointer from shm pointer.\n");
            palError = ERROR_INTERNAL_ERROR;
            goto EXIT;
        }

        if (unix_filename == NULL)
        {
            ERROR("Unexpected lock file name value.\n");
            palError = ERROR_INTERNAL_ERROR;
            goto EXIT;
        }
        
        if (strcmp(unix_filename, filename) == 0)
        {
            filelocksPtr->refCount++;
            goto EXIT;
        }
        
        shmPtrRet = filelocksPtr->next;
    }

    /* the file has never been locked before.*/
    shmPtrRet = 0;
    if (noCreate)
    {
      goto EXIT;
    }

    TRACE("Create a new entry in the file lock list in SHM\n");

    /* Create a new entry in the file lock list in SHM */
    if ((shmPtrRet = SHMalloc(sizeof(SHMFILELOCKS))) == 0)
    {
        ERROR("Can't allocate SHMFILELOCKS structure\n");
        palError = ERROR_NOT_ENOUGH_MEMORY;
        goto EXIT;
    }

    if (SHMPTR_TO_TYPED_PTR_BOOL(SHMFILELOCKS, filelocksPtr, shmPtrRet) == FALSE)
    {
        chakra::Logger::error("Unable to get pointer from shm pointer.\n");
        palError = ERROR_INTERNAL_ERROR;
        goto CLEANUP1;
    }

    filelocksPtr->unix_filename = SHMStrDup(filename);
    if (filelocksPtr->unix_filename == 0)
    {
        ERROR("Can't allocate shared memory for filename\n");
        palError = ERROR_NOT_ENOUGH_MEMORY;
        goto CLEANUP1;
    }

    filelocksPtr->fileLockedRgns = 0;
    filelocksPtr->prev = 0;
    filelocksPtr->next = SHMGetInfo(SIID_FILE_LOCKS);
    filelocksPtr->refCount = 1;
    filelocksPtr->share_mode = SHARE_MODE_NOT_INITALIZED;
    filelocksPtr->nbReadAccess = 0;
    filelocksPtr->nbWriteAccess = 0;

    if (SHMPTR_TO_TYPED_PTR_BOOL(SHMFILELOCKS, nextFilelocksPtr, filelocksPtr->next) == FALSE)
    {
        chakra::Logger::error("Unable to get pointer from shm pointer.\n");
        palError = ERROR_INTERNAL_ERROR;
        goto CLEANUP2;
    }
    
    if (nextFilelocksPtr != NULL)
    {
        nextFilelocksPtr->prev = shmPtrRet;
    }

    SHMSetInfo(SIID_FILE_LOCKS, shmPtrRet);
    goto EXIT;

CLEANUP2:
    SHMfree(filelocksPtr->unix_filename);
CLEANUP1:
    SHMfree(shmPtrRet);
    shmPtrRet = 0;
EXIT:    
    SHMRelease();

    if (NO_ERROR == palError)
    {
        *pshmFileLocks = shmPtrRet;
    }
    
    return palError;
}
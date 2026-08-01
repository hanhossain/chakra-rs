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
FILEAddNewLockedRgn(
    SHMFILELOCKS* fileLocks,
    void * pvControllerInstance,
    SHMFILELOCKRGNS *insertAfter,
    unsigned long lockRgnStart, 
    unsigned long nbBytesToLock, 
    LOCK_TYPE lockType
    );

void
FILECleanUpLockedRgn(
    SHMPTR shmFileLocks,
    uint32_t dwAccessRights,
    void * pvControllerInstance
    );

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

void
CSharedMemoryFileLockController::ReleaseController()
{
    if (SHMNULL != m_shmFileLocks)
    {
        FILECleanUpLockedRgn(
            m_shmFileLocks,
            m_dwAccessRights,
            reinterpret_cast<void *>(this)
            );
    }

    delete this;
}


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

PAL_ERROR
FILEAddNewLockedRgn(
    SHMFILELOCKS* fileLocks,
    void * pvControllerInstance,
    SHMFILELOCKRGNS *insertAfter,
    unsigned long lockRgnStart, 
    unsigned long nbBytesToLock, 
    LOCK_TYPE lockType
    )
{
    PAL_ERROR palError = NO_ERROR;
    SHMFILELOCKRGNS *newLockRgn, *lockRgnPtr;
    SHMPTR shmNewLockRgn = SHMNULL;

    if ((fileLocks == NULL) || (pvControllerInstance == NULL))
    {
        chakra::Logger::error("Invalid Null parameter.\n");
        return FALSE;
    }

    SHMLock();
    
    /* Create a new entry for the new locked region */
    TRACE("Create a new entry for the new lock region (%I64u %I64u)\n", 
          lockRgnStart, nbBytesToLock);
    
    if ((shmNewLockRgn = SHMalloc(sizeof(SHMFILELOCKRGNS))) == SHMNULL)
    {
        ERROR("Can't allocate SHMFILELOCKRGNS structure\n");
        palError = ERROR_NOT_ENOUGH_MEMORY;
        goto EXIT;
    }

    if (SHMPTR_TO_TYPED_PTR_BOOL(SHMFILELOCKRGNS, newLockRgn, shmNewLockRgn) == FALSE)
    {
        chakra::Logger::error("Unable to get pointer from shm pointer.\n");
        palError = ERROR_INTERNAL_ERROR;
        goto EXIT;
    }
    
    newLockRgn->processId = getpid();
    newLockRgn->pvControllerInstance = pvControllerInstance;
    newLockRgn->lockRgnStart = lockRgnStart;
    newLockRgn->nbBytesLocked = nbBytesToLock;
    newLockRgn->lockType = lockType;
    
    /* All locked regions with the same offset should be sorted ascending */
    /* the sort is based on the length of the locked byte range */
    if (insertAfter != NULL)
    {
        if (SHMPTR_TO_TYPED_PTR_BOOL(SHMFILELOCKRGNS, lockRgnPtr, insertAfter->next) == FALSE)
        {
            chakra::Logger::error("Unable to get pointer from shm pointer.\n");
            palError = ERROR_INTERNAL_ERROR;
            goto EXIT;
        }
    }
    else
    {
        if (SHMPTR_TO_TYPED_PTR_BOOL(SHMFILELOCKRGNS, lockRgnPtr, fileLocks->fileLockedRgns) == FALSE)
        {
            chakra::Logger::error("Unable to get pointer from shm pointer.\n");
            palError = ERROR_INTERNAL_ERROR;
            goto EXIT;
        }
    }
    
    while(lockRgnPtr != NULL)
    {
        if ( (lockRgnPtr->lockRgnStart == newLockRgn->lockRgnStart) &&
             (newLockRgn->nbBytesLocked > lockRgnPtr->nbBytesLocked))
        {
            insertAfter = lockRgnPtr;
            if (SHMPTR_TO_TYPED_PTR_BOOL(SHMFILELOCKRGNS, lockRgnPtr, lockRgnPtr->next) == FALSE)
            {
                chakra::Logger::error("Unable to get pointer from shm pointer.\n");
                palError = ERROR_INTERNAL_ERROR;
                goto EXIT;
            }
            continue;
        }

        break;
    }
    
    if (insertAfter != NULL)
    {       
        TRACE("Adding lock after the lock rgn (%I64d %I64d)\n", 
              insertAfter->lockRgnStart,insertAfter->nbBytesLocked);
        newLockRgn->next = insertAfter->next;
        insertAfter->next = shmNewLockRgn;
    }
    else
    {
        TRACE("adding lock into the head of the list\n");
        newLockRgn->next = fileLocks->fileLockedRgns;
        fileLocks->fileLockedRgns = shmNewLockRgn;
    }

EXIT:

    if (NO_ERROR != palError && SHMNULL != shmNewLockRgn)
    {
        SHMfree(shmNewLockRgn);
    }
   
    SHMRelease();
    
    return palError;
}

void
FILECleanUpLockedRgn(
    SHMPTR shmFileLocks,
    uint32_t dwAccessRights,
    void * pvControllerInstance
    )
{
    SHMFILELOCKRGNS *curLockRgn = NULL, *prevLock = NULL;
    SHMFILELOCKS *fileLocks, *prevFileLocks, *nextFileLocks;
    SHMPTR shmcurLockRgn;    

    SHMLock();

    if (SHMPTR_TO_TYPED_PTR_BOOL(SHMFILELOCKS, fileLocks, shmFileLocks) == FALSE)
    {
        chakra::Logger::error("Unable to get pointer from shm pointer.\n");
        goto EXIT;
    }

    if (fileLocks != NULL)
    {
        if(fileLocks->fileLockedRgns !=0)
        {        
            shmcurLockRgn = fileLocks->fileLockedRgns;
            if (SHMPTR_TO_TYPED_PTR_BOOL(SHMFILELOCKRGNS, curLockRgn, shmcurLockRgn) == FALSE)
            {
                chakra::Logger::error("Unable to get pointer from shm pointer.\n");
                goto EXIT;
            }
            
            while(curLockRgn != NULL)
            {
                if ((curLockRgn->pvControllerInstance == pvControllerInstance) && 
                    (curLockRgn->processId == getpid()))
                {
                    /* found the locked rgn to remove from SHM */
                    TRACE("Removing the locked region (%I64u, %I64u) from SMH\n", 
                          curLockRgn->lockRgnStart, curLockRgn->nbBytesLocked);
                    
                    if (prevLock == NULL) 
                    {
                        /* removing the first lock */
                        fileLocks->fileLockedRgns = curLockRgn->next;
                        SHMfree(shmcurLockRgn);
                        shmcurLockRgn = fileLocks->fileLockedRgns;
                        if (SHMPTR_TO_TYPED_PTR_BOOL(SHMFILELOCKRGNS, curLockRgn, shmcurLockRgn) == FALSE)
                        {
                            chakra::Logger::error("Unable to get pointer from shm pointer.\n");
                            goto EXIT;
                        }
                    }
                    else
                    {
                        prevLock->next = curLockRgn->next;
                        SHMfree(shmcurLockRgn);
                        shmcurLockRgn = prevLock->next;
                        if (SHMPTR_TO_TYPED_PTR_BOOL(SHMFILELOCKRGNS, curLockRgn, shmcurLockRgn) == FALSE)
                        {
                            chakra::Logger::error("Unable to get pointer from shm pointer.\n");
                            goto EXIT;
                        }
                    }
                    continue;
                }
                
                prevLock = curLockRgn;
                shmcurLockRgn = curLockRgn->next;
                if (SHMPTR_TO_TYPED_PTR_BOOL(SHMFILELOCKRGNS, curLockRgn, shmcurLockRgn) == FALSE)
                {
                    chakra::Logger::error("Unable to get pointer from shm pointer.\n");
                    goto EXIT;
                }
            }
        }
    
        if (dwAccessRights & GENERIC_READ)
        {   
            fileLocks->nbReadAccess--;      
        }
        if (dwAccessRights & GENERIC_WRITE)
        {
            fileLocks->nbWriteAccess--; 
        }  

        /* remove the SHMFILELOCKS structure from SHM if there's no more locked 
           region left and no more reference to it */        
        if ((--(fileLocks->refCount) == 0) && (fileLocks->fileLockedRgns == 0))
        {            
            TRACE("Removing the SHMFILELOCKS structure from SHM\n");

            if ( (SHMPTR_TO_TYPED_PTR_BOOL(SHMFILELOCKS, prevFileLocks, fileLocks->prev) == FALSE) ||
                 (SHMPTR_TO_TYPED_PTR_BOOL(SHMFILELOCKS, nextFileLocks, fileLocks->next) == FALSE))
            {
                chakra::Logger::error("Unable to get pointer from shm pointer.\n");
                goto EXIT;
            }

            if (prevFileLocks == NULL) 
            {
                /* removing the first lock file*/
                SHMSetInfo(SIID_FILE_LOCKS, fileLocks->next);
            }
            else
            {
                prevFileLocks->next = fileLocks->next;
            }

            if (nextFileLocks != NULL)
            {
                nextFileLocks->prev = fileLocks->prev;
            }

            if (fileLocks->unix_filename)
                SHMfree(fileLocks->unix_filename);

            SHMfree(shmFileLocks);
        }
    }    
EXIT:
    SHMRelease();
    return;
}


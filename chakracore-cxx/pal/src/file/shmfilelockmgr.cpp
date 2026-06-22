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

PAL_ERROR
FILELockFileRegion(
    SHMPTR shmFileLocks,
    void * pvControllerInstance,
    unsigned long lockRgnStart, 
    unsigned long nbBytesToLock,
    LOCK_TYPE lockAction
    );

PAL_ERROR
FILEUnlockFileRegion(
    SHMPTR shmFileLocks,
    void * pvControllerInstance,
    unsigned long unlockRgnStart, 
    unsigned long nbBytesToUnlock,
    LOCK_TYPE unlockType
    );

void
FILECleanUpLockedRgn(
    SHMPTR shmFileLocks,
    uint32_t dwAccessRights,
    void * pvControllerInstance
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
CSharedMemoryFileLockController::GetTransactionLock(
    CPalThread *pThread,                // IN, OPTIONAL
    FileTransactionLockType eLockType,
    uint32_t dwOffsetLow,
    uint32_t dwOffsetHigh,
    uint32_t nNumberOfBytesToLockLow,
    uint32_t nNumberOfBytesToLockHigh,
    IFileTransactionLock **ppTransactionLock    // OUT
    )
{
    PAL_ERROR palError = NO_ERROR;
    unsigned long lockRgnStart;
    unsigned long nbBytesToLock;

    lockRgnStart  = ((unsigned long)dwOffsetHigh) << 32  | dwOffsetLow;
    nbBytesToLock = ((unsigned long)nNumberOfBytesToLockHigh) << 32  | 
                             nNumberOfBytesToLockLow;

    palError = FILELockFileRegion(
        m_shmFileLocks,
        reinterpret_cast<void *>(this),
        lockRgnStart, 
        nbBytesToLock,
        RDWR_LOCK_RGN
        );

    if (NO_ERROR == palError)
    {
        *ppTransactionLock = new CSharedMemoryFileTransactionLock(m_shmFileLocks,
                                                                           reinterpret_cast<void *>(this),
                                                                           lockRgnStart, 
                                                                           nbBytesToLock);
        if (NULL == *ppTransactionLock)
        {
            palError = ERROR_OUTOFMEMORY;
            FILEUnlockFileRegion(
                m_shmFileLocks,
                reinterpret_cast<void *>(this),
                lockRgnStart, 
                nbBytesToLock,
                RDWR_LOCK_RGN
                );
        }
    }

    return palError;
}

PAL_ERROR
CSharedMemoryFileLockController::CreateFileLock(
    CPalThread *pThread,                // IN, OPTIONAL
    uint32_t dwOffsetLow,
    uint32_t dwOffsetHigh,
    uint32_t nNumberOfBytesToLockLow,
    uint32_t nNumberOfBytesToLockHigh,
    FileLockExclusivity eFileLockExclusivity,
    FileLockWaitMode eFileLockWaitMode
    )
{
    PAL_ERROR palError = NO_ERROR;
    unsigned long lockRgnStart;
    unsigned long nbBytesToLock;

    if (ExclusiveFileLock != eFileLockExclusivity
        || FailImmediately != eFileLockWaitMode)
    {
        ASSERT("LockFileEx functionality not yet supported");
        palError = ERROR_NOT_SUPPORTED;
        goto CreateFileLockExit;
    }

    lockRgnStart  = ((unsigned long)dwOffsetHigh) << 32  | dwOffsetLow;
    nbBytesToLock = ((unsigned long)nNumberOfBytesToLockHigh) << 32  | 
                             nNumberOfBytesToLockLow;

    palError = FILELockFileRegion(
        m_shmFileLocks,
        reinterpret_cast<void *>(this),
        lockRgnStart, 
        nbBytesToLock,
        USER_LOCK_RGN
        );

CreateFileLockExit:

    return palError;
}

PAL_ERROR
CSharedMemoryFileLockController::ReleaseFileLock(
    CPalThread *pThread,                // IN, OPTIONAL
    uint32_t dwOffsetLow,
    uint32_t dwOffsetHigh,
    uint32_t nNumberOfBytesToUnlockLow,
    uint32_t nNumberOfBytesToUnlockHigh
    )
{
    PAL_ERROR palError = NO_ERROR;
    unsigned long unlockRgnStart;
    unsigned long nbBytesToUnlock;

    unlockRgnStart  = ((unsigned long)dwOffsetHigh) << 32  | dwOffsetLow;
    nbBytesToUnlock = ((unsigned long)nNumberOfBytesToUnlockHigh) << 32  | 
                             nNumberOfBytesToUnlockLow;

    palError = FILEUnlockFileRegion(
        m_shmFileLocks,
        reinterpret_cast<void *>(this),
        unlockRgnStart, 
        nbBytesToUnlock,
        USER_LOCK_RGN
        );

    return palError;
}

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

void
CSharedMemoryFileTransactionLock::ReleaseLock()
{
    FILEUnlockFileRegion(
        m_shmFileLocks,
        m_pvControllerInstance,
        m_lockRgnStart, 
        m_nbBytesToLock,
        RDWR_LOCK_RGN
        );

    delete this;
}

PAL_ERROR
FILELockFileRegion(
    SHMPTR shmFileLocks,
    void * pvControllerInstance,
    unsigned long lockRgnStart, 
    unsigned long nbBytesToLock,
    LOCK_TYPE lockAction
    )
{
    PAL_ERROR palError = NO_ERROR;
    SHMFILELOCKRGNS *curLock, *prevLock, *insertAfter, 
                     lockRgn, fakeLock = {0,0,0,0};
    SHMFILELOCKS *fileLocks;

    SHMLock();
    
    /* nothing to do if the region to lock is empty */
    if (nbBytesToLock == 0) 
    {
        TRACE("Locking an empty region (%I64d, %I64d)\n", lockRgnStart, nbBytesToLock);
        goto EXIT;
    }
    
    if (SHMPTR_TO_TYPED_PTR_BOOL(SHMFILELOCKS, fileLocks, shmFileLocks) == FALSE || fileLocks == NULL)
    {
        ASSERT("Unable to get pointer from shm pointer.\n");
        palError = ERROR_INTERNAL_ERROR;
        goto EXIT;
    }

    if (fileLocks->fileLockedRgns != 0)
    {        
        prevLock = &fakeLock;
        
        if (SHMPTR_TO_TYPED_PTR_BOOL(SHMFILELOCKRGNS, curLock, fileLocks->fileLockedRgns) 
            == FALSE)
        {
            ASSERT("Unable to get pointer from shm pointer.\n");
            palError = ERROR_INTERNAL_ERROR;
            goto EXIT;
        }
        
        lockRgn.lockRgnStart = lockRgnStart;
        lockRgn.nbBytesLocked = nbBytesToLock;
        lockRgn.pvControllerInstance = pvControllerInstance;
        lockRgn.processId = getpid();
        lockRgn.lockType = lockAction;

        while((curLock != NULL) && IS_LOCK_BEFORE(curLock, &lockRgn))
        {            
            prevLock = curLock; 
            if (SHMPTR_TO_TYPED_PTR_BOOL(SHMFILELOCKRGNS, curLock, curLock->next) == FALSE)
            {
                ASSERT("Unable to get pointer from shm pointer.\n");
                palError = ERROR_INTERNAL_ERROR;
                goto EXIT;
            }
        }
                
        while((curLock != NULL) && IS_LOCK_INTERSECT(curLock, &lockRgn))
        {
            /* we couldn't lock the requested region if it overlap with other 
               region locked explicitly (by LockFile call) by other file pointer */
            if ((lockAction == USER_LOCK_RGN) || 
                ((curLock->lockType  == USER_LOCK_RGN) && 
                 !IS_LOCK_HAVE_SAME_OWNER(curLock, &lockRgn)))
            {
                WARN("The requested lock region overlaps an existing locked region\n");
                palError = ERROR_LOCK_VIOLATION;
                goto EXIT;
            }
            
            prevLock = curLock; 
            if (SHMPTR_TO_TYPED_PTR_BOOL(SHMFILELOCKRGNS, curLock, curLock->next) == FALSE)
            {
                ASSERT("Unable to get pointer from shm pointer.\n");
                palError = ERROR_INTERNAL_ERROR;
                goto EXIT;
            }
        }
        
        /* save the previous lock in case we need to insert the requested lock */
        insertAfter = prevLock;         
                
        while(((curLock != NULL) && IS_LOCK_INTERSECT(&lockRgn, curLock)))
        {
            /* we couldn't lock the requested region if it overlap with other region 
               locked explicitly (by LockFile call) by other file pointer */
            if ((lockAction == USER_LOCK_RGN) || 
                ((curLock->lockType  == USER_LOCK_RGN) &&  
                 !IS_LOCK_HAVE_SAME_OWNER(curLock, &lockRgn)))
            {
                WARN("The requested lock region overlaps an existing locked region\n");
                palError = ERROR_LOCK_VIOLATION;
                goto EXIT;
            }
            
            prevLock = curLock; 
            if (SHMPTR_TO_TYPED_PTR_BOOL(SHMFILELOCKRGNS, curLock, curLock->next) == FALSE)
            {
                ASSERT("Unable to get pointer from shm pointer.\n");
                palError = ERROR_INTERNAL_ERROR;
                goto EXIT;
            }
        }
       
        if (insertAfter == &fakeLock) 
        {
            insertAfter = NULL;
        }

        palError = FILEAddNewLockedRgn(
            fileLocks,
            pvControllerInstance,
            insertAfter,
            lockRgnStart,
            nbBytesToLock,
            lockAction
            );

        if (NO_ERROR != palError)
        {
            WARN("Couldn't add the new locked region into SHM\n");
            goto EXIT;
        }
    }
    else /* lock region list is empty. */
    {
        palError = FILEAddNewLockedRgn(
            fileLocks,
            pvControllerInstance,
            NULL,
            lockRgnStart,
            nbBytesToLock,
            lockAction
            );
        
        if (NO_ERROR != palError)
        {
            ERROR("Couldn't add the first file locked region \n");
            goto EXIT;
        }
    }  

EXIT:
    SHMRelease();
    return palError;
}

PAL_ERROR
FILEUnlockFileRegion(
    SHMPTR shmFileLocks,
    void * pvControllerInstance,
    unsigned long unlockRgnStart, 
    unsigned long nbBytesToUnlock,
    LOCK_TYPE unlockType
    )
{
    PAL_ERROR palError = NO_ERROR;
    SHMFILELOCKRGNS *prevLock = NULL, *curLockRgn = NULL, unlockRgn;
    SHMPTR shmcurLockRgn;
    SHMFILELOCKS *fileLocks;

    SHMLock();

    
    /* check if the region to unlock is empty or not */
    if (nbBytesToUnlock == 0) 
    {
        palError = ERROR_NOT_LOCKED;
        WARN("Attempt to unlock an empty region\n");
        goto EXIT;
    }
    
    if ((SHMPTR_TO_TYPED_PTR_BOOL(SHMFILELOCKS, fileLocks, shmFileLocks) == FALSE) || 
        (fileLocks == NULL) ||
        (SHMPTR_TO_TYPED_PTR_BOOL(SHMFILELOCKRGNS, curLockRgn, fileLocks->fileLockedRgns) == FALSE))
    {
        ASSERT("Unable to get pointer from shm pointer.\n");
        palError = ERROR_INTERNAL_ERROR;
        goto EXIT;
    }
    
    unlockRgn.processId = getpid();
    unlockRgn.pvControllerInstance = pvControllerInstance;
    unlockRgn.lockRgnStart = unlockRgnStart;
    unlockRgn.nbBytesLocked = nbBytesToUnlock;
    unlockRgn.lockType = unlockType;

    shmcurLockRgn = fileLocks->fileLockedRgns;
    
    while((curLockRgn != NULL) && !IS_LOCK_EQUAL(curLockRgn, &unlockRgn))
    {
        prevLock = curLockRgn; 
        shmcurLockRgn = curLockRgn->next;
        if (SHMPTR_TO_TYPED_PTR_BOOL(SHMFILELOCKRGNS, curLockRgn, shmcurLockRgn) == FALSE)
        {
            ASSERT("Unable to get pointer from shm pointer.\n");
            goto EXIT;
        }
    }
    
    if (curLockRgn != NULL) 
    {
        TRACE("removing the lock region (%I64u, %I64u)\n", 
               curLockRgn->lockRgnStart, curLockRgn->nbBytesLocked);

        if (prevLock == NULL) 
        {
            /* removing the first lock */
            fileLocks->fileLockedRgns = curLockRgn->next;
        }
        else
        {
            prevLock->next = curLockRgn->next;
        }
        SHMfree(shmcurLockRgn);
    }
    else
    {
        /* the lock doesn't exist */
        WARN("Attempt to unlock a non locked region\n");
        palError = ERROR_NOT_LOCKED;
        goto EXIT;
    }
    
EXIT:    
    SHMRelease();
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
        ASSERT("Invalid Null parameter.\n");
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
        ASSERT("Unable to get pointer from shm pointer.\n");
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
            ASSERT("Unable to get pointer from shm pointer.\n");
            palError = ERROR_INTERNAL_ERROR;
            goto EXIT;
        }
    }
    else
    {
        if (SHMPTR_TO_TYPED_PTR_BOOL(SHMFILELOCKRGNS, lockRgnPtr, fileLocks->fileLockedRgns) == FALSE)
        {
            ASSERT("Unable to get pointer from shm pointer.\n");
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
                ASSERT("Unable to get pointer from shm pointer.\n");
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
        ASSERT("Unable to get pointer from shm pointer.\n");
        goto EXIT;
    }

    if (fileLocks != NULL)
    {
        if(fileLocks->fileLockedRgns !=0)
        {        
            shmcurLockRgn = fileLocks->fileLockedRgns;
            if (SHMPTR_TO_TYPED_PTR_BOOL(SHMFILELOCKRGNS, curLockRgn, shmcurLockRgn) == FALSE)
            {
                ASSERT("Unable to get pointer from shm pointer.\n");
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
                            ASSERT("Unable to get pointer from shm pointer.\n");
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
                            ASSERT("Unable to get pointer from shm pointer.\n");
                            goto EXIT;
                        }
                    }
                    continue;
                }
                
                prevLock = curLockRgn;
                shmcurLockRgn = curLockRgn->next;
                if (SHMPTR_TO_TYPED_PTR_BOOL(SHMFILELOCKRGNS, curLockRgn, shmcurLockRgn) == FALSE)
                {
                    ASSERT("Unable to get pointer from shm pointer.\n");
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
                ASSERT("Unable to get pointer from shm pointer.\n");
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


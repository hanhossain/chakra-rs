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
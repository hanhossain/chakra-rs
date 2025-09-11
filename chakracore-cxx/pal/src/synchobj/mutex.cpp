//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
//

/*++



Module Name:

    mutex.ccpp

Abstract:

    Implementation of mutex synchroniztion object as described in
    the WIN32 API

Revision History:



--*/

#include "pal/mutex.hpp"
#include "pal/thread.hpp"
#include "pal/dbgmsg.h"

using namespace CorUnix;

/* ------------------- Definitions ------------------------------*/
SET_DEFAULT_DEBUG_CHANNEL(SYNC);

enum
{
    c_cchMaxMutex = MAX_LONGPATH + 1
};

CObjectType CorUnix::otMutex PAL_GLOBAL (
                otiMutex,
                NULL,   // No cleanup routine
                NULL,   // No initialization routine
                0,      // No immutable data
                0,      // No process local data
                0,      // No shared data
                0,      // Should be MUTEX_ALL_ACCESS; currently ignored (no Win32 security)
                CObjectType::SecuritySupported,
                CObjectType::SecurityInfoNotPersisted,
                CObjectType::ObjectCanHaveName,
                CObjectType::CrossProcessDuplicationAllowed,
                CObjectType::WaitableObject,
                CObjectType::ObjectCanBeUnsignaled,
                CObjectType::ThreadReleaseAltersSignalCount,
                CObjectType::OwnershipTracked
                );

CAllowedObjectTypes aotMutex PAL_GLOBAL (otiMutex);

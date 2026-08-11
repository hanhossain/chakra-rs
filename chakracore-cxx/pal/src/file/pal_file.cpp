//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information. 
//

/*++



Module Name:

    file.cpp

Abstract:

    Implementation of the file WIN API for the PAL



--*/

#include <string>
#include "pal/thread.hpp"
#include "pal/file.hpp"
#include <new>
#include "pal/stackstring.hpp"

#include "pal/palinternal.h"
#include "pal/dbgmsg.h"
#include "pal/file.h"
#include "pal/filetime.h"
#include "pal/utils.h"

#include <time.h>
#include <stdio.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <sys/mount.h>
#include <errno.h>
#include <limits.h>
#include "chakra/Logger.h"

// TODO (hanhossain): remove more-pal-file

using namespace CorUnix;

SET_DEFAULT_DEBUG_CHANNEL(FILE);

int MaxWCharToAcpLengthFactor = 3;

void FileCleanupRoutine(CPalThread *pThread, IPalObject *pObjectToCleanup, bool fShutdown, bool);

CObjectType CorUnix::otFile __attribute__((init_priority(200))) (
                otiFile,
                FileCleanupRoutine,
                0,      // No immutable data
                sizeof(CFileProcessLocalData),
                0,      // No shared data
                CObjectType::UnwaitableObject,
                CObjectType::SignalingNotApplicable,
                CObjectType::ThreadReleaseNotApplicable,
                CObjectType::OwnershipNotApplicable
                );

CAllowedObjectTypes CorUnix::aotFile __attribute__((init_priority(200))) (otiFile);

void FileCleanupRoutine(CPalThread *pThread, IPalObject *pObjectToCleanup, bool fShutdown, bool)
{
    PAL_ERROR palError;
    CFileProcessLocalData *pLocalData = NULL;
    IDataLock *pLocalDataLock = NULL;

    palError = pObjectToCleanup->GetProcessLocalData(
        pThread, 
        ReadLock,
        &pLocalDataLock,
        reinterpret_cast<void**>(&pLocalData)
        );

    if (NO_ERROR != palError)
    {
        chakra::Logger::error("Unable to obtain data to cleanup file object");
        return;
    }

    if (!fShutdown && -1 != pLocalData->unix_fd)
    {
        close(pLocalData->unix_fd);
    }

    pLocalDataLock->ReleaseLock(pThread);
}

//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
//

// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

/*++



Module Name:

    semaphore.cpp

Abstract:

    Implementation of the sempahore synchroniztion object as described in
    the WIN32 API

Revision History:



--*/

#include "pal/semaphore.hpp"
#include "pal/thread.hpp"
#include "pal/dbgmsg.h"

using namespace CorUnix;

/* ------------------- Definitions ------------------------------*/
SET_DEFAULT_DEBUG_CHANNEL(SYNC);

CObjectType CorUnix::otSemaphore(
                otiSemaphore,
                NULL,   // No cleanup routine
                NULL,   // No initialization routine
                sizeof(SemaphoreImmutableData),
                // NULL,   // No immutable data copy routine
                // NULL,   // No immutable data cleanup routine
                0,      // No process local data
                // NULL,   // No process local data cleanup routine
                0,      // No shared data
                0,      // Should be SEMAPHORE_ALL_ACCESS; currently ignored (no Win32 security)
                CObjectType::SecuritySupported,
                CObjectType::SecurityInfoNotPersisted,
                CObjectType::UnnamedObject,
                CObjectType::LocalDuplicationOnly,
                CObjectType::WaitableObject,
                CObjectType::ObjectCanBeUnsignaled,
                CObjectType::ThreadReleaseAltersSignalCount,
                CObjectType::NoOwner
                );

CAllowedObjectTypes aotSempahore(otiSemaphore);

/*++
Function:
CreateSemaphoreExA

Note:
lpSemaphoreAttributes currently ignored:
-- Win32 object security not supported
-- handles to semaphore objects are not inheritable

Parameters:
See MSDN doc.
--*/

HANDLE
CreateSemaphoreExA(
         LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
         int32_t lInitialCount,
         int32_t lMaximumCount,
         LPCSTR lpName,
         /*_Reserved_*/  uint32_t dwFlags,
         uint32_t dwDesiredAccess)
{
    // dwFlags is reserved and unused, and dwDesiredAccess is currently
    // only ever used as SEMAPHORE_ALL_ACCESS.  The other parameters
    // all map to CreateSemaphoreA.
    _ASSERTE(SEMAPHORE_ALL_ACCESS == dwDesiredAccess);

    return CreateSemaphoreA(
        lpSemaphoreAttributes,
        lInitialCount,
        lMaximumCount,
        lpName);
}

/*++
Function:
  CreateSemaphoreA

Note:
  lpSemaphoreAttributes currently ignored:
  -- Win32 object security not supported
  -- handles to semaphore objects are not inheritable

Parameters:
  See MSDN doc.
--*/

HANDLE
CreateSemaphoreA(
          LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
          int32_t lInitialCount,
          int32_t lMaximumCount,
          LPCSTR lpName)
{
    HANDLE hSemaphore = NULL;
    CPalThread *pthr = NULL;
    PAL_ERROR palError;

    PERF_ENTRY(CreateSemaphoreA);
    ENTRY("CreateSemaphoreA(lpSemaphoreAttributes=%p, lInitialCount=%d, "
          "lMaximumCount=%d, lpName=%p (%s))\n",
          lpSemaphoreAttributes, lInitialCount, lMaximumCount, lpName, lpName?lpName:"NULL");

    pthr = InternalGetCurrentThread();

    if (lpName != nullptr)
    {
        ASSERT("lpName: Cross-process named objects are not supported in PAL");
        palError = ERROR_NOT_SUPPORTED;
    }
    else
    {
        palError = InternalCreateSemaphore(
            pthr,
            lpSemaphoreAttributes,
            lInitialCount,
            lMaximumCount,
            NULL,
            &hSemaphore
            );
    }

    //
    // We always need to set last error, even on success:
    // we need to protect ourselves from the situation
    // where last error is set to ERROR_ALREADY_EXISTS on
    // entry to the function
    //

    pthr->SetLastError(palError);

    LOGEXIT("CreateSemaphoreA returns HANDLE %p\n", hSemaphore);
    PERF_EXIT(CreateSemaphoreA);
    return hSemaphore;
}

/*++
Function:
CreateSemaphoreExW

Note:
lpSemaphoreAttributes currentely ignored:
-- Win32 object security not supported
-- handles to semaphore objects are not inheritable

Parameters:
See MSDN doc.
--*/

HANDLE
CreateSemaphoreExW(
         LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
         int32_t lInitialCount,
         int32_t lMaximumCount,
         LPCWSTR lpName,
         /*_Reserved_*/  uint32_t dwFlags,
         uint32_t dwDesiredAccess)
{
    // dwFlags is reserved and unused

    return CreateSemaphoreW(
        lpSemaphoreAttributes,
        lInitialCount,
        lMaximumCount,
        lpName);
}

/*++
Function:
  CreateSemaphoreW

Note:
  lpSemaphoreAttributes currentely ignored:
  -- Win32 object security not supported
  -- handles to semaphore objects are not inheritable

Parameters:
  See MSDN doc.
--*/

HANDLE
CreateSemaphoreW(
          LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
          int32_t lInitialCount,
          int32_t lMaximumCount,
          LPCWSTR lpName)
{
    HANDLE hSemaphore = NULL;
    PAL_ERROR palError;
    CPalThread *pthr = NULL;

    PERF_ENTRY(CreateSemaphoreW);
    ENTRY("CreateSemaphoreW(lpSemaphoreAttributes=%p, lInitialCount=%d, "
          "lMaximumCount=%d, lpName=%p (%S))\n",
          lpSemaphoreAttributes, lInitialCount, lMaximumCount,
          lpName, lpName?lpName:W16_NULLSTRING);

    pthr = InternalGetCurrentThread();

    palError = InternalCreateSemaphore(
        pthr,
        lpSemaphoreAttributes,
        lInitialCount,
        lMaximumCount,
        lpName,
        &hSemaphore
        );

    //
    // We always need to set last error, even on success:
    // we need to protect ourselves from the situation
    // where last error is set to ERROR_ALREADY_EXISTS on
    // entry to the function
    //

    pthr->SetLastError(palError);

    LOGEXIT("CreateSemaphoreW returns HANDLE %p\n", hSemaphore);
    PERF_EXIT(CreateSemaphoreW);
    return hSemaphore;
}

/*++
Function:
  InternalCreateSemaphore

Note:
  lpSemaphoreAttributes currentely ignored:
  -- Win32 object security not supported
  -- handles to semaphore objects are not inheritable

Parameters
  pthr -- thread data for calling thread
  phEvent -- on success, receives the allocated semaphore handle

  See MSDN docs on CreateSemaphore for all other parameters.
--*/

PAL_ERROR
CorUnix::InternalCreateSemaphore(
    CPalThread *pthr,
    LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
    int32_t lInitialCount,
    int32_t lMaximumCount,
    LPCWSTR lpName,
    HANDLE *phSemaphore
    )
{
    CObjectAttributes oa(lpName, lpSemaphoreAttributes);
    PAL_ERROR palError = NO_ERROR;
    IPalObject *pobjSemaphore = NULL;
    IPalObject *pobjRegisteredSemaphore = NULL;
    SemaphoreImmutableData *pSemaphoreData;

    _ASSERTE(NULL != pthr);
    _ASSERTE(NULL != phSemaphore);

    ENTRY("InternalCreateSemaphore(pthr=%p, lpSemaphoreAttributes=%p, "
        "lInitialCount=%d, lMaximumCount=%d, lpName=%p, phSemaphore=%p)\n",
        pthr,
        lpSemaphoreAttributes,
        lInitialCount,
        lMaximumCount,
        lpName,
        phSemaphore
        );

    if (lpName != nullptr)
    {
        ASSERT("lpName: Cross-process named objects are not supported in PAL");
        palError = ERROR_NOT_SUPPORTED;
        goto InternalCreateSemaphoreExit;
    }

    if (lMaximumCount <= 0)
    {
        ERROR("lMaximumCount is invalid (%d)\n", lMaximumCount);
        palError = ERROR_INVALID_PARAMETER;
        goto InternalCreateSemaphoreExit;
    }

    if ((lInitialCount < 0) || (lInitialCount > lMaximumCount))
    {
        ERROR("lInitialCount is invalid (%d)\n", lInitialCount);
        palError = ERROR_INVALID_PARAMETER;
        goto InternalCreateSemaphoreExit;
    }

    palError = g_pObjectManager->AllocateObject(
        pthr,
        &otSemaphore,
        &oa,
        &pobjSemaphore
        );

    if (NO_ERROR != palError)
    {
        goto InternalCreateSemaphoreExit;
    }

    palError = pobjSemaphore->GetImmutableData(reinterpret_cast<void**>(&pSemaphoreData));

    if (NO_ERROR != palError)
    {
        ASSERT("Error %d obtaining object data\n", palError);
        goto InternalCreateSemaphoreExit;
    }

    pSemaphoreData->lMaximumCount = lMaximumCount;

    if (0 != lInitialCount)
    {
        ISynchStateController *pssc;

        palError = pobjSemaphore->GetSynchStateController(
            pthr,
            &pssc
            );

        if (NO_ERROR == palError)
        {
            palError = pssc->SetSignalCount(lInitialCount);
            pssc->ReleaseController();
        }

        if (NO_ERROR != palError)
        {
            ASSERT("Unable to set new semaphore state (%d)\n", palError);
            goto InternalCreateSemaphoreExit;
        }
    }

    palError = g_pObjectManager->RegisterObject(
        pthr,
        pobjSemaphore,
        &aotSempahore,
        0, // Should be SEMAPHORE_ALL_ACCESS; currently ignored (no Win32 security)
        phSemaphore,
        &pobjRegisteredSemaphore
        );

    //
    // pobjSemaphore is invalidated by the call to RegisterObject, so NULL it
    // out here to ensure that we don't try to release a reference on
    // it down the line.
    //

    pobjSemaphore = NULL;

InternalCreateSemaphoreExit:

    if (NULL != pobjSemaphore)
    {
        pobjSemaphore->ReleaseReference(pthr);
    }

    if (NULL != pobjRegisteredSemaphore)
    {
        pobjRegisteredSemaphore->ReleaseReference(pthr);
    }

    LOGEXIT("InternalCreateSemaphore returns %d\n", palError);

    return palError;
}


/*++
Function:
  ReleaseSemaphore

Parameters:
  See MSDN doc.
--*/

BOOL
ReleaseSemaphore(
          HANDLE hSemaphore,
          int32_t lReleaseCount,
          int32_t * lpPreviousCount)
{
    PAL_ERROR palError = NO_ERROR;
    CPalThread *pthr = NULL;

    PERF_ENTRY(ReleaseSemaphore);
    ENTRY("ReleaseSemaphore(hSemaphore=%p, lReleaseCount=%d, "
          "lpPreviousCount=%p)\n",
          hSemaphore, lReleaseCount, lpPreviousCount);

    pthr = InternalGetCurrentThread();

    palError = InternalReleaseSemaphore(
        pthr,
        hSemaphore,
        lReleaseCount,
        lpPreviousCount
        );

    if (NO_ERROR != palError)
    {
        pthr->SetLastError(palError);
    }

    LOGEXIT ("ReleaseSemaphore returns BOOL %d\n", (NO_ERROR == palError));
    PERF_EXIT(ReleaseSemaphore);
    return (NO_ERROR == palError);
}

/*++
Function:
  InternalReleaseSemaphore

Parameters:
  pthr -- thread data for calling thread

  See MSDN docs on ReleaseSemaphore for all other parameters
--*/

PAL_ERROR
CorUnix::InternalReleaseSemaphore(
    CPalThread *pthr,
    HANDLE hSemaphore,
    int32_t lReleaseCount,
    int32_t * lpPreviousCount
    )
{
    PAL_ERROR palError = NO_ERROR;
    IPalObject *pobjSemaphore = NULL;
    ISynchStateController *pssc = NULL;
    SemaphoreImmutableData *pSemaphoreData;
    int32_t lOldCount;

    _ASSERTE(NULL != pthr);

    ENTRY("InternalReleaseSempahore(pthr=%p, hSemaphore=%p, lReleaseCount=%d, "
        "lpPreviousCount=%p)\n",
        pthr,
        hSemaphore,
        lReleaseCount,
        lpPreviousCount
        );

    if (0 >= lReleaseCount)
    {
        palError = ERROR_INVALID_PARAMETER;
        goto InternalReleaseSemaphoreExit;
    }

    palError = g_pObjectManager->ReferenceObjectByHandle(
        pthr,
        hSemaphore,
        &aotSempahore,
        0, // Should be SEMAPHORE_MODIFY_STATE; currently ignored (no Win32 security)
        &pobjSemaphore
        );

    if (NO_ERROR != palError)
    {
        ERROR("Unable to obtain object for handle %p (error %d)!\n", hSemaphore, palError);
        goto InternalReleaseSemaphoreExit;
    }

    palError = pobjSemaphore->GetImmutableData(reinterpret_cast<void**>(&pSemaphoreData));

    if (NO_ERROR != palError)
    {
        ASSERT("Error %d obtaining object data\n", palError);
        goto InternalReleaseSemaphoreExit;
    }

    palError = pobjSemaphore->GetSynchStateController(
        pthr,
        &pssc
        );

    if (NO_ERROR != palError)
    {
        ASSERT("Error %d obtaining synch state controller\n", palError);
        goto InternalReleaseSemaphoreExit;
    }

    palError = pssc->GetSignalCount(&lOldCount);

    if (NO_ERROR != palError)
    {
        ASSERT("Error %d obtaining current signal count\n", palError);
        goto InternalReleaseSemaphoreExit;
    }

    _ASSERTE(lOldCount <= pSemaphoreData->lMaximumCount);
    if (lReleaseCount > pSemaphoreData->lMaximumCount - lOldCount)
    {
        palError = ERROR_INVALID_PARAMETER;
        goto InternalReleaseSemaphoreExit;
    }

    palError = pssc->IncrementSignalCount(lReleaseCount);

    if (NO_ERROR != palError)
    {
        ASSERT("Error %d incrementing signal count\n", palError);
        goto InternalReleaseSemaphoreExit;
    }

    if (NULL != lpPreviousCount)
    {
        *lpPreviousCount = lOldCount;
    }

InternalReleaseSemaphoreExit:

    if (NULL != pssc)
    {
        pssc->ReleaseController();
    }

    if (NULL != pobjSemaphore)
    {
        pobjSemaphore->ReleaseReference(pthr);
    }

    LOGEXIT("InternalReleaseSemaphore returns %d\n", palError);

    return palError;
}

// TODO: Implementation of OpenSemaphoreA() doesn't exist, do we need it? More generally, do we need the A versions at all?

/*++
Function:
  OpenSemaphoreW

Note:
  dwDesiredAccess is currently ignored (no Win32 object security support)
  bInheritHandle is currently ignored (handles to semaphore are not inheritable)

Parameters:
  See MSDN doc.
--*/

HANDLE
OpenSemaphoreW(
        uint32_t dwDesiredAccess,
        BOOL bInheritHandle,
        LPCWSTR lpName)
{
    HANDLE hSemaphore = NULL;
    PAL_ERROR palError = NO_ERROR;
    CPalThread *pthr = NULL;

    PERF_ENTRY(OpenSemaphoreW);
    ENTRY("OpenSemaphoreW(dwDesiredAccess=%#x, bInheritHandle=%d, lpName=%p (%S))\n",
          dwDesiredAccess, bInheritHandle, lpName, lpName?lpName:W16_NULLSTRING);

    pthr = InternalGetCurrentThread();

    /* validate parameters */
    if (lpName == nullptr)
    {
        ERROR("lpName is NULL\n");
        palError = ERROR_INVALID_PARAMETER;
    }
    else
    {
        ASSERT("lpName: Cross-process named objects are not supported in PAL");
        palError = ERROR_NOT_SUPPORTED;
    }

    if (NO_ERROR != palError)
    {
        pthr->SetLastError(palError);
    }

    LOGEXIT("OpenSemaphoreW returns HANDLE %p\n", hSemaphore);
    PERF_EXIT(OpenSemaphoreW);

    return hSemaphore;
}

//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
//

// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

/*++



Module Name:

    semaphore.hpp

Abstract:

    Semaphore object structure definition.



--*/

#ifndef _PAL_SEMAPHORE_H_
#define _PAL_SEMAPHORE_H_

#include "corunix.hpp"

namespace CorUnix
{
    extern CObjectType otSemaphore;

    typedef struct
    {
        int32_t lMaximumCount;
    } SemaphoreImmutableData;

    PAL_ERROR
    InternalCreateSemaphore(
        CPalThread *pThread,
        LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
        int32_t lInitialCount,
        int32_t lMaximumCount,
        const char16_t* lpName,
        HANDLE *phSemaphore
        );

    PAL_ERROR
    InternalReleaseSemaphore(
        CPalThread *pThread,
        HANDLE hSemaphore,
        int32_t lReleaseCount,
        int32_t * lpPreviousCount
        );

}

#endif //_PAL_SEMAPHORE_H_

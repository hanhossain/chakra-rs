//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "RecyclerPageAllocator.h"

RecyclerPageAllocator::RecyclerPageAllocator(HeapInfo * heapInfo, AllocationPolicyManager * policyManager,
    Js::ConfigFlagsTable& flagTable, uint maxFreePageCount, uint maxAllocPageCount, bool enableWriteBarrier)
    : IdleDecommitPageAllocator(policyManager,
        PageAllocatorType_Recycler,
        flagTable,
        0, maxFreePageCount,
        true,
        &zeroPageQueue,
        maxAllocPageCount,
        enableWriteBarrier
        )
{
    this->heapInfo = heapInfo;
}

bool RecyclerPageAllocator::IsMemProtectMode()
{
    return heapInfo->GetRecycler()->IsMemProtectMode();
}

//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "CommonMemoryPch.h"
#include "RecyclerPageAllocator.h"

RecyclerPageAllocator::RecyclerPageAllocator(HeapInfo * heapInfo, AllocationPolicyManager * policyManager,
    Js::ConfigFlagsTable& flagTable, uint maxFreePageCount, uint maxAllocPageCount, bool enableWriteBarrier)
    : IdleDecommitPageAllocator(policyManager,
        PageAllocatorType_Recycler,
        flagTable,
        0, maxFreePageCount,
        true,
#if ENABLE_BACKGROUND_PAGE_ZEROING
        &zeroPageQueue,
#endif
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

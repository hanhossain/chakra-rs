//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once
#include "Memory/IdleDecommitPageAllocator.h"

namespace Memory
{

class HeapInfo;
class RecyclerPageAllocator : public IdleDecommitPageAllocator
{
public:
    RecyclerPageAllocator(HeapInfo * heapInfo, AllocationPolicyManager * policyManager,
        Js::ConfigFlagsTable& flagTable, uint maxFreePageCount, uint maxAllocPageCount = PageAllocator::DefaultMaxAllocPageCount, bool enableWriteBarrier = false);

    static uint const DefaultPrimePageCount = 0x1000; // 16MB

private:
    ZeroPageQueue zeroPageQueue;

    HeapInfo * heapInfo;

    bool IsMemProtectMode();
};
}

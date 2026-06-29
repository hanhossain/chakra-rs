//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

inline char *
LargeHeapBucket::TryAlloc(Recycler * recycler, size_t sizeCat, ObjectInfoBits attributes)
{
    Assert((attributes & InternalObjectInfoBitMask) == attributes);

    char * memBlock;

    // Algorithm:
    //  Try bump allocate from a heap block
    //  Otherwise, If free list isn't empty, allocate from it
    //  * Otherwise, try allocate from a larger heap block (TODO)
    //  Otherwise allocate new heap block

    if (this->largeBlockList != nullptr)
    {
        memBlock = this->largeBlockList->Alloc(sizeCat, attributes);
        if (memBlock != nullptr)
        {
            // Don't need to verify zero fill here since we will do it in LargeHeapBucket::Alloc
            return memBlock;
        }
    }

    if (!this->supportFreeList)
    {
        return nullptr;
    }

    memBlock = this->TryAllocFromExplicitFreeList(recycler, sizeCat, attributes);
    if (memBlock != nullptr)
    {
        // Don't need to verify zero fill here since we will do it in LargeHeapBucket::Alloc
        return memBlock;
    }

    return this->TryAllocFromFreeList(recycler, sizeCat, attributes);
}

template <ObjectInfoBits attributes, bool nothrow>
inline char *
LargeHeapBucket::Alloc(Recycler * recycler, size_t sizeCat)
{
    Assert(!HeapInfo::IsMediumObject(sizeCat) || HeapInfo::GetMediumObjectAlignedSizeNoCheck(sizeCat) == this->sizeCat);
    Assert((attributes & InternalObjectInfoBitMask) == attributes);

    char * memBlock = TryAlloc(recycler, sizeCat, attributes);
    if (memBlock == nullptr)
    {
        memBlock = SnailAlloc(recycler, sizeCat, attributes, nothrow);
        Assert(memBlock != nullptr);
    }
    else
    {
        Assert(!IsPageHeapEnabled(attributes));
    }

#ifdef RECYCLER_ZERO_MEM_CHECK
    // TODO: large heap block doesn't separate leaf object on to different page allocator.
    // so all the memory should still be zeroed.
    recycler->VerifyZeroFill(memBlock, sizeCat);
#endif
    return memBlock;
}


template <class Fn>
void
LargeHeapBucket::ForEachLargeHeapBlock(Fn fn)
{
    HeapBlockList::ForEach(fullLargeBlockList, fn);
    HeapBlockList::ForEach(largeBlockList, fn);
    HeapBlockList::ForEach(largePageHeapBlockList, fn);
    HeapBlockList::ForEach(pendingDisposeLargeBlockList, fn);
    HeapBlockList::ForEach(pendingSweepLargeBlockList, fn);
    HeapBlockList::ForEach(partialSweptLargeBlockList, fn);
}

template <class Fn>
void
LargeHeapBucket::ForEachEditingLargeHeapBlock(Fn fn)
{
    HeapBlockList::ForEachEditing(fullLargeBlockList, fn);
    HeapBlockList::ForEachEditing(largeBlockList, fn);
    HeapBlockList::ForEachEditing(largePageHeapBlockList, fn);
    HeapBlockList::ForEachEditing(pendingDisposeLargeBlockList, fn);
    HeapBlockList::ForEachEditing(pendingSweepLargeBlockList, fn);
    HeapBlockList::ForEachEditing(partialSweptLargeBlockList, fn);
}

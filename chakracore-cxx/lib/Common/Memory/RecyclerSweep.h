//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once
#include "Memory/RecyclerWeakReference.h"

namespace Memory
{
// RecyclerSweep - Sweeping algorithm and state
class RecyclerSweep
#if DBG
    : public RecyclerVerifyListConsistencyData
#endif
{
public:

    void BeginSweep(Recycler * recycler, RecyclerSweepManager * recyclerSweepManager, HeapInfo * heapInfo);
    void ShutdownCleanup();

    Recycler * GetRecycler() const;
    RecyclerSweepManager * GetManager() const;

    bool IsBackground() const;
    void FlushPendingTransferDisposedObjects();

    bool HasPendingSweepSmallHeapBlocks() const;
    void SetHasPendingSweepSmallHeapBlocks();
    template <typename TBlockType>
    TBlockType *& GetPendingSweepBlockList(HeapBucketT<TBlockType> const * heapBucket);
    bool HasPendingEmptyBlocks() const;
    template <typename TBlockType> void QueueEmptyHeapBlock(HeapBucketT<TBlockType> const *heapBucket, TBlockType * heapBlock);
    template <typename TBlockType> void TransferPendingEmptyHeapBlocks(HeapBucketT<TBlockType> * heapBucket);


    template <typename TBlockType> void SetPendingMergeNewHeapBlockList(TBlockType * heapBlockList);
    template <typename TBlockType> void MergePendingNewHeapBlockList();
    template <typename TBlockType> void MergePendingNewMediumHeapBlockList();
#if DBG
    bool HasPendingNewHeapBlocks() const;
    template <typename TBlockType> TBlockType * GetSavedNextAllocableBlockHead(HeapBucketT<TBlockType> const * heapBucket);
    template <typename TBlockType> void SaveNextAllocableBlockHead(HeapBucketT<TBlockType> const * heapBucket);
#endif
#if DBG || defined(RECYCLER_SLOW_CHECK_ENABLED)
    template <typename TBlockType> size_t GetHeapBlockCount(HeapBucketT<TBlockType> const * heapBucket);
    size_t GetPendingMergeNewHeapBlockCount(HeapInfo const * heapInfo);
#endif

    bool InPartialCollectMode() const;
    bool InPartialCollect() const;

private:
    template <typename TBlockType>
    struct BucketData
    {
        TBlockType * pendingSweepList;
        TBlockType * pendingFinalizableSweptList;
        TBlockType * pendingEmptyBlockList;
        TBlockType * pendingEmptyBlockListTail;
#if DBG

        TBlockType * savedNextAllocableBlockHead;
#endif
    };

    template <typename TBlockType>
    BucketData<TBlockType>& GetBucketData(HeapBucketT<TBlockType> const * bucket)
    {
        if (TBlockType::HeapBlockAttributes::IsSmallBlock)
        {
            return this->GetData<TBlockType>().bucketData[bucket->GetBucketIndex()];
        }
        else
        {
            Assert(TBlockType::HeapBlockAttributes::IsMediumBlock);
            return this->GetData<TBlockType>().bucketData[bucket->GetMediumBucketIndex()];
        }
    }

    template <typename TBlockType>
    struct Data
    {
        BucketData<TBlockType> bucketData[TBlockType::HeapBlockAttributes::BucketCount];
        TBlockType * pendingMergeNewHeapBlockList;
    };

    template <typename TBlockType> Data<TBlockType>& GetData();
    template <> Data<SmallLeafHeapBlock>& GetData<SmallLeafHeapBlock>() { return leafData; }
    template <> Data<SmallNormalHeapBlock>& GetData<SmallNormalHeapBlock>() { return normalData; }
    template <> Data<SmallFinalizableHeapBlock>& GetData<SmallFinalizableHeapBlock>() { return finalizableData; }
#ifdef RECYCLER_VISITED_HOST
    template <> Data<SmallRecyclerVisitedHostHeapBlock>& GetData<SmallRecyclerVisitedHostHeapBlock>() { return recyclerVisitedHostData; }
#endif
    template <> Data<SmallNormalWithBarrierHeapBlock>& GetData<SmallNormalWithBarrierHeapBlock>() { return withBarrierData; }
    template <> Data<SmallFinalizableWithBarrierHeapBlock>& GetData<SmallFinalizableWithBarrierHeapBlock>() { return finalizableWithBarrierData; }

    template <> Data<MediumLeafHeapBlock>& GetData<MediumLeafHeapBlock>() { return mediumLeafData; }
    template <> Data<MediumNormalHeapBlock>& GetData<MediumNormalHeapBlock>() { return mediumNormalData; }
    template <> Data<MediumFinalizableHeapBlock>& GetData<MediumFinalizableHeapBlock>() { return mediumFinalizableData; }
#ifdef RECYCLER_VISITED_HOST
    template <> Data<MediumRecyclerVisitedHostHeapBlock>& GetData<MediumRecyclerVisitedHostHeapBlock>() { return mediumRecyclerVisitedHostData; }
#endif
    template <> Data<MediumNormalWithBarrierHeapBlock>& GetData<MediumNormalWithBarrierHeapBlock>() { return mediumWithBarrierData; }
    template <> Data<MediumFinalizableWithBarrierHeapBlock>& GetData<MediumFinalizableWithBarrierHeapBlock>() { return mediumFinalizableWithBarrierData; }

private:
    Recycler * recycler;
    RecyclerSweepManager * recyclerSweepManager;
    HeapInfo * heapInfo;

    Data<SmallLeafHeapBlock> leafData;
    Data<SmallNormalHeapBlock> normalData;
    Data<SmallFinalizableHeapBlock> finalizableData;
#ifdef RECYCLER_VISITED_HOST
    Data<SmallRecyclerVisitedHostHeapBlock> recyclerVisitedHostData;
#endif
    Data<SmallNormalWithBarrierHeapBlock> withBarrierData;
    Data<SmallFinalizableWithBarrierHeapBlock> finalizableWithBarrierData;
    Data<MediumLeafHeapBlock> mediumLeafData;
    Data<MediumNormalHeapBlock> mediumNormalData;
    Data<MediumFinalizableHeapBlock> mediumFinalizableData;
#ifdef RECYCLER_VISITED_HOST
    Data<MediumRecyclerVisitedHostHeapBlock> mediumRecyclerVisitedHostData;
#endif
    Data<MediumNormalWithBarrierHeapBlock> mediumWithBarrierData;
    Data<MediumFinalizableWithBarrierHeapBlock> mediumFinalizableWithBarrierData;

    bool hasPendingSweepSmallHeapBlocks;
    bool hasPendingEmptyBlocks;

};

template <typename TBlockType>
TBlockType *&
RecyclerSweep::GetPendingSweepBlockList(HeapBucketT<TBlockType> const * heapBucket)
{
    return this->GetBucketData<TBlockType>(heapBucket).pendingSweepList;
}

template <typename TBlockType>
void
RecyclerSweep::QueueEmptyHeapBlock(HeapBucketT<TBlockType> const *heapBucket, TBlockType * heapBlock)
{
#if ENABLE_BACKGROUND_PAGE_FREEING
    if (CONFIG_FLAG(EnableBGFreeZero))
    {
        auto& bucketData = this->GetBucketData(heapBucket);
        Assert(heapBlock->heapBucket == heapBucket);

        heapBlock->BackgroundReleasePagesSweep(recycler);

        TBlockType * list = bucketData.pendingEmptyBlockList;
        if (list == nullptr)
        {
            Assert(bucketData.pendingEmptyBlockListTail == nullptr);
            bucketData.pendingEmptyBlockListTail = heapBlock;
            this->hasPendingEmptyBlocks = true;
        }
        heapBlock->SetNextBlock(list);
        bucketData.pendingEmptyBlockList = heapBlock;
    }
#endif
}

template <typename TBlockType>
void
RecyclerSweep::TransferPendingEmptyHeapBlocks(HeapBucketT<TBlockType> * heapBucket)
{
    Assert(!this->IsBackground());
    Assert(!heapBucket->IsAllocationStopped());
    RECYCLER_SLOW_CHECK(heapBucket->VerifyHeapBlockCount(false));

    auto& bucketData = this->GetBucketData(heapBucket);
    Assert(bucketData.pendingSweepList == nullptr);
    TBlockType * list = bucketData.pendingEmptyBlockList;
    if (list)
    {
        TBlockType * tail = bucketData.pendingEmptyBlockListTail;
#if DBG || defined(RECYCLER_SLOW_CHECK_ENABLED)
        uint32_t count = 0;
        HeapBlockList::ForEach(list, [tail, &count](TBlockType * heapBlock)
        {
            Assert(heapBlock->GetAddress() == nullptr);
            Assert(heapBlock->GetSegment() == nullptr);
            Assert(heapBlock->GetNextBlock() != nullptr || heapBlock == tail);
            count++;
        });
#if defined(RECYCLER_SLOW_CHECK_ENABLED)
        heapBucket->emptyHeapBlockCount += count;
#endif
#if defined(RECYCLER_SLOW_CHECK_ENABLED)
        heapBucket->heapBlockCount -= count;
#endif
#endif


        tail->SetNextBlock(heapBucket->emptyBlockList);
        heapBucket->emptyBlockList = list;

        bucketData.pendingEmptyBlockList = nullptr;
        RECYCLER_SLOW_CHECK(heapBucket->VerifyHeapBlockCount(false));
    }
    else
    {
        Assert(bucketData.pendingEmptyBlockListTail == nullptr);
    }
}

template <typename TBlockType>
void
RecyclerSweep::SetPendingMergeNewHeapBlockList(TBlockType * heapBlockList)
{
    this->GetData<TBlockType>().pendingMergeNewHeapBlockList = heapBlockList;
}

#if DBG
template <typename TBlockType>
TBlockType *
RecyclerSweep::GetSavedNextAllocableBlockHead(HeapBucketT<TBlockType> const * heapBucket)
{
    return this->GetBucketData(heapBucket).savedNextAllocableBlockHead;
}
template <typename TBlockType>
void
RecyclerSweep::SaveNextAllocableBlockHead(HeapBucketT<TBlockType> const * heapBucket)
{
    this->GetBucketData(heapBucket).savedNextAllocableBlockHead = heapBucket->nextAllocableBlockHead;
}
#endif
#if DBG || defined(RECYCLER_SLOW_CHECK_ENABLED)
template <typename TBlockType>
size_t
RecyclerSweep::GetHeapBlockCount(HeapBucketT<TBlockType> const * heapBucket)
{
    if (heapBucket->heapInfo != this->heapInfo)
    {
        return 0;
    }
    auto& bucketData = this->GetBucketData(heapBucket);
    return HeapBlockList::Count(bucketData.pendingSweepList)
        + HeapBlockList::Count(bucketData.pendingEmptyBlockList);
}
#endif
}

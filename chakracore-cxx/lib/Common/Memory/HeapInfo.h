//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#pragma once
#include "Memory/RecyclerPageAllocator.h"
#include "Memory/RecyclerWriteBarrierManager.h"
#include "Memory/LargeHeapBucket.h"

namespace Memory
{
#if ENABLE_MEM_STATS
class BucketStatsReporter;
#endif
class HeapInfo
{
    friend class ::ScriptMemoryDumper;
public:
    HeapInfo(AllocationPolicyManager * policyManager, Js::ConfigFlagsTable& configFlagsTable, IdleDecommitPageAllocator * leafPageAllocator);
    ~HeapInfo();

    void Initialize(Recycler * recycler
#ifdef RECYCLER_PAGE_HEAP
        , PageHeapMode pageheapmode = PageHeapMode::PageHeapModeOff
        , bool captureAllocCallStack = false
        , bool captureFreeCallStack = false
#endif
        );
#if defined(PROFILE_RECYCLER_ALLOC) || defined(RECYCLER_MEMORY_VERIFY) || defined(MEMSPECT_TRACKING)
    void Initialize(Recycler * recycler, void(*trackNativeAllocCallBack)(Recycler *, void *, size_t)
#ifdef RECYCLER_PAGE_HEAP
        , PageHeapMode pageheapmode = PageHeapMode::PageHeapModeOff
        , bool captureAllocCallStack = false
        , bool captureFreeCallStack = false
#endif
        );
#endif

    void ResetMarks(ResetMarkFlags flags);
    void EnumerateObjects(ObjectInfoBits infoBits, void(*CallBackFunction)(void * address, size_t size));
#ifdef RECYCLER_PAGE_HEAP
    bool IsPageHeapEnabled() const{ return isPageHeapEnabled; }
    static size_t RoundObjectSize(size_t objectSize)
    {
        // triming off the tail part which is not a pointer
        return objectSize - (objectSize % sizeof(void*));
    }

    template<ObjectInfoBits attributes>
    bool IsPageHeapEnabled(size_t size)
    {
        if (IsPageHeapEnabled())
        {
            size_t sizeCat = HeapInfo::GetAlignedSizeNoCheck(size);
            if (HeapInfo::IsSmallObject(size))
            {
                auto& bucket = this->GetBucket<(ObjectInfoBits)(attributes & GetBlockTypeBitMask)>(sizeCat);
                return bucket.IsPageHeapEnabled(attributes);
            }
            else if (HeapInfo::IsMediumObject(size))
            {
                auto& bucket = this->GetMediumBucket<(ObjectInfoBits)(attributes & GetBlockTypeBitMask)>(sizeCat);
                return bucket.IsPageHeapEnabled(attributes);
            }
            else
            {
                return this->largeObjectBucket.IsPageHeapEnabled(attributes);
            }
        }
        return false;
    }

    template <typename TBlockAttributes>
    bool IsPageHeapEnabledForBlock(const size_t objectSize);
#else
    bool IsPageHeapEnabled() const{ return false; }
#endif

#if ENABLE_MEM_STATS
    void GetBucketStats(BucketStatsReporter& report);
#endif

    template <ObjectInfoBits attributes, bool nothrow>
    char * MediumAlloc(Recycler * recycler, size_t sizeCat, size_t size);

    // Small allocator
    template <ObjectInfoBits attributes, bool nothrow>
    char * RealAlloc(Recycler * recycler, size_t sizeCat, size_t size);
    template <ObjectInfoBits attributes>
    bool IntegrateBlock(char * blockAddress, PageSegment * segment, Recycler * recycler, size_t sizeCat);
    template <typename SmallHeapBlockAllocatorType>
    void AddSmallAllocator(SmallHeapBlockAllocatorType * allocator, size_t sizeCat);
    template <typename SmallHeapBlockAllocatorType>
    void RemoveSmallAllocator(SmallHeapBlockAllocatorType * allocator, size_t sizeCat);
    template <ObjectInfoBits attributes, typename SmallHeapBlockAllocatorType>
    char * SmallAllocatorAlloc(Recycler * recycler, SmallHeapBlockAllocatorType * allocator, size_t sizeCat, size_t size);

    // collection functions
    void ScanInitialImplicitRoots();
    void ScanNewImplicitRoots();

    size_t Rescan(RescanFlags flags);
    void SweepPendingObjects(RecyclerSweep& recyclerSweep);
    void Finalize(RecyclerSweep& recyclerSweep);
    void Sweep(RecyclerSweep& recyclerSweep, bool concurrent);

    template <ObjectInfoBits attributes>
    void FreeSmallObject(void* object, size_t bytes);

    template <ObjectInfoBits attributes>
    void FreeMediumObject(void* object, size_t bytes);

#if ENABLE_PARTIAL_GC
    void SweepPartialReusePages(RecyclerSweep& recyclerSweep);
    void FinishPartialCollect(RecyclerSweep * recyclerSweep);
#endif
    void PrepareSweep();
    void TransferPendingHeapBlocks(RecyclerSweep& recyclerSweep);
    void ConcurrentTransferSweptObjects(RecyclerSweep& recyclerSweep);

#if ENABLE_PARTIAL_GC
    void ConcurrentPartialTransferSweptObjects(RecyclerSweep& recyclerSweep);
#endif
    void DisposeObjects();
    void TransferDisposedObjects();

#ifdef RECYCLER_SLOW_CHECK_ENABLED
    void Check();
    template <typename TBlockType>
    static size_t Check(bool expectFull, bool expectPending, TBlockType * list, TBlockType * tail = nullptr);

    void VerifySmallHeapBlockCount();
    void VerifyLargeHeapBlockCount();
#endif

#ifdef RECYCLER_MEMORY_VERIFY
    void Verify();
#endif
#ifdef RECYCLER_VERIFY_MARK
    void VerifyMark();
#endif

public:
    static bool IsSmallObject(size_t nBytes) { return nBytes <= HeapConstants::MaxSmallObjectSize; }
    static bool IsMediumObject(size_t nBytes)
    {
        return nBytes > HeapConstants::MaxSmallObjectSize && nBytes <= HeapConstants::MaxMediumObjectSize;
    }

    static bool IsSmallBlockAllocation(size_t nBytes)
    {
        return HeapInfo::IsSmallObject(nBytes) || HeapInfo::IsMediumObject(nBytes);
    }

    static bool IsLargeObject(size_t nBytes)
    {
        return nBytes > HeapConstants::MaxMediumObjectSize;
    }

    static BOOL IsAlignedSize(size_t sizeCat) { return (sizeCat != 0) && (0 == (sizeCat & HeapInfo::ObjectAlignmentMask)); }
    static BOOL IsAlignedSmallObjectSize(size_t sizeCat) { return (sizeCat != 0) && (HeapInfo::IsSmallObject(sizeCat) && (0 == (sizeCat & HeapInfo::ObjectAlignmentMask))); }
    static BOOL IsAlignedMediumObjectSize(size_t sizeCat) { return (sizeCat != 0) && (HeapInfo::IsMediumObject(sizeCat) && (0 == (sizeCat & HeapInfo::ObjectAlignmentMask))); }
    static size_t GetAlignedSize(size_t size) { return AllocSizeMath::Align(size, HeapConstants::ObjectGranularity); }
    static size_t GetAlignedSizeNoCheck(size_t size) { return Math::Align<size_t>(size, HeapConstants::ObjectGranularity); }
    static size_t GetMediumObjectAlignedSize(size_t size) { return AllocSizeMath::Align(size, HeapConstants::MediumObjectGranularity); }
    static size_t GetMediumObjectAlignedSizeNoCheck(size_t size) { return Math::Align<size_t>(size, HeapConstants::MediumObjectGranularity); }

    static inline uint GetBucketIndex(size_t sizeCat) { Assert(IsAlignedSmallObjectSize(sizeCat)); return (uint)(sizeCat >> HeapConstants::ObjectAllocationShift) - 1; }

    template <typename TBlockAttributes>
    static uint GetObjectSizeForBucketIndex(uint bucketIndex);

    static uint GetMediumBucketIndex(size_t sizeCat) { Assert(IsAlignedMediumObjectSize(sizeCat)); return (uint)((sizeCat - HeapConstants::MaxSmallObjectSize - 1) / HeapConstants::MediumObjectGranularity); }

    static BOOL IsAlignedAddress(void * address) { return (0 == (((size_t)address) & HeapInfo::ObjectAlignmentMask)); }
    static void * GetAlignedAddress(void * address) { return (void*)((uintptr_t)address & ~(uintptr_t)HeapInfo::ObjectAlignmentMask); }
private:
    template <ObjectInfoBits attributes>
    typename SmallHeapBlockType<attributes, SmallAllocationBlockAttributes>::BucketType& GetBucket(size_t sizeCat);
    
    template <ObjectInfoBits attributes>
    typename SmallHeapBlockType<attributes, MediumAllocationBlockAttributes>::BucketType& GetMediumBucket(size_t sizeCat);

    LargeHeapBlock * AddLargeHeapBlock(size_t pageCount);

    template <typename TBlockType>
    void AppendNewHeapBlock(TBlockType * heapBlock, HeapBucketT<TBlockType> * heapBucket)
    {
        TBlockType *& list = this->GetNewHeapBlockList<TBlockType>(heapBucket);
        heapBlock->SetNextBlock(list);
        list = heapBlock;
    }
    template <typename TBlockType> TBlockType *& GetNewHeapBlockList(HeapBucketT<TBlockType> * heapBucket);
    template <>
    SmallLeafHeapBlock *& GetNewHeapBlockList<SmallLeafHeapBlock>(HeapBucketT<SmallLeafHeapBlock> * heapBucket)
    {
        return this->newLeafHeapBlockList;
    }
    template <>
    SmallNormalHeapBlock *& GetNewHeapBlockList<SmallNormalHeapBlock>(HeapBucketT<SmallNormalHeapBlock> * heapBucket)
    {
        return this->newNormalHeapBlockList;
    }
    template <>
    SmallFinalizableHeapBlock *& GetNewHeapBlockList<SmallFinalizableHeapBlock>(HeapBucketT<SmallFinalizableHeapBlock> * heapBucket)
    {
        // Even though we don't concurrent sweep finalizable heap block, the background thread may
        // find some partial swept block to be reused, thus modifying the heapBlockList in the background
        // so new block can't go into heapBlockList
        return this->newFinalizableHeapBlockList;
    }
#ifdef RECYCLER_VISITED_HOST
    template <>
    SmallRecyclerVisitedHostHeapBlock *& GetNewHeapBlockList<SmallRecyclerVisitedHostHeapBlock>(HeapBucketT<SmallRecyclerVisitedHostHeapBlock> * HeapBucket)
    {
        return this->newRecyclerVisitedHostHeapBlockList;
    }
#endif

    template <>
    SmallNormalWithBarrierHeapBlock *& GetNewHeapBlockList<SmallNormalWithBarrierHeapBlock>(HeapBucketT<SmallNormalWithBarrierHeapBlock> * heapBucket)
    {
        return this->newNormalWithBarrierHeapBlockList;
    }

    template <>
    SmallFinalizableWithBarrierHeapBlock *& GetNewHeapBlockList<SmallFinalizableWithBarrierHeapBlock>(HeapBucketT<SmallFinalizableWithBarrierHeapBlock> * heapBucket)
    {
        return this->newFinalizableWithBarrierHeapBlockList;
    }

    template <>
    MediumLeafHeapBlock *& GetNewHeapBlockList<MediumLeafHeapBlock>(HeapBucketT<MediumLeafHeapBlock> * heapBucket)
    {
        return this->newMediumLeafHeapBlockList;
    }
    template <>
    MediumNormalHeapBlock *& GetNewHeapBlockList<MediumNormalHeapBlock>(HeapBucketT<MediumNormalHeapBlock> * heapBucket)
    {
        return this->newMediumNormalHeapBlockList;
    }
    template <>
    MediumFinalizableHeapBlock *& GetNewHeapBlockList<MediumFinalizableHeapBlock>(HeapBucketT<MediumFinalizableHeapBlock> * heapBucket)
    {
        // Even though we don't concurrent sweep finalizable heap block, the background thread may
        // find some partial swept block to be reused, thus modifying the heapBlockList in the background
        // so new block can't go into heapBlockList
        return this->newMediumFinalizableHeapBlockList;
    }

#ifdef RECYCLER_VISITED_HOST
    template <>
    MediumRecyclerVisitedHostHeapBlock *& GetNewHeapBlockList<MediumRecyclerVisitedHostHeapBlock>(HeapBucketT<MediumRecyclerVisitedHostHeapBlock> * HeapBucket)
    {
        return this->newMediumRecyclerVisitedHostHeapBlockList;
    }
#endif

    template <>
    MediumNormalWithBarrierHeapBlock *& GetNewHeapBlockList<MediumNormalWithBarrierHeapBlock>(HeapBucketT<MediumNormalWithBarrierHeapBlock> * heapBucket)
    {
        return this->newMediumNormalWithBarrierHeapBlockList;
    }

    template <>
    MediumFinalizableWithBarrierHeapBlock *& GetNewHeapBlockList<MediumFinalizableWithBarrierHeapBlock>(HeapBucketT<MediumFinalizableWithBarrierHeapBlock> * heapBucket)
    {
        return this->newMediumFinalizableWithBarrierHeapBlockList;
    }

    void SetupBackgroundSweep(RecyclerSweep& recyclerSweep);

    void SweepSmallNonFinalizable(RecyclerSweep& recyclerSweep);

#if DBG || defined(RECYCLER_SLOW_CHECK_ENABLED)
    size_t GetSmallHeapBlockCount(bool checkCount = false) const;
    size_t GetLargeHeapBlockCount(bool checkCount = false) const;
#endif

#if DBG
    bool AllocatorsAreEmpty();
#endif

private:
    template <typename TBlockAttributes>
    class ValidPointersMap
    {
    private:
        static const uint rowSize = TBlockAttributes::MaxSmallObjectCount * 2;
        typedef ushort ValidPointersMapRow[rowSize];
        typedef ValidPointersMapRow ValidPointersMapTable[TBlockAttributes::BucketCount];
        typedef typename SmallHeapBlockT<TBlockAttributes>::SmallHeapBlockBitVector InvalidBitsTable[TBlockAttributes::BucketCount];
        typedef typename SmallHeapBlockT<TBlockAttributes>::BlockInfo BlockInfoMapRow[TBlockAttributes::PageCount];
        typedef BlockInfoMapRow BlockInfoMapTable[TBlockAttributes::BucketCount];

        // Architecture-dependent initialization done in ValidPointersMap/vpm.(32b|64b).h
            ValidPointersMapTable validPointersBuffer;

        BlockInfoMapTable blockInfoBuffer;

    public:
        InvalidBitsTable invalidBitsBuffers;
        ValidPointersMap() 
        { 
            GenerateValidPointersMap(&validPointersBuffer, invalidBitsBuffers, blockInfoBuffer);
        }

        static void GenerateValidPointersMap(ValidPointersMapTable * validTable, InvalidBitsTable& invalidTable, BlockInfoMapTable& blockInfoTable);

        inline const ValidPointers<TBlockAttributes> GetValidPointersForIndex(uint bucketIndex) const
        {
            AnalysisAssert(bucketIndex < TBlockAttributes::BucketCount);
            ushort const * validPointers = nullptr;
            validPointers = validPointersBuffer[bucketIndex];
            return ValidPointers<TBlockAttributes>(validPointers, bucketIndex);
        }

        inline const typename SmallHeapBlockT<TBlockAttributes>::SmallHeapBlockBitVector * GetInvalidBitVector(uint index) const
        {
            AnalysisAssert(index < TBlockAttributes::BucketCount);
            return &invalidBitsBuffers[index];
        }

        inline const typename SmallHeapBlockT<TBlockAttributes>::BlockInfo * GetBlockInfo(uint index) const
        {
            AnalysisAssert(index < TBlockAttributes::BucketCount);
            return blockInfoBuffer[index];
        }

        static int32_t GenerateValidPointersMapForBlockType(FILE* file);
    };

    static ValidPointersMap<SmallAllocationBlockAttributes>  smallAllocValidPointersMap;
    static ValidPointersMap<MediumAllocationBlockAttributes> mediumAllocValidPointersMap;

public:
    template <typename TBlockAttributes>
    static typename SmallHeapBlockT<TBlockAttributes>::SmallHeapBlockBitVector const * GetInvalidBitVector(uint objectSize);

    template <typename TBlockAttributes>
    static typename SmallHeapBlockT<TBlockAttributes>::SmallHeapBlockBitVector const * GetInvalidBitVectorForBucket(uint bucketIndex);

    template <typename TBlockAttributes>
    static ValidPointers<TBlockAttributes> const GetValidPointersMapForBucket(uint bucketIndex);

    Recycler* GetRecycler(){ return recycler; }

    template <typename TBlockAttributes>
    static typename SmallHeapBlockT<TBlockAttributes>::BlockInfo const * GetBlockInfo(uint objectSize);

    Recycler * recycler;
    SmallLeafHeapBlock * newLeafHeapBlockList;
    SmallNormalHeapBlock * newNormalHeapBlockList;
    SmallFinalizableHeapBlock * newFinalizableHeapBlockList;
#ifdef RECYCLER_VISITED_HOST
    SmallRecyclerVisitedHostHeapBlock * newRecyclerVisitedHostHeapBlockList;
#endif

    SmallNormalWithBarrierHeapBlock * newNormalWithBarrierHeapBlockList;
    SmallFinalizableWithBarrierHeapBlock * newFinalizableWithBarrierHeapBlockList;

    MediumLeafHeapBlock * newMediumLeafHeapBlockList;
    MediumNormalHeapBlock * newMediumNormalHeapBlockList;
    MediumFinalizableHeapBlock * newMediumFinalizableHeapBlockList;
#ifdef RECYCLER_VISITED_HOST
    MediumRecyclerVisitedHostHeapBlock* newMediumRecyclerVisitedHostHeapBlockList;
#endif

    MediumNormalWithBarrierHeapBlock * newMediumNormalWithBarrierHeapBlockList;
    MediumFinalizableWithBarrierHeapBlock * newMediumFinalizableWithBarrierHeapBlockList;

#ifdef RECYCLER_PAGE_HEAP
    PageHeapMode pageHeapMode;
    bool isPageHeapEnabled;
    BVStatic<HeapConstants::BucketCount> smallBlockPageHeapBucketFilter;
    BVStatic<HeapConstants::MediumBucketCount> mediumBlockPageHeapBucketFilter;
    bool captureAllocCallStack;
    bool captureFreeCallStack;
    PageHeapBlockTypeFilter pageHeapBlockType;
#endif

    HeapBucketGroup<SmallAllocationBlockAttributes> heapBuckets[HeapConstants::BucketCount];

    HeapBucketGroup<MediumAllocationBlockAttributes> mediumHeapBuckets[HeapConstants::MediumBucketCount];
    LargeHeapBucket largeObjectBucket;
    bool hasPendingTransferDisposedObjects;

    static const size_t ObjectAlignmentMask = HeapConstants::ObjectGranularity - 1;         // 0xF
#if defined(RECYCLER_SLOW_CHECK_ENABLED)
    size_t heapBlockCount[HeapBlock::BlockTypeCount];
#endif
#ifdef RECYCLER_FINALIZE_CHECK
    size_t liveFinalizableObjectCount;
    size_t newFinalizableObjectCount;
    size_t pendingDisposableObjectCount;
    size_t GetFinalizeCount();
#endif

 public:
     // ==============================================================
     // Page allocator APIs
     // ==============================================================
     void Prime();
     void CloseNonLeaf();
     void DecommitNow(bool all);

     void SuspendIdleDecommitNonLeaf();
     void ResumeIdleDecommitNonLeaf();

     void EnterIdleDecommit();
     IdleDecommitSignal LeaveIdleDecommit(bool allowTimer);
#ifdef IDLE_DECOMMIT_ENABLED
     uint32_t IdleDecommit();
#endif
#if DBG
     void ShutdownIdleDecommit();
     void ResetThreadId();
     void SetDisableThreadAccessCheck();
#endif

     size_t GetUsedBytes();
     size_t GetReservedBytes();
     size_t GetCommittedBytes();
     size_t GetNumberOfSegments();

     IdleDecommitPageAllocator * GetRecyclerLeafPageAllocator();
     IdleDecommitPageAllocator * GetRecyclerPageAllocator();
     IdleDecommitPageAllocator * GetRecyclerLargeBlockPageAllocator();
#ifdef RECYCLER_WRITE_BARRIER_ALLOC_SEPARATE_PAGE
     IdleDecommitPageAllocator * GetRecyclerWithBarrierPageAllocator();
#endif

#if ENABLE_BACKGROUND_PAGE_ZEROING
     void StartQueueZeroPage();
     void StopQueueZeroPage();
     void BackgroundZeroQueuedPages();
     void ZeroQueuedPages();
     void FlushBackgroundPages();
#if DBG
     bool HasZeroQueuedPages();
#endif
#endif

#ifdef RECYCLER_MEMORY_VERIFY
     void EnableVerify();
#endif
#ifdef RECYCLER_NO_PAGE_REUSE
     void DisablePageReuse();
#endif
private:
    template<typename Action>
    void ForEachPageAllocator(Action action)
    {
        ForEachNonLeafPageAllocator(action);
        action(this->recyclerLeafPageAllocator);
    }

    template<typename Action>
    void ForEachNonLeafPageAllocator(Action action)
    {
        action(&this->recyclerPageAllocator);
        action(&this->recyclerLargeBlockPageAllocator);
#ifdef RECYCLER_WRITE_BARRIER_ALLOC_SEPARATE_PAGE
        action(&this->recyclerWithBarrierPageAllocator);
#endif
    }


    IdleDecommitPageAllocator * recyclerLeafPageAllocator;

#ifdef RECYCLER_WRITE_BARRIER_ALLOC_SEPARATE_PAGE
    RecyclerPageAllocator recyclerWithBarrierPageAllocator;
#endif
    RecyclerPageAllocator recyclerPageAllocator;
    RecyclerPageAllocator recyclerLargeBlockPageAllocator;

    friend class Recycler;
    friend class HeapBucket;
    friend class HeapBlockMap32;
    friend class LargeHeapBucket;
    template <typename TBlockType>
    friend class HeapBucketT;
    template <typename TBlockType>
    friend class SmallHeapBlockAllocator;
    template <typename TBlockAttributes>
    friend class SmallFinalizableHeapBucketT;

    template <typename TBlockAttributes>
    friend class SmallLeafHeapBucketBaseT;

    template <typename TBlockAttributes>
    friend class SmallHeapBlockT;
    template <typename TBlockAttributes>
    friend class SmallLeafHeapBlockT;
    template <typename TBlockAttributes>
    friend class SmallFinalizableHeapBlockT;
#ifdef RECYCLER_VISITED_HOST
    template <typename TBlockAttributes>
    friend class SmallRecyclerVisitedHostHeapBlockT;
#endif
    friend class LargeHeapBlock;
    friend class RecyclerSweep;

    friend class HeapInfoManager;
};

template <ObjectInfoBits attributes>
typename SmallHeapBlockType<attributes, SmallAllocationBlockAttributes>::BucketType&
HeapInfo::GetBucket(size_t sizeCat)
{
    uint bucket = HeapInfo::GetBucketIndex(sizeCat);
    return this->heapBuckets[bucket].GetBucket<attributes>();
}

template <ObjectInfoBits attributes>
typename SmallHeapBlockType<attributes, MediumAllocationBlockAttributes>::BucketType&
HeapInfo::GetMediumBucket(size_t sizeCat)
{
    uint bucket = HeapInfo::GetMediumBucketIndex(sizeCat);
    return this->mediumHeapBuckets[bucket].GetBucket<attributes>();
}

template <ObjectInfoBits attributes, bool nothrow>
inline char *
HeapInfo::RealAlloc(Recycler * recycler, size_t sizeCat, size_t size)
{
    Assert(HeapInfo::IsAlignedSmallObjectSize(sizeCat));
    auto& bucket = this->GetBucket<(ObjectInfoBits)(attributes & GetBlockTypeBitMask)>(sizeCat);
    return bucket.template RealAlloc<attributes, nothrow>(recycler, sizeCat, size);
}

template <ObjectInfoBits attributes, bool nothrow>
inline char *
HeapInfo::MediumAlloc(Recycler * recycler, size_t sizeCat, size_t size)
{
    auto& bucket = this->GetMediumBucket<(ObjectInfoBits)(attributes & GetBlockTypeBitMask)>(sizeCat);
    return bucket.template RealAlloc<attributes, nothrow>(recycler, sizeCat, size);
}

template <ObjectInfoBits attributes>
void
HeapInfo::FreeSmallObject(void* object, size_t sizeCat)
{
    Assert(HeapInfo::IsAlignedSmallObjectSize(sizeCat));
    return this->GetBucket<(ObjectInfoBits)(attributes & GetBlockTypeBitMask)>(sizeCat).ExplicitFree(object, sizeCat);
}

template <ObjectInfoBits attributes>
inline void
HeapInfo::FreeMediumObject(void* object, size_t sizeCat)
{
    Assert(HeapInfo::IsAlignedMediumObjectSize(sizeCat));
    return this->GetMediumBucket<(ObjectInfoBits)(attributes & GetBlockTypeBitMask)>(sizeCat).ExplicitFree(object, sizeCat);
}

template <ObjectInfoBits attributes>
bool
HeapInfo::IntegrateBlock(char * blockAddress, PageSegment * segment, Recycler * recycler, size_t sizeCat)
{
    // We only support no bit and leaf bit right now, where we don't need to set the object info in either case
    static_assert(attributes == NoBit || attributes == LeafBit);
    Assert(HeapInfo::IsAlignedSmallObjectSize(sizeCat));

    return this->GetBucket<(ObjectInfoBits)(attributes & GetBlockTypeBitMask)>(sizeCat).IntegrateBlock(blockAddress, segment, recycler);
}

template <typename SmallHeapBlockAllocatorType>
void
HeapInfo::AddSmallAllocator(SmallHeapBlockAllocatorType * allocator, size_t sizeCat)
{
    Assert(HeapInfo::IsAlignedSmallObjectSize(sizeCat));
    this->GetBucket<SmallHeapBlockAllocatorType::BlockType::RequiredAttributes>(sizeCat).AddAllocator(allocator);
}

template <typename SmallHeapBlockAllocatorType>
void
HeapInfo::RemoveSmallAllocator(SmallHeapBlockAllocatorType * allocator, size_t sizeCat)
{
    Assert(HeapInfo::IsAlignedSmallObjectSize(sizeCat));
    this->GetBucket<SmallHeapBlockAllocatorType::BlockType::RequiredAttributes>(sizeCat).RemoveAllocator(allocator);
}

template <ObjectInfoBits attributes, typename SmallHeapBlockAllocatorType>
char *
HeapInfo::SmallAllocatorAlloc(Recycler * recycler, SmallHeapBlockAllocatorType * allocator, size_t sizeCat, size_t size)
{
    Assert(HeapInfo::IsAlignedSmallObjectSize(sizeCat));
    static_assert((attributes & SmallHeapBlockAllocatorType::BlockType::RequiredAttributes) == SmallHeapBlockAllocatorType::BlockType::RequiredAttributes);

    auto& bucket = this->GetBucket<SmallHeapBlockAllocatorType::BlockType::RequiredAttributes>(sizeCat);

    // For now, SmallAllocatorAlloc is always throwing- but it's pretty easy to switch it if it's needed
    return bucket.SnailAlloc(recycler, allocator, sizeCat, size, attributes, /* nothrow = */ false);
}

// Forward declaration of explicit specialization before instantiation
template <>
int32_t HeapInfo::ValidPointersMap<SmallAllocationBlockAttributes>::GenerateValidPointersMapForBlockType(FILE* file);
template <>
int32_t HeapInfo::ValidPointersMap<MediumAllocationBlockAttributes>::GenerateValidPointersMapForBlockType(FILE* file);

// Template instantiation
extern template class HeapInfo::ValidPointersMap<SmallAllocationBlockAttributes>;
extern template class HeapInfo::ValidPointersMap<MediumAllocationBlockAttributes>;

template <typename TBlockAttributes>
inline uint HeapInfo::GetObjectSizeForBucketIndex(uint bucketIndex)
{
    return (bucketIndex + 1) << HeapConstants::ObjectAllocationShift;
}

template <>
inline uint HeapInfo::GetObjectSizeForBucketIndex<MediumAllocationBlockAttributes>(uint bucketIndex)
{
    Assert(IsMediumObject(HeapConstants::MaxSmallObjectSize + ((bucketIndex + 1) * HeapConstants::MediumObjectGranularity)));
    return HeapConstants::MaxSmallObjectSize + ((bucketIndex + 1) * HeapConstants::MediumObjectGranularity);
}


template <typename TBlockAttributes>
inline typename SmallHeapBlockT<TBlockAttributes>::SmallHeapBlockBitVector const *
HeapInfo::GetInvalidBitVector(uint objectSize)
{
    return smallAllocValidPointersMap.GetInvalidBitVector(HeapInfo::GetBucketIndex(objectSize));
}

template <typename TBlockAttributes>
inline typename SmallHeapBlockT<TBlockAttributes>::SmallHeapBlockBitVector const *
HeapInfo::GetInvalidBitVectorForBucket(uint bucketIndex)
{
    return smallAllocValidPointersMap.GetInvalidBitVector(bucketIndex);
}

template <typename TBlockAttributes>
inline ValidPointers<TBlockAttributes> const
HeapInfo::GetValidPointersMapForBucket(uint bucketIndex)
{
    return smallAllocValidPointersMap.GetValidPointersForIndex(bucketIndex);
}

template <>
inline typename SmallHeapBlockT<MediumAllocationBlockAttributes>::SmallHeapBlockBitVector const *
HeapInfo::GetInvalidBitVector<MediumAllocationBlockAttributes>(uint objectSize)
{
    return mediumAllocValidPointersMap.GetInvalidBitVector(GetMediumBucketIndex(objectSize));
}

template <>
inline typename SmallHeapBlockT<MediumAllocationBlockAttributes>::SmallHeapBlockBitVector const *
HeapInfo::GetInvalidBitVectorForBucket<MediumAllocationBlockAttributes>(uint bucketIndex)
{
    return mediumAllocValidPointersMap.GetInvalidBitVector(bucketIndex);
}

template <>
inline ValidPointers<MediumAllocationBlockAttributes> const
HeapInfo::GetValidPointersMapForBucket<MediumAllocationBlockAttributes>(uint bucketIndex)
{
    return mediumAllocValidPointersMap.GetValidPointersForIndex(bucketIndex);
}

template <typename TBlockAttributes>
inline typename SmallHeapBlockT<TBlockAttributes>::BlockInfo const *
HeapInfo::GetBlockInfo(uint objectSize)
{
    return smallAllocValidPointersMap.GetBlockInfo(GetBucketIndex(objectSize));
}

template <>
inline typename SmallHeapBlockT<MediumAllocationBlockAttributes>::BlockInfo const *
HeapInfo::GetBlockInfo<MediumAllocationBlockAttributes>(uint objectSize)
{
    return mediumAllocValidPointersMap.GetBlockInfo(GetMediumBucketIndex(objectSize));
}

}

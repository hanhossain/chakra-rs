//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once
#include "Memory/SmallNormalHeapBucket.h"

namespace Memory
{
template <class THeapBlockType>
class SmallFinalizableHeapBucketBaseT : public SmallNormalHeapBucketBase<THeapBlockType>
{
    typedef SmallNormalHeapBucketBase<THeapBlockType> BaseT;

public:
    typedef typename THeapBlockType::HeapBlockAttributes TBlockAttributes;

    SmallFinalizableHeapBucketBaseT();
    ~SmallFinalizableHeapBucketBaseT();
    void DisposeObjects();
    void TransferDisposedObjects();
    void FinalizeAllObjects();
    static void FinalizeHeapBlockList(THeapBlockType * list);

#if ENABLE_MEM_STATS
    void AggregateBucketStats();
#endif
protected:
    void EnumerateObjects(ObjectInfoBits infoBits, void (*CallBackFunction)(void * address, size_t size));

    friend class HeapBucket;
    template <class TBlockAttributes>
    friend class HeapBucketGroup;

    void ResetMarks(ResetMarkFlags flags);
    void Sweep(RecyclerSweep& recyclerSweep);
#if DBG || defined(RECYCLER_SLOW_CHECK_ENABLED)
    size_t GetNonEmptyHeapBlockCount(bool checkCount) const;
#endif
#ifdef RECYCLER_SLOW_CHECK_ENABLED
    size_t Check();
#endif
#ifdef RECYCLER_MEMORY_VERIFY
    void Verify();
#endif
#ifdef RECYCLER_VERIFY_MARK
    void VerifyMark();
#endif
    friend class ::ScriptMemoryDumper;
    template <typename TBlockType>
    friend class HeapBucketT;

protected:
    THeapBlockType * pendingDisposeList;    // list of block that has finalizable object that needs to be disposed

#if DBG || defined(RECYCLER_SLOW_CHECK_ENABLED)
    THeapBlockType * tempPendingDisposeList;
#endif
};

template <class TBlockAttributes> 
class SmallFinalizableHeapBucketT : public SmallFinalizableHeapBucketBaseT<SmallFinalizableHeapBlockT<TBlockAttributes> >
{
};
#ifdef RECYCLER_VISITED_HOST
template <class TBlockAttributes> 
class SmallRecyclerVisitedHostHeapBucketT : public SmallFinalizableHeapBucketBaseT<SmallRecyclerVisitedHostHeapBlockT<TBlockAttributes> >
{
};
#endif
template <class TBlockAttributes>
class SmallFinalizableWithBarrierHeapBucketT : public SmallFinalizableHeapBucketBaseT<SmallFinalizableWithBarrierHeapBlockT<TBlockAttributes> >
{
};

typedef SmallFinalizableHeapBucketT<MediumAllocationBlockAttributes> MediumFinalizableHeapBucket;
typedef SmallFinalizableHeapBucketT<SmallAllocationBlockAttributes> SmallFinalizableHeapBucket;
#ifdef RECYCLER_VISITED_HOST
typedef SmallRecyclerVisitedHostHeapBucketT<MediumAllocationBlockAttributes> MediumRecyclerVisitedHostHeapBucket;
typedef SmallRecyclerVisitedHostHeapBucketT<SmallAllocationBlockAttributes> SmallRecyclerVisitedHostHeapBucket;
#endif
typedef SmallFinalizableWithBarrierHeapBucketT<MediumAllocationBlockAttributes> MediumFinalizableWithBarrierHeapBucket;
typedef SmallFinalizableWithBarrierHeapBucketT<SmallAllocationBlockAttributes> SmallFinalizableWithBarrierHeapBucket;

// GC-TODO: Move this away

template <ObjectInfoBits attributes, class TBlockAttributes>
class SmallHeapBlockType
{
public:
   
    static_assert((attributes & FinalizeBit) != 0);
#ifdef RECYCLER_VISITED_HOST
    // attributes with RecyclerVisitedHostBit must use SmallRecyclerVisitedHostHeap{Bucket|Block}T
    static_assert((attributes & RecyclerVisitedHostBit) == 0);
#endif
    typedef SmallFinalizableHeapBlockT<TBlockAttributes> BlockType;
    typedef SmallFinalizableHeapBucketT<TBlockAttributes> BucketType;
};

#ifdef RECYCLER_VISITED_HOST
template <>
class SmallHeapBlockType<(ObjectInfoBits)(RecyclerVisitedHostBit), SmallAllocationBlockAttributes>
{
public:
    typedef SmallRecyclerVisitedHostHeapBlock BlockType;
    typedef SmallRecyclerVisitedHostHeapBucket BucketType;
};

template <>
class SmallHeapBlockType<(ObjectInfoBits)(RecyclerVisitedHostTracedFinalizableBlockTypeBits), SmallAllocationBlockAttributes>
{
public:
    typedef SmallRecyclerVisitedHostHeapBlock BlockType;
    typedef SmallRecyclerVisitedHostHeapBucket BucketType;
};

template <>
class SmallHeapBlockType<(ObjectInfoBits)(RecyclerVisitedHostFinalizableBlockTypeBits), SmallAllocationBlockAttributes>
{
public:
    typedef SmallRecyclerVisitedHostHeapBlock BlockType;
    typedef SmallRecyclerVisitedHostHeapBucket BucketType;
};
#endif

template <>
class SmallHeapBlockType<LeafBit, SmallAllocationBlockAttributes>
{
public:
    typedef SmallLeafHeapBlock BlockType;
    typedef SmallLeafHeapBucketT<SmallAllocationBlockAttributes> BucketType;
};

template <>
class SmallHeapBlockType<NoBit, SmallAllocationBlockAttributes>
{
public:
    typedef SmallNormalHeapBlock BlockType;
    typedef SmallNormalHeapBucket BucketType;
};

template <>
class SmallHeapBlockType<WithBarrierBit, SmallAllocationBlockAttributes>
{
public:
    typedef SmallNormalWithBarrierHeapBlock BlockType;
    typedef SmallNormalWithBarrierHeapBucket BucketType;
};

template <>
class SmallHeapBlockType<static_cast<ObjectInfoBits>(WithBarrierBit | LeafBit), SmallAllocationBlockAttributes>
{
public:
    typedef SmallLeafHeapBlock BlockType;
    typedef SmallLeafHeapBucketT<SmallAllocationBlockAttributes> BucketType;
};

template <>
class SmallHeapBlockType<FinalizableWithBarrierBit, SmallAllocationBlockAttributes>
{
public:
    typedef SmallFinalizableWithBarrierHeapBlock BlockType;
    typedef SmallFinalizableWithBarrierHeapBucket BucketType;
};

template <>
class SmallHeapBlockType<FinalizeBit, MediumAllocationBlockAttributes>
{
public:
    typedef MediumFinalizableHeapBlock BlockType;
    typedef MediumFinalizableHeapBucket BucketType;
};

#ifdef RECYCLER_VISITED_HOST
template <>
class SmallHeapBlockType<(ObjectInfoBits)(RecyclerVisitedHostBit), MediumAllocationBlockAttributes>
{
public:
    typedef MediumRecyclerVisitedHostHeapBlock BlockType;
    typedef MediumRecyclerVisitedHostHeapBucket BucketType;
};

template <>
class SmallHeapBlockType<(ObjectInfoBits)(RecyclerVisitedHostTracedFinalizableBlockTypeBits), MediumAllocationBlockAttributes>
{
public:
    typedef MediumRecyclerVisitedHostHeapBlock BlockType;
    typedef MediumRecyclerVisitedHostHeapBucket BucketType;
};

template <>
class SmallHeapBlockType<(ObjectInfoBits)(RecyclerVisitedHostFinalizableBlockTypeBits), MediumAllocationBlockAttributes>
{
public:
    typedef MediumRecyclerVisitedHostHeapBlock BlockType;
    typedef MediumRecyclerVisitedHostHeapBucket BucketType;
};
#endif

template <>
class SmallHeapBlockType<LeafBit, MediumAllocationBlockAttributes>
{
public:
    typedef MediumLeafHeapBlock BlockType;
    typedef SmallLeafHeapBucketT<MediumAllocationBlockAttributes> BucketType;
};

template <>
class SmallHeapBlockType<NoBit, MediumAllocationBlockAttributes>
{
public:
    typedef MediumNormalHeapBlock BlockType;
    typedef MediumNormalHeapBucket BucketType;
};

template <>
class SmallHeapBlockType<WithBarrierBit, MediumAllocationBlockAttributes>
{
public:
    typedef MediumNormalWithBarrierHeapBlock BlockType;
    typedef MediumNormalWithBarrierHeapBucket BucketType;
};

template <>
class SmallHeapBlockType<static_cast<ObjectInfoBits>(WithBarrierBit | LeafBit), MediumAllocationBlockAttributes>
{
public:
    typedef MediumLeafHeapBlock BlockType;
    typedef SmallLeafHeapBucketT<MediumAllocationBlockAttributes> BucketType;
};

template <>
class SmallHeapBlockType<FinalizableWithBarrierBit, MediumAllocationBlockAttributes>
{
public:
    typedef MediumFinalizableWithBarrierHeapBlock BlockType;
    typedef MediumFinalizableWithBarrierHeapBucket BucketType;
};

template <class TBlockAttributes>
class HeapBucketGroup
{
    template <ObjectInfoBits objectAttributes>
    class BucketGetter
    {
    public:
        typedef typename SmallHeapBlockType<objectAttributes, TBlockAttributes>::BucketType BucketType;
        static BucketType& GetBucket(HeapBucketGroup<TBlockAttributes> * heapBucketGroup)
        {
            static_assert(objectAttributes & FinalizeBit);
            return heapBucketGroup->finalizableHeapBucket;
        }
    };

    template <>
    class BucketGetter<LeafBit>
    {
    public:
        typedef typename SmallHeapBlockType<LeafBit, TBlockAttributes>::BucketType BucketType;
        static BucketType& GetBucket(HeapBucketGroup<TBlockAttributes> * heapBucketGroup)
        {            
            return heapBucketGroup->leafHeapBucket;
        }
    };

    template <>
    class BucketGetter<NoBit>
    {
    public:
        typedef typename SmallHeapBlockType<NoBit, TBlockAttributes>::BucketType BucketType;
        static BucketType& GetBucket(HeapBucketGroup<TBlockAttributes> * heapBucketGroup)
        {
            return heapBucketGroup->heapBucket;
        }
    };

    template <>
    class BucketGetter<static_cast<ObjectInfoBits>(FinalizeBit | LeafBit)>
    {
    public:
        typedef typename SmallHeapBlockType<static_cast<ObjectInfoBits>(FinalizeBit | LeafBit), TBlockAttributes>::BucketType BucketType;
        static BucketType& GetBucket(HeapBucketGroup<TBlockAttributes> * heapBucketGroup)
        {
            // TODO: SWB implemente finalizable leaf bucket
            return heapBucketGroup->finalizableHeapBucket;
        }
    };

#ifdef RECYCLER_VISITED_HOST
    template <>
    class BucketGetter<(ObjectInfoBits)(RecyclerVisitedHostBit)>
    {
    public:
        typedef typename SmallHeapBlockType<(ObjectInfoBits)(RecyclerVisitedHostBit), TBlockAttributes>::BucketType BucketType;
        static BucketType& GetBucket(HeapBucketGroup<TBlockAttributes> * HeapBucketGroup)
        {
            return HeapBucketGroup->recyclerVisitedHostHeapBucket;
        }
    };

    template <>
    class BucketGetter<(ObjectInfoBits)(RecyclerVisitedHostTracedFinalizableBlockTypeBits)>
    {
    public:
        typedef typename SmallHeapBlockType<(ObjectInfoBits)(RecyclerVisitedHostTracedFinalizableBlockTypeBits), TBlockAttributes>::BucketType BucketType;
        static BucketType& GetBucket(HeapBucketGroup<TBlockAttributes> * HeapBucketGroup)
        {
            return HeapBucketGroup->recyclerVisitedHostHeapBucket;
        }
    };

    template <>
    class BucketGetter<(ObjectInfoBits)(RecyclerVisitedHostFinalizableBlockTypeBits)>
    {
    public:
        typedef typename SmallHeapBlockType<(ObjectInfoBits)(RecyclerVisitedHostFinalizableBlockTypeBits), TBlockAttributes>::BucketType BucketType;
        static BucketType& GetBucket(HeapBucketGroup<TBlockAttributes> * HeapBucketGroup)
        {
            return HeapBucketGroup->recyclerVisitedHostHeapBucket;
        }
    };
#endif

    template <>
    class BucketGetter<WithBarrierBit>
    {
    public:
        typedef typename SmallHeapBlockType<WithBarrierBit, TBlockAttributes>::BucketType BucketType;
        static BucketType& GetBucket(HeapBucketGroup<TBlockAttributes> * heapBucketGroup)
        {
            return heapBucketGroup->smallNormalWithBarrierHeapBucket;
        }
    };

    template <>
    class BucketGetter<static_cast<ObjectInfoBits>(WithBarrierBit | LeafBit)>
    {
    public:
        typedef typename SmallHeapBlockType<static_cast<ObjectInfoBits>(WithBarrierBit | LeafBit), TBlockAttributes>::BucketType BucketType;
        static BucketType& GetBucket(HeapBucketGroup<TBlockAttributes> * heapBucketGroup)
        {
            // WithBarrierBit | LeafBit combination should not exist, this is only for compilation purpose
            Assert(false);
            return heapBucketGroup->leafHeapBucket;
        }
    };

    template <>
    class BucketGetter<FinalizableWithBarrierBit>
    {
    public:
        typedef typename SmallHeapBlockType<FinalizableWithBarrierBit, TBlockAttributes>::BucketType BucketType;
        static BucketType& GetBucket(HeapBucketGroup<TBlockAttributes> * heapBucketGroup)
        {
            return heapBucketGroup->smallFinalizableWithBarrierHeapBucket;
        }
    };
public:

    template <ObjectInfoBits objectAttributes>
    typename SmallHeapBlockType<objectAttributes, TBlockAttributes>::BucketType& GetBucket()
    {
        return BucketGetter<objectAttributes>::GetBucket(this);
    }

    void Initialize(HeapInfo * heapInfo, uint sizeCat);
    void ResetMarks(ResetMarkFlags flags);
    void ScanInitialImplicitRoots(Recycler * recycler);
    void ScanNewImplicitRoots(Recycler * recycler);

    void Sweep(RecyclerSweep& recyclerSweep);
    uint Rescan(Recycler * recycler, RescanFlags flags);
    void SweepPendingObjects(RecyclerSweep& recyclerSweep);
    void SweepPartialReusePages(RecyclerSweep& recyclerSweep);
    void FinishPartialCollect(RecyclerSweep * recyclerSweep);
    void PrepareSweep();
    void SetupBackgroundSweep(RecyclerSweep& recyclerSweep);
    void TransferPendingEmptyHeapBlocks(RecyclerSweep& recyclerSweep);
    void SweepFinalizableObjects(RecyclerSweep& recyclerSweep);
    void DisposeObjects();
    void TransferDisposedObjects();
    void EnumerateObjects(ObjectInfoBits infoBits, void (*CallBackFunction)(void * address, size_t size));
    void FinalizeAllObjects();
    static unsigned int GetHeapBucketOffset() { return offsetof(HeapBucketGroup<TBlockAttributes>, heapBucket); }

#if DBG || defined(RECYCLER_SLOW_CHECK_ENABLED)
    size_t GetNonEmptyHeapBlockCount(bool checkCount) const;
    size_t GetEmptyHeapBlockCount() const;
#endif
#ifdef RECYCLER_SLOW_CHECK_ENABLED
    size_t Check();
#endif
#ifdef RECYCLER_MEMORY_VERIFY
    void Verify();
#endif
#ifdef RECYCLER_VERIFY_MARK
    void VerifyMark();
#endif
#if DBG
    bool AllocatorsAreEmpty();
#endif
private:
    SmallNormalHeapBucketT<TBlockAttributes>       heapBucket;
    SmallLeafHeapBucketT<TBlockAttributes>         leafHeapBucket;
    SmallFinalizableHeapBucketT<TBlockAttributes>  finalizableHeapBucket;
#ifdef RECYCLER_VISITED_HOST
    SmallRecyclerVisitedHostHeapBucketT<TBlockAttributes>  recyclerVisitedHostHeapBucket;
#endif
    SmallNormalWithBarrierHeapBucketT<TBlockAttributes> smallNormalWithBarrierHeapBucket;
    SmallFinalizableWithBarrierHeapBucketT<TBlockAttributes> smallFinalizableWithBarrierHeapBucket;
};

extern template class HeapBucketGroup<SmallAllocationBlockAttributes>;
extern template class HeapBucketGroup<MediumAllocationBlockAttributes>;
}

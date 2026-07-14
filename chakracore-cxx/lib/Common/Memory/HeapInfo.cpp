//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "CommonMemoryPch.h"
#include "Memory/PageHeapBlockTypeFilter.h"
#include "HeapInfo.h"
#include <print>

template char* HeapInfo::RealAlloc<NoBit, false>(Recycler * recycler, size_t sizeCat, size_t size);

const uint SmallAllocationBlockAttributes::MaxSmallObjectCount;
const uint MediumAllocationBlockAttributes::MaxSmallObjectCount;

HeapInfo::ValidPointersMap<SmallAllocationBlockAttributes>  HeapInfo::smallAllocValidPointersMap;
HeapInfo::ValidPointersMap<MediumAllocationBlockAttributes> HeapInfo::mediumAllocValidPointersMap;

template <class TBlockAttributes>
ValidPointers<TBlockAttributes>::ValidPointers(ushort const * validPointers, uint bucketIndex)
    : validPointers(validPointers)
{
#if DBG
    uint localIndexPerObject;
    uint localMaxObjectIndex = CalculateBucketInfo(bucketIndex, &localIndexPerObject);
    for (uint index = 0; index < TBlockAttributes::MaxSmallObjectCount; index++)
    {
        Assert(CalculateAddressIndex(index, localIndexPerObject, localMaxObjectIndex) == validPointers[index]);
        Assert(CalculateInteriorAddressIndex(index, localIndexPerObject, localMaxObjectIndex) == validPointers[index + TBlockAttributes::MaxSmallObjectCount]);
    }
#endif
}

template <class TBlockAttributes>
ushort ValidPointers<TBlockAttributes>::GetAddressIndex(uint index) const
{
    Assert(index < TBlockAttributes::MaxSmallObjectCount);
    return validPointers[index];
}

template <class TBlockAttributes>
ushort ValidPointers<TBlockAttributes>::GetInteriorAddressIndex(uint index) const
{
    Assert(index < TBlockAttributes::MaxSmallObjectCount);
    return validPointers[index + TBlockAttributes::MaxSmallObjectCount];
}

#if DBG
template <class TBlockAttributes>
uint ValidPointers<TBlockAttributes>::CalculateBucketInfo(uint bucketIndex, uint * indexPerObject)
{
    uint bucketSize;
    if (TBlockAttributes::IsSmallBlock)
    {
        bucketSize = TBlockAttributes::MinObjectSize + HeapConstants::ObjectGranularity * bucketIndex;
    }
    else
    {
        bucketSize = TBlockAttributes::MinObjectSize + HeapConstants::MediumObjectGranularity * (bucketIndex + 1);
    }
    *indexPerObject = bucketSize / HeapConstants::ObjectGranularity;
    return ((TBlockAttributes::PageCount * AutoSystemInfo::PageSize) / bucketSize) * bucketSize / HeapConstants::ObjectGranularity;
}

template <class TBlockAttributes>
ushort ValidPointers<TBlockAttributes>::CalculateAddressIndex(uint index, uint indexPerObject, uint maxObjectIndex)
{
    Assert(index < TBlockAttributes::MaxSmallObjectCount);
    if (index >= maxObjectIndex)
    {
        return static_cast<ushort>(-1);
    }
    uint addressIndex = index / indexPerObject;
    Assert(addressIndex < USHRT_MAX);
    if (addressIndex * indexPerObject != index)
    {
        return static_cast<ushort>(-1);
    }
    return static_cast<ushort>(addressIndex);
}

template <class TBlockAttributes>
ushort ValidPointers<TBlockAttributes>::CalculateInteriorAddressIndex(uint index, uint indexPerObject, uint maxObjectIndex)
{
    Assert(index < TBlockAttributes::MaxSmallObjectCount);
    if (index >= maxObjectIndex)
    {
        return static_cast<ushort>(-1);
    }
    uint addressIndex = index / indexPerObject;
    Assert(addressIndex < USHRT_MAX);
    return static_cast<ushort>(addressIndex);
}
#endif

template <class TBlockAttributes>
void HeapInfo::ValidPointersMap<TBlockAttributes>::GenerateValidPointersMap(ValidPointersMapTable * validTable, InvalidBitsTable& invalidTable, BlockInfoMapTable& blockInfoTable)
{
    // Create the valid pointer map to be shared by the buckets.
    // Also create the invalid objects bit vector.
    ushort * buffer = &((*validTable)[0][0]);
    if (buffer)
    {
        memset(buffer, -1, sizeof(ushort) * 2 * TBlockAttributes::MaxSmallObjectCount * TBlockAttributes::BucketCount);
    }

    for (uint i = 0; i < TBlockAttributes::BucketCount; i++)
    {
        // Non-interior first
        ushort * validPointers = buffer;
        if (buffer)
        {            
            buffer += TBlockAttributes::MaxSmallObjectCount;
        }

        typename SmallHeapBlockT<TBlockAttributes>::SmallHeapBlockBitVector * invalidBitVector = &invalidTable[i];
        invalidBitVector->SetAll();

        uint bucketSize;

        if (TBlockAttributes::IsSmallBlock)
        {
            bucketSize = TBlockAttributes::MinObjectSize + HeapConstants::ObjectGranularity * i;
        }
        else
        {
            bucketSize = TBlockAttributes::MinObjectSize + HeapConstants::MediumObjectGranularity * (i + 1);
        }

        uint stride = bucketSize / HeapConstants::ObjectGranularity;
        uint maxObjectCountForBucket = ((TBlockAttributes::PageCount * AutoSystemInfo::PageSize) / bucketSize);

        BlockInfoMapRow* blockInfoRow = &blockInfoTable[i];

        memset(blockInfoRow, 0, sizeof(BlockInfoMapRow));

        for (ushort j = 0; j < maxObjectCountForBucket; j++)
        {
            if (validPointers)
            {
                validPointers[j * stride] = j;
            }

            uintptr_t objectAddress = j * bucketSize;
            Assert(objectAddress / AutoSystemInfo::PageSize < USHRT_MAX);
            ushort pageIndex = static_cast<ushort>(objectAddress / AutoSystemInfo::PageSize);

            (*blockInfoRow)[pageIndex].pageObjectCount++;
            (*blockInfoRow)[pageIndex].lastObjectIndexOnPage = max(j, (*blockInfoRow)[pageIndex].lastObjectIndexOnPage);

            invalidBitVector->Clear(j * stride);
        }

        if (buffer)
        {
            // interior pointer
            ushort * validInteriorPointers = buffer;
            buffer += TBlockAttributes::MaxSmallObjectCount;
            for (ushort j = 0; j < maxObjectCountForBucket; j++)
            {
                uint start = j * stride;
                uint end = min(start + stride, TBlockAttributes::MaxSmallObjectCount);
                for (uint k = start; k < end; k++)
                {
                    validInteriorPointers[k] = j;
                }
            }
        }
    }
}

template <>
int32_t HeapInfo::ValidPointersMap<SmallAllocationBlockAttributes>::GenerateValidPointersMapForBlockType(FILE* file)
{
#define IfErrorGotoCleanup(result) if ((result) < 0) { hr = E_FAIL; goto cleanup; }

    Assert(file != nullptr);
    int32_t hr = S_OK;

    // Use heap to allocate the table so we don't bloat the stack (~64k). We only use this function
    // to generate headers as part of testing.
    ValidPointersMapTable *valid = static_cast<ValidPointersMapTable*>(malloc(sizeof(ValidPointersMapTable)));
    InvalidBitsTable *invalid = static_cast<InvalidBitsTable*>(malloc(sizeof(InvalidBitsTable)));
    BlockInfoMapTable *blockMap = static_cast<BlockInfoMapTable*>(malloc(sizeof(BlockInfoMapTable)));

    if (valid == nullptr || invalid == nullptr || blockMap == nullptr)
    {
        hr = E_FAIL;
        goto cleanup;
    }
    GenerateValidPointersMap(valid, *invalid, *blockMap);

    std::println(file, "const ushort HeapInfo::ValidPointersMap<SmallAllocationBlockAttributes>::validPointersBuffer[HeapConstants::BucketCount][HeapInfo::ValidPointersMap<SmallAllocationBlockAttributes>::rowSize] = \n{{");
    // Generate the full buffer.
    for (unsigned i = 0; i < HeapConstants::BucketCount; ++i)
    {
        std::print(file, "    {{\n        ");
        for (unsigned j = 0; j < rowSize; ++j)
        {
            std::print(file, "0x{:4X}{}", (*valid)[i][j], j < rowSize - 1 ? ", " : "");
        }
        std::println(file, "\n    }}{}", i < HeapConstants::BucketCount - 1 ? "," : "");
    }
    std::println(file, "}};");

    // Generate the invalid bitvectors.
    std::println(
        file,
        "const BVUnit HeapInfo::ValidPointersMap<SmallAllocationBlockAttributes>::invalidBitsData[HeapConstants::BucketCount][SmallHeapBlockT<SmallAllocationBlockAttributes>::SmallHeapBlockBitVector::wordCount] = {{");
    for (unsigned i = 0; i < HeapConstants::BucketCount; ++i)
    {
        std::print(file, "    {{\n        ");

        for (unsigned j = 0; j < (*invalid)[i].wordCount; ++j)
        {
            std::print(file, "0x{:p}{}", static_cast<const void *>(&(*invalid)[i].GetRawData()[j]), j < (*invalid)[i].wordCount - 1 ? ", " : "");
        }
        std::println(file, "\n    }}{}", i < HeapConstants::BucketCount - 1 ? "," : "");
    }
        std::println(file, "}};");
        std::println(file, "// The following is used to construct the InvalidBitsTable statically without forcing BVStatic to be an aggregate");
        std::println(file, "const HeapInfo::ValidPointersMap<SmallAllocationBlockAttributes>::InvalidBitsTable * const HeapInfo::ValidPointersMap<SmallAllocationBlockAttributes>::invalidBitsBuffers =");
        std::println(file, "    reinterpret_cast<const HeapInfo::ValidPointersMap<SmallAllocationBlockAttributes>::InvalidBitsTable *>(&HeapInfo::ValidPointersMap<SmallAllocationBlockAttributes>::invalidBitsData);");

    // Generate the block map table
    std::println(
        file,
        "const SmallHeapBlockT<SmallAllocationBlockAttributes>::BlockInfo  HeapInfo::ValidPointersMap<SmallAllocationBlockAttributes>::blockInfoBuffer[SmallAllocationBlockAttributes::BucketCount][SmallAllocationBlockAttributes::PageCount] = {{");
    for (unsigned i = 0; i < HeapConstants::BucketCount; ++i)
    {
        std::println(file, "    // Bucket: {}, Size: {}", i, static_cast<int>(HeapConstants::ObjectGranularity + (i * SmallAllocationBlockAttributes::BucketGranularity)));
        std::println(file, "    {{");

        for (unsigned j = 0; j < SmallAllocationBlockAttributes::PageCount; ++j)
        {
            std::print(file, "        {{ ");

            std::print(file, "0x{:4X}, 0x{:4X}", (*blockMap)[i][j].lastObjectIndexOnPage, (*blockMap)[i][j].pageObjectCount);
            std::println(file, " }}");
        }
        std::println(file, "    }}{}", (i < HeapConstants::BucketCount - 1 ? "," : ""));
    }

    std::println(file, "}};");

cleanup:
#undef IfErrorGotoCleanup
    free(valid);
    free(invalid);
    return hr;
}

template <>
int32_t HeapInfo::ValidPointersMap<MediumAllocationBlockAttributes>::GenerateValidPointersMapForBlockType(FILE* file)
{
#define IfErrorGotoCleanup(result) if ((result) < 0) { hr = E_FAIL; goto cleanup; }

    Assert(file != nullptr);
    int32_t hr = S_OK;

    // Use heap to allocate the table so we don't bloat the stack (~64k). We only use this function
    // to generate headers as part of testing.
    ValidPointersMapTable *valid = static_cast<ValidPointersMapTable*>(malloc(sizeof(ValidPointersMapTable)));
    InvalidBitsTable *invalid = static_cast<InvalidBitsTable*>(malloc(sizeof(InvalidBitsTable)));
    BlockInfoMapTable *blockMap = static_cast<BlockInfoMapTable*>(malloc(sizeof(BlockInfoMapTable)));

    if (valid == nullptr || invalid == nullptr || blockMap == nullptr)
    {
        hr = E_FAIL;
        goto cleanup;
    }
    GenerateValidPointersMap(valid, *invalid, *blockMap);

    std::println(file, "const ushort HeapInfo::ValidPointersMap<MediumAllocationBlockAttributes>::validPointersBuffer[MediumAllocationBlockAttributes::BucketCount][HeapInfo::ValidPointersMap<MediumAllocationBlockAttributes>::rowSize] = \n{{");
    // Generate the full buffer.
    for (unsigned i = 0; i < HeapConstants::MediumBucketCount; ++i)
    {
        std::print(file, "    {{\n        ");
        for (unsigned j = 0; j < rowSize; ++j)
        {
            std::print(file, "0x{:4X}{}", (*valid)[i][j], (j < rowSize - 1) ? ", " : "");
        }
        std::println(file, "\n    }}{}", (i < HeapConstants::MediumBucketCount - 1 ? "," : ""));
    }
    std::println(file, "}};");

    // Generate the invalid bitvectors.
    std::println(
        file,
        "const BVUnit HeapInfo::ValidPointersMap<MediumAllocationBlockAttributes>::invalidBitsData[MediumAllocationBlockAttributes::BucketCount][SmallHeapBlockT<MediumAllocationBlockAttributes>::SmallHeapBlockBitVector::wordCount] = {{");
    for (unsigned i = 0; i < HeapConstants::MediumBucketCount; ++i)
    {
        std::print(file, "    {{\n        ");

        for (unsigned j = 0; j < (*invalid)[i].wordCount; ++j)
        {
            std::print(file, "0x{:p}{}", static_cast<const void *>(&(*invalid)[i].GetRawData()[j]), j < (*invalid)[i].wordCount - 1 ? ", " : "");
        }
        std::println(file, "\n    }}{}", (i < HeapConstants::MediumBucketCount - 1 ? "," : ""));
    }
        std::println(file, "}};");
        std::println(file, "// The following is used to construct the InvalidBitsTable statically without forcing BVStatic to be an aggregate");
        std::println(file, "const HeapInfo::ValidPointersMap<MediumAllocationBlockAttributes>::InvalidBitsTable * const HeapInfo::ValidPointersMap<MediumAllocationBlockAttributes>::invalidBitsBuffers =");
        std::println(file, "    reinterpret_cast<const HeapInfo::ValidPointersMap<MediumAllocationBlockAttributes>::InvalidBitsTable *>(&HeapInfo::ValidPointersMap<MediumAllocationBlockAttributes>::invalidBitsData);");

    // Generate the block map table
    std::println(
        file,
        "const SmallHeapBlockT<MediumAllocationBlockAttributes>::BlockInfo  HeapInfo::ValidPointersMap<MediumAllocationBlockAttributes>::blockInfoBuffer[MediumAllocationBlockAttributes::BucketCount][MediumAllocationBlockAttributes::PageCount] = {{");

    for (unsigned i = 0; i < HeapConstants::MediumBucketCount; ++i)
    {
        std::println(file, "    // Bucket: {}, Size: {}", i, static_cast<int>(HeapConstants::MaxSmallObjectSize + ((i + 1) * MediumAllocationBlockAttributes::BucketGranularity)));
        std::println(file, "    {{");

        for (unsigned j = 0; j < MediumAllocationBlockAttributes::PageCount; ++j)
        {
            std::print(file, "        {{ ");

            std::print(file, "0x{:4X}, 0x{:4X}", (*blockMap)[i][j].lastObjectIndexOnPage, (*blockMap)[i][j].pageObjectCount);
            std::println(file, " }}{}", (j < MediumAllocationBlockAttributes::PageCount - 1 ? "," : ""));
        }
        std::println(file, "    }}{}", (i < HeapConstants::MediumBucketCount - 1 ? "," : ""));
    }

    std::println(file, "}};");

cleanup:
#undef IfErrorGotoCleanup
    free(valid);
    free(invalid);
    return hr;
}

HeapInfo::HeapInfo(AllocationPolicyManager * policyManager, Js::ConfigFlagsTable& configFlagsTable, IdleDecommitPageAllocator * leafPageAllocator) :
    recycler(nullptr),
    recyclerLeafPageAllocator(leafPageAllocator),
    recyclerPageAllocator(this, policyManager, configFlagsTable, RecyclerHeuristic::Instance.DefaultMaxFreePageCount, RecyclerHeuristic::Instance.DefaultMaxAllocPageCount),
    recyclerLargeBlockPageAllocator(this, policyManager, configFlagsTable, RecyclerHeuristic::Instance.DefaultMaxFreePageCount),
#ifdef RECYCLER_WRITE_BARRIER_ALLOC_SEPARATE_PAGE
    recyclerWithBarrierPageAllocator(this, policyManager, configFlagsTable, RecyclerHeuristic::Instance.DefaultMaxFreePageCount, PageAllocator::DefaultMaxAllocPageCount, true),
#endif
    newLeafHeapBlockList(nullptr),
    newNormalHeapBlockList(nullptr),
    newNormalWithBarrierHeapBlockList(nullptr),
    newFinalizableWithBarrierHeapBlockList(nullptr),
    newFinalizableHeapBlockList(nullptr),
#ifdef RECYCLER_VISITED_HOST
    newRecyclerVisitedHostHeapBlockList(nullptr),
#endif
    newMediumLeafHeapBlockList(nullptr),
    newMediumNormalHeapBlockList(nullptr),
    newMediumNormalWithBarrierHeapBlockList(nullptr),
    newMediumFinalizableWithBarrierHeapBlockList(nullptr),
#ifdef RECYCLER_VISITED_HOST
    newMediumRecyclerVisitedHostHeapBlockList(nullptr),
#endif
    newMediumFinalizableHeapBlockList(nullptr),
#ifdef RECYCLER_FINALIZE_CHECK
    liveFinalizableObjectCount(0),
    pendingDisposableObjectCount(0),
    newFinalizableObjectCount(0),
#endif
    pageHeapMode(PageHeapMode::PageHeapModeOff),
    isPageHeapEnabled(false),
    pageHeapBlockType(PageHeapBlockTypeFilter::PageHeapBlockTypeFilterAll),
    captureAllocCallStack(false),
    captureFreeCallStack(false),
    hasPendingTransferDisposedObjects(false)
{
#if DBG_DUMP
    recyclerPageAllocator.debugName = u"Recycler";
    recyclerLargeBlockPageAllocator.debugName = u"RecyclerLargeBlock";
#ifdef RECYCLER_WRITE_BARRIER_ALLOC_SEPARATE_PAGE
    recyclerWithBarrierPageAllocator.debugName = u"RecyclerWithBarrier";
#endif
#endif
}

HeapInfo::~HeapInfo()
{
    RECYCLER_SLOW_CHECK(this->VerifySmallHeapBlockCount());

    // Finalize all finalizable object first
    for (uint i=0; i < HeapConstants::BucketCount; i++)
    {
        heapBuckets[i].FinalizeAllObjects();
    }

    for (uint i=0; i < HeapConstants::MediumBucketCount; i++)
    {
        mediumHeapBuckets[i].FinalizeAllObjects();
    }

    largeObjectBucket.FinalizeAllObjects();

    SmallFinalizableHeapBucket::FinalizeHeapBlockList(this->newFinalizableHeapBlockList);
    MediumFinalizableHeapBucket::FinalizeHeapBlockList(this->newMediumFinalizableHeapBlockList);
#ifdef RECYCLER_VISITED_HOST
    SmallRecyclerVisitedHostHeapBucket::FinalizeHeapBlockList(this->newRecyclerVisitedHostHeapBlockList);
    MediumRecyclerVisitedHostHeapBucket::FinalizeHeapBlockList(this->newMediumRecyclerVisitedHostHeapBlockList);
#endif
    SmallFinalizableWithBarrierHeapBucket::FinalizeHeapBlockList(this->newFinalizableWithBarrierHeapBlockList);
    MediumFinalizableWithBarrierHeapBucket::FinalizeHeapBlockList(this->newMediumFinalizableWithBarrierHeapBlockList);

#ifdef RECYCLER_FINALIZE_CHECK
    Assert(liveFinalizableObjectCount == 0);
    Assert(pendingDisposableObjectCount == 0);
#endif
    // Delete the heap blocks
    Recycler * recycler = this->recycler;

#ifdef RECYCLER_SLOW_CHECK_ENABLED
    size_t largeBlockCount = this->largeObjectBucket.GetLargeHeapBlockCount(false);

    uint mediumBlockCount = 0;
#endif

    RECYCLER_SLOW_CHECK(Assert(this->heapBlockCount[HeapBlock::HeapBlockType::LargeBlockType] - largeBlockCount - mediumBlockCount == 0));

    SmallLeafHeapBucket::DeleteHeapBlockList(this->newLeafHeapBlockList, recycler);
    SmallNormalHeapBucket::DeleteHeapBlockList(this->newNormalHeapBlockList, recycler);
    SmallNormalWithBarrierHeapBucket::DeleteHeapBlockList(this->newNormalWithBarrierHeapBlockList, recycler);
    SmallFinalizableWithBarrierHeapBucket::DeleteHeapBlockList(this->newFinalizableWithBarrierHeapBlockList, recycler);
    SmallFinalizableHeapBucket::DeleteHeapBlockList(this->newFinalizableHeapBlockList, recycler);

    MediumLeafHeapBucket::DeleteHeapBlockList(this->newMediumLeafHeapBlockList, recycler);
    MediumNormalHeapBucket::DeleteHeapBlockList(this->newMediumNormalHeapBlockList, recycler);
    MediumNormalWithBarrierHeapBucket::DeleteHeapBlockList(this->newMediumNormalWithBarrierHeapBlockList, recycler);
    MediumFinalizableWithBarrierHeapBucket::DeleteHeapBlockList(this->newMediumFinalizableWithBarrierHeapBlockList, recycler);
    MediumFinalizableHeapBucket::DeleteHeapBlockList(this->newMediumFinalizableHeapBlockList, recycler);

#ifdef RECYCLER_VISITED_HOST
    SmallFinalizableHeapBucket::DeleteHeapBlockList(this->newRecyclerVisitedHostHeapBlockList, recycler);
    MediumFinalizableHeapBucket::DeleteHeapBlockList(this->newMediumRecyclerVisitedHostHeapBlockList, recycler);
#endif


    // We do this here, instead of in the Recycler destructor, because the above stuff may
    // generate additional tracking events, particularly ReportUnallocated.
    // Arguably we shouldn't report these things as ReportUnallocated...
    RecyclerMemoryTracking::ReportRecyclerDestroy(recycler);
}

void
HeapInfo::Initialize(Recycler * recycler
    , PageHeapMode pageheapmode
    , bool captureAllocCallStack
    , bool captureFreeCallStack
)
{
    this->recycler = recycler;
#ifdef DUMP_FRAGMENTATION_STATS
    if (recycler->GetRecyclerFlagsTable().DumpFragmentationStats)
    {
        Output::Print(u"[FRAG %d] Start", ::GetTickCount());
    }
#endif

    isPageHeapEnabled = false;
    PageHeapBlockTypeFilter blockTypeFilter = PageHeapBlockTypeFilter::PageHeapBlockTypeFilterAll;
    Js::NumberRange bucketNumberRange;
    Js::NumberRange* pBucketNumberRange = &bucketNumberRange;
    if (pageheapmode == PageHeapMode::PageHeapModeOff)
    {
        isPageHeapEnabled = recycler->GetRecyclerFlagsTable().PageHeap != PageHeapMode::PageHeapModeOff;
        pageheapmode = static_cast<PageHeapMode>(recycler->GetRecyclerFlagsTable().PageHeap);
        blockTypeFilter = static_cast<PageHeapBlockTypeFilter>(recycler->GetRecyclerFlagsTable().PageHeapBlockType);
        pBucketNumberRange = &recycler->GetRecyclerFlagsTable().PageHeapBucketNumber;

    }
    else
    {
        isPageHeapEnabled = true;
    }

    if (isPageHeapEnabled)
    {
        this->captureAllocCallStack = captureAllocCallStack;
        this->captureFreeCallStack = captureFreeCallStack;

        this->captureAllocCallStack = captureAllocCallStack || recycler->GetRecyclerFlagsTable().PageHeapAllocStack;
        this->captureFreeCallStack = captureFreeCallStack || recycler->GetRecyclerFlagsTable().PageHeapFreeStack;
    }

    if (IsPageHeapEnabled())
    {
        this->pageHeapMode = pageheapmode;

        // Use one of the two modes with -PageHeap flag
        Assert(this->pageHeapMode == PageHeapMode::PageHeapModeBlockStart || this->pageHeapMode == PageHeapMode::PageHeapModeBlockEnd);

        this->pageHeapBlockType = blockTypeFilter;

        for (int i = 0; i < HeapConstants::BucketCount + HeapConstants::MediumBucketCount; i++)
        {
            if (pBucketNumberRange->InRange(i))
            {
                if (i < HeapConstants::BucketCount)
                {
                    this->smallBlockPageHeapBucketFilter.Set(i);
                }
                else
                {
                    this->mediumBlockPageHeapBucketFilter.Set(i - HeapConstants::BucketCount);
                }
            }
        }
    }
    else
    {
        // These should not be set if we're not in page heap mode
        Assert(!(captureAllocCallStack || captureFreeCallStack));
    }

    for (uint i = 0; i < HeapConstants::BucketCount; i++)
    {
        heapBuckets[i].Initialize(this, (i + 1) << HeapConstants::ObjectAllocationShift);
    }
    RECYCLER_SLOW_CHECK(memset(this->heapBlockCount, 0, sizeof(this->heapBlockCount)));

    for (uint i = 0; i < HeapConstants::MediumBucketCount; i++)
    {
        mediumHeapBuckets[i].Initialize(this, HeapConstants::MaxSmallObjectSize + ((i + 1) * HeapConstants::MediumObjectGranularity));
    }

    largeObjectBucket.Initialize(this, HeapConstants::MaxMediumObjectSize);
}

#if defined(PROFILE_RECYCLER_ALLOC) || defined(RECYCLER_MEMORY_VERIFY) || defined(MEMSPECT_TRACKING)
void
HeapInfo::Initialize(Recycler * recycler, void(*trackNativeAllocCallBack)(Recycler *, void *, size_t)
, PageHeapMode pageheapmode
, bool captureAllocCallStack
, bool captureFreeCallStack
)
{
    Initialize(recycler
        , pageheapmode
        , captureAllocCallStack
        , captureFreeCallStack
        );

    for (uint i = 0; i < HeapConstants::BucketCount; i++)
    {
        heapBuckets[i].GetBucket<NoBit>().GetAllocator()->SetTrackNativeAllocatedObjectCallBack(trackNativeAllocCallBack);
    }

    for (uint i = 0; i < HeapConstants::MediumBucketCount; i++)
    {
        mediumHeapBuckets[i].GetBucket<NoBit>().GetAllocator()->SetTrackNativeAllocatedObjectCallBack(trackNativeAllocCallBack);
    }
}
#endif

template bool HeapInfo::IsPageHeapEnabledForBlock<MediumAllocationBlockAttributes>(const size_t objectSize);
template bool HeapInfo::IsPageHeapEnabledForBlock<SmallAllocationBlockAttributes>(const size_t objectSize);
template bool HeapInfo::IsPageHeapEnabledForBlock<LargeAllocationBlockAttributes>(const size_t objectSize);

template <typename TBlockAttributes>
bool HeapInfo::IsPageHeapEnabledForBlock(const size_t objectSize)
{
    if (IsPageHeapEnabled())
    {
        if (TBlockAttributes::IsSmallBlock)
        {
            return smallBlockPageHeapBucketFilter.Test(GetBucketIndex(objectSize)) != 0;
        }
        else if (TBlockAttributes::IsMediumBlock)
        {
            return mediumBlockPageHeapBucketFilter.Test(GetMediumBucketIndex(objectSize)) != 0;
        }
        else
        {
            return (static_cast<byte>(this->pageHeapBlockType) & static_cast<byte>(PageHeapBlockTypeFilter::PageHeapBlockTypeFilterLarge)) != 0;
        }
    }
    return false;
}

void
HeapInfo::ResetMarks(ResetMarkFlags flags)
{
    for (uint i=0; i < HeapConstants::BucketCount; i++)
    {
        heapBuckets[i].ResetMarks(flags);
    }
    for (uint i=0; i < HeapConstants::MediumBucketCount; i++)
    {
        mediumHeapBuckets[i].ResetMarks(flags);
    }

    largeObjectBucket.ResetMarks(flags);

    if ((flags & ResetMarkFlags_ScanImplicitRoot) != 0)
    {
        HeapBlockList::ForEach(newLeafHeapBlockList, [](SmallLeafHeapBlock * heapBlock)
        {
            heapBlock->MarkImplicitRoots();
        });

        HeapBlockList::ForEach(newNormalHeapBlockList, [](SmallNormalHeapBlock * heapBlock)
        {
            heapBlock->MarkImplicitRoots();
        });

        HeapBlockList::ForEach(newNormalWithBarrierHeapBlockList, [](SmallNormalWithBarrierHeapBlock * heapBlock)
        {
            heapBlock->MarkImplicitRoots();
        });

        HeapBlockList::ForEach(newFinalizableWithBarrierHeapBlockList, [](SmallFinalizableWithBarrierHeapBlock * heapBlock)
        {
            heapBlock->MarkImplicitRoots();
        });

        HeapBlockList::ForEach(newFinalizableHeapBlockList, [](SmallNormalHeapBlock * heapBlock)
        {
            heapBlock->MarkImplicitRoots();
        });

        HeapBlockList::ForEach(newMediumLeafHeapBlockList, [](MediumLeafHeapBlock * heapBlock)
        {
            heapBlock->MarkImplicitRoots();
        });

        HeapBlockList::ForEach(newMediumNormalHeapBlockList, [](MediumNormalHeapBlock * heapBlock)
        {
            heapBlock->MarkImplicitRoots();
        });

        HeapBlockList::ForEach(newMediumNormalWithBarrierHeapBlockList, [](MediumNormalWithBarrierHeapBlock * heapBlock)
        {
            heapBlock->MarkImplicitRoots();
        });

        HeapBlockList::ForEach(newMediumFinalizableWithBarrierHeapBlockList, [](MediumFinalizableWithBarrierHeapBlock * heapBlock)
        {
            heapBlock->MarkImplicitRoots();
        });
        HeapBlockList::ForEach(newMediumFinalizableHeapBlockList, [](MediumNormalHeapBlock * heapBlock)
        {
            heapBlock->MarkImplicitRoots();
        });
    }
}

void
HeapInfo::ScanInitialImplicitRoots()
{
    for (uint i = 0; i < HeapConstants::BucketCount; i++)
    {
        heapBuckets[i].ScanInitialImplicitRoots(recycler);
    }
    for (uint i = 0; i < HeapConstants::MediumBucketCount; i++)
    {
        mediumHeapBuckets[i].ScanInitialImplicitRoots(recycler);
    }

    largeObjectBucket.ScanInitialImplicitRoots(recycler);

    // NOTE: Don't need to do newLeafHeapBlockList

    HeapBlockList::ForEach(newNormalHeapBlockList, [this](SmallNormalHeapBlock * heapBlock)
    {
        heapBlock->ScanInitialImplicitRoots(recycler);
    });

    HeapBlockList::ForEach(newNormalWithBarrierHeapBlockList, [this](SmallNormalWithBarrierHeapBlock * heapBlock)
    {
        heapBlock->ScanInitialImplicitRoots(recycler);
    });

    HeapBlockList::ForEach(newFinalizableWithBarrierHeapBlockList, [this](SmallFinalizableWithBarrierHeapBlock * heapBlock)
    {
        heapBlock->ScanInitialImplicitRoots(recycler);
    });

    HeapBlockList::ForEach(newFinalizableHeapBlockList, [this](SmallNormalHeapBlock * heapBlock)
    {
        heapBlock->ScanInitialImplicitRoots(recycler);
    });

    // NOTE: Don't need to do newLeafHeapBlockList

    HeapBlockList::ForEach(newMediumNormalHeapBlockList, [this](MediumNormalHeapBlock * heapBlock)
    {
        heapBlock->ScanInitialImplicitRoots(recycler);
    });

    HeapBlockList::ForEach(newMediumNormalWithBarrierHeapBlockList, [this](MediumNormalWithBarrierHeapBlock * heapBlock)
    {
        heapBlock->ScanInitialImplicitRoots(recycler);
    });

    HeapBlockList::ForEach(newMediumFinalizableWithBarrierHeapBlockList, [this](MediumFinalizableWithBarrierHeapBlock * heapBlock)
    {
        heapBlock->ScanInitialImplicitRoots(recycler);
    });

    HeapBlockList::ForEach(newMediumFinalizableHeapBlockList, [this](MediumNormalHeapBlock * heapBlock)
    {
        heapBlock->ScanInitialImplicitRoots(recycler);
    });


}


void
HeapInfo::ScanNewImplicitRoots()
{
    for (uint i = 0; i < HeapConstants::BucketCount; i++)
    {
        heapBuckets[i].ScanNewImplicitRoots(recycler);
    }
    for (uint i = 0; i < HeapConstants::MediumBucketCount; i++)
    {
        mediumHeapBuckets[i].ScanNewImplicitRoots(recycler);
    }

    largeObjectBucket.ScanNewImplicitRoots(recycler);

    // NOTE: need to do newLeafHeapBlockList to find new memory
    HeapBlockList::ForEach(newLeafHeapBlockList, [this](SmallLeafHeapBlock * heapBlock)
    {
        heapBlock->ScanNewImplicitRoots(recycler);
    });

    HeapBlockList::ForEach(newNormalHeapBlockList, [this](SmallNormalHeapBlock * heapBlock)
    {
        heapBlock->ScanNewImplicitRoots(recycler);
    });

    HeapBlockList::ForEach(newNormalWithBarrierHeapBlockList, [this](SmallNormalWithBarrierHeapBlock * heapBlock)
    {
        heapBlock->ScanNewImplicitRoots(recycler);
    });

    HeapBlockList::ForEach(newFinalizableWithBarrierHeapBlockList, [this](SmallFinalizableWithBarrierHeapBlock * heapBlock)
    {
        heapBlock->ScanNewImplicitRoots(recycler);
    });

    HeapBlockList::ForEach(newFinalizableHeapBlockList, [this](SmallNormalHeapBlock * heapBlock)
    {
        heapBlock->ScanNewImplicitRoots(recycler);
    });

    // NOTE: need to do newLeafHeapBlockList to find new memory
    HeapBlockList::ForEach(newMediumLeafHeapBlockList, [this](MediumLeafHeapBlock * heapBlock)
    {
        heapBlock->ScanNewImplicitRoots(recycler);
    });

    HeapBlockList::ForEach(newMediumNormalHeapBlockList, [this](MediumNormalHeapBlock * heapBlock)
    {
        heapBlock->ScanNewImplicitRoots(recycler);
    });

    HeapBlockList::ForEach(newMediumNormalWithBarrierHeapBlockList, [this](MediumNormalWithBarrierHeapBlock * heapBlock)
    {
        heapBlock->ScanNewImplicitRoots(recycler);
    });

    HeapBlockList::ForEach(newMediumFinalizableWithBarrierHeapBlockList, [this](MediumFinalizableWithBarrierHeapBlock * heapBlock)
    {
        heapBlock->ScanNewImplicitRoots(recycler);
    });

    HeapBlockList::ForEach(newMediumFinalizableHeapBlockList, [this](MediumNormalHeapBlock * heapBlock)
    {
        heapBlock->ScanNewImplicitRoots(recycler);
    });

}

LargeHeapBlock *
HeapInfo::AddLargeHeapBlock(size_t size)
{
    // Do a no-throwing allocation here
    return largeObjectBucket.AddLargeHeapBlock(size, /* nothrow = */ true);
}

void
HeapInfo::Finalize(RecyclerSweep& recyclerSweep)
{
    [[maybe_unused]] Recycler * recycler = recyclerSweep.GetRecycler();
#ifdef RECYCLER_STATS
    memset(&recycler->collectionStats.numEmptySmallBlocks, 0, sizeof(recycler->collectionStats.numEmptySmallBlocks));
    recycler->collectionStats.numZeroedOutSmallBlocks = 0;
#endif
#ifdef RECYCLER_FINALIZE_CHECK
    this->newFinalizableObjectCount = 0;
#endif
    RECYCLER_SLOW_CHECK(VerifySmallHeapBlockCount());

    // Merge the new blocks before we sweep the finalizable object in thread
    recyclerSweep.MergePendingNewHeapBlockList<SmallFinalizableHeapBlock>();
    recyclerSweep.MergePendingNewMediumHeapBlockList<MediumFinalizableHeapBlock>();

    recyclerSweep.MergePendingNewHeapBlockList<SmallFinalizableWithBarrierHeapBlock>();
    recyclerSweep.MergePendingNewMediumHeapBlockList<MediumFinalizableWithBarrierHeapBlock>();
#ifdef RECYCLER_VISITED_HOST
    recyclerSweep.MergePendingNewHeapBlockList<SmallRecyclerVisitedHostHeapBlock>();
    recyclerSweep.MergePendingNewMediumHeapBlockList<MediumRecyclerVisitedHostHeapBlock>();
#endif

    RECYCLER_PROFILE_EXEC_BEGIN(recycler, Js::FinalizePhase);    

    largeObjectBucket.Finalize();

    for (uint i = 0; i < HeapConstants::BucketCount; i++)
    {
        heapBuckets[i].SweepFinalizableObjects(recyclerSweep);
    }

    // CONCURRENT-TODO: Allow this in the background as well
    for (uint i = 0; i < HeapConstants::MediumBucketCount; i++)
    {
        mediumHeapBuckets[i].SweepFinalizableObjects(recyclerSweep);
    }

    RECYCLER_PROFILE_EXEC_END(recycler, Js::FinalizePhase);
}

void
HeapInfo::Sweep(RecyclerSweep& recyclerSweep, bool concurrent)
{
    RECYCLER_PROFILE_EXEC_BEGIN(recyclerSweep.GetRecycler(), Js::SweepSmallPhase);

    if (concurrent)
    {
        RECYCLER_SLOW_CHECK(VerifySmallHeapBlockCount());
        RECYCLER_SLOW_CHECK(VerifyLargeHeapBlockCount());
    }

    if (concurrent)
    {
        this->SetupBackgroundSweep(recyclerSweep);
    }
    else
    {
        this->SweepSmallNonFinalizable(recyclerSweep);
    }

    RECYCLER_PROFILE_EXEC_CHANGE(recyclerSweep.GetRecycler(), Js::SweepSmallPhase, Js::SweepLargePhase);

    largeObjectBucket.Sweep(recyclerSweep);

    RECYCLER_PROFILE_EXEC_END(recyclerSweep.GetRecycler(), Js::SweepLargePhase);

    RECYCLER_SLOW_CHECK(VerifyLargeHeapBlockCount());
    RECYCLER_SLOW_CHECK(Assert(this->newFinalizableObjectCount == 0));
}

void
HeapInfo::SetupBackgroundSweep(RecyclerSweep& recyclerSweep)
{
    for (uint i = 0; i < HeapConstants::BucketCount; i++)
    {
        this->heapBuckets[i].SetupBackgroundSweep(recyclerSweep);
    }

    for (uint i = 0; i < HeapConstants::MediumBucketCount; i++)
    {
        this->mediumHeapBuckets[i].SetupBackgroundSweep(recyclerSweep);
    }
}

void
HeapInfo::SweepSmallNonFinalizable(RecyclerSweep& recyclerSweep)
{
    recyclerSweep.MergePendingNewHeapBlockList<SmallLeafHeapBlock>();
    recyclerSweep.MergePendingNewHeapBlockList<SmallNormalHeapBlock>();
    recyclerSweep.MergePendingNewMediumHeapBlockList<MediumLeafHeapBlock>();
    recyclerSweep.MergePendingNewMediumHeapBlockList<MediumNormalHeapBlock>();
    recyclerSweep.MergePendingNewHeapBlockList<SmallNormalWithBarrierHeapBlock>();
    recyclerSweep.MergePendingNewMediumHeapBlockList<MediumNormalWithBarrierHeapBlock>();

    // Finalizable are already merge before in SweepHeap
    Assert(!recyclerSweep.HasPendingNewHeapBlocks());
    if (!recyclerSweep.IsBackground())
    {
        // finalizer may trigger arena allocations, do don't suspend the leaf (thread) page allocator
        // until  we are going to sweep leaf pages.
        this->GetRecyclerLeafPageAllocator()->SuspendIdleDecommit();
    }
    for (uint i=0; i<HeapConstants::BucketCount; i++)
    {
        heapBuckets[i].Sweep(recyclerSweep);
    }

    for (uint i = 0; i < HeapConstants::MediumBucketCount; i++)
    {
        mediumHeapBuckets[i].Sweep(recyclerSweep);
    }

    if (!recyclerSweep.IsBackground())
    {
        // large block don't use the leaf page allocator, we can resume idle decommit now
        this->GetRecyclerLeafPageAllocator()->ResumeIdleDecommit();

        RECYCLER_SLOW_CHECK(VerifySmallHeapBlockCount());
        RECYCLER_SLOW_CHECK(VerifyLargeHeapBlockCount());
    }
}

size_t
HeapInfo::Rescan(RescanFlags flags)
{
    size_t scannedPageCount = 0;

    for (uint i = 0; i < HeapConstants::BucketCount; i++)
    {
        scannedPageCount += heapBuckets[i].Rescan(recycler, flags);
    }

    for (uint i = 0; i < HeapConstants::MediumBucketCount; i++)
    {
        scannedPageCount += mediumHeapBuckets[i].Rescan(recycler, flags);
    }

    scannedPageCount += largeObjectBucket.Rescan(flags);

    return scannedPageCount;
}


#if ENABLE_MEM_STATS
#include "BucketStatsReporter.h"
void
HeapInfo::GetBucketStats(BucketStatsReporter& report)
{
    // Pre aggregate pass on all the heap blocks that are not merged into bucket's lists yet
    report.PreAggregateBucketStats(this->newNormalHeapBlockList);
    report.PreAggregateBucketStats(this->newLeafHeapBlockList);
    report.PreAggregateBucketStats(this->newFinalizableHeapBlockList);
    report.PreAggregateBucketStats(this->newNormalWithBarrierHeapBlockList);
    report.PreAggregateBucketStats(this->newFinalizableWithBarrierHeapBlockList);
#ifdef RECYCLER_VISITED_HOST
    report.PreAggregateBucketStats(this->newRecyclerVisitedHostHeapBlockList);
#endif

    report.PreAggregateBucketStats(this->newMediumNormalHeapBlockList);
    report.PreAggregateBucketStats(this->newMediumLeafHeapBlockList);
    report.PreAggregateBucketStats(this->newMediumFinalizableHeapBlockList);
    report.PreAggregateBucketStats(this->newMediumNormalWithBarrierHeapBlockList);
    report.PreAggregateBucketStats(this->newMediumFinalizableWithBarrierHeapBlockList);
#ifdef RECYCLER_VISITED_HOST
    report.PreAggregateBucketStats(this->newMediumRecyclerVisitedHostHeapBlockList);
#endif

    for (uint i = 0; i < HeapConstants::BucketCount; i++)
    {
        report.GetBucketStats<SmallAllocationBlockAttributes, NoBit>(heapBuckets[i]);
        report.GetBucketStats<SmallAllocationBlockAttributes, LeafBit>(heapBuckets[i]);
        report.GetBucketStats<SmallAllocationBlockAttributes, FinalizeBit>(heapBuckets[i]);
        report.GetBucketStats<SmallAllocationBlockAttributes, WithBarrierBit>(heapBuckets[i]);
        report.GetBucketStats<SmallAllocationBlockAttributes, FinalizableWithBarrierBit>(heapBuckets[i]);
#ifdef RECYCLER_VISITED_HOST
        report.GetBucketStats<SmallAllocationBlockAttributes, RecyclerVisitedHostBit>(heapBuckets[i]);
#endif
    }

    for (uint i = 0; i < HeapConstants::MediumBucketCount; i++)
    {
        report.GetBucketStats<MediumAllocationBlockAttributes, NoBit>(mediumHeapBuckets[i]);
        report.GetBucketStats<MediumAllocationBlockAttributes, LeafBit>(mediumHeapBuckets[i]);
        report.GetBucketStats<MediumAllocationBlockAttributes, FinalizeBit>(mediumHeapBuckets[i]);
        report.GetBucketStats<MediumAllocationBlockAttributes, WithBarrierBit>(mediumHeapBuckets[i]);
        report.GetBucketStats<MediumAllocationBlockAttributes, FinalizableWithBarrierBit>(mediumHeapBuckets[i]);
#ifdef RECYCLER_VISITED_HOST
        report.GetBucketStats<MediumAllocationBlockAttributes, RecyclerVisitedHostBit>(mediumHeapBuckets[i]);
#endif
    }

    report.GetBucketStats(largeObjectBucket);
}

#endif  // ENABLE_MEM_STATS


void
HeapInfo::SweepPartialReusePages(RecyclerSweep& recyclerSweep)
{
    RECYCLER_PROFILE_EXEC_THREAD_BEGIN(recyclerSweep.IsBackground(), recyclerSweep.GetRecycler(), Js::SweepPartialReusePhase);

    for (uint i = 0; i < HeapConstants::BucketCount; i++)
    {
        heapBuckets[i].SweepPartialReusePages(recyclerSweep);
    }

    for (uint i = 0; i < HeapConstants::MediumBucketCount; i++)
    {
        mediumHeapBuckets[i].SweepPartialReusePages(recyclerSweep);
    }

    RECYCLER_PROFILE_EXEC_THREAD_END(recyclerSweep.IsBackground(), recyclerSweep.GetRecycler(), Js::SweepPartialReusePhase);

    // GC-TODO: LargeHeapBlock don't reuse object, so we don't need to keep
    // pages with low free space from being reused.

    // Only count the byte that we would have freed but we are not reusing it if we are doing a partial GC
    // This will increase the GC pressure and make partial less and less likely.
    if (recyclerSweep.GetManager()->InPartialCollect())
    {
        recyclerSweep.GetRecycler()->autoHeap.unusedPartialCollectFreeBytes += recyclerSweep.GetManager()->GetPartialUnusedFreeByteCount();
    }
}

void HeapInfo::FinishPartialCollect(RecyclerSweep * recyclerSweep)
{
    for (uint i = 0; i < HeapConstants::BucketCount; i++)
    {
        heapBuckets[i].FinishPartialCollect(recyclerSweep);
    }

    for (uint i = 0; i < HeapConstants::MediumBucketCount; i++)
    {
        mediumHeapBuckets[i].FinishPartialCollect(recyclerSweep);
    }

    largeObjectBucket.FinishPartialCollect(recyclerSweep);
}

void
HeapInfo::PrepareSweep()
{
    for (uint i = 0; i < HeapConstants::BucketCount; i++)
    {
        heapBuckets[i].PrepareSweep();
    }

    for (uint i = 0; i < HeapConstants::MediumBucketCount; i++)
    {
        mediumHeapBuckets[i].PrepareSweep();
    }
}

void
HeapInfo::SweepPendingObjects(RecyclerSweep& recyclerSweep)
{
    if (recyclerSweep.HasPendingSweepSmallHeapBlocks())
    {
        for (uint i = 0; i < HeapConstants::BucketCount; i++)
        {
            heapBuckets[i].SweepPendingObjects(recyclerSweep);
        }

        for (uint i = 0; i < HeapConstants::MediumBucketCount; i++)
        {
            mediumHeapBuckets[i].SweepPendingObjects(recyclerSweep);
        }
    }

    largeObjectBucket.SweepPendingObjects(recyclerSweep);
}

void
HeapInfo::TransferPendingHeapBlocks(RecyclerSweep& recyclerSweep)
{
    Assert(!recyclerSweep.IsBackground());
    RECYCLER_SLOW_CHECK(VerifySmallHeapBlockCount());
    if (recyclerSweep.HasPendingEmptyBlocks())
    {
        for (uint i = 0; i < HeapConstants::BucketCount; i++)
        {
            heapBuckets[i].TransferPendingEmptyHeapBlocks(recyclerSweep);
        }

        for (uint i = 0; i < HeapConstants::MediumBucketCount; i++)
        {
            mediumHeapBuckets[i].TransferPendingEmptyHeapBlocks(recyclerSweep);
        }

        RECYCLER_SLOW_CHECK(VerifySmallHeapBlockCount());
    }

    // We might still have block that has been disposed but not made allocable
    // which happens if we finish disposing object during concurrent sweep
    // and can't modify the block lists
    recyclerSweep.FlushPendingTransferDisposedObjects();
}

void
HeapInfo::ConcurrentTransferSweptObjects(RecyclerSweep& recyclerSweep)
{
    Assert(!recyclerSweep.InPartialCollectMode());
    Assert(!recyclerSweep.IsBackground());
    TransferPendingHeapBlocks(recyclerSweep);

    largeObjectBucket.ConcurrentTransferSweptObjects(recyclerSweep);
}

void
HeapInfo::ConcurrentPartialTransferSweptObjects(RecyclerSweep& recyclerSweep)
{
    Assert(recyclerSweep.InPartialCollectMode());
    Assert(!recyclerSweep.IsBackground());
    TransferPendingHeapBlocks(recyclerSweep);

    RECYCLER_SLOW_CHECK(this->VerifyLargeHeapBlockCount());

    largeObjectBucket.ConcurrentPartialTransferSweptObjects(recyclerSweep);

    RECYCLER_SLOW_CHECK(this->VerifyLargeHeapBlockCount());
}

void
HeapInfo::DisposeObjects()
{
    Recycler * recycler = this->recycler;
    do
    {
        recycler->hasDisposableObject = false;

        // finalizing the objects
        for (uint i = 0; i < HeapConstants::BucketCount; i++)
        {
            heapBuckets[i].DisposeObjects();
        }

        for (uint i = 0; i < HeapConstants::MediumBucketCount; i++)
        {
            mediumHeapBuckets[i].DisposeObjects();
        }

        largeObjectBucket.DisposeObjects();
    }
    // Calling dispose may enter the GC again and dispose more objects, loop until we don't have any more
    while (recycler->hasDisposableObject);

    this->hasPendingTransferDisposedObjects = true;
    if (!recycler->IsConcurrentExecutingState())
    {
        // Can't transfer disposed object when the background thread is walking the heap block list
        // That includes reset mark, background rescan and concurrent sweep. Delay the transfer later.
        // NOTE1: During concurrent sweep,  we can't do this only if the bucket has "stopped" allocation
        // After it resume allocation, we don't walk the list in the background thread any more
        // (except for checking heap block count). But this is easier to detect via the collection state
        // without walking all buckets.
        // NOTE2: During transitive closure mark, we don't walk the heap block list, but we can continue
        // to do background rescan.  Since we don't have synchronization for that, we can't really enable
        // able this just for the transitive closure,  so just do all the background executing state.
        TransferDisposedObjects();
    }
}

void
HeapInfo::TransferDisposedObjects()
{
    Assert(this->hasPendingTransferDisposedObjects);
    Assert(!this->recycler->IsConcurrentExecutingState());
    this->hasPendingTransferDisposedObjects = false;

    // move the disposed object back to the free lists
    for (uint i = 0; i < HeapConstants::BucketCount; i++)
    {
        heapBuckets[i].TransferDisposedObjects();
    }

    for (uint i = 0; i < HeapConstants::MediumBucketCount; i++)
    {
        mediumHeapBuckets[i].TransferDisposedObjects();
    }

    largeObjectBucket.TransferDisposedObjects();
}
void
HeapInfo::EnumerateObjects(ObjectInfoBits infoBits, void (*CallBackFunction)(void * address, size_t size))
{
    for (uint i = 0; i < HeapConstants::BucketCount; i++)
    {
        heapBuckets[i].EnumerateObjects(infoBits, CallBackFunction);
    }

    for (uint i = 0; i < HeapConstants::MediumBucketCount; i++)
    {
        mediumHeapBuckets[i].EnumerateObjects(infoBits, CallBackFunction);
    }

    largeObjectBucket.EnumerateObjects(infoBits, CallBackFunction);

    HeapBucket::EnumerateObjects(newLeafHeapBlockList, infoBits, CallBackFunction);
    HeapBucket::EnumerateObjects(newNormalHeapBlockList, infoBits, CallBackFunction);
    HeapBucket::EnumerateObjects(newNormalWithBarrierHeapBlockList, infoBits, CallBackFunction);
    HeapBucket::EnumerateObjects(newFinalizableWithBarrierHeapBlockList, infoBits, CallBackFunction);

#ifdef RECYCLER_VISITED_HOST
    HeapBucket::EnumerateObjects(newRecyclerVisitedHostHeapBlockList, infoBits, CallBackFunction);
#endif
    HeapBucket::EnumerateObjects(newFinalizableHeapBlockList, infoBits, CallBackFunction);

    HeapBucket::EnumerateObjects(newMediumLeafHeapBlockList, infoBits, CallBackFunction);
    HeapBucket::EnumerateObjects(newMediumNormalHeapBlockList, infoBits, CallBackFunction);
    HeapBucket::EnumerateObjects(newMediumNormalWithBarrierHeapBlockList, infoBits, CallBackFunction);
    HeapBucket::EnumerateObjects(newMediumFinalizableWithBarrierHeapBlockList, infoBits, CallBackFunction);

#ifdef RECYCLER_VISITED_HOST
    HeapBucket::EnumerateObjects(newMediumRecyclerVisitedHostHeapBlockList, infoBits, CallBackFunction);
#endif
    HeapBucket::EnumerateObjects(newMediumFinalizableHeapBlockList, infoBits, CallBackFunction);
}

#if DBG || defined(RECYCLER_SLOW_CHECK_ENABLED)
size_t
HeapInfo::GetSmallHeapBlockCount(bool checkCount) const
{
    size_t currentSmallHeapBlockCount = 0;
    for (uint i = 0; i < HeapConstants::BucketCount; i++)
    {
        currentSmallHeapBlockCount += heapBuckets[i].GetNonEmptyHeapBlockCount(checkCount);
        currentSmallHeapBlockCount += heapBuckets[i].GetEmptyHeapBlockCount();
    }

    for (uint i = 0; i < HeapConstants::MediumBucketCount; i++)
    {
        currentSmallHeapBlockCount += mediumHeapBuckets[i].GetNonEmptyHeapBlockCount(checkCount);
        currentSmallHeapBlockCount += mediumHeapBuckets[i].GetEmptyHeapBlockCount();
    }

    currentSmallHeapBlockCount += HeapBlockList::Count(this->newLeafHeapBlockList);
    currentSmallHeapBlockCount += HeapBlockList::Count(this->newNormalHeapBlockList);
    currentSmallHeapBlockCount += HeapBlockList::Count(this->newFinalizableHeapBlockList);
#ifdef RECYCLER_VISITED_HOST
    currentSmallHeapBlockCount += HeapBlockList::Count(this->newRecyclerVisitedHostHeapBlockList);
#endif
    currentSmallHeapBlockCount += HeapBlockList::Count(this->newNormalWithBarrierHeapBlockList);
    currentSmallHeapBlockCount += HeapBlockList::Count(this->newFinalizableWithBarrierHeapBlockList);

    currentSmallHeapBlockCount += HeapBlockList::Count(this->newMediumLeafHeapBlockList);
    currentSmallHeapBlockCount += HeapBlockList::Count(this->newMediumNormalHeapBlockList);
#ifdef RECYCLER_VISITED_HOST
    currentSmallHeapBlockCount += HeapBlockList::Count(this->newMediumRecyclerVisitedHostHeapBlockList);
#endif
    currentSmallHeapBlockCount += HeapBlockList::Count(this->newMediumFinalizableHeapBlockList);
    currentSmallHeapBlockCount += HeapBlockList::Count(this->newMediumNormalWithBarrierHeapBlockList);
    currentSmallHeapBlockCount += HeapBlockList::Count(this->newMediumFinalizableWithBarrierHeapBlockList);

    // TODO: Update recycler sweep
    // Recycler can be null if we have OOM in the ctor
    if (this->recycler && this->recycler->recyclerSweepManager != nullptr)
    {
        // This function can't be called in the background
        Assert(!this->recycler->recyclerSweepManager->IsBackground());
        currentSmallHeapBlockCount += this->recycler->recyclerSweepManager->GetPendingMergeNewHeapBlockCount(this);
    }
#ifdef RECYCLER_SLOW_CHECK_ENABLED
    size_t expectedHeapBlockCount =
        this->heapBlockCount[HeapBlock::HeapBlockType::SmallNormalBlockType]
        + this->heapBlockCount[HeapBlock::HeapBlockType::SmallLeafBlockType]
        + this->heapBlockCount[HeapBlock::HeapBlockType::SmallFinalizableBlockType]
#ifdef RECYCLER_VISITED_HOST
        + this->heapBlockCount[HeapBlock::HeapBlockType::SmallRecyclerVisitedHostBlockType]
        + this->heapBlockCount[HeapBlock::HeapBlockType::MediumRecyclerVisitedHostBlockType]
#endif
        + this->heapBlockCount[HeapBlock::HeapBlockType::MediumNormalBlockType]
        + this->heapBlockCount[HeapBlock::HeapBlockType::MediumLeafBlockType]
        + this->heapBlockCount[HeapBlock::HeapBlockType::MediumFinalizableBlockType];


    expectedHeapBlockCount +=
        this->heapBlockCount[HeapBlock::HeapBlockType::SmallNormalBlockWithBarrierType]
        + this->heapBlockCount[HeapBlock::HeapBlockType::SmallFinalizableBlockWithBarrierType]
        + this->heapBlockCount[HeapBlock::HeapBlockType::MediumNormalBlockWithBarrierType]
        + this->heapBlockCount[HeapBlock::HeapBlockType::MediumFinalizableBlockWithBarrierType];

    Assert(!checkCount || currentSmallHeapBlockCount == expectedHeapBlockCount);
#endif

    return currentSmallHeapBlockCount;
}

size_t
HeapInfo::GetLargeHeapBlockCount(bool checkCount) const
{
    size_t currentLargeHeapBlockCount = 0;

    currentLargeHeapBlockCount += largeObjectBucket.GetLargeHeapBlockCount(checkCount);

    RECYCLER_SLOW_CHECK(Assert(!checkCount || currentLargeHeapBlockCount == this->heapBlockCount[HeapBlock::HeapBlockType::LargeBlockType]));
    return currentLargeHeapBlockCount;
}

#endif

#ifdef RECYCLER_SLOW_CHECK_ENABLED
void
HeapInfo::Check()
{
    Assert(!this->recycler->CollectionInProgress());

    size_t currentSmallHeapBlockCount = 0;
    for (uint i = 0; i < HeapConstants::BucketCount; i++)
    {
        currentSmallHeapBlockCount += heapBuckets[i].Check();
        currentSmallHeapBlockCount += heapBuckets[i].GetEmptyHeapBlockCount();
    }

    size_t currentLargeHeapBlockCount = 0;
    for (uint i = 0; i < HeapConstants::MediumBucketCount; i++)
    {
        currentSmallHeapBlockCount += mediumHeapBuckets[i].Check();
        currentSmallHeapBlockCount += mediumHeapBuckets[i].GetEmptyHeapBlockCount();
    }

    currentSmallHeapBlockCount += Check(true, false, this->newLeafHeapBlockList);
    currentSmallHeapBlockCount += Check(true, false, this->newNormalHeapBlockList);
    currentSmallHeapBlockCount += Check(true, false, this->newNormalWithBarrierHeapBlockList);
    currentSmallHeapBlockCount += Check(true, false, this->newFinalizableWithBarrierHeapBlockList);
    currentSmallHeapBlockCount += Check(true, false, this->newFinalizableHeapBlockList);
#ifdef RECYCLER_VISITED_HOST
    currentSmallHeapBlockCount += Check(true, false, this->newRecyclerVisitedHostHeapBlockList);
    currentSmallHeapBlockCount += Check(true, false, this->newMediumRecyclerVisitedHostHeapBlockList);
#endif

    currentSmallHeapBlockCount += Check(true, false, this->newMediumLeafHeapBlockList);
    currentSmallHeapBlockCount += Check(true, false, this->newMediumNormalHeapBlockList);
    currentSmallHeapBlockCount += Check(true, false, this->newMediumNormalWithBarrierHeapBlockList);
    currentSmallHeapBlockCount += Check(true, false, this->newMediumFinalizableWithBarrierHeapBlockList);
    currentSmallHeapBlockCount += Check(true, false, this->newMediumFinalizableHeapBlockList);

    size_t expectedHeapBlockCount =
        this->heapBlockCount[HeapBlock::HeapBlockType::SmallNormalBlockType]
        + this->heapBlockCount[HeapBlock::HeapBlockType::SmallLeafBlockType]
        + this->heapBlockCount[HeapBlock::HeapBlockType::SmallFinalizableBlockType]
#ifdef RECYCLER_VISITED_HOST
        + this->heapBlockCount[HeapBlock::HeapBlockType::SmallRecyclerVisitedHostBlockType]
        + this->heapBlockCount[HeapBlock::HeapBlockType::MediumRecyclerVisitedHostBlockType]
#endif
        + this->heapBlockCount[HeapBlock::HeapBlockType::MediumNormalBlockType]
        + this->heapBlockCount[HeapBlock::HeapBlockType::MediumLeafBlockType]
        + this->heapBlockCount[HeapBlock::HeapBlockType::MediumFinalizableBlockType];

    expectedHeapBlockCount +=
        this->heapBlockCount[HeapBlock::HeapBlockType::SmallNormalBlockWithBarrierType]
        + this->heapBlockCount[HeapBlock::HeapBlockType::SmallFinalizableBlockWithBarrierType]
        + this->heapBlockCount[HeapBlock::HeapBlockType::MediumNormalBlockWithBarrierType]
        + this->heapBlockCount[HeapBlock::HeapBlockType::MediumFinalizableBlockWithBarrierType];


    Assert(currentSmallHeapBlockCount == expectedHeapBlockCount);

    currentLargeHeapBlockCount += largeObjectBucket.Check();

    Assert(currentLargeHeapBlockCount == this->heapBlockCount[HeapBlock::HeapBlockType::LargeBlockType]);
}

template <typename TBlockType>
size_t
HeapInfo::Check(bool expectFull, bool expectPending, TBlockType * list, TBlockType * tail)
{
    size_t heapBlockCount = 0;
    HeapBlockList::ForEach(list, tail, [&heapBlockCount, expectFull, expectPending](TBlockType * heapBlock)
    {
        heapBlock->Check(expectFull, expectPending);
        heapBlockCount++;
    });
    return heapBlockCount;
}

template size_t HeapInfo::Check<SmallNormalHeapBlock>(bool expectFull, bool expectPending, SmallNormalHeapBlock * list, SmallNormalHeapBlock * tail);
template size_t HeapInfo::Check<SmallLeafHeapBlock>(bool expectFull, bool expectPending, SmallLeafHeapBlock * list, SmallLeafHeapBlock * tail);
template size_t HeapInfo::Check<SmallFinalizableHeapBlock>(bool expectFull, bool expectPending, SmallFinalizableHeapBlock * list, SmallFinalizableHeapBlock * tail);
#ifdef RECYCLER_VISITED_HOST
template size_t HeapInfo::Check<SmallRecyclerVisitedHostHeapBlock>(bool expectFull, bool expectPending, SmallRecyclerVisitedHostHeapBlock * list, SmallRecyclerVisitedHostHeapBlock * tail);
template size_t HeapInfo::Check<MediumRecyclerVisitedHostHeapBlock>(bool expectFull, bool expectPending, MediumRecyclerVisitedHostHeapBlock * list, MediumRecyclerVisitedHostHeapBlock * tail);
#endif
template size_t HeapInfo::Check<LargeHeapBlock>(bool expectFull, bool expectPending, LargeHeapBlock * list, LargeHeapBlock * tail);
template size_t HeapInfo::Check<SmallNormalWithBarrierHeapBlock>(bool expectFull, bool expectPending, SmallNormalWithBarrierHeapBlock * list, SmallNormalWithBarrierHeapBlock * tail);
template size_t HeapInfo::Check<SmallFinalizableWithBarrierHeapBlock>(bool expectFull, bool expectPending, SmallFinalizableWithBarrierHeapBlock * list, SmallFinalizableWithBarrierHeapBlock * tail);

template size_t HeapInfo::Check<MediumNormalHeapBlock>(bool expectFull, bool expectPending, MediumNormalHeapBlock * list, MediumNormalHeapBlock * tail);
template size_t HeapInfo::Check<MediumLeafHeapBlock>(bool expectFull, bool expectPending, MediumLeafHeapBlock * list, MediumLeafHeapBlock * tail);
template size_t HeapInfo::Check<MediumFinalizableHeapBlock>(bool expectFull, bool expectPending, MediumFinalizableHeapBlock * list, MediumFinalizableHeapBlock * tail);
template size_t HeapInfo::Check<LargeHeapBlock>(bool expectFull, bool expectPending, LargeHeapBlock * list, LargeHeapBlock * tail);
template size_t HeapInfo::Check<MediumNormalWithBarrierHeapBlock>(bool expectFull, bool expectPending, MediumNormalWithBarrierHeapBlock * list, MediumNormalWithBarrierHeapBlock * tail);
template size_t HeapInfo::Check<MediumFinalizableWithBarrierHeapBlock>(bool expectFull, bool expectPending, MediumFinalizableWithBarrierHeapBlock * list, MediumFinalizableWithBarrierHeapBlock * tail);

void
HeapInfo::VerifySmallHeapBlockCount()
{
    GetSmallHeapBlockCount(true);
}
void
HeapInfo::VerifyLargeHeapBlockCount()
{
    GetLargeHeapBlockCount(true);
}
#endif

#ifdef RECYCLER_MEMORY_VERIFY
void
HeapInfo::Verify()
{
    Assert(!this->recycler->CollectionInProgress());
    for (uint i = 0; i < HeapConstants::BucketCount; i++)
    {
        heapBuckets[i].Verify();
    }

    for (uint i = 0; i < HeapConstants::MediumBucketCount; i++)
    {
        mediumHeapBuckets[i].Verify();
    }

    largeObjectBucket.Verify();

    HeapBlockList::ForEach(newLeafHeapBlockList, [](SmallLeafHeapBlock * heapBlock)
    {
        heapBlock->Verify();
    });
    HeapBlockList::ForEach(newNormalHeapBlockList, [](SmallNormalHeapBlock * heapBlock)
    {
        heapBlock->Verify();
    });
    HeapBlockList::ForEach(newNormalWithBarrierHeapBlockList, [](SmallNormalWithBarrierHeapBlock * heapBlock)
    {
        heapBlock->Verify();
    });
    HeapBlockList::ForEach(newFinalizableWithBarrierHeapBlockList, [](SmallFinalizableWithBarrierHeapBlock * heapBlock)
    {
        heapBlock->Verify();
    });
#ifdef RECYCLER_VISITED_HOST
    HeapBlockList::ForEach(newRecyclerVisitedHostHeapBlockList, [](SmallFinalizableHeapBlock * heapBlock)
    {
        heapBlock->Verify();
    });
#endif
    HeapBlockList::ForEach(newFinalizableHeapBlockList, [](SmallFinalizableHeapBlock * heapBlock)
    {
        heapBlock->Verify();
    });

    HeapBlockList::ForEach(newMediumLeafHeapBlockList, [](MediumLeafHeapBlock * heapBlock)
    {
        heapBlock->Verify();
    });
    HeapBlockList::ForEach(newMediumNormalHeapBlockList, [](MediumNormalHeapBlock * heapBlock)
    {
        heapBlock->Verify();
    });
    HeapBlockList::ForEach(newMediumNormalWithBarrierHeapBlockList, [](MediumNormalWithBarrierHeapBlock * heapBlock)
    {
        heapBlock->Verify();
    });
    HeapBlockList::ForEach(newMediumFinalizableWithBarrierHeapBlockList, [](MediumFinalizableWithBarrierHeapBlock * heapBlock)
    {
        heapBlock->Verify();
    });
#ifdef RECYCLER_VISITED_HOST
    HeapBlockList::ForEach(newMediumRecyclerVisitedHostHeapBlockList, [](MediumFinalizableHeapBlock * heapBlock)
    {
        heapBlock->Verify();
    });
#endif
    HeapBlockList::ForEach(newMediumFinalizableHeapBlockList, [](MediumFinalizableHeapBlock * heapBlock)
    {
        heapBlock->Verify();
    });
}
#endif

#ifdef RECYCLER_VERIFY_MARK
void
HeapInfo::VerifyMark()
{
    for (uint i = 0; i < HeapConstants::BucketCount; i++)
    {
        heapBuckets[i].VerifyMark();
    }

    for (uint i = 0; i < HeapConstants::MediumBucketCount; i++)
    {
        mediumHeapBuckets[i].VerifyMark();
    }

    largeObjectBucket.VerifyMark();

    HeapBlockList::ForEach(newLeafHeapBlockList, [](SmallLeafHeapBlock * heapBlock)
    {
        heapBlock->VerifyMark();
    });
    HeapBlockList::ForEach(newNormalHeapBlockList, [](SmallNormalHeapBlock * heapBlock)
    {
        heapBlock->VerifyMark();
    });
    HeapBlockList::ForEach(newNormalWithBarrierHeapBlockList, [](SmallNormalWithBarrierHeapBlock * heapBlock)
    {
        heapBlock->VerifyMark();
    });
    HeapBlockList::ForEach(newFinalizableWithBarrierHeapBlockList, [](SmallFinalizableWithBarrierHeapBlock * heapBlock)
    {
        heapBlock->VerifyMark();
    });
#ifdef RECYCLER_VISITED_HOST
    HeapBlockList::ForEach(newRecyclerVisitedHostHeapBlockList, [](SmallFinalizableHeapBlock * heapBlock)
    {
        heapBlock->VerifyMark();
    });
#endif
    HeapBlockList::ForEach(newFinalizableHeapBlockList, [](SmallFinalizableHeapBlock * heapBlock)
    {
        heapBlock->VerifyMark();
    });

    HeapBlockList::ForEach(newMediumLeafHeapBlockList, [](MediumLeafHeapBlock * heapBlock)
    {
        heapBlock->VerifyMark();
    });
    HeapBlockList::ForEach(newMediumNormalHeapBlockList, [](MediumNormalHeapBlock * heapBlock)
    {
        heapBlock->VerifyMark();
    });
    HeapBlockList::ForEach(newMediumNormalWithBarrierHeapBlockList, [](MediumNormalWithBarrierHeapBlock * heapBlock)
    {
        heapBlock->VerifyMark();
    });
    HeapBlockList::ForEach(newMediumFinalizableWithBarrierHeapBlockList, [](MediumFinalizableWithBarrierHeapBlock * heapBlock)
    {
        heapBlock->VerifyMark();
    });
#ifdef RECYCLER_VISITED_HOST
    HeapBlockList::ForEach(newMediumRecyclerVisitedHostHeapBlockList, [](MediumFinalizableHeapBlock * heapBlock)
    {
        heapBlock->VerifyMark();
    });
#endif
    HeapBlockList::ForEach(newMediumFinalizableHeapBlockList, [](MediumFinalizableHeapBlock * heapBlock)
    {
        heapBlock->VerifyMark();
    });
}
#endif

#ifdef RECYCLER_FINALIZE_CHECK
size_t
HeapInfo::GetFinalizeCount()
{
    return this->liveFinalizableObjectCount - this->newFinalizableObjectCount - this->pendingDisposableObjectCount;
}
#endif

#ifdef RECYCLER_MEMORY_VERIFY
void
HeapInfo::EnableVerify()
{
    ForEachPageAllocator([](IdleDecommitPageAllocator* pageAlloc)
    {
        pageAlloc->EnableVerify();
    });
}
#endif

#ifdef RECYCLER_NO_PAGE_REUSE
void
HeapInfo::DisablePageReuse()
{
    ForEachPageAllocator([](IdleDecommitPageAllocator* pageAlloc)
    {
        pageAlloc->DisablePageReuse();
    });
}
#endif
#if DBG
bool
HeapInfo::AllocatorsAreEmpty()
{
    for (uint i = 0; i < HeapConstants::BucketCount; i++)
    {
        if (!heapBuckets[i].AllocatorsAreEmpty())
        {
            return false;
        }
    }

    for (uint i = 0; i < HeapConstants::MediumBucketCount; i++)
    {
        if (!mediumHeapBuckets[i].AllocatorsAreEmpty())
        {
            return false;
        }
    }

    return true;
}
#endif

// ==============================================================
// Page allocator APIs
// ==============================================================
void
HeapInfo::Prime()
{
    ForEachPageAllocator([](IdleDecommitPageAllocator * pageAlloc)
    {
        pageAlloc->Prime(RecyclerPageAllocator::DefaultPrimePageCount);
    });
}

void
HeapInfo::CloseNonLeaf()
{
    ForEachNonLeafPageAllocator([](IdleDecommitPageAllocator * pageAlloc)
    {
        pageAlloc->Close();
    });
}

void
HeapInfo::SuspendIdleDecommitNonLeaf()
{
    ForEachNonLeafPageAllocator([](IdleDecommitPageAllocator * pageAlloc)
    {
        pageAlloc->SuspendIdleDecommit();
    });
}

void HeapInfo::ResumeIdleDecommitNonLeaf()
{
    ForEachNonLeafPageAllocator([](IdleDecommitPageAllocator * pageAlloc)
    {
        pageAlloc->ResumeIdleDecommit();
    });
}

void
HeapInfo::EnterIdleDecommit()
{
    ForEachPageAllocator([](IdleDecommitPageAllocator * pageAlloc)
    {
        pageAlloc->EnterIdleDecommit();
    });
}

IdleDecommitSignal
HeapInfo::LeaveIdleDecommit(bool allowTimer)
{
    IdleDecommitSignal idleDecommitSignal = IdleDecommitSignal_None;
    ForEachPageAllocator([&idleDecommitSignal, allowTimer](IdleDecommitPageAllocator * pageAlloc)
    {
        IdleDecommitSignal signal = pageAlloc->LeaveIdleDecommit(allowTimer);
        idleDecommitSignal = max(idleDecommitSignal, signal);
    });
    return idleDecommitSignal;
}

uint32_t HeapInfo::IdleDecommit()
{
    uint32_t waitTime = INFINITE;
    ForEachPageAllocator([&](IdleDecommitPageAllocator * pageAlloc)
    {
        uint32_t pageAllocatorWaitTime = pageAlloc->IdleDecommit();
        waitTime = min(waitTime, pageAllocatorWaitTime);
    });
    return waitTime;
}

#if DBG
void
HeapInfo::ShutdownIdleDecommit()
{
    ForEachPageAllocator([](IdleDecommitPageAllocator * pageAlloc)
    {
        pageAlloc->ShutdownIdleDecommit();
    });
}
#endif

void
HeapInfo::DecommitNow(bool all)
{
    ForEachPageAllocator([=](IdleDecommitPageAllocator * pageAlloc)
    {
        pageAlloc->DecommitNow(all);
    });
}

size_t
HeapInfo::GetUsedBytes()
{
    if (CONFIG_FLAG(ForceSoftwareWriteBarrier))
    {
        Assert(recyclerPageAllocator.usedBytes == 0);
    }

    size_t usedBytes = 0;
    ForEachPageAllocator([&](IdleDecommitPageAllocator * pageAlloc)
    {
        usedBytes += pageAlloc->usedBytes;
    });
    return usedBytes;
}

size_t
HeapInfo::GetReservedBytes()
{
    size_t reservedBytes = 0;
    ForEachPageAllocator([&](IdleDecommitPageAllocator * pageAlloc)
    {
        reservedBytes += pageAlloc->reservedBytes;
    });
    return reservedBytes;
}

size_t
HeapInfo::GetCommittedBytes()
{
    size_t committedBytes = 0;
    ForEachPageAllocator([&](IdleDecommitPageAllocator * pageAlloc)
    {
        committedBytes += pageAlloc->committedBytes;
    });
    return committedBytes;
}

size_t
HeapInfo::GetNumberOfSegments()
{
    size_t numberOfSegments = 0;
    ForEachPageAllocator([&](IdleDecommitPageAllocator * pageAlloc)
    {
        numberOfSegments += pageAlloc->numberOfSegments;
    });
    return numberOfSegments;
}

IdleDecommitPageAllocator*
HeapInfo::GetRecyclerPageAllocator()
{
    // TODO: SWB this is for Finalizable leaf allocation, which we didn't implement leaf bucket for it
    // remove this after the finalizable leaf bucket is implemented
    if (CONFIG_FLAG(ForceSoftwareWriteBarrier))
    {
        return &this->recyclerWithBarrierPageAllocator;
    }
    else
    {
#if !defined(RECYCLER_WRITE_BARRIER_ALLOC_SEPARATE_PAGE)
        return &this->recyclerPageAllocator;
#else
        return &this->recyclerWithBarrierPageAllocator;
#endif
    }
}

IdleDecommitPageAllocator*
HeapInfo::GetRecyclerLargeBlockPageAllocator()
{
    return &this->recyclerLargeBlockPageAllocator;
}

IdleDecommitPageAllocator*
HeapInfo::GetRecyclerLeafPageAllocator()
{
    return this->recyclerLeafPageAllocator;
}

#ifdef RECYCLER_WRITE_BARRIER_ALLOC_SEPARATE_PAGE
IdleDecommitPageAllocator*
HeapInfo::GetRecyclerWithBarrierPageAllocator()
{
    return &this->recyclerWithBarrierPageAllocator;
}
#endif

void
HeapInfo::StartQueueZeroPage()
{
    // Only queue up non-leaf pages- leaf pages don't need to be zeroed out
    ForEachNonLeafPageAllocator([](IdleDecommitPageAllocator * pageAlloc)
    {
        pageAlloc->StartQueueZeroPage();
    });
}

void
HeapInfo::StopQueueZeroPage()
{
    ForEachNonLeafPageAllocator([](IdleDecommitPageAllocator * pageAlloc)
    {
        pageAlloc->StopQueueZeroPage();
    });
}

void
HeapInfo::BackgroundZeroQueuedPages()
{
    ForEachNonLeafPageAllocator([](IdleDecommitPageAllocator * pageAlloc)
    {
        pageAlloc->BackgroundZeroQueuedPages();
    });
}

void
HeapInfo::ZeroQueuedPages()
{
    ForEachNonLeafPageAllocator([](IdleDecommitPageAllocator * pageAlloc)
    {
        pageAlloc->ZeroQueuedPages();
    });
}

void
HeapInfo::FlushBackgroundPages()
{
    ForEachPageAllocator([](IdleDecommitPageAllocator * pageAlloc)
    {
        pageAlloc->SuspendIdleDecommit();
        pageAlloc->FlushBackgroundPages();
        pageAlloc->ResumeIdleDecommit();
    });
}

#if DBG
bool HeapInfo::HasZeroQueuedPages()
{
    bool hasZeroQueuedPage = false;
    ForEachNonLeafPageAllocator([&](IdleDecommitPageAllocator * pageAlloc)
    {
        hasZeroQueuedPage = hasZeroQueuedPage || pageAlloc->HasZeroQueuedPages();
    });
    return hasZeroQueuedPage;
}
#endif

#if DBG
void
HeapInfo::ResetThreadId()
{
    ForEachPageAllocator([](IdleDecommitPageAllocator* pageAlloc)
    {
        pageAlloc->ClearConcurrentThreadId();
    });
}

void
HeapInfo::SetDisableThreadAccessCheck()
{
    ForEachNonLeafPageAllocator([](IdleDecommitPageAllocator* pageAlloc)
    {
        pageAlloc->SetDisableThreadAccessCheck();
    });
}
#endif


// Block attribute functions
/* static */
BOOL SmallAllocationBlockAttributes::IsAlignedObjectSize(size_t sizeCat)
{
    return HeapInfo::IsAlignedSmallObjectSize(sizeCat);
}

/* static */
BOOL MediumAllocationBlockAttributes::IsAlignedObjectSize(size_t sizeCat)
{
    return HeapInfo::IsAlignedMediumObjectSize(sizeCat);
}

namespace Memory
{
template class HeapInfo::ValidPointersMap<SmallAllocationBlockAttributes>;
template class ValidPointers<SmallAllocationBlockAttributes>;

template class HeapInfo::ValidPointersMap<MediumAllocationBlockAttributes>;
template class ValidPointers<MediumAllocationBlockAttributes>;
};

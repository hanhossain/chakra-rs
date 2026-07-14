//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "CommonMemoryPch.h"
#include "LargeHeapBucket.h"

//=====================================================================================================
// Initialization
//=====================================================================================================
LargeHeapBucket::~LargeHeapBucket()
{
    Recycler* recycler = this->heapInfo->recycler;
    HeapInfo* autoHeap = this->heapInfo;

    ForEachEditingLargeHeapBlock([recycler, autoHeap](LargeHeapBlock * heapBlock)
    {
        heapBlock->ReleasePagesShutdown(recycler);
        LargeHeapBlock::Delete(heapBlock);
        RECYCLER_SLOW_CHECK(autoHeap->heapBlockCount[HeapBlock::HeapBlockType::LargeBlockType]--);
    });
}

void
LargeHeapBucket::Initialize(HeapInfo * heapInfo, uint sizeCat, bool supportFreeList)
{
    this->heapInfo = heapInfo;
    this->sizeCat = sizeCat;
    this->isPageHeapEnabled = heapInfo->IsPageHeapEnabledForBlock<LargeAllocationBlockAttributes>(sizeCat);
    this->supportFreeList = supportFreeList;
}

//=====================================================================================================
// Allocation
//=====================================================================================================
char *
LargeHeapBucket::TryAllocFromNewHeapBlock(Recycler * recycler, size_t sizeCat, size_t size, ObjectInfoBits attributes, bool nothrow)
{
    Assert((attributes & InternalObjectInfoBitMask) == attributes);

    if (IsPageHeapEnabled(attributes))
    {
        return this->PageHeapAlloc(recycler, sizeCat, size, attributes, this->heapInfo->pageHeapMode, true);
    }

    LargeHeapBlock * heapBlock = AddLargeHeapBlock(sizeCat, nothrow);
    if (heapBlock == nullptr)
    {
        return nullptr;
    }
    char * memBlock = heapBlock->Alloc(sizeCat, attributes);
    Assert(memBlock != nullptr);
    return memBlock;
}

char *
LargeHeapBucket::SnailAlloc(Recycler * recycler, size_t sizeCat, size_t size, ObjectInfoBits attributes, bool nothrow)
{
    char * memBlock;

    Assert((attributes & InternalObjectInfoBitMask) == attributes);

    // No free memory, try to collect with allocated bytes and time heuristic, and concurrently
    BOOL collected = recycler->disableCollectOnAllocationHeuristics ? recycler->FinishConcurrent<FinishConcurrentOnAllocation>() :
        recycler->CollectNow<CollectOnAllocation>();

    if (!collected)
    {
        memBlock = TryAllocFromNewHeapBlock(recycler, sizeCat, size, attributes, nothrow);
        if (memBlock != nullptr)
        {
            return memBlock;
        }
        // Can't even allocate a new block, we need force a collection and
        // allocate some free memory, add a new heap block again, or throw out of memory
        AllocationVerboseTrace(recycler->GetRecyclerFlagsTable(), u"LargeHeapBucket::AddLargeHeapBlock failed, forcing in-thread collection\n");
        recycler->CollectNow<CollectNowForceInThread>();
    }

    memBlock = TryAlloc(recycler, sizeCat, attributes);
    if (memBlock != nullptr)
    {
        return memBlock;
    }

    memBlock = TryAllocFromNewHeapBlock(recycler, sizeCat, size, attributes, nothrow);
    if (memBlock != nullptr)
    {
        return memBlock;
    }

    if (nothrow == false)
    {
        // Can't add a heap block, we are out of memory
        // Since nothrow is false, we can throw right here
        recycler->OutOfMemory();
    }

    return nullptr;
}

char*
LargeHeapBucket::PageHeapAlloc(Recycler * recycler, size_t sizeCat, size_t size, ObjectInfoBits attributes, PageHeapMode mode, bool nothrow)
{
    auto throwOrReturn = [&](char* memBlock) ->char*
    {
        if (memBlock==nullptr && !nothrow)
        {
            recycler->OutOfMemory();
        }
        return memBlock;
    };

    PageHeapData* pageHeapData = NoMemProtectHeapNewNoThrowZ(PageHeapData);
    if (pageHeapData == nullptr)
    {
        return throwOrReturn(nullptr);
    }


    if (size < sizeof(void*))
    {
        attributes = static_cast<ObjectInfoBits>(attributes | LeafBit);
    }

#ifdef RECYCLER_MEMORY_VERIFY
    if (recycler->VerifyEnabled())
    {
        // with recycler verify enabled, it uses the unaligned bytes
        size = sizeCat;
    }
#endif

    size_t pageCount = LargeHeapBlock::GetPagesNeeded(size, false);
    Assert(pageCount != 0);
    size_t actualPageCount = pageCount + 1; // 1 for guard page
    auto pageAllocator = heapInfo->GetRecyclerLargeBlockPageAllocator();
    Segment * segment;
    char * baseAddress = pageAllocator->Alloc(&actualPageCount, &segment);
    Assert(reinterpret_cast<size_t>(baseAddress)%AutoSystemInfo::PageSize == 0);
    if (baseAddress == nullptr)
    {
        NoMemProtectHeapDelete(pageHeapData);
        return throwOrReturn(nullptr);
    }

    size_t guardPageCount = actualPageCount - pageCount; // pageAllocator can return more than asked pages
    char* headerAddress = nullptr;
    char* guardPageAddress = nullptr;
    size_t sizeWithHeader = size + sizeof(LargeObjectHeader);
    pageHeapData->pageHeapMode = heapInfo->pageHeapMode;
    if (pageHeapData->pageHeapMode == PageHeapMode::PageHeapModeBlockStart)
    {
        headerAddress = baseAddress + AutoSystemInfo::PageSize * guardPageCount;
        guardPageAddress = baseAddress;

        pageHeapData->paddingBytes = 0;
        pageHeapData->unusedBytes = (AutoSystemInfo::PageSize - (sizeWithHeader%AutoSystemInfo::PageSize)) % AutoSystemInfo::PageSize;

        pageHeapData->objectPageAddr = headerAddress;
    }
    else if (pageHeapData->pageHeapMode == PageHeapMode::PageHeapModeBlockEnd)
    {
        pageHeapData->unusedBytes = (HeapConstants::ObjectGranularity - (sizeWithHeader%HeapConstants::ObjectGranularity)) % HeapConstants::ObjectGranularity;
        pageHeapData->paddingBytes = static_cast<ushort>((AutoSystemInfo::PageSize - pageHeapData->unusedBytes - sizeWithHeader % AutoSystemInfo::PageSize) %
            AutoSystemInfo::PageSize);
        Assert(pageHeapData->paddingBytes%HeapConstants::ObjectGranularity == 0);

        headerAddress = baseAddress + pageHeapData->paddingBytes;
        guardPageAddress = baseAddress + pageCount * AutoSystemInfo::PageSize;

        pageHeapData->objectPageAddr = baseAddress;
    }
    else
    {
        AnalysisAssert(false);
    }

    LargeHeapBlock * heapBlock = LargeHeapBlock::New(headerAddress, pageCount, segment, 1, this);
    if (!heapBlock)
    {
        pageAllocator->SuspendIdleDecommit();
        pageAllocator->Release(baseAddress, actualPageCount, segment);
        pageAllocator->ResumeIdleDecommit();
        NoMemProtectHeapDelete(pageHeapData);
        return throwOrReturn(nullptr);
    }


    // adjust the addressEnd
    if (pageHeapData->pageHeapMode == PageHeapMode::PageHeapModeBlockStart)
    {
        heapBlock->addressEnd = baseAddress + AutoSystemInfo::PageSize * actualPageCount;
    }
    else
    {
        heapBlock->addressEnd = baseAddress + AutoSystemInfo::PageSize * pageCount;
    }

    pageHeapData->actualPageCount = static_cast<uint>(actualPageCount);
    pageHeapData->guardPageAddress = guardPageAddress;

    heapBlock->heapInfo = this->heapInfo;
    heapBlock->pageHeapData = pageHeapData;
    
    bool decommitGuardPage = true;
    decommitGuardPage = this->GetRecycler()->GetRecyclerFlagsTable().PageHeapDecommitGuardPage;
#if defined(RECYCLER_NO_PAGE_REUSE)
    decommitGuardPage |= heapBlock->GetPageAllocator(heapInfo)->IsPageReuseDisabled();
#endif
    if (decommitGuardPage)
    {
#pragma prefast(suppress:6250, "Calling 'VirtualFree' without the MEM_RELEASE flag might free memory but not address descriptors (VADs).")
        if (VirtualFree(guardPageAddress, AutoSystemInfo::PageSize * guardPageCount, MEM_DECOMMIT))
        {
            pageHeapData->isGuardPageDecommitted = true;
        }
        else
        {
            Js::Throw::FatalInternalError();
        }
    }
    else
    {
        uint32_t oldProtect;
        if (VirtualProtect(guardPageAddress, AutoSystemInfo::PageSize * guardPageCount, PAGE_NOACCESS, &oldProtect))
        {
            pageHeapData->isGuardPageDecommitted = false;
        }
        else
        {
            Js::Throw::FatalInternalError();
        }        
    }

    pageHeapData->objectAddress = heapBlock->Alloc(size, attributes);
    Assert(pageHeapData->objectAddress != nullptr);
    if (!pageHeapData->objectAddress)
    {
        pageAllocator->SuspendIdleDecommit();
        heapBlock->ReleasePages(recycler);
        pageAllocator->ResumeIdleDecommit();
        LargeHeapBlock::Delete(heapBlock);
        return throwOrReturn(nullptr);
    }

    pageHeapData->objectEndAddr = pageHeapData->objectAddress + size;

    // fill pattern before set pageHeapMode, so background scan stack may verify the pattern
    memset(pageHeapData->objectPageAddr, PageHeapMemFill, pageHeapData->paddingBytes);
    memset(pageHeapData->objectAddress + size, PageHeapMemFill, pageHeapData->unusedBytes);

    if (!recycler->heapBlockMap.SetHeapBlock(headerAddress, pageCount, heapBlock, HeapBlock::HeapBlockType::LargeBlockType, 0))
    {
        pageAllocator->SuspendIdleDecommit();
        heapBlock->ReleasePages(recycler);
        pageAllocator->ResumeIdleDecommit();
        LargeHeapBlock::Delete(heapBlock);
        return throwOrReturn(nullptr);
    }

    heapBlock->SetNextBlock(this->largePageHeapBlockList);
    this->largePageHeapBlockList = heapBlock;

    recycler->autoHeap.uncollectedNewPageCount += pageCount;

    RECYCLER_SLOW_CHECK(this->heapInfo->heapBlockCount[HeapBlock::HeapBlockType::LargeBlockType]++);
    RECYCLER_PERF_COUNTER_ADD(FreeObjectSize, heapBlock->GetPageCount() * AutoSystemInfo::PageSize);

#ifdef STACK_BACK_TRACE
    if (recycler->ShouldCapturePageHeapAllocStack())
    {
        heapBlock->CapturePageHeapAllocStack();
    }
#endif

    return throwOrReturn(pageHeapData->objectAddress);
}

LargeHeapBlock*
LargeHeapBucket::AddLargeHeapBlock(size_t size, bool nothrow)
{
    Recycler* recycler = this->heapInfo->recycler;
    Segment * segment;
    size_t pageCount = LargeHeapBlock::GetPagesNeeded(size, this->supportFreeList);
    if (pageCount == 0)
    {
        if (nothrow == false)
        {
            // overflow
            // Since nothrow is false here, it's okay to throw
            recycler->OutOfMemory();
        }

        return nullptr;
    }

    char * address = nullptr;

    size_t realPageCount = pageCount;
    address = heapInfo->GetRecyclerLargeBlockPageAllocator()->Alloc(&realPageCount, &segment);
    pageCount = realPageCount;

    if (address == nullptr)
    {
        return nullptr;
    }
#ifdef RECYCLER_ZERO_MEM_CHECK
    recycler->VerifyZeroFill(address, pageCount * AutoSystemInfo::PageSize);
#endif
    uint objectCount = LargeHeapBlock::GetMaxLargeObjectCount(pageCount, size);
    LargeHeapBlock * heapBlock = LargeHeapBlock::New(address, pageCount, segment, objectCount, this);
#if DBG
    LargeAllocationVerboseTrace(recycler->GetRecyclerFlagsTable(), u"Allocated new large heap block 0x%p for sizeCat 0x%x\n", heapBlock, sizeCat);
#endif

    if (!heapBlock)
    {       
        heapInfo->GetRecyclerLargeBlockPageAllocator()->SuspendIdleDecommit();
        heapInfo->GetRecyclerLargeBlockPageAllocator()->Release(address, pageCount, segment);
        heapInfo->GetRecyclerLargeBlockPageAllocator()->ResumeIdleDecommit();
        return nullptr;
    }
    recycler->autoHeap.uncollectedNewPageCount += pageCount;

    RECYCLER_SLOW_CHECK(this->heapInfo->heapBlockCount[HeapBlock::HeapBlockType::LargeBlockType]++);

    heapBlock->heapInfo = this->heapInfo;

    heapBlock->lastCollectAllocCount = 0;

    Assert(recycler->collectionState != CollectionStateMark);

    if (!recycler->heapBlockMap.SetHeapBlock(address, pageCount, heapBlock, HeapBlock::HeapBlockType::LargeBlockType, 0))
    {
        heapInfo->GetRecyclerLargeBlockPageAllocator()->SuspendIdleDecommit();
        heapBlock->ReleasePages(recycler);
        heapInfo->GetRecyclerLargeBlockPageAllocator()->ResumeIdleDecommit();
        LargeHeapBlock::Delete(heapBlock);
        RECYCLER_SLOW_CHECK(this->heapInfo->heapBlockCount[HeapBlock::HeapBlockType::LargeBlockType]--);
        return nullptr;
    }

    heapBlock->SetNextBlock(this->largeBlockList);
    this->largeBlockList = heapBlock;

    RECYCLER_PERF_COUNTER_ADD(FreeObjectSize, heapBlock->GetPageCount() * AutoSystemInfo::PageSize);
    return heapBlock;
}

char *
LargeHeapBucket::TryAllocFromFreeList(Recycler * recycler, size_t sizeCat, ObjectInfoBits attributes)
{
    Assert((attributes & InternalObjectInfoBitMask) == attributes);

    LargeHeapBlockFreeList* freeListEntry = this->freeList;

    // Walk through the free list, find the first entry that can fit our desired size
    while (freeListEntry)
    {
        LargeHeapBlock* heapBlock = freeListEntry->heapBlock;

        char * memBlock = heapBlock->TryAllocFromFreeList(sizeCat, attributes);
        if (memBlock)
        {
            // Don't need to verify zero fill here since we will do it in LargeHeapBucket::Alloc
            return memBlock;
        }
        else
        {
#if DBG
            LargeAllocationVerboseTrace(recycler->GetRecyclerFlagsTable(), u"Unable to allocate object of size 0x%x from freelist\n", sizeCat);
#endif
        }

        freeListEntry = freeListEntry->next;
    }
    return nullptr;
}

char *
LargeHeapBucket::TryAllocFromExplicitFreeList(Recycler * recycler, size_t sizeCat, ObjectInfoBits attributes)
{
    Assert((attributes & InternalObjectInfoBitMask) == attributes);

    FreeObject * currFreeObject = this->explicitFreeList;
    FreeObject * prevFreeObject = nullptr;
    while (currFreeObject != nullptr)
    {
        char * memBlock = reinterpret_cast<char*>(currFreeObject);
        LargeObjectHeader * header = LargeHeapBlock::GetHeaderFromAddress(memBlock);
        Assert(header->isExplicitFreed);
        Assert(HeapInfo::GetMediumObjectAlignedSizeNoCheck(header->objectSize) == this->sizeCat);
        if (header->objectSize < sizeCat)
        {
            prevFreeObject = currFreeObject;
            currFreeObject = currFreeObject->GetNext();
            continue;
        }

        DebugOnly(header->isExplicitFreed = false);
        if (prevFreeObject)
        {
            prevFreeObject->SetNext(currFreeObject->GetNext());
        }
        else
        {
            this->explicitFreeList = currFreeObject->GetNext();
        }

#ifdef RECYCLER_MEMORY_VERIFY
        HeapBlock* heapBlockVerify = recycler->FindHeapBlock(memBlock);
        Assert(heapBlockVerify != nullptr);
        Assert(heapBlockVerify->IsLargeHeapBlock());
        LargeHeapBlock * largeHeapBlock = static_cast<LargeHeapBlock*>(heapBlockVerify);
        LargeObjectHeader * dbgHeader;
        Assert(largeHeapBlock->GetObjectHeader(memBlock, &dbgHeader));
        Assert(dbgHeader == header);

        reinterpret_cast<FreeObject*>(memBlock)->DebugFillNext();
#endif
#ifdef RECYCLER_ZERO_MEM_CHECK
        // TODO: large heap block doesn't separate leaf object on to different page allocator.
        // so all the memory should still be zeroed.
        memset(memBlock, 0, sizeof(FreeObject));
#endif
        header->SetAttributes(recycler->Cookie, (attributes & StoredObjectInfoBitMask));

        if ((attributes & ObjectInfoBits::FinalizeBit) != 0)
        {
            LargeHeapBlock* heapBlock = static_cast<LargeHeapBlock*>(recycler->FindHeapBlock(memBlock));
            heapBlock->finalizeCount++;
#ifdef RECYCLER_FINALIZE_CHECK
            heapInfo->liveFinalizableObjectCount++;
            heapInfo->newFinalizableObjectCount++;
#endif
        }
        return memBlock;
    }

    return nullptr;
}
//=====================================================================================================
// Free
//=====================================================================================================
void
LargeHeapBucket::ExplicitFree(void * object, size_t sizeCat)
{
    Assert(HeapInfo::GetMediumObjectAlignedSizeNoCheck(sizeCat) == this->sizeCat);
    LargeObjectHeader * header = LargeHeapBlock::GetHeaderFromAddress(object);
    Assert(header->GetAttributes(this->heapInfo->recycler->Cookie) == ObjectInfoBits::NoBit || header->GetAttributes(this->heapInfo->recycler->Cookie) == ObjectInfoBits::LeafBit);
    Assert(!header->isExplicitFreed);
    DebugOnly(header->isExplicitFreed = true);
    Assert(header->objectSize >= sizeCat);

#if DBG
    HeapBlock* heapBlock = this->GetRecycler()->FindHeapBlock(object);
    Assert(heapBlock != nullptr);
    Assert(heapBlock->IsLargeHeapBlock());

    LargeHeapBlock * largeHeapBlock = static_cast<LargeHeapBlock*>(heapBlock);
    LargeObjectHeader * dbgHeader;
    Assert(largeHeapBlock->GetObjectHeader(object, &dbgHeader));
    Assert(dbgHeader == header);
#endif

    FreeObject * freeObject = static_cast<FreeObject*>(object);
    freeObject->SetNext(this->explicitFreeList);
    this->explicitFreeList = freeObject;
    header->SetAttributes(this->heapInfo->recycler->Cookie, ObjectInfoBits::LeafBit);       // We can stop scanning it now.


}

//=====================================================================================================
// Collections
//=====================================================================================================
void
LargeHeapBucket::ResetMarks(ResetMarkFlags flags)
{
    Recycler* recycler = this->heapInfo->recycler;

    HeapBlockList::ForEach(largeBlockList, [flags, recycler](LargeHeapBlock * heapBlock)
    {
        heapBlock->ResetMarks(flags, recycler);
    });
    HeapBlockList::ForEach(largePageHeapBlockList, [flags, recycler](LargeHeapBlock * heapBlock)
    {
        heapBlock->ResetMarks(flags, recycler);
    });
    HeapBlockList::ForEach(fullLargeBlockList, [flags, recycler](LargeHeapBlock * heapBlock)
    {
        heapBlock->ResetMarks(flags, recycler);
    });
    HeapBlockList::ForEach(pendingDisposeLargeBlockList, [flags, recycler](LargeHeapBlock * heapBlock)
    {
        heapBlock->ResetMarks(flags, recycler);
    });
    Assert(pendingSweepLargeBlockList == nullptr);
}

void
LargeHeapBucket::ScanInitialImplicitRoots(Recycler * recycler)
{
    HeapBlockList::ForEach(largeBlockList, [recycler](LargeHeapBlock * heapBlock)
    {
        heapBlock->ScanInitialImplicitRoots(recycler);
    });
    HeapBlockList::ForEach(largePageHeapBlockList, [recycler](LargeHeapBlock * heapBlock)
    {
        heapBlock->ScanInitialImplicitRoots(recycler);
    });
    HeapBlockList::ForEach(fullLargeBlockList, [recycler](LargeHeapBlock * heapBlock)
    {
        heapBlock->ScanInitialImplicitRoots(recycler);
    });
    HeapBlockList::ForEach(pendingDisposeLargeBlockList, [recycler](LargeHeapBlock * heapBlock)
    {
        heapBlock->ScanInitialImplicitRoots(recycler);
    });
    Assert(pendingSweepLargeBlockList == nullptr);
}

void
LargeHeapBucket::ScanNewImplicitRoots(Recycler * recycler)
{
    HeapBlockList::ForEach(largeBlockList, [recycler](LargeHeapBlock * heapBlock)
    {
        heapBlock->ScanNewImplicitRoots(recycler);
    });
    HeapBlockList::ForEach(largePageHeapBlockList, [recycler](LargeHeapBlock * heapBlock)
    {
        heapBlock->ScanNewImplicitRoots(recycler);
    });
    HeapBlockList::ForEach(fullLargeBlockList, [recycler](LargeHeapBlock * heapBlock)
    {
        heapBlock->ScanNewImplicitRoots(recycler);
    });
    HeapBlockList::ForEach(pendingDisposeLargeBlockList, [recycler](LargeHeapBlock * heapBlock)
    {
        heapBlock->ScanNewImplicitRoots(recycler);
    });
    Assert(pendingSweepLargeBlockList == nullptr);
}

//=====================================================================================================
// Sweep
//=====================================================================================================
#pragma region Sweep

void
LargeHeapBucket::Sweep(RecyclerSweep& recyclerSweep)
{
    // CONCURRENT-TODO: large buckets are not swept in the background currently.
    Assert(!recyclerSweep.GetRecycler()->IsConcurrentExecutingState());

    LargeHeapBlock * currentLargeObjectBlocks = largeBlockList;
    LargeHeapBlock * currentLargePageHeapObjectBlocks = largePageHeapBlockList;
    LargeHeapBlock * currentFullLargeObjectBlocks = fullLargeBlockList;
    LargeHeapBlock * currentDisposeLargeBlockList = pendingDisposeLargeBlockList;
    this->largeBlockList = nullptr;
    this->largePageHeapBlockList = nullptr;
    this->fullLargeBlockList = nullptr;

    // Clear the free list before sweep
    // We'll reconstruct the free list during sweep
    if (this->supportFreeList)
    {
#if DBG
        LargeAllocationVerboseTrace(recyclerSweep.GetRecycler()->GetRecyclerFlagsTable(), u"Resetting free list for 0x%x bucket\n", this->sizeCat);
#endif
        this->freeList = nullptr;
        this->explicitFreeList = nullptr;
    }

    Assert(this->pendingSweepLargeBlockList == nullptr);
    SweepLargeHeapBlockList(recyclerSweep, currentLargeObjectBlocks);
    SweepLargeHeapBlockList(recyclerSweep, currentLargePageHeapObjectBlocks);
    SweepLargeHeapBlockList(recyclerSweep, currentFullLargeObjectBlocks);
    SweepLargeHeapBlockList(recyclerSweep, currentDisposeLargeBlockList);
}

void
LargeHeapBucket::SweepLargeHeapBlockList(RecyclerSweep& recyclerSweep, LargeHeapBlock * heapBlockList)
{
    Recycler * recycler = recyclerSweep.GetRecycler();
    HeapBlockList::ForEachEditing(heapBlockList, [this, &recyclerSweep, recycler](LargeHeapBlock * heapBlock)
    {
        this->UnregisterFreeList(heapBlock->GetFreeList());

        // CONCURRENT-TODO: Allow large block to be sweep in the background
        SweepState state = heapBlock->Sweep(recyclerSweep, false);

        // If the block is already in the pending dispose list (re-entrant GC scenario), do nothing, leave it there
        if (heapBlock->IsInPendingDisposeList()) return;

        switch (state)
        {
        case SweepStateEmpty:
            heapBlock->ReleasePagesSweep(recycler);
            LargeHeapBlock::Delete(heapBlock);
            RECYCLER_SLOW_CHECK(this->heapInfo->heapBlockCount[HeapBlock::HeapBlockType::LargeBlockType]--);
            break;
        case SweepStateFull:
            heapBlock->SetNextBlock(this->fullLargeBlockList);
            this->fullLargeBlockList = heapBlock;
            break;
        case SweepStateSwept:
            if (supportFreeList)
            {
                ConstructFreelist(heapBlock);
            }
            else
            {
                ReinsertLargeHeapBlock(heapBlock);
            }

            break;
        case SweepStatePendingDispose:
            Assert(!recyclerSweep.IsBackground());
            Assert(!this->heapInfo->hasPendingTransferDisposedObjects);
            heapBlock->SetNextBlock(this->pendingDisposeLargeBlockList);
            this->pendingDisposeLargeBlockList = heapBlock;
            heapBlock->SetIsInPendingDisposeList(true);
#if DBG
            heapBlock->SetHasDisposeBeenCalled(false);
#endif
            recycler->hasDisposableObject = true;
            break;
        case SweepStatePendingSweep:
            heapBlock->SetNextBlock(this->pendingSweepLargeBlockList);
            this->pendingSweepLargeBlockList = heapBlock;
            break;
        }
    });
}

void
LargeHeapBucket::ReinsertLargeHeapBlock(LargeHeapBlock * heapBlock)
{
    Assert(!heapBlock->hasPartialFreeObjects);
    Assert(!heapBlock->IsInPendingDisposeList());

    if (this->largeBlockList != nullptr && heapBlock->GetFreeSize() > this->largeBlockList->GetFreeSize())
    {
        heapBlock->SetNextBlock(this->largeBlockList->GetNextBlock());
        this->largeBlockList->SetNextBlock(this->fullLargeBlockList);
        this->fullLargeBlockList = this->largeBlockList;
        this->largeBlockList = heapBlock;
    }
    else
    {
        heapBlock->SetNextBlock(this->fullLargeBlockList);
        this->fullLargeBlockList = heapBlock;
    }
}

void
LargeHeapBucket::RegisterFreeList(LargeHeapBlockFreeList* freeList)
{
    Assert(freeList->next == nullptr);
    Assert(freeList->previous == nullptr);

    LargeHeapBlockFreeList* head = this->freeList;

    if (head)
    {
        head->previous = freeList;
    }

    freeList->next = head;
    this->freeList = freeList;
}

void
LargeHeapBucket::UnregisterFreeList(LargeHeapBlockFreeList* freeList)
{
    LargeHeapBlockFreeList* next = freeList->next;
    LargeHeapBlockFreeList* previous = freeList->previous;

    if (previous)
    {
        previous->next = next;
    }

    if (next)
    {
        next->previous = previous;
    }

    freeList->next = nullptr;
    freeList->previous = nullptr;

    if (freeList == this->freeList)
    {
        this->freeList = next;
    }
}

void
LargeHeapBucket::ConstructFreelist(LargeHeapBlock * heapBlock)
{
    Assert(!heapBlock->hasPartialFreeObjects);
    Assert(!heapBlock->IsInPendingDisposeList());

    // The free list is the only way we reuse heap block entries
    // so if the heap block is allocated from directly, it'll not
    // invalidate the free list
    LargeHeapBlockFreeList* freeList = heapBlock->GetFreeList();
    Assert(freeList);

    if (freeList->entries)
    {
        this->RegisterFreeList(freeList);

#if DBG
        LargeAllocationVerboseTrace(this->GetRecycler()->GetRecyclerFlagsTable(), u"Free list created for 0x%x bucket\n", this->sizeCat);
#endif
    }

    ReinsertLargeHeapBlock(heapBlock);
}

#pragma endregion

size_t
LargeHeapBucket::Rescan(LargeHeapBlock * list, Recycler * recycler, bool isPartialSwept, RescanFlags flags)
{
    size_t scannedPageCount = 0;
    HeapBlockList::ForEach(list, [recycler, isPartialSwept, flags, &scannedPageCount](LargeHeapBlock * heapBlock)
    {
        scannedPageCount += heapBlock->Rescan(recycler, isPartialSwept, flags);
    });
    return scannedPageCount;
}

size_t
LargeHeapBucket::Rescan(RescanFlags flags)
{
    Assert(pendingSweepLargeBlockList == nullptr);

    size_t scannedPageCount = 0;
    Recycler* recycler = this->heapInfo->recycler;

    scannedPageCount += LargeHeapBucket::Rescan(largeBlockList, recycler, false, flags);
    scannedPageCount += LargeHeapBucket::Rescan(largePageHeapBlockList, recycler, false, flags);
    scannedPageCount += LargeHeapBucket::Rescan(fullLargeBlockList, recycler, false, flags);
    scannedPageCount += LargeHeapBucket::Rescan(pendingDisposeLargeBlockList, recycler, true, flags);

    Assert(recycler->inPartialCollectMode || partialSweptLargeBlockList == nullptr);
    if (recycler->inPartialCollectMode)
    {
        scannedPageCount += LargeHeapBucket::Rescan(partialSweptLargeBlockList, recycler, true, flags);
    }
    return scannedPageCount;
}

void
LargeHeapBucket::SweepPendingObjects(RecyclerSweep& recyclerSweep)
{
    if (recyclerSweep.IsBackground())
    {
        Recycler * recycler = recyclerSweep.GetRecycler();
        if (recycler->inPartialCollectMode)
        {
            HeapBlockList::ForEach(this->pendingSweepLargeBlockList, [recycler](LargeHeapBlock * heapBlock)
            {
                // Page heap blocks are never swept concurrently
                heapBlock->SweepObjects<SweepMode_ConcurrentPartial>(recycler);
            });
        }
        else
        {
            HeapBlockList::ForEach(this->pendingSweepLargeBlockList, [recycler](LargeHeapBlock * heapBlock)
            {
                // Page heap blocks are never swept concurrently
                heapBlock->SweepObjects<SweepMode_Concurrent>(recycler);
            });
        }
    }
    else
    {
        Assert(this->pendingSweepLargeBlockList == nullptr);
    }
}

void
LargeHeapBucket::ConcurrentPartialTransferSweptObjects(RecyclerSweep& recyclerSweep)
{
    Assert(recyclerSweep.InPartialCollectMode());
    Assert(!recyclerSweep.IsBackground());

    RECYCLER_SLOW_CHECK(this->VerifyLargeHeapBlockCount());

    LargeHeapBlock * list = this->pendingSweepLargeBlockList;
    this->pendingSweepLargeBlockList = nullptr;
    HeapBlockList::ForEachEditing(list, [this](LargeHeapBlock * heapBlock)
    {
        // GC-REVIEW: We could maybe reuse the large objects
        heapBlock->PartialTransferSweptObjects();
        heapBlock->SetNextBlock(this->partialSweptLargeBlockList);
        this->partialSweptLargeBlockList = heapBlock;
    });

    RECYCLER_SLOW_CHECK(this->VerifyLargeHeapBlockCount());
}

void
LargeHeapBucket::FinishPartialCollect(RecyclerSweep * recyclerSweep)
{
    Recycler* recycler = this->heapInfo->recycler;

    if (recyclerSweep && recyclerSweep->IsBackground())
    {
        // Leave it in the partialSweptLargeBlockList if we are processing it in the background
        // ConcurrentTransferSweptObjects will put it back.
        HeapBlockList::ForEachEditing(partialSweptLargeBlockList, [recycler](LargeHeapBlock * heapBlock)
        {
            heapBlock->FinishPartialCollect(recycler);
        });
    }
    else
    {
        HeapBlockList::ForEachEditing(partialSweptLargeBlockList, [this, recycler](LargeHeapBlock * heapBlock)
        {
            heapBlock->FinishPartialCollect(recycler);
            this->ReinsertLargeHeapBlock(heapBlock);
        });
        this->partialSweptLargeBlockList = nullptr;
    }
}

void
LargeHeapBucket::ConcurrentTransferSweptObjects(RecyclerSweep& recyclerSweep)
{
    Assert(!recyclerSweep.InPartialCollectMode());
    Assert(!recyclerSweep.IsBackground());

    HeapBlockList::ForEachEditing(this->pendingSweepLargeBlockList, [this](LargeHeapBlock * heapBlock)
    {
        heapBlock->TransferSweptObjects();
        ReinsertLargeHeapBlock(heapBlock);
    });
    this->pendingSweepLargeBlockList = nullptr;

    // If we did a background finish partial collect, we have left the partialSweptLargeBlockList
    // there because can't reinsert the heap block in the background, do it here now.
    HeapBlockList::ForEachEditing(this->partialSweptLargeBlockList, [this](LargeHeapBlock * heapBlock)
    {
        ReinsertLargeHeapBlock(heapBlock);
    });
    this->partialSweptLargeBlockList = nullptr;
}



void
LargeHeapBucket::FinalizeAllObjects()
{
    ForEachLargeHeapBlock([](LargeHeapBlock * heapBlock) { heapBlock->FinalizeAllObjects(); });
}


void
LargeHeapBucket::Finalize(Recycler * recycler, LargeHeapBlock * heapBlockList)
{
    HeapBlockList::ForEachEditing(heapBlockList, [recycler](LargeHeapBlock * heapBlock)
    {
        heapBlock->FinalizeObjects(recycler);
    });
}

void
LargeHeapBucket::Finalize()
{
    Recycler* recycler = this->heapInfo->recycler;

    // Finalize any free objects in the non-filled large heap blocks
    Finalize(recycler, largeBlockList);

    Finalize(recycler, largePageHeapBlockList);

    // Finalize any free objects in the filled large heap blocks
    Finalize(recycler, fullLargeBlockList);

    // Finalize any free objects in the large heap blocks which have objects pending dispose
    // This is to handle the case where if during dispose, a GC is triggered, we might have
    // found more objects to free. These objects might reside in a block that was moved to the
    // pendingDisposeLargeBlockList during the outer GC. So we need to walk through this list
    // again and finalize any objects that need to be finalized. If we don't, they would
    // not get finalized
    Finalize(recycler, pendingDisposeLargeBlockList);
}

void
LargeHeapBucket::DisposeObjects()
{
    Recycler * recycler = this->heapInfo->recycler;
    HeapBlockList::ForEach(this->pendingDisposeLargeBlockList, [recycler](LargeHeapBlock * heapBlock)
    {
        heapBlock->DisposeObjects(recycler);
    });
}

void
LargeHeapBucket::TransferDisposedObjects()
{
#if defined(DBG)
    Recycler * recycler = this->heapInfo->recycler;
    Assert(!recycler->IsConcurrentExecutingState());
#endif
    HeapBlockList::ForEachEditing(this->pendingDisposeLargeBlockList, [this](LargeHeapBlock * heapBlock)
    {
        /* GC-TODO: large heap block doesn't support free list yet */
        heapBlock->SetIsInPendingDisposeList(false);
        ReinsertLargeHeapBlock(heapBlock);
    });

    this->pendingDisposeLargeBlockList = nullptr;
}

void
LargeHeapBucket::EnumerateObjects(ObjectInfoBits infoBits, void (*CallBackFunction)(void * address, size_t size))
{
    HeapBucket::EnumerateObjects(largeBlockList, infoBits, CallBackFunction);
    HeapBucket::EnumerateObjects(largePageHeapBlockList, infoBits, CallBackFunction);
    HeapBucket::EnumerateObjects(fullLargeBlockList, infoBits, CallBackFunction);

    // Pending dispose large block list need not be null
    // When we enumerate over this list, anything that has been swept/finalized won't be
    // enumerated since it needs to have the object header for enumeration
    // and we set the header to null upon sweep/finalize
    HeapBucket::EnumerateObjects(pendingDisposeLargeBlockList, infoBits, CallBackFunction);
    Assert(this->pendingSweepLargeBlockList == nullptr);
    HeapBucket::EnumerateObjects(partialSweptLargeBlockList, infoBits, CallBackFunction);
}

#if DBG || defined(RECYCLER_SLOW_CHECK_ENABLED)

size_t
LargeHeapBucket::GetLargeHeapBlockCount(bool checkCount) const
{
    size_t currentLargeHeapBlockCount = HeapBlockList::Count(fullLargeBlockList);
    currentLargeHeapBlockCount += HeapBlockList::Count(largeBlockList);
    currentLargeHeapBlockCount += HeapBlockList::Count(largePageHeapBlockList);
    currentLargeHeapBlockCount += HeapBlockList::Count(pendingDisposeLargeBlockList);
    currentLargeHeapBlockCount += HeapBlockList::Count(pendingSweepLargeBlockList);
    currentLargeHeapBlockCount += HeapBlockList::Count(partialSweptLargeBlockList);

    return currentLargeHeapBlockCount;
}
#endif

#ifdef RECYCLER_SLOW_CHECK_ENABLED
size_t
LargeHeapBucket::Check()
{
    size_t currentLargeHeapBlockCount = Check(false, false, largeBlockList);
    currentLargeHeapBlockCount += Check(true, false, largePageHeapBlockList);
    currentLargeHeapBlockCount += Check(true, false, fullLargeBlockList);

    Assert(pendingSweepLargeBlockList == nullptr);
    currentLargeHeapBlockCount += Check(false, false, partialSweptLargeBlockList);
    currentLargeHeapBlockCount += Check(false, true, pendingDisposeLargeBlockList);
    return currentLargeHeapBlockCount;
}

template <typename TBlockType>
size_t
LargeHeapBucket::Check(bool expectFull, bool expectPending, TBlockType * list, TBlockType * tail)
{
    size_t heapBlockCount = 0;
    HeapBlockList::ForEach(list, tail, [&heapBlockCount, expectFull, expectPending](TBlockType * heapBlock)
    {
        heapBlock->Check(expectFull, expectPending);
        heapBlockCount++;
    });
    return heapBlockCount;
}

template size_t LargeHeapBucket::Check<LargeHeapBlock>(bool expectFull, bool expectPending, LargeHeapBlock * list, LargeHeapBlock * tail);

void
LargeHeapBucket::VerifyLargeHeapBlockCount()
{
    GetLargeHeapBlockCount(true);
}
#endif

#ifdef RECYCLER_MEMORY_VERIFY
void
LargeHeapBucket::Verify()
{
    Recycler * recycler = this->heapInfo->recycler;
    HeapBlockList::ForEach(largeBlockList, [recycler](LargeHeapBlock * largeHeapBlock)
    {
        largeHeapBlock->Verify(recycler);
    });
    HeapBlockList::ForEach(largePageHeapBlockList, [recycler](LargeHeapBlock * largeHeapBlock)
    {
        largeHeapBlock->Verify(recycler);
    });
    HeapBlockList::ForEach(fullLargeBlockList, [recycler](LargeHeapBlock * largeHeapBlock)
    {
        largeHeapBlock->Verify(recycler);
    });
    HeapBlockList::ForEach(pendingDisposeLargeBlockList, [recycler](LargeHeapBlock * largeHeapBlock)
    {
        largeHeapBlock->Verify(recycler);
    });
}
#endif

#ifdef RECYCLER_VERIFY_MARK
void
LargeHeapBucket::VerifyMark()
{
    HeapBlockList::ForEach(largeBlockList, [](LargeHeapBlock * largeHeapBlock)
    {
        largeHeapBlock->VerifyMark();
    });
    HeapBlockList::ForEach(fullLargeBlockList, [](LargeHeapBlock * largeHeapBlock)
    {
        largeHeapBlock->VerifyMark();
    });
    HeapBlockList::ForEach(pendingDisposeLargeBlockList, [](LargeHeapBlock * largeHeapBlock)
    {
        largeHeapBlock->VerifyMark();
    });
}
#endif

#if ENABLE_MEM_STATS
void
LargeHeapBucket::AggregateBucketStats()
{
    HeapBucket::AggregateBucketStats();  // call super

    auto blockStatsAggregator = [this](LargeHeapBlock* largeHeapBlock) {
        largeHeapBlock->AggregateBlockStats(this->memStats);
    };

    HeapBlockList::ForEach(largeBlockList, blockStatsAggregator);
    HeapBlockList::ForEach(fullLargeBlockList, blockStatsAggregator);
    HeapBlockList::ForEach(pendingDisposeLargeBlockList, blockStatsAggregator);
    HeapBlockList::ForEach(pendingSweepLargeBlockList, blockStatsAggregator);
    HeapBlockList::ForEach(partialSweptLargeBlockList, blockStatsAggregator);
}
#endif

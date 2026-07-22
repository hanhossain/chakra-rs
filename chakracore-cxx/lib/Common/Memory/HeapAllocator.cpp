//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include <string>
#include "HeapAllocator.h"

// Initialization order
//  AB AutoSystemInfo
//  AD PerfCounter
//  AE PerfCounterSet
//  AM Output/Configuration
//  AP DbgHelpSymbolManager
//  AQ CFGLogger
//  AS JavascriptDispatch/RecyclerObjectDumper
//  AT HeapAllocator/RecyclerHeuristic
//  AU RecyclerWriteBarrierManager
#pragma warning(disable:4075)       // initializers put in unrecognized initialization area on purpose
#pragma init_seg(".CRT$XCAT")

#ifdef HEAP_TRACK_ALLOC
std::recursive_mutex HeapAllocator::cs;
#endif

HeapAllocator HeapAllocator::Instance;
NoThrowHeapAllocator NoThrowHeapAllocator::Instance;
NoCheckHeapAllocator NoCheckHeapAllocator::Instance;

template <bool noThrow>
char * HeapAllocator::AllocT(size_t byteSize)
{
#ifdef HEAP_TRACK_ALLOC
    size_t requestedBytes = byteSize;
    byteSize = AllocSizeMath::Add(requestedBytes, ::Math::Align<size_t>(sizeof(HeapAllocRecord), MEMORY_ALLOCATION_ALIGNMENT));
    TrackAllocData allocData;
    ClearTrackAllocInfo(&allocData);
#elif defined(HEAP_PERF_COUNTERS)
    size_t requestedBytes = byteSize;
    byteSize = AllocSizeMath::Add(requestedBytes, ::Math::Align<size_t>(sizeof(size_t), MEMORY_ALLOCATION_ALIGNMENT));
#endif

    char * buffer;
    {
        buffer = static_cast<char*>(malloc(byteSize));
    }

    if (!noThrow && buffer == nullptr)
    {
        Js::Throw::OutOfMemory();
    }

#if defined(HEAP_TRACK_ALLOC) || defined(HEAP_PERF_COUNTERS)
    if (!noThrow || buffer != nullptr)
    {
#ifdef HEAP_TRACK_ALLOC
        cs.lock();
        data.LogAlloc(reinterpret_cast<HeapAllocRecord*>(buffer), requestedBytes, allocData);
        cs.unlock();
        buffer += ::Math::Align<size_t>(sizeof(HeapAllocRecord), MEMORY_ALLOCATION_ALIGNMENT);
#else
        *(size_t *)buffer = requestedBytes;
        buffer += ::Math::Align<size_t>(sizeof(size_t), MEMORY_ALLOCATION_ALIGNMENT);

#endif
        HEAP_PERF_COUNTER_INC(LiveObject);
        HEAP_PERF_COUNTER_ADD(LiveObjectSize, requestedBytes);
    }
#endif
    return buffer;
}

template char * HeapAllocator::AllocT<true>(size_t byteSize);
template char * HeapAllocator::AllocT<false>(size_t byteSize);


void HeapAllocator::Free(void * buffer, size_t byteSize)
{
#ifdef HEAP_TRACK_ALLOC
    if (buffer != nullptr)
    {
        HeapAllocRecord * record = reinterpret_cast<HeapAllocRecord*>(static_cast<char*>(buffer) - ::Math::Align<size_t>(
            sizeof(HeapAllocRecord), MEMORY_ALLOCATION_ALIGNMENT));
        Assert(byteSize == static_cast<size_t>(-1) || record->size == byteSize);

        HEAP_PERF_COUNTER_DEC(LiveObject);
        HEAP_PERF_COUNTER_SUB(LiveObjectSize, record->size);

        cs.lock();
        data.LogFree(record);
        cs.unlock();

        buffer = record;
#if DBG
        memset(buffer, DbgMemFill, record->size + ::Math::Align<size_t>(sizeof(HeapAllocRecord), MEMORY_ALLOCATION_ALIGNMENT));
#endif
    }
#elif defined(HEAP_PERF_COUNTERS)
    if (buffer != nullptr)
    {
        HEAP_PERF_COUNTER_DEC(LiveObject);
        size_t * allocSize = (size_t *)(((char *)buffer) - ::Math::Align<size_t>(sizeof(size_t), MEMORY_ALLOCATION_ALIGNMENT));
        HEAP_PERF_COUNTER_SUB(LiveObjectSize, *allocSize);
        buffer = allocSize;
    }
#endif

    free(buffer);
}

#ifdef TRACK_ALLOC
#ifdef HEAP_TRACK_ALLOC
thread_local TrackAllocData HeapAllocator::nextAllocData;
#endif

HeapAllocator * HeapAllocator::TrackAllocInfo(TrackAllocData const& data)
{
#ifdef HEAP_TRACK_ALLOC
    Assert(nextAllocData.IsEmpty());
    nextAllocData = data;
#endif
    return this;
}

void HeapAllocator::ClearTrackAllocInfo(TrackAllocData* data/* = NULL*/)
{
#ifdef HEAP_TRACK_ALLOC
    Assert(!nextAllocData.IsEmpty());
    if (data)
    {
        *data = nextAllocData;
    }
    nextAllocData.Clear();
#endif
}
#endif

#ifdef HEAP_TRACK_ALLOC
//static
bool HeapAllocator::CheckLeaks()
{
    return Instance.data.CheckLeaks();
}
#endif // HEAP_TRACK_ALLOC

char * NoThrowHeapAllocator::AllocZero(size_t byteSize)
{
    return HeapAllocator::Instance.NoThrowAllocZero(byteSize);
}

char * NoThrowHeapAllocator::Alloc(size_t byteSize)
{
    return HeapAllocator::Instance.NoThrowAlloc(byteSize);
}

void NoThrowHeapAllocator::Free(void * buffer, size_t byteSize)
{
    HeapAllocator::Instance.Free(buffer, byteSize);
}

#ifdef TRACK_ALLOC
NoThrowHeapAllocator * NoThrowHeapAllocator::TrackAllocInfo(TrackAllocData const& data)
{
    HeapAllocator::Instance.TrackAllocInfo(data);
    return this;
}
#endif // TRACK_ALLOC

#ifdef TRACK_ALLOC
void NoThrowHeapAllocator::ClearTrackAllocInfo(TrackAllocData* data /*= NULL*/)
{
    HeapAllocator::Instance.ClearTrackAllocInfo(data);
}
#endif // TRACK_ALLOC

HeapAllocator * HeapAllocator::GetNoMemProtectInstance()
{
    return &Instance;
}

HeapAllocator::HeapAllocator(bool useAllocMemProtect)
    : m_privateHeap(nullptr)
{
}

HeapAllocator::~HeapAllocator()
{
}

#ifdef HEAP_TRACK_ALLOC
void
HeapAllocatorData::LogAlloc(HeapAllocRecord * record, size_t requestedBytes, TrackAllocData const& data)
{
    record->prev = nullptr;
    record->size = requestedBytes;

    record->data = this;
    record->next = head;
    record->allocId = allocCount;
    record->allocData = data;
    if (head != nullptr)
    {
        head->prev = record;
    }
    head = record;
    outstandingBytes += requestedBytes;
    allocCount++;

#if defined(CHECK_MEMORY_LEAK)
#ifdef STACK_BACK_TRACE
    // REVIEW: Okay to use global flags?
    if (Js::Configuration::Global.flags.LeakStackTrace)
    {
        // Allocation done before the flags is parse doesn't get a stack trace
        record->stacktrace = StackBackTrace::Capture(&NoCheckHeapAllocator::Instance, 1, StackTraceDepth);
    }
    else
    {
        record->stacktrace = nullptr;
    }
#endif
#endif
}

void
HeapAllocatorData::LogFree(HeapAllocRecord * record)
{
    Assert(record->data == this);

    // This is an expensive check for double free
#if 0
    HeapAllocRecord * curr = head;
    while (curr != nullptr)
    {
        if (curr == record)
        {
            break;
        }
        curr = curr->next;
    }
    Assert(curr != nullptr);
#endif
    if (record->next != nullptr)
    {
        record->next->prev = record->prev;
    }
    if (record->prev == nullptr)
    {
        head = record->next;
    }
    else
    {
        record->prev->next = record->next;
    }

    deleteCount++;
    outstandingBytes -= record->size;
#if defined(CHECK_MEMORY_LEAK)
#ifdef STACK_BACK_TRACE
    if (record->stacktrace != nullptr)
    {
        record->stacktrace->Delete(&NoCheckHeapAllocator::Instance);
    }
#endif
#endif
}

bool
HeapAllocatorData::CheckLeaks()
{
#if !DBG
    bool needPause = false;
#endif
    if (allocCount != deleteCount)
    {
#if !DBG
        needPause = true;
#endif

        HeapAllocRecord * current = head;
        while (current != nullptr)
        {
            Output::Print(u"%S%s", current->allocData.GetTypeInfo()->name(),
                current->allocData.GetCount() == static_cast<size_t>(-1)? u"" : u"[]");
            Output::SkipToColumn(50);
            Output::Print(u"- %p - %10d bytes\n",
                reinterpret_cast<char*>(current) + ::Math::Align<size_t>(sizeof(HeapAllocRecord), MEMORY_ALLOCATION_ALIGNMENT),
                current->size);
#if defined(CHECK_MEMORY_LEAK)
#ifdef STACK_BACK_TRACE
            // REVIEW: Okay to use global flags?
            if (Js::Configuration::Global.flags.LeakStackTrace && current->stacktrace)
            {
                // Allocation done before the flags is parse doesn't get a stack trace
                Output::Print(u" Allocation Stack:\n");
                current->stacktrace->Print();
            }
#endif
#endif
            current = current->next;
        }
    }
    else if (outstandingBytes != 0)
    {
#if !DBG
        needPause = true;
#endif
        Output::Print(u"Unbalanced new/delete size: %d\n", outstandingBytes);
    }

    Output::Flush();

#if !DBG
    // REVIEW: Okay to use global flags?
    if (needPause && Js::Configuration::Global.flags.Console)
    {
        //This is not defined for WinCE
        HANDLE handle = GetStdHandle( STD_INPUT_HANDLE );

        FlushConsoleInputBuffer(handle);

        Output::Print(u"Press any key to continue...\n");
        Output::Flush();

        WaitForSingleObject(handle, INFINITE);

    }
#endif
    return allocCount == deleteCount && outstandingBytes == 0;
}

#endif


#ifdef CHECK_MEMORY_LEAK
MemoryLeakCheck::~MemoryLeakCheck()
{
    if (head != nullptr)
    {
        if (enableOutput)
        {
            Output::Print(u"FATAL ERROR: Memory Leak Detected\n");
        }
        LeakRecord * current = head;
        do
        {
            if (enableOutput)
            {
                Output::PrintBuffer(current->dump, std::u16string(current->dump).length());
            }
            LeakRecord * prev = current;
            current = current->next;
            free(const_cast<void*>(static_cast<const void*>(prev->dump)));
            NoCheckHeapDelete(prev);
        }
        while (current != nullptr);
        if (enableOutput)
        {
            Output::Print(u"-------------------------------------------------------------------------------------\n");
            Output::Print(u"Total leaked: %d bytes (%d objects)\n", leakedBytes, leakedCount);
            Output::Flush();
        }
    }
}

#endif

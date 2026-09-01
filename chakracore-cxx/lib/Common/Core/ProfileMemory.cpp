//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#ifdef PROFILE_MEM
#include "Memory/AutoPtr.h"
#include "Core/ProfileMemory.h"

thread_local MemoryProfiler * MemoryProfiler::Instance = nullptr;

std::recursive_mutex MemoryProfiler::s_cs;
AutoPtr<MemoryProfiler, NoCheckHeapAllocator> MemoryProfiler::profilers(nullptr);

MemoryProfiler::MemoryProfiler() :
    pageAllocator(nullptr, Js::Configuration::Global.flags,
    PageAllocatorType_Max, 0, false
        , nullptr
        ),
    alloc(u"MemoryProfiler", &pageAllocator, Js::Throw::OutOfMemory),
    arenaDataMap(&alloc, 10)
{
    threadId = ::GetCurrentThreadId();
    memset(&pageMemoryData, 0, sizeof(pageMemoryData));
    memset(&recyclerMemoryData, 0, sizeof(recyclerMemoryData));
}

MemoryProfiler::~MemoryProfiler()
{
#if DBG
    pageAllocator.SetDisableThreadAccessCheck();
#endif
    if (next != nullptr)
    {
        NoCheckHeapDelete(next);
    }
}

MemoryProfiler *
MemoryProfiler::EnsureMemoryProfiler()
{
    MemoryProfiler * memoryProfiler = MemoryProfiler::Instance;

    if (memoryProfiler == nullptr)
    {
        memoryProfiler = NoCheckHeapNew(MemoryProfiler);

        {
            std::unique_lock autocs(s_cs);
            memoryProfiler->next = MemoryProfiler::profilers.Detach();
            MemoryProfiler::profilers = memoryProfiler;
        }

        MemoryProfiler::Instance = memoryProfiler;
    }
    return memoryProfiler;
}

void
MemoryProfiler::Reset(const char16_t* name, ArenaMemoryData * memoryData)
{
    MemoryProfiler * memoryProfiler = memoryData->profiler;
    ArenaMemoryDataSummary * arenaMemoryDataSummary;
    bool hasItem = memoryProfiler->arenaDataMap.TryGetValue(const_cast<char16_t*>(name), &arenaMemoryDataSummary);
    Assert(hasItem);


    AccumulateData(arenaMemoryDataSummary, memoryData, true);
    memoryData->allocatedBytes = 0;
    memoryData->alignmentBytes = 0;
    memoryData->requestBytes = 0;
    memoryData->requestCount = 0;
    memoryData->reuseBytes = 0;
    memoryData->reuseCount = 0;
    memoryData->freelistBytes = 0;
    memoryData->freelistCount = 0;
    memoryData->resetCount++;
}

void
MemoryProfiler::End(const char16_t* name, ArenaMemoryData * memoryData)
{
    MemoryProfiler * memoryProfiler = memoryData->profiler;
    ArenaMemoryDataSummary * arenaMemoryDataSummary;
    bool hasItem = memoryProfiler->arenaDataMap.TryGetValue(const_cast<char16_t*>(name), &arenaMemoryDataSummary);
    Assert(hasItem);

    if (memoryData->next != nullptr)
    {
        memoryData->next->prev = memoryData->prev;
    }

    if (memoryData->prev != nullptr)
    {
        memoryData->prev->next = memoryData->next;
    }
    else
    {
        Assert(arenaMemoryDataSummary->data == memoryData);
        arenaMemoryDataSummary->data = memoryData->next;
    }
    AccumulateData(arenaMemoryDataSummary, memoryData);
}

void
MemoryProfiler::AccumulateData(ArenaMemoryDataSummary * arenaMemoryDataSummary,  ArenaMemoryData * memoryData, bool reset)
{
    arenaMemoryDataSummary->total.alignmentBytes += memoryData->alignmentBytes;
    arenaMemoryDataSummary->total.allocatedBytes += memoryData->allocatedBytes;
    arenaMemoryDataSummary->total.freelistBytes += memoryData->freelistBytes;
    arenaMemoryDataSummary->total.freelistCount += memoryData->freelistCount;
    arenaMemoryDataSummary->total.requestBytes += memoryData->requestBytes;
    arenaMemoryDataSummary->total.requestCount += memoryData->requestCount;
    arenaMemoryDataSummary->total.reuseCount += memoryData->reuseCount;
    arenaMemoryDataSummary->total.reuseBytes += memoryData->reuseBytes;
    if (!reset)
    {
        arenaMemoryDataSummary->total.resetCount += memoryData->resetCount;
    }

    arenaMemoryDataSummary->max.alignmentBytes = max(arenaMemoryDataSummary->max.alignmentBytes, memoryData->alignmentBytes);
    arenaMemoryDataSummary->max.allocatedBytes = max(arenaMemoryDataSummary->max.allocatedBytes, memoryData->allocatedBytes);
    arenaMemoryDataSummary->max.freelistBytes = max(arenaMemoryDataSummary->max.freelistBytes, memoryData->freelistBytes);
    arenaMemoryDataSummary->max.freelistCount = max(arenaMemoryDataSummary->max.freelistCount, memoryData->freelistCount);
    arenaMemoryDataSummary->max.requestBytes = max(arenaMemoryDataSummary->max.requestBytes, memoryData->requestBytes);
    arenaMemoryDataSummary->max.requestCount = max(arenaMemoryDataSummary->max.requestCount, memoryData->requestCount);
    arenaMemoryDataSummary->max.reuseCount = max(arenaMemoryDataSummary->max.reuseCount, memoryData->reuseCount);
    arenaMemoryDataSummary->max.reuseBytes = max(arenaMemoryDataSummary->max.reuseBytes, memoryData->reuseBytes);
    if (!reset)
    {
        arenaMemoryDataSummary->max.resetCount = max(arenaMemoryDataSummary->max.resetCount, memoryData->resetCount);
    }
}

void
MemoryProfiler::PrintPageMemoryData(PageMemoryData const& pageMemoryData, char const * title)
{
    if (pageMemoryData.allocSegmentCount != 0)
    {
        Output::Print(u"%-10S:%9d %10d | %4d %10d | %4d %10d | %10d | %10d | %10d | %10d\n", title,
            pageMemoryData.currentCommittedPageCount * AutoSystemInfo::PageSize, pageMemoryData.peakCommittedPageCount * AutoSystemInfo::PageSize,
            pageMemoryData.allocSegmentCount, pageMemoryData.allocSegmentBytes,
            pageMemoryData.releaseSegmentCount, pageMemoryData.releaseSegmentBytes,
            pageMemoryData.allocPageCount * AutoSystemInfo::PageSize,
            pageMemoryData.releasePageCount * AutoSystemInfo::PageSize,
            pageMemoryData.decommitPageCount * AutoSystemInfo::PageSize,
            pageMemoryData.recommitPageCount * AutoSystemInfo::PageSize);
    }
}

void
MemoryProfiler::PrintArenaHeader(char16_t const * title)
{
    Output::Print(u"--------------------------------------------------------------------------------------------------------\n");

    Output::Print(u"%-20s:%7s %9s %9s %9s %6s %9s %6s %9s %5s | %5s\n",
            title,
            u"#Alloc",
            u"AllocByte",
            u"ReqBytes",
            u"AlignByte",
            u"#Reuse",
            u"ReuseByte",
            u"#Free",
            u"FreeBytes",
            u"Reset",
            u"Count");

    Output::Print(u"--------------------------------------------------------------------------------------------------------\n");
}

int MemoryProfiler::CreateArenaUsageSummary(ArenaAllocator * alloc, bool liveOnly,
    _Outptr_result_buffer_(return) char16_t* ** name_ptr, _Outptr_result_buffer_(return) ArenaMemoryDataSummary *** summaries_ptr)
{
    Assert(alloc);

    char16_t* *& name = *name_ptr;
    ArenaMemoryDataSummary **& summaries = *summaries_ptr;

    int count = arenaDataMap.Count();
    name = AnewArray(alloc, char16_t*, count);
    int i = 0;
    arenaDataMap.Map([&i, name](char16_t* key, ArenaMemoryDataSummary*)
    {
        name[i++] = key;
    });

    qsort_s(name, count, sizeof(char16_t*), [](void *, const void *a, const void *b)
    {
        return DefaultComparer<char16_t*>::Compare(*static_cast<char16_t**>(const_cast<void*>(a)),
                                                   *static_cast<char16_t**>(const_cast<void*>(b)));
    }, nullptr);

    summaries = AnewArray(alloc, ArenaMemoryDataSummary *, count);

    for (int j = 0; j < count; j++)
    {
        ArenaMemoryDataSummary * summary = arenaDataMap.Item(name[j]);
        ArenaMemoryData * data = summary->data;

        ArenaMemoryDataSummary * localSummary;
        if (liveOnly)
        {
            if (data == nullptr)
            {
                summaries[j] = nullptr;
                continue;
            }
            localSummary = AnewStructZ(alloc, ArenaMemoryDataSummary);
        }
        else
        {
            localSummary = Anew(alloc, ArenaMemoryDataSummary, *summary);
        }

        while (data != nullptr)
        {
            localSummary->outstandingCount++;
            AccumulateData(localSummary, data);
            data = data->next;
        }

        if (liveOnly)
        {
            localSummary->arenaCount = localSummary->outstandingCount;
        }
        summaries[j] = localSummary;
    }

    return count;
}

// TODO (hanhossain): can probably remove?
bool
MemoryProfiler::IsEnabled()
{
    return false;
}

bool
MemoryProfiler::DoTrackRecyclerAllocation()
{
    return MemoryProfiler::IsEnabled();
}
#endif

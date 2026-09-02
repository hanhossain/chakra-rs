//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include <string>
#include "HeapAllocator.h"

// Initialization order
//  AB AutoSystemInfo
//  AM Output/Configuration
//  AP DbgHelpSymbolManager
//  AQ CFGLogger
//  AS JavascriptDispatch/RecyclerObjectDumper
//  AT HeapAllocator/RecyclerHeuristic
//  AU RecyclerWriteBarrierManager
#pragma warning(disable:4075)       // initializers put in unrecognized initialization area on purpose
#pragma init_seg(".CRT$XCAT")

HeapAllocator HeapAllocator::Instance;
NoThrowHeapAllocator NoThrowHeapAllocator::Instance;
NoCheckHeapAllocator NoCheckHeapAllocator::Instance;

template <bool noThrow>
char * HeapAllocator::AllocT(size_t byteSize)
{
    char * buffer;
    {
        buffer = static_cast<char*>(malloc(byteSize));
    }

    if (!noThrow && buffer == nullptr)
    {
        Js::Throw::OutOfMemory();
    }
    return buffer;
}

template char * HeapAllocator::AllocT<true>(size_t byteSize);
template char * HeapAllocator::AllocT<false>(size_t byteSize);


void HeapAllocator::Free(void * buffer, size_t byteSize)
{
    free(buffer);
}

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

//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include <string>
#include "Backend.h"

NativeCodeData::NativeCodeData(DataChunk * chunkList)
    : chunkList(chunkList)
{
}

NativeCodeData::~NativeCodeData()
{
    {
        NativeCodeData::DeleteChunkList(this->noFixupChunkList);
    }
}

template <class DataChunkT>
void
NativeCodeData::DeleteChunkList(DataChunkT * chunkList)
{
    DataChunkT * next = chunkList;
    while (next != nullptr)
    {
        DataChunkT * current = next;
        next = next->next;
        HeapDeletePlus(current->len, current);
    }
}

NativeCodeData::Allocator::Allocator()
    : chunkList(nullptr),
    lastChunkList(nullptr)
{
    this->totalSize = 0;
    this->allocCount = 0;
#if DBG
    this->finalized = false;
#endif
}

NativeCodeData::Allocator::~Allocator()
{
    Assert(!finalized || this->chunkList == nullptr);
    {
        NativeCodeData::DeleteChunkList(this->noFixupChunkList);
    }
}

char *
NativeCodeData::Allocator::Alloc(size_t requestSize)
{
    Assert(!finalized);
    char * data = nullptr;
    requestSize = Math::Align(requestSize, sizeof(void*));

    {
        DataChunkNoFixup * newChunk = HeapNewStructPlus(requestSize, DataChunkNoFixup);
        newChunk->len = (unsigned int)requestSize;
        newChunk->next = this->noFixupChunkList;
        this->noFixupChunkList = newChunk;
        data = newChunk->data;
    }

    return data;
}

char *
NativeCodeData::Allocator::AllocLeaf(size_t requestSize)
{
    return Alloc(requestSize);
}

char *
NativeCodeData::Allocator::AllocZero(size_t requestSize)
{
    char * data = Alloc(requestSize);
    // Allocated with HeapNewStructPlusZ for chk build
    memset(data, 0, requestSize);
    return data;
}

NativeCodeData *
NativeCodeData::Allocator::Finalize()
{
    NativeCodeData * data = nullptr;
    if (this->chunkList != nullptr)
    {
        data = HeapNew(NativeCodeData, this->chunkList);
        this->chunkList = nullptr;
    }
#if DBG
    this->finalized = true;
#endif
    return data;
}

//////////////////////////////////////////////////////////////////////////
//NativeCodeData::Allocator::Free
//This function should not be called at all because the life time is active during the run time
//This function is added to enable Dictionary(has calls to Free() Method - which will never be called as it will be
//allocated as a NativeAllocator to be allocated with NativeAllocator)
//////////////////////////////////////////////////////////////////////////
void
NativeCodeData::Allocator::Free(void * buffer, size_t byteSize)
{
}

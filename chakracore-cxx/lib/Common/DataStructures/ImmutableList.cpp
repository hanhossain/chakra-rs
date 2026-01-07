//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "CommonDataStructuresPch.h"
#include "Option.h"
#include "ImmutableList.h"

template<int chunkSize>
void regex::ImmutableStringBuilder<chunkSize>::AppendInt32(int32_t value)
{
    char16_t buffer[11]; // -2,147,483,648 w.o ',' + \0
    int32_t hr = S_OK;
    hr = StringCchPrintfW(buffer, _countof(buffer), u"%d", value);
    AssertMsg(SUCCEEDED(hr), "StringCchPrintfW");
    if (FAILED(hr) )
    {
        Js::Throw::OutOfMemory();
    }

    // append to the stringBuilder string
    this->AppendWithCopy(buffer);
}

template<int chunkSize>
void regex::ImmutableStringBuilder<chunkSize>::AppendUInt64(unsigned long value)
{
    char16_t buffer[21]; // 18,446,744,073,709,551,615 w.o ',' + \0
    int32_t hr = S_OK;
    hr = StringCchPrintfW(buffer, _countof(buffer), u"%llu", value);
    AssertMsg(SUCCEEDED(hr), "StringCchPrintfW");
    if (FAILED(hr) )
    {
        Js::Throw::OutOfMemory();
    }

    // append to the stringBuilder string
    this->AppendWithCopy(buffer);
}

template<int chunkSize>
void regex::ImmutableStringBuilder<chunkSize>::AppendWithCopy(_In_z_ const char16_t* str)
{
    AssertMsg(str != nullptr, "str != nullptr");
    size_t strLength = wcslen(str) + 1; // include null-terminated

    char16_t* buffer = HeapNewNoThrowArray(char16_t, strLength);
    IfNullThrowOutOfMemory(buffer);
    wcsncpy_s(buffer, strLength, str, strLength);

    // append in front of the tracked allocated strings
    AllocatedStringChunk* newAllocatedString = HeapNewNoThrow(AllocatedStringChunk);
    if (newAllocatedString == nullptr)
    {
        // cleanup
        HeapDeleteArray(strLength, buffer);
        Js::Throw::OutOfMemory();
    }

    newAllocatedString->dataPtr = buffer;
    newAllocatedString->next = this->allocatedStringChunksHead;
    this->allocatedStringChunksHead = newAllocatedString;

    // append to the stringBuilder string
    this->Append(buffer);
}

// template instantiation
template void regex::ImmutableStringBuilder<8>::AppendInt32(int32_t value);
template void regex::ImmutableStringBuilder<8>::AppendUInt64(unsigned long value);

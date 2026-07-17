//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include <string>
#include "Option.h"
#include "ImmutableList.h"

template<int chunkSize>
void regex::ImmutableStringBuilder<chunkSize>::AppendWithCopy(_In_z_ const char16_t* str)
{
    AssertMsg(str != nullptr, "str != nullptr");
    size_t strLength = std::u16string(str).length() + 1; // include null-terminated

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

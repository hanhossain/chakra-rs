//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "CommonDataStructuresPch.h"
#include "DataStructures/CharacterBuffer.h"
#include "DataStructures/InternalString.h"
#include "Memory/ArenaAllocator.h"
#include "Memory/Recycler.h"

namespace Js
{
    InternalString::InternalString(const char16_t* content, charcount_t length, unsigned char offset):
        m_content(content),
        m_charLength(length),
        m_offset(offset)
    {
        AssertMsg(length < INT_MAX, "Length should be a valid string length");
    }

    InternalString::InternalString(const char16_t* content, _no_write_barrier_tag, charcount_t length, unsigned char offset) :
        m_content(NO_WRITE_BARRIER_TAG(content)),
        m_charLength(length),
        m_offset(offset)
    {
        AssertMsg(length < INT_MAX, "Length should be a valid string length");
    }

    // This will make a copy of the entire buffer
    InternalString *InternalString::New(ArenaAllocator* alloc, const char16_t* content, charcount_t length)
    {
        size_t bytelength = sizeof(char16_t) * length;
        uint32_t* allocbuffer = (uint32_t*)alloc->Alloc(sizeof(uint32_t) + bytelength + sizeof(char16_t));
        allocbuffer[0] = (uint32_t) bytelength;

        char16_t* buffer = (char16_t*)(allocbuffer+1);
        js_memcpy_s(buffer, bytelength, content, bytelength);
        buffer[length] = u'\0';
        InternalString* newInstance = Anew(alloc, InternalString, buffer, length);
        return newInstance;
    }

    // This will make a copy of the entire buffer
    // Allocated using recycler memory
    InternalString *InternalString::New(Recycler* recycler, const char16_t* content, charcount_t length)
    {
        size_t bytelength = sizeof(char16_t) * length;

        // Allocate 3 extra bytes, two for the first uint32_t with the size, the third for the null character
        // This is so that we can pretend that internal strings are BSTRs for purposes of clients who want to use
        // it as thus
        const unsigned char offset = sizeof(uint32_t)/sizeof(char16_t);
        InternalString* newInstance = RecyclerNewPlusLeaf(recycler, bytelength + (sizeof(uint32_t) + sizeof(char16_t)), InternalString, nullptr, length, offset);
        uint32_t* allocbuffer = (uint32_t*) (newInstance + 1);
        allocbuffer[0] = (uint32_t) bytelength;
        char16_t* buffer = (char16_t*)(allocbuffer + 1);
        js_memcpy_s(buffer, bytelength, content, bytelength);
        buffer[length] = u'\0';
        newInstance->m_content = (const char16_t*) allocbuffer;

        return newInstance;
    }


    // This will only store the pointer and length, not making a copy of the buffer
    InternalString *InternalString::NewNoCopy(ArenaAllocator* alloc, const char16_t* content, charcount_t length)
    {
        InternalString* newInstance = Anew(alloc, InternalString, const_cast<char16_t*> (content), length);
        return newInstance;
    }
}

//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

#include "Interface/CommonBasic.h"

namespace Memory
{
struct FreeObject
{
public:
    FreeObject * GetNext() const
    {
        AssertMsg((taggedNext & TaggedBit) == TaggedBit, "Free list corrupted");
        return reinterpret_cast<FreeObject*>(taggedNext & ~TaggedBit);
    }

    void SetNext(FreeObject * next)
    {
        Assert((reinterpret_cast<long>(next) & TaggedBit) == 0);
        taggedNext = reinterpret_cast<long>(next) | TaggedBit;
    }
    void ZeroNext() { taggedNext = 0; }
#ifdef RECYCLER_MEMORY_VERIFY
#pragma warning(suppress:4310)
    void DebugFillNext() { taggedNext = static_cast<long>(0xCACACACACACACACA); }
#endif
private:
    long taggedNext;
    static long const TaggedBit = 0x1;
};
}

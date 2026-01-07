//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "CommonDataStructuresPch.h"
#include "Common/UInt32Math.h"
#include "DataStructures/GrowingArray.h"

namespace JsUtil
{
    template <>
    GrowingUint32HeapArray* GrowingArray<uint32_t, HeapAllocator>::Create(uint32_t _length)
    {
        return HeapNew(GrowingUint32HeapArray, &HeapAllocator::Instance, _length);
    }

}

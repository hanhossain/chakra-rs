//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

namespace Js
{
    struct PropertyIdArray
    {
        typename WriteBarrierFieldTypeTraits<uint32_t>::Type count;
        typename WriteBarrierFieldTypeTraits<byte>::Type extraSlots;
        typename WriteBarrierFieldTypeTraits<bool>::Type   hadDuplicates;
        typename WriteBarrierFieldTypeTraits<bool>::Type   has__proto__; // Only used for object literal
        typename WriteBarrierFieldTypeTraits<bool>::Type   hasNonSimpleParams;
        typename WriteBarrierFieldTypeTraits<PropertyId>::Type elements[];

        PropertyIdArray(uint32_t count, byte extraSlots, bool hadDuplicates = false, bool has__proto__ = false, bool hasNonSimpleParams = false) :
            count(count),
            extraSlots(extraSlots),
            hadDuplicates(hadDuplicates),
            has__proto__(has__proto__),
            hasNonSimpleParams(hasNonSimpleParams)
        {
        }

        size_t GetDataSize() const { return sizeof(PropertyIdArray) + sizeof(PropertyId) * (count + extraSlots); }
    };
};

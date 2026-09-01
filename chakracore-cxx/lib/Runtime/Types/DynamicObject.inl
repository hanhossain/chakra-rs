//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#pragma once

namespace Js
{
    template <class T>
    inline T * DynamicObject::NewObject(Recycler * recycler, DynamicType * type)
    {
        size_t inlineSlotsSize = type->GetTypeHandler()->GetInlineSlotsSize();
        if (inlineSlotsSize)
        {
            return RecyclerNewPlusZ(recycler, inlineSlotsSize, T, type);
        }
        else
        {
            return RecyclerNew(recycler, T, type);
        }
    }
}

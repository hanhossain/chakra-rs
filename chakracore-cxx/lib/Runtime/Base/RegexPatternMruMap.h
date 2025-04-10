//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

#include "DataStructures/MruDictionary.h"
#include "DataStructures/RegexKey.h"

namespace Js
{
    typedef JsUtil::MruDictionary<UnifiedRegex::RegexKey, UnifiedRegex::RegexPattern*> RegexPatternMruMap;
};

namespace JsUtil
{
    template <>
    inline void ClearValue<Js::RegexPatternMruMap::MruDictionaryData>::Clear(Js::RegexPatternMruMap::MruDictionaryData* value)
    {
        *value = 0;
    }
}

//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

namespace JsUtil
{
    template <typename T>
    class NoCaseComparer;

    template <>
    class NoCaseComparer<JsUtil::CharacterBuffer<char16_t>>
    {
    public:
        static bool Equals(JsUtil::CharacterBuffer<char16_t> const& x, JsUtil::CharacterBuffer<char16_t> const& y);
        static uint GetHashCode(JsUtil::CharacterBuffer<char16_t> const& i);
    private:
        static int Compare(JsUtil::CharacterBuffer<char16_t> const& x, JsUtil::CharacterBuffer<char16_t> const& y);
    };

}

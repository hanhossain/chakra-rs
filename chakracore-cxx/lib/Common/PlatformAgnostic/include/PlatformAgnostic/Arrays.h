//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

namespace PlatformAgnostic
{
namespace Arrays
{
#ifndef ENABLE_GLOBALIZATION

    class ArrayLocalization
    {
        char16_t localeSeparator;

        static const uint16 BufSize = 256;

    public:

        ArrayLocalization();

        inline char16_t GetLocaleSeparator() { return localeSeparator; }
    };

#endif   

    static const uint32_t SeparatorBufferSize = 6;

    // According to MSDN the maximum number of characters for the list separator (LOCALE_SLIST) is four, including a terminating null character.
    bool GetLocaleSeparator(char16_t* szSeparator, uint32_t* sepOutLength, uint32_t sepBufferSize);

    template <uint32_t sepBufferSize>
    inline bool GetLocaleSeparator(char16_t(&szSeparator)[sepBufferSize], uint32_t *sepOutLength)
    {
        return GetLocaleSeparator(szSeparator, sepOutLength, sepBufferSize);
    }

} // namespace Arrays
} // namespace PlatformAgnostic

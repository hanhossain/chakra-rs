//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once
#include <string>
#include "Utf8Codex.h"

namespace utf8
{

    inline int32_t NarrowStringToWideNoAlloc(_In_ const char * sourceString, size_t sourceCount,
        __out_ecount(destBufferCount) char16_t* destString, size_t destBufferCount, _Out_ charcount_t* destCount)
    {
        size_t sourceStart = 0;
        size_t cbSourceString = sourceCount;

        if (sourceCount >= std::numeric_limits<uint32_t>::max())
        {
            destString[0] = char16_t(0);
            return E_OUTOFMEMORY;
        }

        if (destString == nullptr)
        {
            return E_INVALIDARG;
        }

        if (sourceCount >= destBufferCount)
        {
            destString[0] = char16_t(0);
            return E_INVALIDARG;
        }

        for (; sourceStart < sourceCount; sourceStart++)
        {
            const char ch = sourceString[sourceStart];
            if ( ! (ch > 0 && ch < 0x0080) )
            {
                size_t fallback = sourceStart > 3 ? 3 : sourceStart; // 3 + 1 -> fallback at least 1 unicode char
                sourceStart -= fallback;
                break;
            }
            destString[sourceStart] = static_cast<char16_t>(ch);
        }

        if (sourceStart == sourceCount)
        {
            *destCount = static_cast<charcount_t>(sourceCount);
            destString[sourceCount] = char16_t(0);
        }
        else
        {
            LPCUTF8 remSourceString = reinterpret_cast<LPCUTF8>(sourceString) + sourceStart;
            char16_t *remDestString = destString + sourceStart;

            charcount_t cchDestString = utf8::ByteIndexIntoCharacterIndex(remSourceString, cbSourceString - sourceStart);
            cchDestString += static_cast<charcount_t>(sourceStart);
            if (cchDestString > sourceCount)
            {
                return E_OUTOFMEMORY;
            }

            // Some node tests depend on the utf8 decoder not swallowing invalid unicode characters
            // instead of replacing them with the "replacement" chracter. Pass a flag to our
            // decoder to require such behavior
            utf8::DecodeUnitsIntoAndNullTerminateNoAdvance(remDestString, remSourceString, reinterpret_cast<LPCUTF8>(sourceString) + cbSourceString, DecodeOptions::doAllowInvalidWCHARs);

            static_assert(sizeof(utf8char_t) == sizeof(char), "Needs to be valid for cast");
            *destCount = cchDestString;
        }

        assert(destString[*destCount] == 0);

        return S_OK;
    }
} // namespace utf8

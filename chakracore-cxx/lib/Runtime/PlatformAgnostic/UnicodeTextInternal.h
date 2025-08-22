//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

namespace PlatformAgnostic
{
namespace UnicodeText
{

// Instantiate templates here rather than in each implementing file
template charcount_t ChangeStringLinguisticCase<true, true>(const char16_t* sourceString, charcount_t sourceLength, char16_t* destString, charcount_t destLength, ApiError* pErrorOut);
template charcount_t ChangeStringLinguisticCase<true, false>(const char16_t* sourceString, charcount_t sourceLength, char16_t* destString, charcount_t destLength, ApiError* pErrorOut);
template charcount_t ChangeStringLinguisticCase<false, true>(const char16_t* sourceString, charcount_t sourceLength, char16_t* destString, charcount_t destLength, ApiError* pErrorOut);
template charcount_t ChangeStringLinguisticCase<false, false>(const char16_t* sourceString, charcount_t sourceLength, char16_t* destString, charcount_t destLength, ApiError* pErrorOut);

namespace Internal
{

int LogicalStringCompareImpl(const char16_t* p1, int p1size, const char16_t* p2, int p2size);

}; // namespace Internal
}; // namespace UnicodeText
}; // namespace PlatformAgnostic

//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

namespace PlatformAgnostic
{
namespace Numbers
{
    class Utility
    {
#ifndef ENABLE_GLOBALIZATION
        class NumbersLocale
        {
            char16_t localeThousands;
            char16_t localeDecimal;
            char16_t localeNegativeSign;

            int maxDigitsAfterDecimals;
            char16_t defaultDecimalDot;
            char16_t defaultDecimalComma;

        public:

            NumbersLocale();

            inline int   GetMaxDigitsAfterDecimals() { return maxDigitsAfterDecimals; }
            inline char16_t GetLocaleThousands()        { return localeThousands; }
            inline bool  HasLocaleThousands()        { return localeThousands != 0; }
            inline char16_t GetLocaleDecimal()          { return localeDecimal; }
            inline char16_t GetLocaleNegativeSign()     { return localeNegativeSign; }

            inline bool  IsDecimalPoint(const char16_t wc)
                                                     { return wc == defaultDecimalDot
                                                           || wc == defaultDecimalComma; }
        };

        // non-ICU implementation keeps user locale intact process wide
        // xplat-todo: While implementing ICU option, make both per context.
        static NumbersLocale numbersLocale;
#endif
    public:

        static size_t NumberToDefaultLocaleString(const char16_t *number_string,
                                                  const size_t length,
                                                  char16_t *buffer,
                                                  const size_t pre_allocated_buffer_size);
    };
} // namespace Numbers
} // namespace PlatformAgnostic

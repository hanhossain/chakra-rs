//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

namespace UnifiedRegex
{
    template <typename C>
    struct Chars
    {
        typedef C Char;
    };

    template <>
    struct Chars<uint8_t>
    {
        typedef uint8_t Char;
        typedef uint8_t UChar;

        static const int CharWidth = sizeof(char) * 8;
        static const int NumChars = 1 << CharWidth;
        static const uint MaxUChar = static_cast<uint8_t>(-1);
        static const uint MaxUCharAscii = (1 << 7) - 1;
        static const Char MinChar = 0;
        static const Char MaxChar = MaxUChar;

        // Char to unsigned int
        static inline uint CTU(Char c)
        {
            return c;
        }

        // Unsigned int to Char
        static inline Char UTC(uint u) {
            Assert(u <= MaxUChar);
            return static_cast<Char>(u);
        }

        // int to Char
        static inline Char ITC(int i) {
            Assert(i >= 0 && i <= MaxUChar);
            return static_cast<Char>(i);
        }

        // Char to char16_t
        static inline char16_t CTW(Char c)
        {
            return c;
        }

        // Offset, same buffer
        static inline CharCount OSB(const Char* ph, const Char* pl)
        {
            Assert(ph >= pl && ph - pl <= MaxCharCount);
            return static_cast<CharCount>(ph - pl);
        }

        static inline Char Shift(Char c, int n)
        {
            return UTC(CTU(c) + n);
        }
    };

    template <>
    struct Chars<char>
    {
        typedef char Char;
        typedef uint8_t UChar;

        static const int CharWidth = sizeof(char) * 8;
        static const int NumChars = 1 << CharWidth;
        static const uint MaxUChar = static_cast<uint8_t>(-1);
        static const uint MaxUCharAscii = (1 << 7) - 1;
        static const Char MinChar = 0;
        static const Char MaxChar = static_cast<Char>(MaxUChar);

        // Char to unsigned int
        static inline uint CTU(Char c)
        {
            return static_cast<uint8_t>(c);
        }

        // Unsigned int to Char
        static inline Char UTC(uint u) {
            Assert(u <= MaxUChar);
            return static_cast<Char>(u);
        }

        // int to Char
        static inline Char ITC(int i) {
            Assert(i >= 0 && i <= MaxUChar);
            return static_cast<Char>(static_cast<uint8_t>(i));
        }

        // Char to char16_t
        static inline char16_t CTW(Char c)
        {
            return static_cast<uint8_t>(c);
        }

        // Offset, same buffer
        static inline CharCount OSB(const Char* ph, const Char* pl)
        {
            Assert(ph >= pl && ph - pl <= MaxCharCount);
            return static_cast<CharCount>(ph - pl);
        }

        static inline Char Shift(Char c, int n)
        {
            return UTC(CTU(c) + n);
        }
    };

    template <>
    struct Chars<char16_t>
    {
        typedef char16_t Char;
        typedef uint16 UChar;

        static const int CharWidth = sizeof(char16_t) * 8;
        static const int NumChars = 1 << CharWidth;
        static const uint MaxUChar = static_cast<uint16>(-1);
        static const uint MaxUCharAscii = (1 << 7) - 1;
        static const Char MinChar = 0;
        static const Char MaxChar = MaxUChar;

        // Char to unsigned int
        static inline uint CTU(Char c)
        {
            return static_cast<uint16>(c);
        }

        // Unsigned int to Char
        static inline Char UTC(uint u)
        {
            Assert(u <= MaxUChar);
            return static_cast<Char>(u);
        }

        // int to Char
        static inline Char ITC(int i) {
            Assert(i >= 0 && i <= MaxUChar);
            return static_cast<Char>(static_cast<uint16>(i));
        }

        // Char to char16_t
        static inline char16_t CTW(Char c)
        {
            return c;
        }

        // Offset, same buffer
        static inline CharCount OSB(const Char* ph, const Char* pl)
        {
            Assert(ph >= pl && ph - pl <= MaxCharCount);
            return static_cast<CharCount>(ph - pl);
        }

        static inline Char Shift(Char c, int n)
        {
            return UTC(CTU(c) + n);
        }
    };

    template <>
    struct Chars<codepoint_t>
    {
        typedef codepoint_t Char;
        typedef codepoint_t UChar;

        static const int CharWidth = sizeof(codepoint_t) * 8;
        static const int NumChars = 0x110000;
        static const uint MaxUChar = (NumChars) - 1;
        static const uint MaxUCharAscii = (1 << 7) - 1;
        static const Char MinChar = 0;
        static const Char MaxChar = MaxUChar;

        // Char to unsigned int
        static inline uint CTU(Char c)
        {
            Assert(c <= MaxChar);
            return c;
        }

        // Unsigned int to Char
        static inline Char UTC(uint u)
        {
            Assert(u <= MaxUChar);
            return u;
        }

        // int to Char
        static inline Char ITC(int i) {
            Assert(i >= 0 && i <= MaxUChar);
            return static_cast<codepoint_t>(i);
        }

        // Char to char16_t
        static inline char16_t CTW(Char c)
        {
            Assert(c < Chars<char16_t>::MaxUChar);
            return static_cast<char16_t>(c);
        }

        // Offset, same buffer
        static inline CharCount OSB(const Char* ph, const Char* pl)
        {
            Assert(ph >= pl && ph - pl <= MaxCharCount);
            return static_cast<CharCount>(ph - pl);
        }

        static inline Char Shift(Char c, int n)
        {
            return UTC(CTU(c) + n);
        }
    };
}

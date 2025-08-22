//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

namespace JSON
{
    class JSONParser;

    // Small scanner for exclusive JSON purpose. The general
    // JScript scanner is not appropriate here because of the JSON restricted lexical grammar
    // token enums and structures are shared although the token semantics is slightly different.
    class JSONScanner
    {
    public:
        JSONScanner();
        tokens Scan();
        void Init(const char16_t* input, uint len, Token* pOutToken,
            ::Js::ScriptContext* sc, const char16_t* current, ArenaAllocator* allocator);

        void Finalizer();
        char16_t* GetCurrentString() { return currentString; } 
        uint GetCurrentStringLen() { return currentIndex; }
        uint GetScanPosition() { return uint(currentChar - inputText); }

        void __declspec(noreturn) ThrowSyntaxError(int wErr)
        {
            char16_t scanPos[16];
            ::_itow_s(GetScanPosition(), scanPos, _countof(scanPos), 10);
            Js::JavascriptError::ThrowSyntaxError(scriptContext, wErr, scanPos);
        }

    private:

        // Data structure for unescaping strings
        struct RangeCharacterPair {
        public:
            uint m_rangeStart;
            uint m_rangeLength;
            char16_t m_char;
            RangeCharacterPair() {}
            RangeCharacterPair(uint rangeStart, uint rangeLength, char16_t ch) : m_rangeStart(rangeStart), m_rangeLength(rangeLength), m_char(ch) {}
        };

        typedef JsUtil::List<RangeCharacterPair, ArenaAllocator> RangeCharacterPairList;

        RangeCharacterPairList* currentRangeCharacterPairList;

        Js::TempGuestArenaAllocatorObject* allocatorObject;
        ArenaAllocator* allocator;
        void BuildUnescapedString(bool shouldSkipLastCharacter);

        RangeCharacterPairList* GetCurrentRangeCharacterPairList(void);

        inline char16_t ReadNextChar(void)
        {
            return *currentChar++;
        }

        inline char16_t PeekNextChar(void)
        {
            return *currentChar;
        }

        tokens ScanString();
        bool IsJSONNumber();

        const char16_t* inputText;
        uint    inputLen;
        const char16_t* currentChar;
        const char16_t* pTokenString;

        Token*   pToken;
        ::Js::ScriptContext* scriptContext;

        uint     currentIndex;
        char16_t* currentString;
        __field_ecount(stringBufferLength) char16_t* stringBuffer;
        int      stringBufferLength;

        friend class JSONParser;
    };
} // namespace JSON

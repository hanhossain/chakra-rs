//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

namespace Js
{
    class SubString : public JavascriptString
    {
        typename WriteBarrierFieldTypeTraits<void const *>::Type originalFullStringReference;          // Only here to prevent recycler to free this buffer.
        typename WriteBarrierFieldTypeTraits<void const *>::Type unused; // Recycler would allocate this space anyway due to bucket sizing, so make it explicit

        SubString(void const * originalFullStringReference, const char16_t* subString, charcount_t length, ScriptContext *scriptContext);

    protected:
        DEFINE_VTABLE_CTOR(SubString, JavascriptString);

    public:
        static JavascriptString* New(JavascriptString* string, charcount_t start, charcount_t length);
        static JavascriptString* New(const char16_t* stringStr, charcount_t start, charcount_t length, ScriptContext *scriptContext);
        const char16_t* GetSz() override;
        void const * GetOriginalStringReference() override;
        size_t GetAllocatedByteCount() const override;
        bool IsSubstring() const override;
        void CachePropertyRecord(_In_ PropertyRecord const* propertyRecord) override;
    };
}

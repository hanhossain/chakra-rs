//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

namespace Js
{
    // Used when the final string has a known final length
    class BufferStringBuilder
    {
    public:
        BufferStringBuilder(charcount_t charLength, ScriptContext* scriptContext)
            : m_string( WritableString::New(charLength, scriptContext) )
        {
        }

        JavascriptString* ToString();

        void DbgAssertNotFrozen() const
        {
            AssertMsg(this->m_string != nullptr, "Do not call BufferStringBuilder methods after ToString() has been called.");
        }

        void SetContent(
            const char16_t* prefix, charcount_t cchPrefix,
            const char16_t* content, charcount_t cchContent,
            const char16_t* suffix, charcount_t cchSuffix)
        {
            DbgAssertNotFrozen();
            this->m_string->SetContent(prefix, cchPrefix, content, cchContent, suffix, cchSuffix);
        }

        // Caution: Do not retain the writable buffer after ToString has been called
        char16_t* DangerousGetWritableBuffer()
        {
            DbgAssertNotFrozen();
            return this->m_string->GetWritableBuffer();
        }

        class WritableString sealed : public JavascriptString
        {
        public:
            static WritableString* New(charcount_t length, ScriptContext* scriptContext);
            char16_t* GetWritableBuffer() const
            {
                return const_cast< char16_t* >( this->UnsafeGetBuffer() );
            }

            void SetContent(const char16_t* content, charcount_t offset, charcount_t length);
            void SetContent(const char16_t* prefix, charcount_t cchPrefix,
                            const char16_t* content, charcount_t cchContent,
                            const char16_t* suffix, charcount_t cchSuffix);

        protected:
            DEFINE_VTABLE_CTOR(WritableString, JavascriptString);

        private:
            WritableString(StaticType * type, charcount_t length, const char16_t* szValue)
                : JavascriptString(type, length, szValue)
            {
            }

            static char16_t* SafeCopyAndAdvancePtr(__out_ecount(cchDst) char16_t* dst, charcount_t& cchDst, __in_ecount(cch) const char16_t* ptr, charcount_t cch);
        };

    private:
        WritableString* m_string;
#if DBG
        static const char16_t k_dbgFill = _u('\xCDCD');
#endif
    };

    // Needed by diagnostics vtable access
    typedef BufferStringBuilder::WritableString BufferStringBuilderWritableString;

} // namespace Js

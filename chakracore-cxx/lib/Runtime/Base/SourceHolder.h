//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once
#include "DataStructures/CharacterBuffer.h"

namespace Js
{
    class SimpleSourceHolder;
    class ISourceHolder : public FinalizableObject
    {
    private:
        static SimpleSourceHolder const emptySourceHolder;
        static LPCUTF8 const emptyString;

    public:
        static ISourceHolder *GetEmptySourceHolder()
        {
            return (ISourceHolder *)&emptySourceHolder;
        }

        virtual LPCUTF8 GetSource(const char16_t* reasonString) = 0;
        virtual size_t GetByteLength(const char16_t* reasonString) = 0;
        virtual bool Equals(ISourceHolder* other) = 0;
        virtual hash_t GetHashCode() = 0;
        virtual bool IsEmpty() = 0;
        virtual bool IsDeferrable() = 0;
        virtual void Unload() = 0;
    };

    class SimpleSourceHolder : public ISourceHolder
    {
        friend class ISourceHolder;
    protected:
        typename WriteBarrierFieldTypeTraits<LPCUTF8>::Type source;
        typename WriteBarrierFieldTypeTraits<size_t>::Type byteLength;
        typename WriteBarrierFieldTypeTraits<bool>::Type isEmpty;

        SimpleSourceHolder(LPCUTF8 source, _no_write_barrier_tag, size_t byteLength, bool isEmpty)
            : source(source, _no_write_barrier_tag()),
            byteLength(byteLength),
            isEmpty(isEmpty)
        {
        }

    public:
        SimpleSourceHolder(LPCUTF8 source, size_t byteLength)
            : source(source),
            byteLength(byteLength),
            isEmpty(false)
        {
        }

        LPCUTF8 GetSource(const char16_t* reasonString) override
        {
            return source;
        }

        size_t GetByteLength(const char16_t* reasonString) override { return byteLength; }

        bool Equals(ISourceHolder* other) override
        {
          const char16_t* reason = u"Equal Comparison";
            return this == other ||
                (this->GetByteLength(reason) == other->GetByteLength(reason)
                    && (this->GetSource(reason) == other->GetSource(reason)
                        || memcmp(this->GetSource(reason), other->GetSource(reason), this->GetByteLength(reason)) == 0 ));
        }

        bool IsEmpty() override
        {
            return this->isEmpty;
        }

        hash_t GetHashCode() override
        {
            Assert(byteLength < std::numeric_limits<uint32_t>::max());
            return JsUtil::CharacterBuffer<utf8char_t>::StaticGetHashCode(source, (charcount_t)byteLength);
        }

        void Finalize(bool isShutdown) override
        {
        }

        void Dispose(bool isShutdown) override
        {
        }

        void Mark(Recycler * recycler) override
        {
        }

        void Unload() override { }

        bool IsDeferrable() override
        {
            return CONFIG_FLAG(DeferLoadingAvailableSource);
        }
    };

    class HeapSourceHolder : public SimpleSourceHolder
    {
    public:
        HeapSourceHolder(LPCUTF8 source, size_t byteLength, uint8_t* originalSourceBuffer):
            SimpleSourceHolder(source, byteLength),
            shouldFreeSource(true),
            originalSourceBuffer(originalSourceBuffer)
        { }

        void Unload() override;
        void Dispose(bool isShutdown) override;

    private:
        bool shouldFreeSource;
        uint8_t* originalSourceBuffer;
    };
}

//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
// Buffer builder is used to layout out binary content which contains offsets
// from one part of the content to another.
// It works in two-pass fashion:
// - Pass one fixes the real offset of each element (BufferBuilder) of the
//   content.
// - Pass two writes the actual content including any relative offset values.
//----------------------------------------------------------------------------

#pragma once

struct _SIMDValue;
typedef _SIMDValue SIMDValue;

namespace Js
{
    // The base buffer builder class
    class BufferBuilder
    {
    protected:
        const char16_t* clue;
        BufferBuilder(const char16_t* clue)
            : clue(clue), offset(0xffffffff) { }
    public:
        uint32_t offset;
        virtual uint32_t FixOffset(uint32_t offset) = 0;
        virtual void Write(__in_bcount(bufferSize) byte * buffer, uint32_t bufferSize) const = 0;
#if DBG
    protected:
        void TraceOutput(byte * buffer, uint32_t size) const;
#endif
    };

#define VARIABLE_INT_TAGBIT_COUNT (1)
#define VARIABLE_INT_BYTE_SHIFT (8 - VARIABLE_INT_TAGBIT_COUNT)
#define VARIABLE_INT_BYTE_MAX (1 << VARIABLE_INT_BYTE_SHIFT)
#define VARIABLE_INT_BYTE_MASK ~((byte) VARIABLE_INT_BYTE_MAX)
#define SENTINEL_BYTE_COUNT 1

#define ONE_BYTE_MAX (static_cast<byte>(0xfd))
#define TWO_BYTE_MAX (static_cast<uint16>(0xffff))

#define TWO_BYTE_SENTINEL   ONE_BYTE_MAX + 1
#define FOUR_BYTE_SENTINEL  ONE_BYTE_MAX + 2

#define MIN_SENTINEL TWO_BYTE_SENTINEL

#if INSTRUMENT_BUFFER_INTS
    static uint Counts[] = { 0, 0, 0, 0 };
#endif

    // Templatized buffer builder for writing fixed-size content
    template<typename T, bool useVariableIntEncoding>
    struct BufferBuilderOf : BufferBuilder
    {
        typedef T value_type;
        value_type value;

        BufferBuilderOf(const char16_t* clue, const T & value)
            : BufferBuilder(clue), value(value)
        { }

        //  Assume that the value is 0- for negative values of value, we'll just use the default encoding
        bool UseOneByte() const
        {
            return value >= 0 && value <= ONE_BYTE_MAX;
        }

        bool UseTwoBytes() const
        {
            return value > ONE_BYTE_MAX && value <= TWO_BYTE_MAX;
        }

        uint32_t FixOffset(uint32_t offset) override
        {
            this->offset = offset;

            if (useVariableIntEncoding)
            {
                if (UseOneByte())
                {
                    return this->offset + sizeof(byte);
                }
                else if (UseTwoBytes())
                {
                    return this->offset + sizeof(uint16) + SENTINEL_BYTE_COUNT;
                }

                return this->offset + sizeof(T) + SENTINEL_BYTE_COUNT;
            }
            else
            {
                return this->offset + sizeof(T);
            }
        }

        void Write(__in_bcount(bufferSize) byte * buffer, uint32_t bufferSize) const
        {
            DebugOnly(uint32_t size = sizeof(T));

#if INSTRUMENT_BUFFER_INTS
            if (value < ((1 << 8)))
            {
                Counts[0]++;
            }
            else if (value < ((1 << 16)))
            {
                Counts[1]++;
            }
            else if (value < ((1 << 24)))
            {
                Counts[2]++;
            }
            else
            {
                Counts[3]++;
            }
#endif

            if (useVariableIntEncoding)
            {
                if (UseOneByte())
                {
                    if (bufferSize - this->offset < sizeof(byte))
                    {
                        Throw::FatalInternalError();
                    }
                    DebugOnly(size = sizeof(byte));
                    *static_cast<byte*>(buffer + this->offset) = static_cast<byte>(value);
                }
                else if (UseTwoBytes())
                {
                    if (bufferSize - this->offset < sizeof(uint16) + sizeof(byte))
                    {
                        Throw::FatalInternalError();
                    }
                    DebugOnly(size = sizeof(uint16) + 1);
                    *static_cast<byte*>(buffer + this->offset) = TWO_BYTE_SENTINEL;
                    *reinterpret_cast<uint16*>(buffer + this->offset + SENTINEL_BYTE_COUNT) = static_cast<uint16>(this->value);
                }
                else
                {
                    if (bufferSize - this->offset < sizeof(T) + sizeof(byte))
                    {
                        Throw::FatalInternalError();
                    }
                    *static_cast<byte*>(buffer + this->offset) = FOUR_BYTE_SENTINEL;
                    *reinterpret_cast<T*>(buffer + this->offset + SENTINEL_BYTE_COUNT) = this->value;
                    DebugOnly(size = sizeof(T) + 1);
#if INSTRUMENT_BUFFER_INTS
                    Output::Print(u"[BCGENSTATS] %d, %d\n", value, sizeof(T));
#endif
                }
            }
            else
            {
                if (bufferSize - this->offset<sizeof(T))
                {
                    Throw::FatalInternalError();
                }
                *reinterpret_cast<T*>(buffer + this->offset) = value;
            }
            DebugOnly(TraceOutput(buffer, size));
        }
    };

    template<typename T>
    struct BufferBuilderOf<T, false>: BufferBuilder
    {
        typedef T value_type;
        value_type value;

        BufferBuilderOf(const char16_t* clue, const T & value)
            : BufferBuilder(clue), value(value)
        { }

        uint32_t FixOffset(uint32_t offset) override
        {
            this->offset = offset;

            return this->offset + sizeof(T);
        }

        void Write(__in_bcount(bufferSize) byte * buffer, uint32_t bufferSize) const
        {
            if (bufferSize - this->offset<sizeof(T))
            {
                Throw::FatalInternalError();
            }

            *reinterpret_cast<T*>(buffer + this->offset) = value;
            DebugOnly(TraceOutput(buffer, sizeof(T)));
        }
    };


    template <typename T>
    struct ConstantSizedBufferBuilderOf : BufferBuilderOf<T, false>
    {
        ConstantSizedBufferBuilderOf(const char16_t* clue, const T & value)
        : BufferBuilderOf<T, false>(clue, value)
        { }
    };

    typedef BufferBuilderOf<int16, true> BufferBuilderInt16;
    typedef BufferBuilderOf<int, true> BufferBuilderInt32;
    typedef ConstantSizedBufferBuilderOf<byte> BufferBuilderByte;
    typedef ConstantSizedBufferBuilderOf<float> BufferBuilderFloat;
    typedef ConstantSizedBufferBuilderOf<double> BufferBuilderDouble;
    typedef ConstantSizedBufferBuilderOf<SIMDValue> BufferBuilderSIMD;

    // A buffer builder which contains a list of buffer builders
    struct BufferBuilderList : BufferBuilder
    {
        regex::ImmutableList<BufferBuilder*> * list;
        BufferBuilderList(const char16_t* clue)
            : BufferBuilder(clue), list(nullptr)
        { }

        uint32_t FixOffset(uint32_t offset) override
        {
            this->offset = offset;
            return list->Accumulate(offset,[](uint32_t size, BufferBuilder * builder)->uint32_t {
                return builder->FixOffset(size);
            });
        }

        void Write(__in_bcount(bufferSize) byte * buffer, uint32_t bufferSize) const
        {
            return list->Iterate([&](BufferBuilder * builder) {
                builder->Write(buffer, bufferSize);
            });
        }

    };

    // A buffer builder which points to another buffer builder.
    // At write time, it will write the offset from the start of the raw buffer to
    // the pointed-to location.
    struct BufferBuilderRelativeOffset : BufferBuilder
    {
        BufferBuilder * pointsTo;
        uint32_t additionalOffset;
        BufferBuilderRelativeOffset(const char16_t* clue, BufferBuilder * pointsTo, uint32_t additionalOffset)
            : BufferBuilder(clue), pointsTo(pointsTo), additionalOffset(additionalOffset)
        { }
        BufferBuilderRelativeOffset(const char16_t* clue, BufferBuilder * pointsTo)
            : BufferBuilder(clue), pointsTo(pointsTo), additionalOffset(0)
        { }

        uint32_t FixOffset(uint32_t offset) override
        {
            this->offset = offset;
            return this->offset + sizeof(int);
        }

        void Write(__in_bcount(bufferSize) byte * buffer, uint32_t bufferSize) const
        {
            if (bufferSize - this->offset<sizeof(int))
            {
                Throw::FatalInternalError();
            }

            int offsetOfPointedTo = pointsTo->offset;
            *reinterpret_cast<int*>(buffer + this->offset) = offsetOfPointedTo + additionalOffset;
            DebugOnly(TraceOutput(buffer, sizeof(int)));
        }
    };

    // A buffer builder which holds a raw byte buffer
    struct BufferBuilderRaw : BufferBuilder
    {
        uint32_t size;
        const byte * raw;
        BufferBuilderRaw(const char16_t* clue, uint32_t size, __in_bcount(size) const byte * raw)
            : BufferBuilder(clue), size(size), raw(raw)
        { }

        uint32_t FixOffset(uint32_t offset) override
        {
            this->offset = offset;
            return this->offset + size;
        }

        void Write(__in_bcount(bufferSize) byte * buffer, uint32_t bufferSize) const
        {
            if (bufferSize - this->offset<size)
            {
                Throw::FatalInternalError();
            }

            js_memcpy_s(buffer + this->offset, bufferSize-this->offset, raw, size);
            DebugOnly(TraceOutput(buffer, size));
        }
    };

    // A buffer builder which aligns its contents to the specified alignment
    struct BufferBuilderAligned : BufferBuilder
    {
        BufferBuilder * content;
        uint32_t alignment;
        uint32_t padding;

        BufferBuilderAligned(const char16_t* clue, BufferBuilder * content, uint32_t alignment)
            : BufferBuilder(clue), content(content), alignment(alignment), padding(0)
        { }

        uint32_t FixOffset(uint32_t offset) override
        {
            this->offset = offset;

            // Calculate padding
            offset = ::Math::Align(this->offset, this->alignment);
            this->padding = offset - this->offset;
            Assert(this->padding < this->alignment);

            return content->FixOffset(offset);
        }

        void Write(__in_bcount(bufferSize) byte * buffer, uint32_t bufferSize) const
        {
            if (bufferSize - this->offset < this->padding)
            {
                Throw::FatalInternalError();
            }

            for (uint32_t i = 0; i < this->padding; i++)
            {
                buffer[this->offset + i] = 0;
            }

            this->content->Write(buffer, bufferSize);
        }
    };

}

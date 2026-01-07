//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

namespace Js
{
    class SparseArraySegmentBase
    {
    public:
        static const uint32_t MaxLength;

        Field(uint32_t) left; // TODO: (leish)(swb) this can easily be recycler false positive on x86, or on x64 if combine with length field
                            // find a way to either tag this or find a better solution
        Field(uint32_t) length; //we use length instead of right so that we can denote a segment is empty
        Field(uint32_t) size;
        Field(SparseArraySegmentBase*) next;

        static const uint32_t CHUNK_SIZE = 16;
        static const uint32_t HEAD_CHUNK_SIZE = 16;
        static const uint32_t INLINE_CHUNK_SIZE = 64; // Max number of elements in a segment that is initialized inline within the array.
        static const uint32_t SMALL_CHUNK_SIZE = 4;
        static const uint32_t BigLeft = 1 << 20;

        SparseArraySegmentBase(uint32_t left, uint32_t length, uint32_t size);

        bool    HasIndex(uint32_t index) { return (left <= index) && index < (left + length); };

        uint32_t  RemoveUndefined(ScriptContext* scriptContext); //returns count of undefined removed
        void    EnsureSizeInBound();
        void    CheckLengthvsSize() { AssertOrFailFast(this->length <= this->size); }

        static uint32_t GetOffsetOfLeft() { return offsetof(SparseArraySegmentBase, left); }
        static uint32_t GetOffsetOfLength() { return offsetof(SparseArraySegmentBase, length); }
        static uint32_t GetOffsetOfSize() { return offsetof(SparseArraySegmentBase, size); }
        static uint32_t GetOffsetOfNext() { return offsetof(SparseArraySegmentBase, next); }

        static bool DoNativeArrayLeafSegment() { return !PHASE_OFF1(Js::NativeArrayLeafSegmentPhase); }
        static bool IsLeafSegment(SparseArraySegmentBase *seg, Recycler *recycler);

    protected:
        static void EnsureSizeInBound(uint32_t left, uint32_t length, uint32_t& size, SparseArraySegmentBase* next);
    };

    template<typename T>
    class SparseArraySegment : public SparseArraySegmentBase
    {
    public:
        SparseArraySegment(uint32_t left, uint32_t length, uint32_t size) :
            SparseArraySegmentBase(left, length, size) {}

        Field(T) elements[]; // actual elements will follow this determined by size

        void FillSegmentBuffer(uint start, uint size);
        T GetElement(uint32_t index);
        void SetElement(Recycler *recycler, uint32_t index, T value); // sets elements within the segment
        void RemoveElement(Recycler *recycler, uint32_t index); // NOTE: RemoveElement calls memmove, for perf reasons use SetElement(index, null)

        SparseArraySegment<T> *GrowBy(Recycler *recycler, uint32_t n);

        SparseArraySegment<T>* GrowByMin(Recycler *recycler, uint32_t minValue);
        SparseArraySegment<T>* GrowByMinMax(Recycler *recycler, uint32_t minValue, uint32_t maxValue);
        SparseArraySegment<T>* GrowFrontByMax(Recycler *recycler, uint32_t n);

        void ReverseSegment(Recycler *recycler);
        void    Truncate(uint32_t index);

        //following will change the current segment allocation
        SparseArraySegment<T> *SetElementGrow(Recycler *recycler, SparseArraySegmentBase* prev, uint32_t index, T value);

        static SparseArraySegment<T> *AllocateLiteralHeadSegment(Recycler *const recycler, const uint32_t length);
        static SparseArraySegment<T> * AllocateSegment(Recycler* recycler, uint32_t left, uint32_t length, SparseArraySegmentBase *nextSeg);
        static SparseArraySegment<T> * AllocateSegment(Recycler* recycler, uint32_t left, uint32_t length, uint32_t size, SparseArraySegmentBase *nextSeg);
        static SparseArraySegment<T> * AllocateSegment(Recycler* recycler, SparseArraySegmentBase* prev, uint32_t index);
        template<bool isLeaf>
        static SparseArraySegment<T> * AllocateSegmentImpl(Recycler* recycler, uint32_t left, uint32_t length, SparseArraySegmentBase *nextSeg);

        template<bool isLeaf>
        static SparseArraySegment<T> * AllocateSegmentImpl(Recycler* recycler, uint32_t left, uint32_t length, uint32_t size, SparseArraySegmentBase *nextSeg);

        template<bool isLeaf>
        static SparseArraySegment<T> * AllocateSegmentImpl(Recycler* recycler, SparseArraySegmentBase* prev, uint32_t index);

        template<bool isLeaf>
        static SparseArraySegment<T> *AllocateLiteralHeadSegmentImpl(Recycler *const recycler, const uint32_t length);

        static void ClearElements(__out_ecount(len) Field(T)* elements, uint32_t len);
        static SparseArraySegment<T>* CopySegment(Recycler *recycler, SparseArraySegment<T>* dst, uint32_t dstIndex, SparseArraySegment<T>* src, uint32_t srcIndex, uint32_t inputLen);

        static T GetMissingItem();
        static Var GetMissingItemVar();
        static bool IsMissingItem(const T* value);

        template <class S>
        static bool IsMissingItem(const WriteBarrierPtr<S>* value)
        {
            return IsMissingItem(AddressOf(value[0]));
        }

        static uint32_t GetAlignedSize(uint32_t size);

        static inline SparseArraySegment* From(SparseArraySegmentBase* seg)
        {
            return static_cast<SparseArraySegment*>(seg);
        }

        static inline Field(SparseArraySegment*)* AddressFrom(Field(SparseArraySegmentBase*) *addr)
        {
            return reinterpret_cast<Field(SparseArraySegment*)*>(addr);
        }

    private:
        template<bool isLeaf>
        static SparseArraySegment<T>* Allocate(Recycler* recycler, uint32_t left, uint32_t length, uint32_t size, uint32_t fillStart = 0);

        template<bool isLeaf>
        SparseArraySegment<T> *GrowByImpl(Recycler *recycler, uint32_t n);

        template<bool isLeaf>
        SparseArraySegment<T>* GrowFrontByMaxImpl(Recycler *recycler, uint32_t n);

        uint32_t GetGrowByFactor();
    };

    template<typename T>
    T SparseArraySegment<T>::GetMissingItem()
    {
        return (T)JavascriptArray::MissingItem;
    }

    template<>
    inline int32 SparseArraySegment<int32>::GetMissingItem()
    {
        return IntMissingItemPattern;
    }

    template<>
    inline double SparseArraySegment<double>::GetMissingItem()
    {
        return *(double*)&FloatMissingItemPattern;
    }

    template<typename T>
    Var SparseArraySegment<T>::GetMissingItemVar()
    {
        return JavascriptArray::MissingItem;
    }
    template<> Var SparseArraySegment<int32>::GetMissingItemVar();
    template<> Var SparseArraySegment<double>::GetMissingItemVar();

    template<>
    inline bool SparseArraySegment<double>::IsMissingItem(const double* value)
    {
        return *(unsigned long*)value == FloatMissingItemPattern;
    }

    template<typename T>
    bool SparseArraySegment<T>::IsMissingItem(const T* value)
    {
        return *value == SparseArraySegment<T>::GetMissingItem();
    }
} // namespace Js

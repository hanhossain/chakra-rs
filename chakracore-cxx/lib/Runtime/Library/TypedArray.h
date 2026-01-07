//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Copyright (c) ChakraCore Project Contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
//  Implements typed array.
//----------------------------------------------------------------------------
#pragma once

namespace Js
{
    typedef Var (*PFNCreateTypedArray)(Js::ArrayBufferBase* arrayBuffer, uint32_t offSet, uint32_t mappedLength, Js::JavascriptLibrary* javascriptLibrary);

    template<typename T> bool TypedArrayCompareElementsHelper(JavascriptArray::CompareVarsInfo* cvInfo, const void* elem1, const void* elem2);

    class TypedArrayBase : public ArrayBufferParent
    {
        friend ArrayBuffer;
        friend ArrayBufferBase;

    protected:
        DEFINE_VTABLE_CTOR_ABSTRACT(TypedArrayBase, ArrayBufferParent);

    public:
        static JavascriptFunction* GetDefaultConstructor(Var object, ScriptContext* scriptContext);

        class EntryInfo
        {
        public:
            static FunctionInfo NewInstance;
            static FunctionInfo Set;
            static FunctionInfo Subarray;

            static FunctionInfo From;
            static FunctionInfo Of;
            static FunctionInfo At;
            static FunctionInfo CopyWithin;
            static FunctionInfo Entries;
            static FunctionInfo Every;
            static FunctionInfo Fill;
            static FunctionInfo Filter;
            static FunctionInfo Find;
            static FunctionInfo FindIndex;
            static FunctionInfo FindLast;
            static FunctionInfo FindLastIndex;
            static FunctionInfo ForEach;
            static FunctionInfo IndexOf;
            static FunctionInfo Includes;
            static FunctionInfo Join;
            static FunctionInfo Keys;
            static FunctionInfo LastIndexOf;
            static FunctionInfo Map;
            static FunctionInfo Reduce;
            static FunctionInfo ReduceRight;
            static FunctionInfo Reverse;
            static FunctionInfo Slice;
            static FunctionInfo Some;
            static FunctionInfo Sort;
            static FunctionInfo Values;

            static FunctionInfo GetterBuffer;
            static FunctionInfo GetterByteLength;
            static FunctionInfo GetterByteOffset;
            static FunctionInfo GetterLength;
            static FunctionInfo GetterSymbolToStringTag;
            static FunctionInfo GetterSymbolSpecies;
        };

        TypedArrayBase(ArrayBufferBase* arrayBuffer, uint byteOffset, uint mappedLength, uint elementSize, DynamicType* type);

        static Var NewInstance(RecyclableObject* function, CallInfo callInfo, ...);

        static Var EntrySet(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntrySubarray(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryFrom(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryOf(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryAt(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryCopyWithin(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryEntries(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryEvery(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryFill(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryFilter(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryFind(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryFindIndex(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryFindLast(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryFindLastIndex(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryForEach(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryIndexOf(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryIncludes(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryJoin(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryKeys(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryLastIndexOf(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryMap(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryReduce(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryReduceRight(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryReverse(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntrySlice(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntrySome(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntrySort(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryValues(RecyclableObject* function, CallInfo callInfo, ...);

        static Var EntryGetterBuffer(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryGetterByteLength(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryGetterByteOffset(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryGetterLength(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryGetterSymbolToStringTag(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryGetterSymbolSpecies(RecyclableObject* function, CallInfo callInfo, ...);

        virtual DescriptorFlags GetSetter(PropertyId propertyId, Var *setterValue, PropertyValueInfo* info, ScriptContext* requestContext) override;
        virtual DescriptorFlags GetSetter(JavascriptString* propertyNameString, Var *setterValue, PropertyValueInfo* info, ScriptContext* requestContext) override;
        virtual DescriptorFlags GetItemSetter(uint32_t index, Var* setterValue, ScriptContext* requestContext) override;
        virtual PropertyQueryFlags HasPropertyQuery(Js::PropertyId propertyId, _Inout_opt_ PropertyValueInfo* info) override;
        virtual BOOL HasOwnProperty(Js::PropertyId propertyId) override;
        virtual PropertyQueryFlags GetPropertyQuery(Js::Var originalInstance, Js::PropertyId propertyId, Js::Var* value, Js::PropertyValueInfo* info, Js::ScriptContext* requestContext) override;
        virtual PropertyQueryFlags GetPropertyQuery(Js::Var originalInstance, Js::JavascriptString* propertyNameString, Js::Var* value, Js::PropertyValueInfo* info, Js::ScriptContext* requestContext) override;
        virtual PropertyQueryFlags GetPropertyReferenceQuery(Js::Var originalInstance, Js::PropertyId propertyId, Js::Var* value, Js::PropertyValueInfo* info, Js::ScriptContext* requestContext) override;
        virtual PropertyQueryFlags HasItemQuery(uint32_t index) override;
        virtual BOOL DeleteItem(uint32_t index, Js::PropertyOperationFlags flags) override;
        virtual PropertyQueryFlags GetItemQuery(Js::Var originalInstance, uint32_t index, Js::Var* value, Js::ScriptContext * requestContext) override;
        virtual BOOL SetItem(uint32_t index, Js::Var value, Js::PropertyOperationFlags flags = PropertyOperation_None) override;
        virtual BOOL SetProperty(Js::PropertyId propertyId, Js::Var value, Js::PropertyOperationFlags flags, Js::PropertyValueInfo* info) override;
        virtual BOOL SetProperty(Js::JavascriptString* propertyNameString, Js::Var value, Js::PropertyOperationFlags flags, Js::PropertyValueInfo* info) override;
        virtual BOOL DeleteProperty(Js::PropertyId propertyId, Js::PropertyOperationFlags flags) override;
        virtual BOOL DeleteProperty(JavascriptString *propertyNameString, Js::PropertyOperationFlags flags) override;
        virtual PropertyQueryFlags GetItemReferenceQuery(Js::Var originalInstance, uint32_t index, Js::Var* value, Js::ScriptContext * requestContext) override;
        virtual BOOL GetEnumerator(JavascriptStaticEnumerator * enumerator, EnumeratorFlags flags, ScriptContext* requestContext, EnumeratorCache * enumeratorCache = nullptr) override;
        virtual JavascriptEnumerator * GetIndexEnumerator(EnumeratorFlags flags, ScriptContext * requestContext) override;

        virtual BOOL IsEnumerable(PropertyId propertyId)  override;
        virtual BOOL IsConfigurable(PropertyId propertyId)  override;
        virtual BOOL IsWritable(PropertyId propertyId)  override;
        virtual BOOL SetEnumerable(PropertyId propertyId, BOOL value) override;
        virtual BOOL SetWritable(PropertyId propertyId, BOOL value) override;
        virtual BOOL SetConfigurable(PropertyId propertyId, BOOL value) override;
        virtual BOOL SetAttributes(PropertyId propertyId, PropertyAttributes attributes) override;
        virtual BOOL SetAccessors(PropertyId propertyId, Var getter, Var setter, PropertyOperationFlags flags) override;

        virtual BOOL InitProperty(Js::PropertyId propertyId, Js::Var value, PropertyOperationFlags flags = PropertyOperation_None, Js::PropertyValueInfo* info = NULL) override;
        virtual BOOL SetPropertyWithAttributes(PropertyId propertyId, Var value, PropertyAttributes attributes, PropertyValueInfo* info, PropertyOperationFlags flags = PropertyOperation_None, SideEffects possibleSideEffects = SideEffects_Any) override;
        static BOOL Is(TypeId typeId);
        // Returns false if this is not a TypedArray or it's not detached
        static BOOL IsDetachedTypedArray(Var aValue);
        static int32_t GetBuffer(Var aValue, ArrayBuffer** outBuffer, uint32_t* outOffset, uint32_t* outLength);
        static TypedArrayBase * ValidateTypedArray(Var aValue, ScriptContext *scriptContext, const char16_t* apiName);
        static TypedArrayBase * ValidateTypedArray(Arguments &args, ScriptContext *scriptContext, const char16_t* apiName);
        static Var TypedArrayCreate(Var constructor, Arguments *args, uint32_t length, ScriptContext *scriptContext);

        virtual BOOL DirectSetItem(uint32_t index, Js::Var value) = 0;
        virtual BOOL DirectSetItemNoSet(uint32_t index, Js::Var value) = 0;
        virtual Var  DirectGetItem(uint32_t index) = 0;
        virtual BOOL DirectSetItemNoDetachCheck(uint32_t index, Js::Var value) = 0;
        virtual Var  DirectGetItemNoDetachCheck(uint32_t index) = 0;

        virtual Var TypedAdd(uint32_t index, Var second) = 0;
        virtual Var TypedAnd(uint32_t index, Var second) = 0;
        virtual Var TypedLoad(uint32_t index) = 0;
        virtual Var TypedOr(uint32_t index, Var second) = 0;
        virtual Var TypedStore(uint32_t index, Var second) = 0;
        virtual Var TypedSub(uint32_t index, Var second) = 0;
        virtual Var TypedXor(uint32_t index, Var second) = 0;
        virtual Var TypedExchange(uint32_t index, Var second) = 0;
        virtual Var TypedCompareExchange(uint32_t index, Var comparand, Var replacementValue) = 0;

        uint32_t GetByteLength() const { return length * BYTES_PER_ELEMENT; }
        uint32_t GetByteOffset() const { return byteOffset; }
        uint32_t GetBytesPerElement() const { return BYTES_PER_ELEMENT; }
        byte*  GetByteBuffer() const { return buffer; };
        bool IsDetachedBuffer() const { return this->GetArrayBuffer()->IsDetached(); }
        void ClearLengthAndBufferOnDetach();
        static Var CommonSet(Arguments& args);
        static Var CommonSubarray(Arguments& args);

        void SetObject(RecyclableObject* arraySource, uint32_t targetLength, uint32_t offset = 0);
        void SetObjectNoDetachCheck(RecyclableObject* arraySource, uint32_t targetLength, uint32_t offset = 0);
        void Set(TypedArrayBase* typedArraySource, uint32_t offset = 0);

        virtual BOOL GetDiagValueString(StringBuilder<ArenaAllocator>* stringBuilder, ScriptContext* requestContext) override;
        virtual BOOL GetDiagTypeString(StringBuilder<ArenaAllocator>* stringBuilder, ScriptContext* requestContext) override;

        static bool TryGetLengthForOptimizedTypedArray(const Var var, uint32_t *const lengthRef, TypeId *const typeIdRef);
        BOOL ValidateIndexAndDirectSetItem(Js::Var index, Js::Var value, bool * isNumericIndex);
        uint32_t ValidateAndReturnIndex(Js::Var index, bool * skipOperation, bool * isNumericIndex);

        // objectArray support
        virtual BOOL SetItemWithAttributes(uint32_t index, Var value, PropertyAttributes attributes) override;
        virtual BOOL IsObjectArrayFrozen() override;

        Var FindMinOrMax(Js::ScriptContext * scriptContext, TypeId typeId, bool findMax);
        template<typename T, bool checkNaNAndNegZero> Var FindMinOrMax(Js::ScriptContext * scriptContext, bool findMax);

        static Var GetKeysEntriesValuesHelper(Arguments& args, ScriptContext *scriptContext, const char16_t* apiName, JavascriptArrayIteratorKind kind);

        static uint32_t GetFromIndex(Var arg, uint32_t length, ScriptContext *scriptContext);

    private:
        uint32_t GetSourceLength(RecyclableObject* arraySource, uint32_t targetLength, uint32_t offset);

    protected:
        static Var CreateNewInstanceFromIterator(RecyclableObject *iterator, ScriptContext *scriptContext, uint32_t elementSize, PFNCreateTypedArray pfnCreateTypedArray);
        static Var CreateNewInstance(Arguments& args, ScriptContext* scriptContext, uint32_t elementSize, PFNCreateTypedArray pfnCreateTypedArray );
        static bool ArrayIteratorPrototypeHasUserDefinedNext(ScriptContext *scriptContext);
        static BOOL CanonicalNumericIndexString(PropertyId propertyId, ScriptContext *scriptContext);
        static BOOL CanonicalNumericIndexString(JavascriptString *propertyString, ScriptContext *scriptContext);

        virtual void SortHelper(byte* listBuffer, uint32_t length, RecyclableObject* compareFn, ScriptContext* scriptContext, ArenaAllocator* allocator) = 0;

        virtual Var Subarray(uint32_t begin, uint32_t end) = 0;
        Field(int32_t) BYTES_PER_ELEMENT;
        Field(uint32_t) byteOffset;
        FieldNoBarrier(uint8_t*) buffer;   // beginning of mapped array.

    public:
        static uint32_t GetOffsetOfBuffer()  { return offsetof(TypedArrayBase, buffer); }
        static uint32_t GetOffsetOfLength()  { return offsetof(TypedArrayBase, length); }

#if ENABLE_TTD
    public:
        virtual TTD::NSSnapObjects::SnapObjectType GetSnapTag_TTD() const override;
        virtual void ExtractSnapObjectDataInto(TTD::NSSnapObjects::SnapObject* objData, TTD::SlabAllocator& alloc) override;
#endif
    };

    template <> inline bool VarIsImpl<TypedArrayBase>(RecyclableObject* obj)
    {
        return TypedArrayBase::Is(JavascriptOperators::GetTypeId(obj));
    }

    template <typename TypeName, bool clamped = false, bool virtualAllocated = false>
    class TypedArray;

    // These are referenced in the TypedArray implementation, so we need to forward-typedef these.

    typedef TypedArray<int8_t>                Int8Array;
    typedef TypedArray<uint8_t,false>         Uint8Array;
    typedef TypedArray<uint8_t,true>          Uint8ClampedArray;
    typedef TypedArray<int16>               Int16Array;
    typedef TypedArray<uint16>              Uint16Array;
    typedef TypedArray<int32_t>               Int32Array;
    typedef TypedArray<uint32_t>              Uint32Array;
    typedef TypedArray<float>               Float32Array;
    typedef TypedArray<double>              Float64Array;
    typedef TypedArray<long>               Int64Array;
    typedef TypedArray<unsigned long>              Uint64Array;
    typedef TypedArray<bool>                BoolArray;
    typedef TypedArray<int8_t, false, true>   Int8VirtualArray;
    typedef TypedArray<uint8_t, false, true>  Uint8VirtualArray;
    typedef TypedArray<uint8_t, true, true>   Uint8ClampedVirtualArray;
    typedef TypedArray<int16, false, true>  Int16VirtualArray;
    typedef TypedArray<uint16, false, true> Uint16VirtualArray;
    typedef TypedArray<int32_t, false, true>  Int32VirtualArray;
    typedef TypedArray<uint32_t, false, true> Uint32VirtualArray;
    typedef TypedArray<float, false, true>  Float32VirtualArray;
    typedef TypedArray<double, false, true> Float64VirtualArray;


    template <typename TypeName, bool clamped /*= false*/, bool virtualAllocated /*= false*/>
    class TypedArray : public TypedArrayBase
    {
    protected:
        DEFINE_VTABLE_CTOR(TypedArray, TypedArrayBase);
        DEFINE_MARSHAL_OBJECT_TO_SCRIPT_CONTEXT(TypedArray);

        TypedArray(DynamicType *type): TypedArrayBase(nullptr, 0, 0, sizeof(TypeName), type) { buffer = nullptr; }

    public:
        typedef TypeName TypedArrayType;
        class EntryInfo
        {
        public:
            static FunctionInfo NewInstance;
            static FunctionInfo Set;
        };

        TypedArray(ArrayBufferBase* arrayBuffer, uint32_t byteOffset, uint32_t mappedLength, DynamicType* type);

        static Var Create(ArrayBufferBase* arrayBuffer, uint32_t byteOffSet, uint32_t mappedLength, JavascriptLibrary* javascriptLibrary);
        static Var NewInstance(RecyclableObject* function, CallInfo callInfo, ...);

        static Var EntrySet(RecyclableObject* function, CallInfo callInfo, ...);

        Var Subarray(uint32_t begin, uint32_t end);

        static BOOL HasVirtualTableInfo(Var aValue)
        {
            return VirtualTableInfo<TypedArray<TypeName, clamped, virtualAllocated>>::HasVirtualTable(aValue) || VirtualTableInfo<CrossSiteObject<TypedArray<TypeName, clamped, virtualAllocated>>>::HasVirtualTable(aValue);
        }

        inline Var BaseTypedDirectGetItem(uint32_t index)
        {
            if (this->IsDetachedBuffer()) // 9.4.5.8 IntegerIndexedElementGet
            {
                JavascriptError::ThrowTypeError(GetScriptContext(), JSERR_DetachedTypedArray);
            }

            if (index < GetLength())
            {
                Assert((index + 1)* sizeof(TypeName)+GetByteOffset() <= GetArrayBuffer()->GetByteLength());
                TypeName* typedBuffer = (TypeName*)buffer;
                return JavascriptNumber::ToVar(typedBuffer[index], GetScriptContext());
            }
            return GetLibrary()->GetUndefined();
        }

        inline Var TypedDirectGetItemWithCheck(uint32_t index)
        {
            if (this->IsDetachedBuffer()) // 9.4.5.8 IntegerIndexedElementGet
            {
                JavascriptError::ThrowTypeError(GetScriptContext(), JSERR_DetachedTypedArray);
            }

            if (index < GetLength())
            {
                Assert((index + 1)* sizeof(TypeName)+GetByteOffset() <= GetArrayBuffer()->GetByteLength());
                TypeName* typedBuffer = (TypeName*)buffer;
                return JavascriptNumber::ToVarWithCheck(typedBuffer[index], GetScriptContext());
            }
            return GetLibrary()->GetUndefined();
        }

        inline Var BaseTypedDirectGetItemNoDetachCheck(uint32_t index)
        {
            Assert(!IsDetachedBuffer());
            Assert(index < GetLength());
            Assert((index + 1)* sizeof(TypeName) + GetByteOffset() <= GetArrayBuffer()->GetByteLength());
            TypeName* typedBuffer = (TypeName*)buffer;
            return JavascriptNumber::ToVar(typedBuffer[index], GetScriptContext());
        }

        inline Var DirectGetItemVarCheckNoDetachCheck(uint32_t index)
        {
            Assert(!IsDetachedBuffer());
            Assert(index < GetLength());
            Assert((index + 1)* sizeof(TypeName) + GetByteOffset() <= GetArrayBuffer()->GetByteLength());
            TypeName* typedBuffer = (TypeName*)buffer;
            return JavascriptNumber::ToVarWithCheck(typedBuffer[index], GetScriptContext());
        }

        inline BOOL DirectSetItemAtRange(TypedArray *fromArray, int32_t iSrcStart, int32_t iDstStart, uint32_t length, TypeName(*convFunc)(Var value, ScriptContext* scriptContext))
        {
            TypeName* dstBuffer = (TypeName*)buffer;
            TypeName* srcBuffer = (TypeName*)fromArray->buffer;
            Assert(srcBuffer && dstBuffer);
            Assert(length <= ArrayBuffer::MaxArrayBufferLength / sizeof(TypeName));
            // caller checks that src and dst index are the same
            Assert(iSrcStart == iDstStart);

            if (this->IsDetachedBuffer() || fromArray->IsDetachedBuffer())
            {
                JavascriptError::ThrowTypeError(GetScriptContext(), JSERR_DetachedTypedArray);
            }

            // Fixup destination start in case it's negative
            uint32_t start = iDstStart;
            if (iDstStart < 0)
            {
                if ((long)(length) + iDstStart < 0)
                {
                    // nothing to do, all index are no-op
                    return true;
                }

                length += iDstStart;
                start = 0;
            }

            uint32_t dstLength = UInt32Math::Add(start, length) < GetLength() ? length : GetLength() > start ? GetLength() - start : 0;
            uint32_t srcLength = start + length < fromArray->GetLength() ? length : (fromArray->GetLength() > start ? fromArray->GetLength() - start : 0);

            // length is the minimum of length, srcLength and dstLength
            length = length < srcLength ? (length < dstLength ? length : dstLength) : (srcLength < dstLength ? srcLength : dstLength);

            const size_t byteSize = sizeof(TypeName) * length;
            Assert(byteSize >= length); // check for overflow
            js_memcpy_s(dstBuffer + start, dstLength * sizeof(TypeName), srcBuffer + start, byteSize);

            if (dstLength > length)
            {
                TypeName undefinedValue = convFunc(GetLibrary()->GetUndefined(), GetScriptContext());
                for (uint32_t i = length; i < dstLength; i++)
                {
                    dstBuffer[i] = undefinedValue;
                }
            }

            return true;
        }

        inline BOOL DirectSetItemAtRange(int32_t start, uint32_t length, TypeName typedValue)
        {
            if (CrossSite::IsCrossSiteObjectTyped(this))
            {
                return false;
            }

            if (this->IsDetachedBuffer()) // 9.4.5.9 IntegerIndexedElementSet
            {
                JavascriptError::ThrowTypeError(GetScriptContext(), JSERR_DetachedTypedArray);
            }
            uint32_t newStart = start, newLength = length;

            if (start < 0)
            {
                if ((long)(length) + start < 0)
                {
                    // nothing to do, all index are no-op
                    return true;
                }
                newStart = 0;
                // fixup the length with the change
                newLength += start;
            }
            if (newStart >= GetLength())
            {
                // If we want to start copying past the length of the array, all index are no-op
                return true;
            }
            if (UInt32Math::Add(newStart, newLength) > GetLength())
            {
                newLength = GetLength() - newStart;
            }

            TypeName* typedBuffer = (TypeName*)buffer;

            if (typedValue == 0 || sizeof(TypeName) == 1)
            {
                const size_t byteSize = sizeof(TypeName) * newLength;
                Assert(byteSize >= newLength); // check for overflow
                memset(typedBuffer + newStart, (int)typedValue, byteSize);
            }
            else
            {
                for (uint32_t i = 0; i < newLength; i++)
                {
                    typedBuffer[newStart + i] = typedValue;
                }
            }

            return TRUE;
        }

        inline BOOL BaseTypedDirectSetItem(uint32_t index, Js::Var value, TypeName (*convFunc)(Var value, ScriptContext* scriptContext))
        {
            // This call can potentially invoke user code, and may end up detaching the underlying array (this).
            // Therefore it was brought out and above the IsDetached check
            TypeName typedValue = convFunc(value, GetScriptContext());

            if (this->IsDetachedBuffer()) // 9.4.5.9 IntegerIndexedElementSet
            {
                JavascriptError::ThrowTypeError(GetScriptContext(), JSERR_DetachedTypedArray);
            }

            if (index >= GetLength())
            {
                return FALSE;
            }

            AssertMsg(index < GetLength(), "Trying to set out of bound index for typed array.");
            Assert((index + 1)* sizeof(TypeName) + GetByteOffset() <= GetArrayBuffer()->GetByteLength());
            TypeName* typedBuffer = (TypeName*)buffer;

            typedBuffer[index] = typedValue;

            return TRUE;
        }

        inline BOOL BaseTypedDirectSetItemNoSet(uint32_t index, Js::Var value, TypeName (*convFunc)(Var value, ScriptContext* scriptContext))
        {
            // This call can potentially invoke user code, and may end up detaching the underlying array (this).
            // Therefore it was brought out and above the IsDetached check
            convFunc(value, GetScriptContext());

            if (this->IsDetachedBuffer()) // 9.4.5.9 IntegerIndexedElementSet
            {
                JavascriptError::ThrowTypeError(GetScriptContext(), JSERR_DetachedTypedArray);
            }

            return FALSE;
        }

        inline BOOL BaseTypedDirectSetItemNoDetachCheck(uint32_t index, Js::Var value, TypeName(*convFunc)(Var value, ScriptContext* scriptContext))
        {
            TypeName typedValue = convFunc(value, GetScriptContext());

            // The caller of the function made sure that no IsDetached check required.
            // The caller of the function also made sure that no length check required.

            Assert(!IsDetachedBuffer());
            AssertMsg(index < GetLength(), "Trying to set out of bound index for typed array.");
            Assert((index + 1)* sizeof(TypeName) + GetByteOffset() <= GetArrayBuffer()->GetByteLength());
            TypeName* typedBuffer = (TypeName*)buffer;

            typedBuffer[index] = typedValue;

            return TRUE;
        }

        virtual BOOL DirectSetItem(uint32_t index, Js::Var value) override;
        virtual BOOL DirectSetItemNoSet(uint32_t index, Js::Var value) override;
        virtual Var  DirectGetItem(uint32_t index) override;
        virtual BOOL DirectSetItemNoDetachCheck(uint32_t index, Js::Var value) override;
        virtual Var  DirectGetItemNoDetachCheck(uint32_t index) override;
        virtual Var TypedAdd(uint32_t index, Var second) override;
        virtual Var TypedAnd(uint32_t index, Var second) override;
        virtual Var TypedLoad(uint32_t index) override;
        virtual Var TypedOr(uint32_t index, Var second) override;
        virtual Var TypedStore(uint32_t index, Var second) override;
        virtual Var TypedSub(uint32_t index, Var second) override;
        virtual Var TypedXor(uint32_t index, Var second) override;
        virtual Var TypedExchange(uint32_t index, Var second) override;
        virtual Var TypedCompareExchange(uint32_t index, Var comparand, Var replacementValue) override;

        static BOOL DirectSetItem(TypedArray* arr, uint32_t index, Js::Var value)
        {
            AssertMsg(arr != nullptr, "Array shouldn't be nullptr.");

            return arr->DirectSetItem(index, value);
        }

    protected:
        void SortHelper(byte* listBuffer, uint32_t length, RecyclableObject* compareFn, ScriptContext* scriptContext, ArenaAllocator* allocator)
        {
            TypeName* list = reinterpret_cast<TypeName*>(listBuffer);
            JavascriptArray::CompareVarsInfo cvInfo;
            cvInfo.scriptContext = scriptContext;
            cvInfo.compFn = compareFn;
            cvInfo.compareType = &TypedArrayCompareElementsHelper<TypeName>;
            JavascriptArray::TypedArraySort<TypeName>(list, length, &cvInfo, allocator);
        }

    public:
        virtual VTableValue DummyVirtualFunctionToHinderLinkerICF();
    };

    // in windows build environment, char16_t is not an intrinsic type, and we cannot do the type
    // specialization
    class CharArray : public TypedArrayBase
    {
    protected:
        DEFINE_VTABLE_CTOR(CharArray, TypedArrayBase);
        DEFINE_MARSHAL_OBJECT_TO_SCRIPT_CONTEXT(CharArray);

    public:
        class EntryInfo
        {
        public:
            static FunctionInfo NewInstance;
            static FunctionInfo Set;
            static FunctionInfo Subarray;
        };

        static Var EntrySet(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntrySubarray(RecyclableObject* function, CallInfo callInfo, ...);

        CharArray(ArrayBufferBase* arrayBuffer, uint32_t byteOffset, uint32_t mappedLength, DynamicType* type) :
        TypedArrayBase(arrayBuffer, byteOffset, mappedLength, sizeof(char16_t), type)
        {
            AssertMsg(arrayBuffer->GetByteLength() >= byteOffset, "invalid offset");
            AssertMsg(mappedLength*sizeof(char16_t)+byteOffset <= GetArrayBuffer()->GetByteLength(), "invalid length");
            buffer = arrayBuffer->GetBuffer() + byteOffset;
        }

        static Var Create(ArrayBufferBase* arrayBuffer, uint32_t byteOffSet, uint32_t mappedLength, JavascriptLibrary* javascriptLibrary);
        static Var NewInstance(RecyclableObject* function, CallInfo callInfo, ...);

        Var Subarray(uint32_t begin, uint32_t end);

        virtual BOOL DirectSetItem(uint32_t index, Js::Var value) override;
        virtual BOOL DirectSetItemNoSet(uint32_t index, Js::Var value) override;
        virtual Var  DirectGetItem(uint32_t index) override;
        virtual BOOL DirectSetItemNoDetachCheck(uint32_t index, Js::Var value) override;
        virtual Var  DirectGetItemNoDetachCheck(uint32_t index) override;

        virtual Var TypedAdd(uint32_t index, Var second) override;
        virtual Var TypedAnd(uint32_t index, Var second) override;
        virtual Var TypedLoad(uint32_t index) override;
        virtual Var TypedOr(uint32_t index, Var second) override;
        virtual Var TypedStore(uint32_t index, Var second) override;
        virtual Var TypedSub(uint32_t index, Var second) override;
        virtual Var TypedXor(uint32_t index, Var second) override;
        virtual Var TypedExchange(uint32_t index, Var second) override;
        virtual Var TypedCompareExchange(uint32_t index, Var comparand, Var replacementValue) override;

    protected:
        void SortHelper(byte* listBuffer, uint32_t length, RecyclableObject* compareFn, ScriptContext* scriptContext, ArenaAllocator* allocator)
        {
            char16_t* list = reinterpret_cast<char16_t*>(listBuffer);
            JavascriptArray::CompareVarsInfo cvInfo;
            cvInfo.scriptContext = scriptContext;
            cvInfo.compFn = compareFn;
            cvInfo.compareType = &TypedArrayCompareElementsHelper<char16_t>;
            JavascriptArray::TypedArraySort<char16_t>(list, length, &cvInfo, allocator);
        }

    public:
        virtual VTableValue DummyVirtualFunctionToHinderLinkerICF()
        {
            return VTableValue::VtableCharArray;
        }
    };

    template <> bool VarIsImpl<CharArray>(RecyclableObject* obj);

    template <typename TypeName, bool clamped, bool virtualAllocated>
    TypedArray<TypeName, clamped, virtualAllocated>::TypedArray(ArrayBufferBase* arrayBuffer, uint32_t byteOffset, uint32_t mappedLength, DynamicType* type) :
        TypedArrayBase(arrayBuffer, byteOffset, mappedLength, sizeof(TypeName), type)
    {
        AssertMsg(arrayBuffer->GetByteLength() >= byteOffset, "invalid offset");
        AssertMsg(mappedLength*sizeof(TypeName)+byteOffset <= arrayBuffer->GetByteLength(), "invalid length");
        buffer = arrayBuffer->GetBuffer() + byteOffset;
        if (arrayBuffer->IsValidVirtualBufferLength(arrayBuffer->GetByteLength()) &&
             (byteOffset == 0) &&
             (mappedLength == (arrayBuffer->GetByteLength() / sizeof(TypeName)))
           )
        {
            // update the vtable
            switch (type->GetTypeId())
            {
            case TypeIds_Int8Array:
                VirtualTableInfo<Int8VirtualArray>::SetVirtualTable(this);
                break;
            case TypeIds_Uint8Array:
                VirtualTableInfo<Uint8VirtualArray>::SetVirtualTable(this);
                break;
            case TypeIds_Uint8ClampedArray:
                VirtualTableInfo<Uint8ClampedVirtualArray>::SetVirtualTable(this);
                break;
            case TypeIds_Int16Array:
                VirtualTableInfo<Int16VirtualArray>::SetVirtualTable(this);
                break;
            case TypeIds_Uint16Array:
                VirtualTableInfo<Uint16VirtualArray>::SetVirtualTable(this);
                break;
            case TypeIds_Int32Array:
                VirtualTableInfo<Int32VirtualArray>::SetVirtualTable(this);
                break;
            case TypeIds_Uint32Array:
                VirtualTableInfo<Uint32VirtualArray>::SetVirtualTable(this);
                break;
            case TypeIds_Float32Array:
                VirtualTableInfo<Float32VirtualArray>::SetVirtualTable(this);
                break;
            case TypeIds_Float64Array:
                VirtualTableInfo<Float64VirtualArray>::SetVirtualTable(this);
                break;
            default:
                break;
            }
        }
    }

    template <typename TypeName, bool clamped, bool virtualAllocated>
    Var TypedArray<TypeName, clamped, virtualAllocated>::Create(ArrayBufferBase* arrayBuffer, uint32_t byteOffSet, uint32_t mappedLength, JavascriptLibrary* javascriptLibrary)
    {
        uint32_t totalLength, mappedByteLength;

        if (UInt32Math::Mul(mappedLength, sizeof(TypeName), &mappedByteLength) ||
            UInt32Math::Add(byteOffSet, mappedByteLength, &totalLength) ||
            (totalLength > arrayBuffer->GetByteLength()))
        {
            JavascriptError::ThrowRangeError(arrayBuffer->GetScriptContext(), JSERR_InvalidTypedArrayLength);
        }

        DynamicType *type = javascriptLibrary->GetTypedArrayType<TypeName, clamped>(0);
        return RecyclerNew(javascriptLibrary->GetRecycler(), TypedArray, arrayBuffer, byteOffSet, mappedLength, type);
    }

#if defined(__clang__)
// hack for clang message: "...add an explicit instantiation declaration to .."
#define __EXPLICIT_INSTANTINATE_TA(x) \
            template<> FunctionInfo Js::x::EntryInfo::NewInstance;\
            template<> FunctionInfo Js::x::EntryInfo::Set
#else // MSVC
#define __EXPLICIT_INSTANTINATE_TA(x)
#endif

    __EXPLICIT_INSTANTINATE_TA(Int8Array);
    __EXPLICIT_INSTANTINATE_TA(Uint8Array);
    __EXPLICIT_INSTANTINATE_TA(Uint8ClampedArray);
    __EXPLICIT_INSTANTINATE_TA(Int16Array);
    __EXPLICIT_INSTANTINATE_TA(Uint16Array);
    __EXPLICIT_INSTANTINATE_TA(Int32Array);
    __EXPLICIT_INSTANTINATE_TA(Uint32Array);
    __EXPLICIT_INSTANTINATE_TA(Float32Array);
    __EXPLICIT_INSTANTINATE_TA(Float64Array);
    __EXPLICIT_INSTANTINATE_TA(Int64Array);
    __EXPLICIT_INSTANTINATE_TA(Uint64Array);
    __EXPLICIT_INSTANTINATE_TA(BoolArray);
    __EXPLICIT_INSTANTINATE_TA(Int8VirtualArray);
    __EXPLICIT_INSTANTINATE_TA(Uint8VirtualArray);
    __EXPLICIT_INSTANTINATE_TA(Uint8ClampedVirtualArray);
    __EXPLICIT_INSTANTINATE_TA(Int16VirtualArray);
    __EXPLICIT_INSTANTINATE_TA(Uint16VirtualArray);
    __EXPLICIT_INSTANTINATE_TA(Int32VirtualArray);
    __EXPLICIT_INSTANTINATE_TA(Uint32VirtualArray);
    __EXPLICIT_INSTANTINATE_TA(Float32VirtualArray);
    __EXPLICIT_INSTANTINATE_TA(Float64VirtualArray);

#undef __EXPLICIT_INSTANTINATE_TA
}

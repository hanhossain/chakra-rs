//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

namespace Js
{
    class ArgumentsObject : public DynamicObject
    {
    private:
        static PropertyId specialPropertyIds[];

    protected:
        DEFINE_VTABLE_CTOR_ABSTRACT(ArgumentsObject, DynamicObject);
    public:
        ArgumentsObject(DynamicType * type) : DynamicObject(type)
        {
            Assert(type->GetTypeId() == TypeIds_Arguments);
        }

        BOOL GetDiagValueString(StringBuilder<ArenaAllocator>* stringBuilder, ScriptContext* requestContext) override;
        BOOL GetDiagTypeString(StringBuilder<ArenaAllocator>* stringBuilder, ScriptContext* requestContext) override;
        BOOL GetEnumerator(JavascriptStaticEnumerator * enumerator, EnumeratorFlags flags, ScriptContext* requestContext, EnumeratorCache * enumeratorCache = nullptr) override;

        virtual uint32_t GetNumberOfArguments() const = 0;
        virtual uint32_t GetNextFormalArgIndex(uint32_t index, BOOL enumNonEnumerable = FALSE, PropertyAttributes* attributes = nullptr) const = 0;
        virtual Var GetHeapArguments() = 0;
        virtual void SetHeapArguments(HeapArgumentsObject *args) = 0;
        virtual BOOL AdvanceWalkerToArgsFrame(JavascriptStackWalker *walker) = 0;
    };

    template <> inline bool VarIsImpl<ArgumentsObject>(RecyclableObject* obj)
    {
        return JavascriptOperators::GetTypeId(obj) == TypeIds_Arguments;
    }

    class ES5HeapArgumentsObject;

    // The arguments passed to a function are cached as follows:
    //   1) any formal (i.e. named) arguments are copied to the ActivationObject hung off
    //      a HeapArgumentObject where they can be referenced by name
    //   2) any additional actual arguments are stored as an index property on that HeapArgumentObject
    //
    // See JavascriptOperators::LoadHeapArguments for details of HeapArgumentObject creation.
    //
    // If a function has named arguments, the array elements of the Arguments object are synonymous for
    // the local variables that hold the function arguments. This no longer holds true, however, if an
    // array element of the Argument object is deleted.

    class HeapArgumentsObject : public ArgumentsObject
    {
        friend class ::ActiveScriptProfilerHeapEnum;
    protected:
        DEFINE_VTABLE_CTOR(HeapArgumentsObject, ArgumentsObject);

    private:
        // We currently support only 2^24 arguments
        typename WriteBarrierFieldTypeTraits<uint32_t>::Type              numOfArguments:31;
        typename WriteBarrierFieldTypeTraits<uint32_t>::Type              callerDeleted:1;

        friend class Js::CrossSiteObject<HeapArgumentsObject>;
        void MarshalToScriptContext(Js::ScriptContext *scriptContext) override
        {
            Assert(this->GetScriptContext() != scriptContext);
            AssertMsg(VirtualTableInfo<HeapArgumentsObject>::HasVirtualTable(this),
                      "Derived class need to define marshal to script context");
            VirtualTableInfo<Js::CrossSiteObject<HeapArgumentsObject>>::SetVirtualTable(this);
        };

    protected:
        typename WriteBarrierFieldTypeTraits<uint32_t>::Type              formalCount;
        typename WriteBarrierFieldTypeTraits<ActivationObject*>::Type   frameObject;
        typename WriteBarrierFieldTypeTraits<BVSparse<Recycler>*>::Type deletedArgs;

    public:
        HeapArgumentsObject(DynamicType * type);
        HeapArgumentsObject(Recycler *recycler, ActivationObject* obj, uint32_t formalCount, DynamicType * type);
        void SetNumberOfArguments(uint32_t len);

        static HeapArgumentsObject* As(Var aValue);

        BOOL HasItemAt(uint32_t index);
        virtual BOOL GetItemAt(uint32_t index, Var *value, ScriptContext * scriptContext);
        virtual BOOL SetItemAt(uint32_t index, Var value);
        virtual BOOL DeleteItemAt(uint32_t index);

        PropertyQueryFlags HasPropertyQuery(PropertyId propertyId, _Inout_opt_ PropertyValueInfo* info) override;
        PropertyQueryFlags GetPropertyQuery(Var originalInstance, PropertyId propertyId, Var* value, PropertyValueInfo* info, ScriptContext* requestContext) override;
        PropertyQueryFlags GetPropertyQuery(Var originalInstance, JavascriptString* propertyNameString, Var* value, PropertyValueInfo* info, ScriptContext* requestContext) override;
        PropertyQueryFlags GetPropertyReferenceQuery(Var originalInstance, PropertyId propertyId, Var* value, PropertyValueInfo* info, ScriptContext* requestContext) override;
        BOOL SetProperty(PropertyId propertyId, Var value, PropertyOperationFlags flags, PropertyValueInfo* info) override;
        BOOL SetProperty(JavascriptString* propertyNameString, Var value, PropertyOperationFlags flags, PropertyValueInfo* info) override;
        PropertyQueryFlags HasItemQuery(uint32_t index) override;
        PropertyQueryFlags GetItemQuery(Var originalInstance, uint32_t index, Var* value, ScriptContext * requestContext) override;
        PropertyQueryFlags GetItemReferenceQuery(Var originalInstance, uint32_t index, Var* value, ScriptContext * requestContext) override;
        BOOL SetItem(uint32_t index, Var value, PropertyOperationFlags flags) override;
        BOOL DeleteItem(uint32_t index, PropertyOperationFlags flags) override;

        uint32_t GetNumberOfArguments() const override;
        uint32_t GetNextFormalArgIndex(uint32_t index, BOOL enumNonEnumerable = FALSE, PropertyAttributes* attributes = nullptr) const override;
        Var GetHeapArguments() override { return this; }
        void SetHeapArguments(HeapArgumentsObject *args) override {
            AssertMsg(false, "Should never get here");
        }
        BOOL AdvanceWalkerToArgsFrame(JavascriptStackWalker *walker) override;

        BOOL SetConfigurable(PropertyId propertyId, BOOL value) override;
        BOOL SetEnumerable(PropertyId propertyId, BOOL value) override;
        BOOL SetWritable(PropertyId propertyId, BOOL value) override;
        BOOL SetAccessors(PropertyId propertyId, Var getter, Var setter, PropertyOperationFlags flags = PropertyOperation_None) override;
        BOOL SetPropertyWithAttributes(PropertyId propertyId, Var value, PropertyAttributes attributes, PropertyValueInfo* info, PropertyOperationFlags flags = PropertyOperation_None, SideEffects possibleSideEffects = SideEffects_Any) override;
        BOOL PreventExtensions() override;
        BOOL Seal() override;
        BOOL Freeze() override;

        uint32_t GetFormalCount() const
        {
            return this->formalCount;
        }

        void SetFormalCount(uint32_t value)
        {
            this->formalCount = value;
        }

        ES5HeapArgumentsObject* ConvertToUnmappedArgumentsObject(bool overwriteArgsUsingFrameObject = true);
        const ActivationObject* GetFrameObject() { return frameObject; }
        void SetFrameObject(ActivationObject * value)
        {
            AssertMsg(frameObject == nullptr, "Setting the frame object again?");
            Assert(!value || VarIsCorrectType(value));
            frameObject = value;
        }

    private:
        ES5HeapArgumentsObject* ConvertToES5HeapArgumentsObject(bool overwriteArgsUsingFrameObject = true);

    protected:
        BOOL IsFormalArgument(uint32_t index);
        BOOL IsFormalArgument(PropertyId propertyId);
        BOOL IsFormalArgument(PropertyId propertyId, uint32_t* pIndex);    // Checks whether property is numeric, and on success sets that index.
        BOOL IsArgumentDeleted(uint32_t index) const;
    };

    // ES5 version of the HeapArgumentsObject: support for attributes on formal arguments.
    //   - Unless the user deals with SetWritable/Enumerable/Configurable/Accessors, regular HeapArgumentsObject is used.
    //   - When SetWritable/Enumerable/Configurable/Accessors is called, convert HeapArgumentsObject  to ES5HeapArgumentsObject by swapping vtable.
    //   - Override GetItemAt/SetItemAt -- for disconnected items force GetItem/SetItem to use "this" rather than from frameObject.
    //   - Override GetEnumerator -- take care of enumerable = false arguments.
    //   - The "disconnect" happens when (as ES5 spec says) setting writable to false and setting accessors.
    // Note: This implementation depends on v-table swapping so that HeapArgumentsObject instance can be
    //       converted to ES5HeapArgumentsObject at runtime when ES5 attribute/getter/setter support is needed.
    //       Thus, CAUTION: as a result, this class can't add any new instance fields,
    //       as the size of the instance must be same.
    class ES5HeapArgumentsObject : public HeapArgumentsObject
    {
        friend struct AutoObjectArrayItemExistsValidator;
        friend class ES5ArgumentsObjectEnumerator;

        // Helper class to make sure that object array item exists for ES5HeapArgumentsObject
        // and remove the item to roll back to original state if something fails/throws.
        struct AutoObjectArrayItemExistsValidator
        {
            ES5HeapArgumentsObject* m_args;
            bool m_isReleaseItemNeeded;
            uint32_t m_index;

            AutoObjectArrayItemExistsValidator(ES5HeapArgumentsObject* args, uint32_t index);
            ~AutoObjectArrayItemExistsValidator();
        };

    private:
        DEFINE_VTABLE_CTOR(ES5HeapArgumentsObject, HeapArgumentsObject);
        friend class Js::CrossSiteObject<ES5HeapArgumentsObject>;
        void MarshalToScriptContext(Js::ScriptContext *scriptContext) override
        {
            Assert(this->GetScriptContext() != scriptContext);
            AssertMsg(VirtualTableInfo<ES5HeapArgumentsObject>::HasVirtualTable(this),
                      "Derived class need to define marshal to script context");
            VirtualTableInfo<Js::CrossSiteObject<ES5HeapArgumentsObject>>::SetVirtualTable(this);
        };
        uint32_t GetNextFormalArgIndexHelper(uint32_t index, BOOL enumNonEnumerable, PropertyAttributes* attributes = nullptr) const;

    public:
        ES5HeapArgumentsObject(Recycler *recycler, ActivationObject* obj, uint32_t formalCount, DynamicType * type)
            : HeapArgumentsObject(recycler, obj, formalCount, type)
        {
        }

        BOOL SetConfigurable(PropertyId propertyId, BOOL value) override;
        BOOL SetEnumerable(PropertyId propertyId, BOOL value) override;
        BOOL SetWritable(PropertyId propertyId, BOOL value) override;
        BOOL SetAccessors(PropertyId propertyId, Var getter, Var setter, PropertyOperationFlags flags) override;
        BOOL SetPropertyWithAttributes(PropertyId propertyId, Var value, PropertyAttributes attributes, PropertyValueInfo* info, PropertyOperationFlags flags = PropertyOperation_None, SideEffects possibleSideEffects = SideEffects_Any) override;
        BOOL GetEnumerator(JavascriptStaticEnumerator * enumerator, EnumeratorFlags flags, ScriptContext* requestContext, EnumeratorCache * enumeratorCache = nullptr) override;
        BOOL PreventExtensions() override;
        BOOL Seal() override;
        BOOL Freeze() override;

        uint32_t GetNextFormalArgIndex(uint32_t index, BOOL enumNonEnumerable = FALSE, PropertyAttributes* attributes = nullptr) const override;
        BOOL GetItemAt(uint32_t index, Var *value, ScriptContext * scriptContext) override;
        BOOL SetItemAt(uint32_t index, Var value) override;
        BOOL DeleteItemAt(uint32_t index) override;

        void DisconnectFormalFromNamedArgument(uint32_t index);
        BOOL IsFormalDisconnectedFromNamedArgument(uint32_t index);
        BOOL IsEnumerableByIndex(uint32_t index);

        BOOL SetConfigurableForFormal(uint32_t index, PropertyId propertyId, BOOL value);
        BOOL SetEnumerableForFormal(uint32_t index, PropertyId propertyId, BOOL value);
        BOOL SetWritableForFormal(uint32_t index, PropertyId propertyId, BOOL value);
        BOOL SetAccessorsForFormal(uint32_t index, PropertyId propertyId, Var getter, Var setter, PropertyOperationFlags flags = PropertyOperation_None);
        BOOL SetPropertyWithAttributesForFormal(uint32_t index, PropertyId propertyId, Var value, PropertyAttributes attributes, PropertyValueInfo* info, PropertyOperationFlags flags = PropertyOperation_None, SideEffects possibleSideEffects = SideEffects_Any);

   };
}

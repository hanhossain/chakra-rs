//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

namespace Js
{
    class ES5ArrayType : public DynamicType
    {
        friend class ES5Array;

    protected:
        ES5ArrayType(DynamicType * type);
    };
}

namespace Js
{
    //
    // ES5Array supports attribute/getter/setter for index property names.
    //
    // This implementation depends on v-table swapping so that a normal JavascriptArray instance can be
    // converted to an ES5Array at runtime when ES5 attribute/getter/setter support is needed. As a result,
    // this class can't add any new fields to JavascriptArray. The extra index attribute/getter/setter info
    // are maintained in the private ES5ArrayTypeHandler.
    //
    // ES5Array does not reimplement Array.prototype methods. It depends on JavascriptArray implementations
    // to go through generic object route as ES5Array has a new TypeId and is treated as an object.
    //
    class ES5Array : public JavascriptArray
    {
    protected:
        DEFINE_VTABLE_CTOR(ES5Array, JavascriptArray);
        friend class Js::CrossSiteObject<ES5Array>;
        void MarshalToScriptContext(Js::ScriptContext *scriptContext) override
        {
            Assert(this->GetScriptContext() != scriptContext);
            AssertMsg(VirtualTableInfo<ES5Array>::HasVirtualTable(this),
                      "Derived class need to define marshal to script context");
            VirtualTableInfo<Js::CrossSiteObject<ES5Array>>::SetVirtualTable(this);
        };

    private:
        bool GetPropertyBuiltIns(PropertyId propertyId, Var* value, BOOL* result);
        bool SetPropertyBuiltIns(PropertyId propertyId, Var value, PropertyOperationFlags flags, BOOL* result);
        bool GetSetterBuiltIns(PropertyId propertyId, PropertyValueInfo* info, DescriptorFlags* result);

    public:
        static uint32_t ToLengthValue(Var value, ScriptContext* scriptContext);
        bool IsLengthWritable() const;

        DynamicType * DuplicateType() override;

        // Enumerate
        BOOL IsValidDescriptorToken(void * descriptorValidationToken) const;
        uint32_t GetNextDescriptor(uint32_t key, IndexPropertyDescriptor** descriptor, void ** descriptorValidationToken);
        BOOL GetDescriptor(uint32_t index, Js::IndexPropertyDescriptor **ppDescriptor);

        //
        // To skip JavascriptArray overrides
        //
        PropertyQueryFlags HasPropertyQuery(PropertyId propertyId, _Inout_opt_ PropertyValueInfo* info) override;
        BOOL IsWritable(PropertyId propertyId) override;
        BOOL SetEnumerable(PropertyId propertyId, BOOL value) override;
        BOOL SetWritable(PropertyId propertyId, BOOL value) override;
        BOOL SetConfigurable(PropertyId propertyId, BOOL value) override;
        BOOL SetAttributes(PropertyId propertyId, PropertyAttributes attributes) override;

        PropertyQueryFlags GetPropertyQuery(Var originalInstance, PropertyId propertyId, Var* value, PropertyValueInfo* info, ScriptContext* requestContext) override;
        PropertyQueryFlags GetPropertyQuery(Var originalInstance, JavascriptString* propertyNameString, Var* value, PropertyValueInfo* info, ScriptContext* requestContext) override;
        PropertyQueryFlags GetPropertyReferenceQuery(Var originalInstance, PropertyId propertyId, Var* value, PropertyValueInfo* info, ScriptContext* requestContext) override;
        BOOL SetProperty(PropertyId propertyId, Var value, PropertyOperationFlags flags, PropertyValueInfo* info) override;
        BOOL SetProperty(JavascriptString* propertyNameString, Var value, PropertyOperationFlags flags, PropertyValueInfo* info) override;
        BOOL SetPropertyWithAttributes(PropertyId propertyId, Var value, PropertyAttributes attributes, PropertyValueInfo* info, PropertyOperationFlags flags = PropertyOperation_None, SideEffects possibleSideEffects = SideEffects_Any) override;

        PropertyQueryFlags HasItemQuery(uint32_t index) override;
        PropertyQueryFlags GetItemQuery(Var originalInstance, uint32_t index, Var* value, ScriptContext * requestContext) override;
        PropertyQueryFlags GetItemReferenceQuery(Var originalInstance, uint32_t index, Var* value, ScriptContext * requestContext) override;
        BOOL SetItem(uint32_t index, Var value, PropertyOperationFlags flags) override;
        BOOL DeleteItem(uint32_t index, PropertyOperationFlags flags) override;
        DescriptorFlags GetSetter(PropertyId propertyId, Var *setterValue, PropertyValueInfo* info, ScriptContext* requestContext) override;
        DescriptorFlags GetSetter(JavascriptString* propertyNameString, Var *setterValue, PropertyValueInfo* info, ScriptContext* requestContext) override;

        DescriptorFlags GetItemSetter(uint32_t index, Var* setterValue, ScriptContext* requestContext) override;

        BOOL SetAccessors(PropertyId propertyId, Var getter, Var setter, PropertyOperationFlags flags) override;
        BOOL PreventExtensions() override;
        BOOL Seal() override;
        BOOL Freeze() override;

        BOOL GetEnumerator(JavascriptStaticEnumerator * enumerator, EnumeratorFlags flags, ScriptContext* requestContext, EnumeratorCache * enumeratorCache = nullptr) override;

        // objectArray support
        BOOL SetItemWithAttributes(uint32_t index, Var value, PropertyAttributes attributes) override;
        BOOL SetItemAttributes(uint32_t index, PropertyAttributes attributes) override;
        BOOL SetItemAccessors(uint32_t index, Var getter, Var setter) override;
        BOOL IsObjectArrayFrozen() override;
        JavascriptEnumerator * GetIndexEnumerator(EnumeratorFlags flags, ScriptContext* requestContext) override;

        // for SCA
        BOOL IsItemEnumerable(uint32_t index) override;
    };

    template <> inline bool VarIsImpl<ES5Array>(RecyclableObject* instance)
    {
        return JavascriptOperators::GetTypeId(instance) == TypeIds_ES5Array;
    }
}

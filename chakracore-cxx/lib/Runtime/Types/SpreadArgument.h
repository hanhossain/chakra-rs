//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once
namespace Js
{

    class SpreadArgument : public DynamicObject
    {
    private:
        typedef JsUtil::List<Var, Recycler> VarList;
        typename WriteBarrierFieldTypeTraits<VarList*>::Type iteratorIndices;

        void AssertAndFailFast() { AssertMsg(false, "This function should not be invoked"); Js::Throw::InternalError();}
    protected:
        DEFINE_VTABLE_CTOR(SpreadArgument, DynamicObject);
        friend class Js::CrossSiteObject<SpreadArgument>;
        void MarshalToScriptContext(Js::ScriptContext *scriptContext) override
        {
            Assert(this->GetScriptContext() != scriptContext);
            AssertMsg(VirtualTableInfo<SpreadArgument>::HasVirtualTable(this),
                      "Derived class need to define marshal to script context");
            VirtualTableInfo<Js::CrossSiteObject<SpreadArgument>>::SetVirtualTable(this);
        };

    public:
        SpreadArgument(Var iterator, bool useDirectCall, DynamicType * type);
        const Var* GetArgumentSpread() const { return iteratorIndices ? iteratorIndices->GetBuffer() : nullptr; }
        uint GetArgumentSpreadCount()  const { return iteratorIndices ? iteratorIndices->Count() : 0; }

        // A SpreadArgument should never call the Functions defined below this comment
        PropertyQueryFlags HasPropertyQuery(PropertyId propertyId, _Inout_opt_ PropertyValueInfo* info) override { AssertAndFailFast();  return PropertyQueryFlags::Property_NotFound; };
        BOOL HasOwnProperty(PropertyId propertyId) override { AssertAndFailFast(); return FALSE; };
        BOOL SetProperty(PropertyId propertyId, Var value, PropertyOperationFlags flags, PropertyValueInfo* info) override { AssertAndFailFast(); return FALSE; };
        PropertyQueryFlags GetPropertyQuery(Var originalInstance, PropertyId propertyId, Var* value, PropertyValueInfo* info, ScriptContext* requestContext) override { AssertAndFailFast(); return PropertyQueryFlags::Property_NotFound; };
        BOOL DeleteProperty(PropertyId propertyId, PropertyOperationFlags flags) override{ AssertAndFailFast(); return FALSE;};
        BOOL DeleteProperty(JavascriptString * propertyNameString, PropertyOperationFlags flags) override { AssertAndFailFast(); return FALSE; };
        PropertyQueryFlags GetPropertyReferenceQuery(Var originalInstance, PropertyId propertyId, Var* value, PropertyValueInfo* info, ScriptContext* requestContext) override { AssertAndFailFast(); return PropertyQueryFlags::Property_NotFound; };
        BOOL SetProperty(JavascriptString* propertyNameString, Var value, PropertyOperationFlags flags, PropertyValueInfo* info) override { AssertAndFailFast(); return FALSE; };
        PropertyQueryFlags GetPropertyQuery(Var originalInstance, JavascriptString* propertyNameString, Var* value, PropertyValueInfo* info, ScriptContext* requestContext) override { AssertAndFailFast(); return PropertyQueryFlags::Property_NotFound; };
        DescriptorFlags GetSetter(PropertyId propertyId, Var *setterValue, PropertyValueInfo* info, ScriptContext* requestContext) override { AssertAndFailFast(); return None; };
        DescriptorFlags GetSetter(JavascriptString* propertyNameString, Var *setterValue, PropertyValueInfo* info, ScriptContext* requestContext) override { AssertAndFailFast(); return None; };
        int GetPropertyCount() override { AssertAndFailFast(); return 0; };
        PropertyId GetPropertyId(PropertyIndex index) override { AssertAndFailFast();  return Constants::NoProperty; };
        PropertyId GetPropertyId(BigPropertyIndex index) override { AssertAndFailFast(); return Constants::NoProperty;; };
        BOOL SetInternalProperty(PropertyId internalPropertyId, Var value, PropertyOperationFlags flags, PropertyValueInfo* info) override { AssertAndFailFast(); return FALSE; };
        BOOL InitProperty(PropertyId propertyId, Var value, PropertyOperationFlags flags = PropertyOperation_None, PropertyValueInfo* info = NULL) override { AssertAndFailFast(); return FALSE; };
        BOOL SetPropertyWithAttributes(PropertyId propertyId, Var value, PropertyAttributes attributes, PropertyValueInfo* info, PropertyOperationFlags flags = PropertyOperation_None, SideEffects possibleSideEffects = SideEffects_Any) override { AssertAndFailFast(); return FALSE; };
#if ENABLE_FIXED_FIELDS
        virtual BOOL IsFixedProperty(PropertyId propertyId) override { AssertAndFailFast(); return FALSE; };
#endif
        PropertyQueryFlags HasItemQuery(uint32_t index) override { AssertAndFailFast(); return PropertyQueryFlags::Property_NotFound; };
        BOOL HasOwnItem(uint32_t index) override { AssertAndFailFast(); return FALSE; };
        PropertyQueryFlags GetItemQuery(Var originalInstance, uint32_t index, Var* value, ScriptContext * requestContext) override { AssertAndFailFast(); return PropertyQueryFlags::Property_NotFound; };
        PropertyQueryFlags GetItemReferenceQuery(Var originalInstance, uint32_t index, Var* value, ScriptContext * requestContext) override { AssertAndFailFast(); return PropertyQueryFlags::Property_NotFound; };
        DescriptorFlags GetItemSetter(uint32_t index, Var* setterValue, ScriptContext* requestContext) override { AssertAndFailFast(); return None; };
        BOOL SetItem(uint32_t index, Var value, PropertyOperationFlags flags) override { AssertAndFailFast(); return FALSE; };
        BOOL DeleteItem(uint32_t index, PropertyOperationFlags flags) override { AssertAndFailFast(); return FALSE; };
        BOOL ToPrimitive(JavascriptHint hint, Var* result, ScriptContext * requestContext) override { AssertAndFailFast(); return FALSE; };
        BOOL GetEnumerator(JavascriptStaticEnumerator * enumerator, EnumeratorFlags flags, ScriptContext* requestContext, EnumeratorCache * enumeratorCache = nullptr) override { AssertAndFailFast(); return FALSE; };
        BOOL SetAccessors(PropertyId propertyId, Var getter, Var setter, PropertyOperationFlags flags = PropertyOperation_None) override { AssertAndFailFast(); return FALSE; };
        _Check_return_ _Success_(return) BOOL GetAccessors(PropertyId propertyId, _Outptr_result_maybenull_ Var* getter, _Outptr_result_maybenull_ Var* setter, ScriptContext * requestContext) override { AssertAndFailFast();  return FALSE; };
        BOOL IsWritable(PropertyId propertyId) override { AssertAndFailFast(); return FALSE; };
        BOOL IsConfigurable(PropertyId propertyId) override { AssertAndFailFast(); return FALSE; };
        BOOL IsEnumerable(PropertyId propertyId) override { AssertAndFailFast(); return FALSE; };
        BOOL SetEnumerable(PropertyId propertyId, BOOL value) override { AssertAndFailFast(); return FALSE; };
        BOOL SetWritable(PropertyId propertyId, BOOL value) override { AssertAndFailFast(); return FALSE; };
        BOOL SetConfigurable(PropertyId propertyId, BOOL value) override { AssertAndFailFast(); return FALSE; };
        BOOL SetAttributes(PropertyId propertyId, PropertyAttributes attributes) override { AssertAndFailFast(); return FALSE; };
        BOOL IsExtensible() override { AssertAndFailFast(); return FALSE; };
        BOOL PreventExtensions() override { AssertAndFailFast(); return FALSE; };
        BOOL Seal() override { AssertAndFailFast(); return FALSE; };
        BOOL Freeze() override { AssertAndFailFast(); return FALSE; };
        BOOL IsSealed() override { AssertAndFailFast(); return FALSE; };
        BOOL IsFrozen() override { AssertAndFailFast(); return FALSE; };
        BOOL GetDiagValueString(StringBuilder<ArenaAllocator>* stringBuilder, ScriptContext* requestContext) override { AssertAndFailFast(); return FALSE; };
        Var GetTypeOfString(ScriptContext * requestContext) override { AssertAndFailFast(); return RecyclableObject::GetTypeOfString(requestContext); };
    };

    template <> inline bool VarIsImpl<SpreadArgument>(RecyclableObject* obj)
    {
        return JavascriptOperators::GetTypeId(obj) == TypeIds_SpreadArgument;
    }
}

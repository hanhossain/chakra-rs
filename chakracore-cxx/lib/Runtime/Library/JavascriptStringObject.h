//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

namespace Js
{
    class JavascriptStringObject : public DynamicObject
    {
    private:
        static PropertyId const specialPropertyIds[];

    protected:
        typename WriteBarrierFieldTypeTraits<JavascriptString*>::Type value;

        DEFINE_VTABLE_CTOR(JavascriptStringObject, DynamicObject);
        friend class Js::CrossSiteObject<JavascriptStringObject>;
        void MarshalToScriptContext(Js::ScriptContext *scriptContext) override
        {
            Assert(this->GetScriptContext() != scriptContext);
            AssertMsg(VirtualTableInfo<JavascriptStringObject>::HasVirtualTable(this),
                      "Derived class need to define marshal to script context");
            VirtualTableInfo<Js::CrossSiteObject<JavascriptStringObject>>::SetVirtualTable(this);
        };
        JavascriptString* InternalUnwrap();

    private:
        bool GetPropertyBuiltIns(PropertyId propertyId, Var* value, ScriptContext* requestContext, BOOL* result);
        bool SetPropertyBuiltIns(PropertyId propertyId, PropertyOperationFlags flags, bool* result);
        bool GetSetterBuiltIns(PropertyId propertyId, PropertyValueInfo* info, DescriptorFlags* descriptorFlags);
        bool IsValidIndex(PropertyId propertyId, bool conditionMetBehavior);

    public:
        JavascriptStringObject(DynamicType * type);
        JavascriptStringObject(JavascriptString* value, DynamicType * type);

        void Initialize(JavascriptString* value);
        JavascriptString* Unwrap() { return InternalUnwrap(); }

        bool HasReadOnlyPropertiesInvisibleToTypeHandler() override { return true; }

        DescriptorFlags GetItemSetter(uint32_t index, Var* setterValue, ScriptContext* requestContext) override;
        PropertyQueryFlags HasPropertyQuery(PropertyId propertyId, _Inout_opt_ PropertyValueInfo* info) override;
        BOOL IsConfigurable(PropertyId propertyId) override;
        BOOL IsEnumerable(PropertyId propertyId) override;
        BOOL IsWritable(PropertyId propertyId) override;

        PropertyQueryFlags GetPropertyQuery(Var originalInstance, PropertyId propertyId, Var* value, PropertyValueInfo* info, ScriptContext* requestContext) override;
        PropertyQueryFlags GetPropertyQuery(Var originalInstance, JavascriptString* propertyNameString, Var* value, PropertyValueInfo* info, ScriptContext* requestContext) override;
        PropertyQueryFlags GetPropertyReferenceQuery(Var originalInstance, PropertyId propertyId, Var* value, PropertyValueInfo* info, ScriptContext* requestContext) override;
        BOOL SetProperty(PropertyId propertyId, Var value, PropertyOperationFlags flags, PropertyValueInfo* info) override;
        BOOL SetProperty(JavascriptString* propertyNameString, Var value, PropertyOperationFlags flags, PropertyValueInfo* info) override;
        BOOL DeleteProperty(PropertyId propertyId, PropertyOperationFlags flags) override;
        BOOL DeleteProperty(JavascriptString *propertyNameString, PropertyOperationFlags flags) override;
        DescriptorFlags GetSetter(PropertyId propertyId, Var* setterValue, PropertyValueInfo* info, ScriptContext* requestContext) override;
        DescriptorFlags GetSetter(JavascriptString* propertyNameString, Var* setterValue, PropertyValueInfo* info, ScriptContext* requestContext) override;
        PropertyQueryFlags HasItemQuery(uint32_t index) override;
        PropertyQueryFlags GetItemQuery(Var originalInstance, uint32_t index, Var* value, ScriptContext * requestContext) override;
        PropertyQueryFlags GetItemReferenceQuery(Var originalInstance, uint32_t index, Var* value, ScriptContext * requestContext) override;
        BOOL SetItem(uint32_t index, Var value, PropertyOperationFlags flags) override;
        BOOL GetEnumerator(JavascriptStaticEnumerator * enumerator, EnumeratorFlags flags, ScriptContext* requestContext, EnumeratorCache * enumeratorCache = nullptr) override;
        BOOL GetDiagValueString(StringBuilder<ArenaAllocator>* stringBuilder, ScriptContext* requestContext) override;
        BOOL GetDiagTypeString(StringBuilder<ArenaAllocator>* stringBuilder, ScriptContext* requestContext) override;
        BOOL GetSpecialPropertyName(uint32_t index, JavascriptString ** propertyName, ScriptContext * requestContext) override;
        uint GetSpecialPropertyCount() const override;
        PropertyId const * GetSpecialPropertyIds() const override;
    };

    template <> inline bool VarIsImpl<JavascriptStringObject>(RecyclableObject* obj)
    {
        return JavascriptOperators::GetTypeId(obj) == TypeIds_StringObject;
    }
}

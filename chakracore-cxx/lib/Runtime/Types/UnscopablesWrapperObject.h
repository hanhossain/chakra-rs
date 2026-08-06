//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
//
// The UnscopablesWrapperObject is a psuedo-object that provides us with convenience. By wrapping a normal object in this class,
// we can intercept object operations requiring checks to @@unscopables. For all other operations, the object needs to be
// unwrapped. The caller is responsible for unwrapping, which must happen only once the @@unscopables check has happened.
// For example, a getter needs to have the propId checked against @@unscopables, but for the actual getter call the
// object passed needs to be unwrapped.
//
#pragma once

#define UNWRAP_FAILFAST() AssertOrFailFastMsg(false, "This UnscopablesWrapperObject must be unwrapped by the caller handling the scope before performing this operation.")

namespace Js
{
    class UnscopablesWrapperObject : public RecyclableObject
    {
        private:
            typename WriteBarrierFieldTypeTraits<RecyclableObject *>::Type wrappedObject;

        protected:
            DEFINE_VTABLE_CTOR(UnscopablesWrapperObject, RecyclableObject);

        public:
            UnscopablesWrapperObject(RecyclableObject *wrappedObject, StaticType * type) : RecyclableObject(type), wrappedObject(wrappedObject) {}
            RecyclableObject *GetWrappedObject() const { return wrappedObject; }
            RecyclableObject * GetThisAndUnwrappedInstance(Var* thisVar) const override;
            PropertyQueryFlags HasPropertyQuery(PropertyId propertyId, _Inout_opt_ PropertyValueInfo* info) override;
            BOOL HasOwnProperty(PropertyId propertyId) override;
            BOOL SetProperty(PropertyId propertyId, Var value, PropertyOperationFlags flags, PropertyValueInfo* info) override;
            PropertyQueryFlags GetPropertyQuery(Var originalInstance, PropertyId propertyId, Var* value, PropertyValueInfo* info, ScriptContext* requestContext) override;
            BOOL DeleteProperty(PropertyId propertyId, PropertyOperationFlags flags) override;
            PropertyQueryFlags GetPropertyReferenceQuery(Var originalInstance, PropertyId propertyId, Var* value, PropertyValueInfo* info, ScriptContext* requestContext) override;
            DescriptorFlags GetSetter(PropertyId propertyId, Var *setterValue, PropertyValueInfo* info, ScriptContext* requestContext) override;

            // A UnscopablesWrapperObject should never call the Functions defined below this comment
            BOOL SetProperty(JavascriptString* propertyNameString, Var value, PropertyOperationFlags flags, PropertyValueInfo* info) override { UNWRAP_FAILFAST(); return FALSE; };
            PropertyQueryFlags GetPropertyQuery(Var originalInstance, JavascriptString* propertyNameString, Var* value, PropertyValueInfo* info, ScriptContext* requestContext) override { UNWRAP_FAILFAST(); return PropertyQueryFlags::Property_NotFound; };
            DescriptorFlags GetSetter(JavascriptString* propertyNameString, Var *setterValue, PropertyValueInfo* info, ScriptContext* requestContext) override { UNWRAP_FAILFAST(); return None; };
            int GetPropertyCount() override { UNWRAP_FAILFAST(); return 0; };
            PropertyId GetPropertyId(PropertyIndex index) override { UNWRAP_FAILFAST();  return Constants::NoProperty; };
            PropertyId GetPropertyId(BigPropertyIndex index) override { UNWRAP_FAILFAST(); return Constants::NoProperty;; };
            BOOL SetInternalProperty(PropertyId internalPropertyId, Var value, PropertyOperationFlags flags, PropertyValueInfo* info) override { UNWRAP_FAILFAST(); return FALSE; };
            BOOL InitProperty(PropertyId propertyId, Var value, PropertyOperationFlags flags = PropertyOperation_None, PropertyValueInfo* info = NULL) override { UNWRAP_FAILFAST(); return FALSE; };
            BOOL SetPropertyWithAttributes(PropertyId propertyId, Var value, PropertyAttributes attributes, PropertyValueInfo* info, PropertyOperationFlags flags = PropertyOperation_None, SideEffects possibleSideEffects = SideEffects_Any) override { UNWRAP_FAILFAST(); return FALSE; };
#if ENABLE_FIXED_FIELDS
            virtual BOOL IsFixedProperty(PropertyId propertyId) override { UNWRAP_FAILFAST(); return FALSE; };
#endif
            PropertyQueryFlags HasItemQuery(uint32_t index) override { UNWRAP_FAILFAST(); return PropertyQueryFlags::Property_NotFound; };
            BOOL HasOwnItem(uint32_t index) override { UNWRAP_FAILFAST(); return FALSE; };
            PropertyQueryFlags GetItemQuery(Var originalInstance, uint32_t index, Var* value, ScriptContext * requestContext) override { UNWRAP_FAILFAST(); return PropertyQueryFlags::Property_NotFound; };
            PropertyQueryFlags GetItemReferenceQuery(Var originalInstance, uint32_t index, Var* value, ScriptContext * requestContext) override { UNWRAP_FAILFAST(); return PropertyQueryFlags::Property_NotFound; };
            DescriptorFlags GetItemSetter(uint32_t index, Var* setterValue, ScriptContext* requestContext) override { UNWRAP_FAILFAST(); return None; };
            BOOL SetItem(uint32_t index, Var value, PropertyOperationFlags flags) override { UNWRAP_FAILFAST(); return FALSE; };
            BOOL DeleteItem(uint32_t index, PropertyOperationFlags flags) override { UNWRAP_FAILFAST(); return FALSE; };
            BOOL ToPrimitive(JavascriptHint hint, Var* result, ScriptContext * requestContext) override { UNWRAP_FAILFAST(); return FALSE; };
            BOOL GetEnumerator(JavascriptStaticEnumerator * enumerator, EnumeratorFlags flags, ScriptContext* requestContext, EnumeratorCache * enumeratorCache = nullptr) override { UNWRAP_FAILFAST(); return FALSE; };
            BOOL SetAccessors(PropertyId propertyId, Var getter, Var setter, PropertyOperationFlags flags = PropertyOperation_None) override { UNWRAP_FAILFAST(); return FALSE; };
            _Check_return_ _Success_(return) BOOL GetAccessors(PropertyId propertyId, _Outptr_result_maybenull_ Var* getter, _Outptr_result_maybenull_ Var* setter, ScriptContext * requestContext) override { UNWRAP_FAILFAST(); return FALSE; };
            BOOL IsWritable(PropertyId propertyId) override { UNWRAP_FAILFAST(); return FALSE; };
            BOOL IsConfigurable(PropertyId propertyId) override { UNWRAP_FAILFAST(); return FALSE; };
            BOOL IsEnumerable(PropertyId propertyId) override { UNWRAP_FAILFAST(); return FALSE; };
            BOOL SetEnumerable(PropertyId propertyId, BOOL value) override { UNWRAP_FAILFAST(); return FALSE; };
            BOOL SetWritable(PropertyId propertyId, BOOL value) override { UNWRAP_FAILFAST(); return FALSE; };
            BOOL SetConfigurable(PropertyId propertyId, BOOL value) override { UNWRAP_FAILFAST(); return FALSE; };
            BOOL SetAttributes(PropertyId propertyId, PropertyAttributes attributes) override { UNWRAP_FAILFAST(); return FALSE; };
            BOOL IsExtensible() override { UNWRAP_FAILFAST(); return FALSE; };
            BOOL PreventExtensions() override { UNWRAP_FAILFAST(); return FALSE; };
            BOOL Seal() override { UNWRAP_FAILFAST(); return FALSE; };
            BOOL Freeze() override { UNWRAP_FAILFAST(); return FALSE; };
            BOOL IsSealed() override { UNWRAP_FAILFAST(); return FALSE; };
            BOOL IsFrozen() override { UNWRAP_FAILFAST(); return FALSE; };
            BOOL GetDiagValueString(StringBuilder<ArenaAllocator>* stringBuilder, ScriptContext* requestContext) override { UNWRAP_FAILFAST(); return FALSE; };
            Var GetTypeOfString(ScriptContext * requestContext) override { UNWRAP_FAILFAST(); return RecyclableObject::GetTypeOfString(requestContext); };
    };

    template <> inline bool VarIsImpl<UnscopablesWrapperObject>(RecyclableObject* obj)
    {
        return JavascriptOperators::GetTypeId(obj) == TypeIds_UnscopablesWrapperObject;
    }
} // namespace Js

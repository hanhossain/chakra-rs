//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

namespace Js
{
    static_assert(sizeof(ES5Array) == sizeof(JavascriptArray));

    ES5ArrayType::ES5ArrayType(DynamicType* type)
        : DynamicType(type->GetScriptContext(), TypeIds_ES5Array, type->GetPrototype(), type->GetEntryPoint(), type->GetTypeHandler(), false, false)
    {
    }

    DynamicType* ES5Array::DuplicateType()
    {
        return RecyclerNew(GetScriptContext()->GetRecycler(), ES5ArrayType, this->GetDynamicType());
    }

    bool ES5Array::IsLengthWritable() const
    {
        return GetTypeHandler()->IsLengthWritable();
    }

    PropertyQueryFlags ES5Array::HasPropertyQuery(PropertyId propertyId, _Inout_opt_ PropertyValueInfo* info)
    {
        if (propertyId == PropertyIds::length)
        {
            return PropertyQueryFlags::Property_Found;
        }

        // Skip JavascriptArray override
        return DynamicObject::HasPropertyQuery(propertyId, info);
    }

    BOOL ES5Array::IsWritable(PropertyId propertyId)
    {
        if (propertyId == PropertyIds::length)
        {
            return IsLengthWritable();
        }

        return __super::IsWritable(propertyId);
    }

    BOOL ES5Array::SetEnumerable(PropertyId propertyId, BOOL value)
    {
        // Skip JavascriptArray override
        return DynamicObject::SetEnumerable(propertyId, value);
    }

    BOOL ES5Array::SetWritable(PropertyId propertyId, BOOL value)
    {
        // Skip JavascriptArray override
        return DynamicObject::SetWritable(propertyId, value);
    }

    BOOL ES5Array::SetConfigurable(PropertyId propertyId, BOOL value)
    {
        // Skip JavascriptArray override
        return DynamicObject::SetConfigurable(propertyId, value);
    }

    BOOL ES5Array::SetAttributes(PropertyId propertyId, PropertyAttributes attributes)
    {
        // Skip JavascriptArray override
        return DynamicObject::SetAttributes(propertyId, attributes);
    }

    PropertyQueryFlags ES5Array::GetPropertyQuery(Var originalInstance, PropertyId propertyId, Var* value, PropertyValueInfo* info, ScriptContext* requestContext)
    {
        BOOL result;
        if (GetPropertyBuiltIns(propertyId, value, &result))
        {
            return JavascriptConversion::BooleanToPropertyQueryFlags(result);
        }

        // Skip JavascriptArray override
        return DynamicObject::GetPropertyQuery(originalInstance, propertyId, value, info, requestContext);
    }

    PropertyQueryFlags ES5Array::GetPropertyQuery(Var originalInstance, JavascriptString* propertyNameString, Var* value, PropertyValueInfo* info, ScriptContext* requestContext)
    {
        BOOL result;
        PropertyRecord const* propertyRecord;
        this->GetScriptContext()->FindPropertyRecord(propertyNameString, &propertyRecord);

        if (propertyRecord != nullptr && GetPropertyBuiltIns(propertyRecord->GetPropertyId(), value, &result))
        {
            return JavascriptConversion::BooleanToPropertyQueryFlags(result);
        }

        // Skip JavascriptArray override
        return DynamicObject::GetPropertyQuery(originalInstance, propertyNameString, value, info, requestContext);
    }

    bool ES5Array::GetPropertyBuiltIns(PropertyId propertyId, Var* value, BOOL* result)
    {
        if (propertyId == PropertyIds::length)
        {
            *value = JavascriptNumber::ToVar(this->GetLength(), GetScriptContext());
            *result = true;
            return true;
        }

        return false;
    }

    PropertyQueryFlags ES5Array::GetPropertyReferenceQuery(Var originalInstance, PropertyId propertyId, Var* value, PropertyValueInfo* info, ScriptContext* requestContext)
    {
        return ES5Array::GetPropertyQuery(originalInstance, propertyId, value, info, requestContext);
    }

    // Convert a Var to array length, throw RangeError if value is not valid for array length.
    uint32_t ES5Array::ToLengthValue(Var value, ScriptContext* scriptContext)
    {
        if (TaggedInt::Is(value))
        {
            int32_t newLen = TaggedInt::ToInt32(value);
            if (newLen < 0)
            {
                JavascriptError::ThrowRangeError(scriptContext, JSERR_ArrayLengthAssignIncorrect);
            }
            return static_cast<uint32_t>(newLen);
        }
        else
        {
            uint32_t newLen = JavascriptConversion::ToUInt32(value, scriptContext);
            if (newLen != JavascriptConversion::ToNumber(value, scriptContext))
            {
                JavascriptError::ThrowRangeError(scriptContext, JSERR_ArrayLengthAssignIncorrect);
            }

            // Conversion can change the type (e.g. from String), invalidating assumptions made by the JIT
            scriptContext->GetThreadContext()->AddImplicitCallFlags(ImplicitCall_Accessor);

            return newLen;
        }
    }

    DescriptorFlags ES5Array::GetSetter(PropertyId propertyId, Var *setterValue, PropertyValueInfo* info, ScriptContext* requestContext)
    {
        DescriptorFlags result;
        if (GetSetterBuiltIns(propertyId, info, &result))
        {
            return result;
        }

        return DynamicObject::GetSetter(propertyId, setterValue, info, requestContext);
    }

    DescriptorFlags ES5Array::GetSetter(JavascriptString* propertyNameString, Var *setterValue, PropertyValueInfo* info, ScriptContext* requestContext)
    {
        DescriptorFlags result;
        PropertyRecord const* propertyRecord;
        this->GetScriptContext()->FindPropertyRecord(propertyNameString, &propertyRecord);

        if (propertyRecord != nullptr && GetSetterBuiltIns(propertyRecord->GetPropertyId(), info, &result))
        {
            return result;
        }

        return DynamicObject::GetSetter(propertyNameString, setterValue, info, requestContext);
    }

    bool ES5Array::GetSetterBuiltIns(PropertyId propertyId, PropertyValueInfo* info, DescriptorFlags* result)
    {
        if (propertyId == PropertyIds::length)
        {
            PropertyValueInfo::SetNoCache(info, this);
            *result =  IsLengthWritable() ? WritableData : Data;
            return true;
        }

        return false;
    }

    BOOL ES5Array::SetProperty(PropertyId propertyId, Var value, PropertyOperationFlags propertyOperationFlags, PropertyValueInfo* info)
    {
        BOOL result;
        if (SetPropertyBuiltIns(propertyId, value, propertyOperationFlags, &result))
        {
            return result;
        }

        return __super::SetProperty(propertyId, value, propertyOperationFlags, info);
    }

    BOOL ES5Array::SetProperty(JavascriptString* propertyNameString, Var value, PropertyOperationFlags propertyOperationFlags, PropertyValueInfo* info)
    {
        BOOL result;
        PropertyRecord const* propertyRecord;
        this->GetScriptContext()->FindPropertyRecord(propertyNameString, &propertyRecord);

        if (propertyRecord != nullptr && SetPropertyBuiltIns(propertyRecord->GetPropertyId(), value, propertyOperationFlags, &result))
        {
            return result;
        }

        return __super::SetProperty(propertyNameString, value, propertyOperationFlags, info);
    }

    bool ES5Array::SetPropertyBuiltIns(PropertyId propertyId, Var value, PropertyOperationFlags propertyOperationFlags, BOOL* result)
    {
        ScriptContext* scriptContext = GetScriptContext();

        if (propertyId == PropertyIds::length)
        {
            if (!GetTypeHandler()->IsLengthWritable())
            {
                *result = false; // reject
                return true;
            }

            uint32_t newLen = ToLengthValue(value, scriptContext);
            uint32_t assignedLen = GetTypeHandler()->SetLength(this, newLen, propertyOperationFlags);
            if (newLen != assignedLen)
            {
                scriptContext->GetThreadContext()->AddImplicitCallFlags(ImplicitCall_NoOpSet);
            }
            *result = true;
            return true;
        }

        return false;
    }

    BOOL ES5Array::SetPropertyWithAttributes(PropertyId propertyId, Var value, PropertyAttributes attributes, PropertyValueInfo* info, PropertyOperationFlags flags, SideEffects possibleSideEffects)
    {
        if (propertyId == PropertyIds::length)
        {
            Assert(attributes == PropertyWritable);
            Assert(IsWritable(propertyId) && !IsConfigurable(propertyId) && !IsEnumerable(propertyId));

            uint32_t newLen = ToLengthValue(value, GetScriptContext());
            GetTypeHandler()->SetLength(this, newLen, PropertyOperation_None);
            return true;
        }

        return __super::SetPropertyWithAttributes(propertyId, value, attributes, info, flags, possibleSideEffects);
    }

    BOOL ES5Array::DeleteItem(uint32_t index, PropertyOperationFlags flags)
    {
        // Skip JavascriptArray override
        return DynamicObject::DeleteItem(index, flags);
    }

    PropertyQueryFlags ES5Array::HasItemQuery(uint32_t index)
    {
        // Skip JavascriptArray override
        return DynamicObject::HasItemQuery(index);
    }

    PropertyQueryFlags ES5Array::GetItemQuery(Var originalInstance, uint32_t index, Var* value, ScriptContext * requestContext)
    {
        // Skip JavascriptArray override
        return DynamicObject::GetItemQuery(originalInstance, index, value, requestContext);
    }

    PropertyQueryFlags ES5Array::GetItemReferenceQuery(Var originalInstance, uint32_t index, Var* value, ScriptContext * requestContext)
    {
        // Skip JavascriptArray override
        return DynamicObject::GetItemReferenceQuery(originalInstance, index, value, requestContext);
    }

    DescriptorFlags ES5Array::GetItemSetter(uint32_t index, Var* setterValue, ScriptContext* requestContext)
    {
        // Skip JavascriptArray override
        return DynamicObject::GetItemSetter(index, setterValue, requestContext);
    }

    BOOL ES5Array::SetItem(uint32_t index, Var value, PropertyOperationFlags flags)
    {
        // Skip JavascriptArray override
        return DynamicObject::SetItem(index, value, flags);
    }

    BOOL ES5Array::SetAccessors(PropertyId propertyId, Var getter, Var setter, PropertyOperationFlags flags)
    {
        // Skip JavascriptArray override
        return DynamicObject::SetAccessors(propertyId, getter, setter, flags);
    }

    BOOL ES5Array::PreventExtensions()
    {
        // Skip JavascriptArray override
        return DynamicObject::PreventExtensions();
    }

    BOOL ES5Array::Seal()
    {
        // Skip JavascriptArray override
        return DynamicObject::Seal();
    }

    BOOL ES5Array::Freeze()
    {
        // Skip JavascriptArray override
        return DynamicObject::Freeze();
    }

    BOOL ES5Array::GetEnumerator(JavascriptStaticEnumerator * enumerator, EnumeratorFlags flags, ScriptContext* requestContext, EnumeratorCache * enumeratorCache)
    {
        return enumerator->Initialize(nullptr, this, this, flags, requestContext, enumeratorCache);
    }

    JavascriptEnumerator * ES5Array::GetIndexEnumerator(EnumeratorFlags flags, ScriptContext* requestContext)
    {
        // ES5Array does not support compat mode, ignore preferSnapshotSemantics
        return RecyclerNew(GetScriptContext()->GetRecycler(), ES5ArrayIndexEnumerator, this, flags, requestContext);
    }

    BOOL ES5Array::IsItemEnumerable(uint32_t index)
    {
        return GetTypeHandler()->IsItemEnumerable(this, index);
    }

    BOOL ES5Array::SetItemWithAttributes(uint32_t index, Var value, PropertyAttributes attributes)
    {
        return GetTypeHandler()->SetItemWithAttributes(this, index, value, attributes);
    }

    BOOL ES5Array::SetItemAttributes(uint32_t index, PropertyAttributes attributes)
    {
        return GetTypeHandler()->SetItemAttributes(this, index, attributes);
    }

    BOOL ES5Array::SetItemAccessors(uint32_t index, Var getter, Var setter)
    {
        return GetTypeHandler()->SetItemAccessors(this, index, getter, setter);
    }

    BOOL ES5Array::IsObjectArrayFrozen()
    {
        return GetTypeHandler()->IsObjectArrayFrozen(this);
    }

    BOOL ES5Array::IsValidDescriptorToken(void * descriptorValidationToken) const
    {
        return GetTypeHandler()->IsValidDescriptorToken(descriptorValidationToken);
    }
    uint32_t ES5Array::GetNextDescriptor(uint32_t key, IndexPropertyDescriptor** descriptor, void ** descriptorValidationToken)
    {
        return GetTypeHandler()->GetNextDescriptor(key, descriptor, descriptorValidationToken);
    }

    BOOL ES5Array::GetDescriptor(uint32_t index, Js::IndexPropertyDescriptor **ppDescriptor)
    {
        return GetTypeHandler()->GetDescriptor(index, ppDescriptor);
    }
}

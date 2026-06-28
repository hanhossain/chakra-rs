//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

namespace Js
{
    class ES5ArrayIndexEnumerator : public JavascriptArrayIndexEnumeratorBase
    {
    private:
        typename WriteBarrierFieldTypeTraits<uint32_t>::Type initialLength;                   // The initial array length when this enumerator is created
        typename WriteBarrierFieldTypeTraits<uint32_t>::Type dataIndex;                       // Current data index
        typename WriteBarrierFieldTypeTraits<uint32_t>::Type descriptorIndex;                 // Current descriptor index
        typename WriteBarrierFieldTypeTraits<IndexPropertyDescriptor*>::Type descriptor;    // Current descriptor
        typename WriteBarrierFieldTypeTraits<void *>::Type descriptorValidationToken;
    protected:
        DEFINE_VTABLE_CTOR(ES5ArrayIndexEnumerator, JavascriptArrayIndexEnumeratorBase);

    private:
        ES5Array* GetArray() const { return VarTo<ES5Array>(arrayObject); }

    public:
        ES5ArrayIndexEnumerator(ES5Array* arrayObject, EnumeratorFlags flags, ScriptContext* scriptContext);
        virtual void Reset() override;
        virtual JavascriptString * MoveAndGetNext(PropertyId& propertyId, PropertyAttributes* attributes = nullptr) override;
    };
}

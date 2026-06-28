//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

namespace Js
{
    class ArgumentsObjectPrefixEnumerator : public JavascriptEnumerator
    {
    protected:
        typename WriteBarrierFieldTypeTraits<ArgumentsObject*>::Type argumentsObject;
        typename WriteBarrierFieldTypeTraits<uint32_t>::Type formalArgIndex;
        typename WriteBarrierFieldTypeTraits<bool>::Type doneFormalArgs;
        typename WriteBarrierFieldTypeTraits<EnumeratorFlags>::Type flags;

    protected:
        DEFINE_VTABLE_CTOR(ArgumentsObjectPrefixEnumerator, JavascriptEnumerator);

    public:
        ArgumentsObjectPrefixEnumerator(ArgumentsObject* argumentsObject, EnumeratorFlags flags, ScriptContext* requestContext);
        virtual void Reset() override;
        virtual JavascriptString * MoveAndGetNext(PropertyId& propertyId, PropertyAttributes* attributes = nullptr) override;
        virtual uint32_t GetCurrentItemIndex()  override { return formalArgIndex; }
    };

    class ES5ArgumentsObjectEnumerator : public ArgumentsObjectPrefixEnumerator
    {
    protected:
        DEFINE_VTABLE_CTOR(ES5ArgumentsObjectEnumerator, ArgumentsObjectPrefixEnumerator);
        ES5ArgumentsObjectEnumerator(ArgumentsObject* argumentsObject, EnumeratorFlags flags, ScriptContext* requestContext);
        BOOL Init(EnumeratorCache * enumeratorCache);
    public:
        static ES5ArgumentsObjectEnumerator * New(ArgumentsObject* argumentsObject, EnumeratorFlags flags, ScriptContext* requestContext, EnumeratorCache * enumeratorCache);
        virtual void Reset() override;
        virtual JavascriptString * MoveAndGetNext(PropertyId& propertyId, PropertyAttributes* attributes = nullptr) override;
    private:
        typename WriteBarrierFieldTypeTraits<JavascriptStaticEnumerator>::Type objectEnumerator;
        typename WriteBarrierFieldTypeTraits<uint>::Type enumeratedFormalsInObjectArrayCount;  // The number of enumerated formals for far.
    };
}

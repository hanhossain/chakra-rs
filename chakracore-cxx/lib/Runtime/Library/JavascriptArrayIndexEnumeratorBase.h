//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

namespace Js
{
    class JavascriptArrayIndexEnumeratorBase : public JavascriptEnumerator
    {
    protected:
        typename WriteBarrierFieldTypeTraits<JavascriptArray*>::Type arrayObject;
        typename WriteBarrierFieldTypeTraits<uint32_t>::Type index;
        typename WriteBarrierFieldTypeTraits<bool>::Type doneArray;
        typename WriteBarrierFieldTypeTraits<EnumeratorFlags>::Type flags;

        DEFINE_VTABLE_CTOR_ABSTRACT(JavascriptArrayIndexEnumeratorBase, JavascriptEnumerator)

        JavascriptArrayIndexEnumeratorBase(JavascriptArray* arrayObject, EnumeratorFlags flags, ScriptContext* scriptContext);
        virtual uint32_t GetCurrentItemIndex()  override { return index; }
    };
}

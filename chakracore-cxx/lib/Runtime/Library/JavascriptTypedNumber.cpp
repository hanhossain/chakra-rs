//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "RuntimeLibraryPch.h"

namespace Js
{
    template <>
    Var JavascriptTypedNumber<long>::ToVar(long value, ScriptContext* scriptContext)
    {
        if (!TaggedInt::IsOverflow(value))
        {
            return TaggedInt::ToVarUnchecked((int)value);
        }
        JavascriptTypedNumber<long>* number = RecyclerNewLeaf(scriptContext->GetRecycler(), JavascriptInt64Number, value,
            scriptContext->GetLibrary()->GetInt64TypeStatic());
        return number;
    }

    template <>
    Var JavascriptTypedNumber<unsigned long>::ToVar(unsigned long value, ScriptContext* scriptContext)
    {
        if (!TaggedInt::IsOverflow(value))
        {
            return TaggedInt::ToVarUnchecked((uint)value);
        }
        JavascriptTypedNumber<unsigned long>* number = RecyclerNewLeaf(scriptContext->GetRecycler(), JavascriptUInt64Number, value,
            scriptContext->GetLibrary()->GetUInt64TypeStatic());
        return number;
    }

    template <>
    JavascriptString* JavascriptTypedNumber<long>::ToString(Var value, ScriptContext* scriptContext)
    {
        char16_t szBuffer[22];
        long val = VarTo<JavascriptTypedNumber<long>>(value)->GetValue();
        int pos = TaggedInt::SignedToString(val, szBuffer, 22);
        return JavascriptString::NewCopyBuffer(szBuffer + pos, (_countof(szBuffer) - 1) - pos, scriptContext);
    }

    template <>
    JavascriptString* JavascriptTypedNumber<unsigned long>::ToString(Var value, ScriptContext* scriptContext)
    {
        char16_t szBuffer[22];
        unsigned long val = VarTo<JavascriptUInt64Number>(value)->GetValue();
        int pos = TaggedInt::UnsignedToString(val, szBuffer, 22);
        return JavascriptString::NewCopyBuffer(szBuffer + pos, (_countof(szBuffer) - 1) - pos, scriptContext);
    }

    template <typename T>
    RecyclableObject* JavascriptTypedNumber<T>::ToObject(ScriptContext * requestContext)
    {
        return requestContext->GetLibrary()->CreateNumberObjectWithCheck((double)m_value);
    }
}

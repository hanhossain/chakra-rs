//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#include "SCACorePch.h"


#define IfFailGoto(expr, label) \
    do                          \
    {                           \
        hr = (expr);            \
        if (FAILED (hr))        \
        {                       \
            goto label;         \
        }                       \
    } while (FALSE)             \

#define IfFailGo(expr) IfFailGoto(expr, Error)

namespace Js
{
    SCAPropBag::SCAPropBag(ScriptContext* scriptContext)
        : ScriptContextHolder(scriptContext), m_refCount(1)
    {
        Recycler* recycler = GetScriptContext()->GetRecycler();
        m_properties.Root(RecyclerNew(recycler, PropertyDictionary, recycler), recycler);        
    }

    SCAPropBag::~SCAPropBag()
    {
        Recycler* recycler = GetScriptContext()->GetRecycler();
        m_properties.Unroot(recycler);
    }

    void SCAPropBag::CreateInstance(ScriptContext* scriptContext, SCAPropBag** ppInstance)
    {
        *ppInstance = HeapNew(SCAPropBag, scriptContext);
    }

    uint32_t SCAPropBag::AddRef()
    {
        Assert(m_refCount > 0);
        return InterlockedIncrement(&m_refCount);
    }

    uint32_t SCAPropBag::Release()
    {
        Assert(m_refCount > 0);
        uint32_t ref = InterlockedDecrement(&m_refCount);
        if (ref == 0)
        {
            HeapDelete(this);
        }
        return ref;
    }

    int32_t SCAPropBag::QueryInterface(REFIID riid, void** ppv)
    {
        if (ppv == nullptr)
        {
            return E_POINTER;
        }

        *ppv = nullptr;
        return E_NOINTERFACE;
    }

    int32_t SCAPropBag::Add(const char16_t* name, Var value)
    {
        int32_t hr = S_OK;
        ScriptContext* scriptContext = GetScriptContext();

        BEGIN_TRANSLATE_EXCEPTION_AND_ERROROBJECT_TO_HRESULT
        {
            BEGIN_JS_RUNTIME_CALL_EX(scriptContext, false)
            {
                charcount_t len;
                IfFailGo(ULongLongToUInt(wcslen(name), &len));
                IfFailGo(InternalAdd(name, len, value));
Error:
                ; // Fall through
            }
            END_JS_RUNTIME_CALL(scriptContext);
        }
        END_TRANSLATE_EXCEPTION_AND_ERROROBJECT_TO_HRESULT(hr);

        return hr;
    }

    int32_t SCAPropBag::Get(const char16_t* name, Var* pValue)
    {
        int32_t hr = S_OK;
        ScriptContext* scriptContext = GetScriptContext();

        BEGIN_JS_RUNTIME_CALL_EX(scriptContext, false)
        {
            int len;
            IfFailGo(ULongLongToInt(wcslen(name), &len));

            if (!m_properties->TryGetValue(InternalString(name, len), pValue))
            {
                hr = E_FAIL;
            }
        }
        END_JS_RUNTIME_CALL(scriptContext);
Error:
        return hr;
    }

    int32_t SCAPropBag::InternalAdd(const char16_t* name, charcount_t len, Var value)
    {
        char16_t* buf = nullptr;
        int32_t hr = S_OK;
        ScriptContext* scriptContext = GetScriptContext();
        Recycler* recycler = scriptContext->GetRecycler();
        charcount_t fullLen;
        charcount_t byteLen;
        
        // fullLen == len + 1
        IfFailGo(UIntAdd(len, 1, &fullLen));
        
        // byte length (excluding null terminator)
        IfFailGo(UIntMult(len, sizeof(char16_t), &byteLen));

        // Make a copy of name
        buf = RecyclerNewArrayLeaf(recycler, char16_t, fullLen);
        js_memcpy_s(buf, byteLen, name, byteLen);
        buf[len] = u'\0';

        // Add to the property bag
        IfFailGo(InternalAddNoCopy(buf, len, value));

Error:
        return hr;
    }

    int32_t SCAPropBag::InternalAddNoCopy(const char16_t* name, charcount_t len, Var value)
    {
        int32_t hr = S_OK;

        int intLen;
        IfFailGo(UIntToInt(len, &intLen));
        m_properties->Item(InternalString(name, intLen), value);

Error:
        return hr;
    }

    bool SCAPropBag::PropBagEnumerator::MoveNext()
    {
        while (++m_curIndex < m_properties->Count())
        {
            if (m_properties->GetValueAt(m_curIndex))
            {
                return true;
            }
        }
        return false;
    }
}

//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#pragma once
namespace Js
{
    //
    // Implements ISCAPropBag.
    //
    class SCAPropBag :
        public ScriptContextHolder,
        public IUnknown
    {
        typedef JsUtil::BaseDictionary<InternalString, Var, RecyclerNonLeafAllocator, PowerOf2SizePolicy,
            DefaultComparer, JsUtil::DictionaryEntry> PropertyDictionary;

    private:
        RecyclerRootPtr<PropertyDictionary> m_properties;
        uint32_t m_refCount;

        SCAPropBag(ScriptContext* scriptContext);
        int32_t InternalAdd(const char16_t* name, charcount_t len, Var value);

    public:
        ~SCAPropBag();
        static void CreateInstance(ScriptContext* scriptContext, SCAPropBag** ppInstance);

        STDMETHODIMP_(uint32_t) AddRef();
        STDMETHODIMP_(uint32_t) Release();
        STDMETHODIMP QueryInterface(REFIID riid, void** ppv);

        int32_t Add(const char16_t* name, Var value);
        int32_t Get(const char16_t* name, Var* pValue);

        int32_t InternalAddNoCopy(const char16_t* name, charcount_t len, Var value);

        //
        // PropBag property enumerator for WriteObjectProperties.
        //
        class PropBagEnumerator
        {
        private:
            PropertyDictionary* m_properties;
            int m_curIndex;

        public:
            PropBagEnumerator(SCAPropBag* propBag)
                : m_properties(propBag->m_properties), m_curIndex(-1)
            {
            }

            bool MoveNext();

            const char16_t* GetNameString() const
            {
                return m_properties->GetKeyAt(m_curIndex).GetBuffer();
            }

            charcount_t GetNameLength() const
            {
                return m_properties->GetKeyAt(m_curIndex).GetLength();
            }

            Var GetValue() const
            {
                return m_properties->GetValueAt(m_curIndex);
            }
        };
    };
}

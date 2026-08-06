//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

namespace Js
{
    enum class JavascriptSetIteratorKind
    {
        Value,
        KeyAndValue,
    };

    class JavascriptSetIterator : public DynamicObject
    {
    private:
        typename WriteBarrierFieldTypeTraits<JavascriptSet*>::Type                          m_set;
        typename WriteBarrierFieldTypeTraits<JavascriptSet::SetDataList::Iterator>::Type    m_setIterator;
        typename WriteBarrierFieldTypeTraits<JavascriptSetIteratorKind>::Type               m_kind;

    protected:
        DEFINE_VTABLE_CTOR(JavascriptSetIterator, DynamicObject);
        friend class Js::CrossSiteObject<JavascriptSetIterator>;
        void MarshalToScriptContext(Js::ScriptContext *scriptContext) override
        {
            Assert(this->GetScriptContext() != scriptContext);
            AssertMsg(VirtualTableInfo<JavascriptSetIterator>::HasVirtualTable(this),
                      "Derived class need to define marshal to script context");
            VirtualTableInfo<Js::CrossSiteObject<JavascriptSetIterator>>::SetVirtualTable(this);
        };

    public:
        JavascriptSetIterator(DynamicType* type, JavascriptSet* set, JavascriptSetIteratorKind kind);

        class EntryInfo
        {
        public:
            static FunctionInfo Next;
        };

        static Var EntryNext(RecyclableObject* function, CallInfo callInfo, ...);

    public:
        JavascriptSet* GetSetForHeapEnum() { return m_set; }
    };

    template <> inline bool VarIsImpl<JavascriptSetIterator>(RecyclableObject* obj)
    {
        return JavascriptOperators::GetTypeId(obj) == TypeIds_SetIterator;
    }
} // namespace Js

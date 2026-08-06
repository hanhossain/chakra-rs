//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

namespace Js
{
    class JavascriptListIterator : public DynamicObject
    {
    private:
        typename WriteBarrierFieldTypeTraits<ListForListIterator*>::Type listForIterator;
        typename WriteBarrierFieldTypeTraits<uint>::Type index;
        typename WriteBarrierFieldTypeTraits<uint>::Type count;

    protected:
        DEFINE_VTABLE_CTOR(JavascriptListIterator, DynamicObject);
        friend class Js::CrossSiteObject<JavascriptListIterator>;
        void MarshalToScriptContext(Js::ScriptContext *scriptContext) override
        {
            Assert(this->GetScriptContext() != scriptContext);
            AssertMsg(VirtualTableInfo<JavascriptListIterator>::HasVirtualTable(this),
                      "Derived class need to define marshal to script context");
            VirtualTableInfo<Js::CrossSiteObject<JavascriptListIterator>>::SetVirtualTable(this);
        };

    public:
        JavascriptListIterator(DynamicType* type, ListForListIterator* list);

        class EntryInfo
        {
        public:
            static FunctionInfo Next;
        };

        static Var EntryNext(RecyclableObject* function, CallInfo callInfo, ...);

    };

    template <> inline bool VarIsImpl<JavascriptListIterator>(RecyclableObject* obj)
    {
        return JavascriptOperators::GetTypeId(obj) == TypeIds_ListIterator;
    }
} // namespace Js


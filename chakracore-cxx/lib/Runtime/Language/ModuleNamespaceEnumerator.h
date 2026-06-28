//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

namespace Js
{
    class ModuleNamespaceEnumerator : public JavascriptEnumerator
    {
    protected:
        DEFINE_VTABLE_CTOR(ModuleNamespaceEnumerator, JavascriptEnumerator);
        ModuleNamespaceEnumerator(ModuleNamespace* nsObject, EnumeratorFlags flags, ScriptContext* scriptContext);
        BOOL Init(EnumeratorCache * enumeratorCache);

    public:
        static ModuleNamespaceEnumerator* New(ModuleNamespace* nsObject, EnumeratorFlags flags, ScriptContext* scriptContext, EnumeratorCache * enumeratorCache);
        virtual void Reset() override;
        virtual JavascriptString * MoveAndGetNext(PropertyId& propertyId, PropertyAttributes* attributes = nullptr) override;
        virtual Var GetCurrentValue() { Assert(false); return nullptr; }

    private:
        typename WriteBarrierFieldTypeTraits<ModuleNamespace*>::Type nsObject;
        typename WriteBarrierFieldTypeTraits<JavascriptStaticEnumerator>::Type symbolEnumerator;
        typename WriteBarrierFieldTypeTraits<ModuleNamespace::UnambiguousExportMap*>::Type nonLocalMap;
        typename WriteBarrierFieldTypeTraits<BigPropertyIndex>::Type currentLocalMapIndex;
        typename WriteBarrierFieldTypeTraits<BigPropertyIndex>::Type currentNonLocalMapIndex;
        typename WriteBarrierFieldTypeTraits<bool>::Type doneWithExports;
        typename WriteBarrierFieldTypeTraits<bool>::Type doneWithSymbol;
        typename WriteBarrierFieldTypeTraits<EnumeratorFlags>::Type flags;
    };
}

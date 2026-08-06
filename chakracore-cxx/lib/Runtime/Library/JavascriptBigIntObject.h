//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

namespace Js
{
    class JavascriptBigIntObject : public DynamicObject
    {
    private:
        typename WriteBarrierFieldTypeTraits<JavascriptBigInt*>::Type value;

        DEFINE_VTABLE_CTOR(JavascriptBigIntObject, DynamicObject);
        friend class Js::CrossSiteObject<JavascriptBigIntObject>;
        void MarshalToScriptContext(Js::ScriptContext *scriptContext) override
        {
            Assert(this->GetScriptContext() != scriptContext);
            AssertMsg(VirtualTableInfo<JavascriptBigIntObject>::HasVirtualTable(this),
                      "Derived class need to define marshal to script context");
            VirtualTableInfo<Js::CrossSiteObject<JavascriptBigIntObject>>::SetVirtualTable(this);
        };
    public:
        JavascriptBigIntObject(JavascriptBigInt* value, DynamicType * type);

        JavascriptBigInt* GetValue() const;
    };

    template <> inline bool VarIsImpl<JavascriptBigIntObject>(RecyclableObject* obj)
    {
        return JavascriptOperators::GetTypeId(obj) == TypeIds_BigIntObject;
    }
}

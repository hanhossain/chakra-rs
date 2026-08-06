//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

namespace Js
{
    class JavascriptBooleanObject : public DynamicObject
    {
    private:
        typename WriteBarrierFieldTypeTraits<JavascriptBoolean*>::Type value;

        DEFINE_VTABLE_CTOR(JavascriptBooleanObject, DynamicObject);
        friend class Js::CrossSiteObject<JavascriptBooleanObject>;
        void MarshalToScriptContext(Js::ScriptContext *scriptContext) override
        {
            Assert(this->GetScriptContext() != scriptContext);
            AssertMsg(VirtualTableInfo<JavascriptBooleanObject>::HasVirtualTable(this),
                      "Derived class need to define marshal to script context");
            VirtualTableInfo<Js::CrossSiteObject<JavascriptBooleanObject>>::SetVirtualTable(this);
        };
    public:
        JavascriptBooleanObject(JavascriptBoolean* value, DynamicType * type);

        BOOL GetValue() const;
        void Initialize(JavascriptBoolean* value);

        BOOL GetDiagValueString(StringBuilder<ArenaAllocator>* stringBuilder, ScriptContext* requestContext) override;
        BOOL GetDiagTypeString(StringBuilder<ArenaAllocator>* stringBuilder, ScriptContext* requestContext) override;
    };

    template <> inline bool VarIsImpl<JavascriptBooleanObject>(RecyclableObject* obj)
    {
        return JavascriptOperators::GetTypeId(obj) == TypeIds_BooleanObject;
    }
}

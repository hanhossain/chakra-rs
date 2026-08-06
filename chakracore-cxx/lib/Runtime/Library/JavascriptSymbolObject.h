//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

namespace Js
{
    class JavascriptSymbolObject : public DynamicObject
    {
    private:
        typename WriteBarrierFieldTypeTraits<JavascriptSymbol*>::Type value;

        DEFINE_VTABLE_CTOR(JavascriptSymbolObject, DynamicObject);
        friend class Js::CrossSiteObject<JavascriptSymbolObject>;
        void MarshalToScriptContext(Js::ScriptContext *scriptContext) override
        {
            Assert(this->GetScriptContext() != scriptContext);
            AssertMsg(VirtualTableInfo<JavascriptSymbolObject>::HasVirtualTable(this),
                      "Derived class need to define marshal to script context");
            VirtualTableInfo<Js::CrossSiteObject<JavascriptSymbolObject>>::SetVirtualTable(this);
        };

    public:
        JavascriptSymbolObject(JavascriptSymbol* value, DynamicType * type);

        inline const PropertyRecord* GetValue()
        {
            if (value == nullptr)
            {
                return nullptr;
            }
            return value->GetValue();
        }

        Var Unwrap() const;

        BOOL GetDiagValueString(StringBuilder<ArenaAllocator>* stringBuilder, ScriptContext* requestContext) override;
        BOOL GetDiagTypeString(StringBuilder<ArenaAllocator>* stringBuilder, ScriptContext* requestContext) override;
    };

    template <> inline bool VarIsImpl<JavascriptSymbolObject>(RecyclableObject* obj)
    {
        return JavascriptOperators::GetTypeId(obj) == TypeIds_SymbolObject;
    }
}

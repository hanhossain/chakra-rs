//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

namespace Js
{
    class JavascriptNumberObject : public DynamicObject
    {
    private:
        typename WriteBarrierFieldTypeTraits<Var>::Type value;

        DEFINE_VTABLE_CTOR(JavascriptNumberObject, DynamicObject);
        DEFINE_MARSHAL_OBJECT_TO_SCRIPT_CONTEXT(JavascriptNumberObject);
    protected:
        JavascriptNumberObject(DynamicType * type);

    public:
        JavascriptNumberObject(Var value, DynamicType * type);
        double GetValue() const;
        void SetValue(Var value);
        Var Unwrap() const;

        virtual BOOL GetDiagValueString(StringBuilder<ArenaAllocator>* stringBuilder, ScriptContext* requestContext) override;
        virtual BOOL GetDiagTypeString(StringBuilder<ArenaAllocator>* stringBuilder, ScriptContext* requestContext) override;
    };

    template <> inline bool VarIsImpl<JavascriptNumberObject>(RecyclableObject* obj)
    {
        return JavascriptOperators::GetTypeId(obj) == TypeIds_NumberObject;
    }
}

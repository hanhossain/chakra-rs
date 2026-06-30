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
        DEFINE_MARSHAL_OBJECT_TO_SCRIPT_CONTEXT(JavascriptBooleanObject);
    public:
        JavascriptBooleanObject(JavascriptBoolean* value, DynamicType * type);

        BOOL GetValue() const;
        void Initialize(JavascriptBoolean* value);

        virtual BOOL GetDiagValueString(StringBuilder<ArenaAllocator>* stringBuilder, ScriptContext* requestContext) override;
        virtual BOOL GetDiagTypeString(StringBuilder<ArenaAllocator>* stringBuilder, ScriptContext* requestContext) override;
    };

    template <> inline bool VarIsImpl<JavascriptBooleanObject>(RecyclableObject* obj)
    {
        return JavascriptOperators::GetTypeId(obj) == TypeIds_BooleanObject;
    }
}

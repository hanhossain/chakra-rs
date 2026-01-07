//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#pragma once

namespace Js
{
    class WebAssemblyTable : public DynamicObject
    {
    protected:
        DEFINE_VTABLE_CTOR( WebAssemblyTable, DynamicObject );
        DEFINE_MARSHAL_OBJECT_TO_SCRIPT_CONTEXT( WebAssemblyTable );
#ifdef ENABLE_WASM
    public:
        class EntryInfo
        {
        public:
            static FunctionInfo NewInstance;
            static FunctionInfo GetterLength;
            static FunctionInfo Grow;
            static FunctionInfo Get;
            static FunctionInfo Set;
        };
        WebAssemblyTable(
            Field(Var) * values, uint32_t currentLength, uint32_t initialLength, uint32_t maxLength, DynamicType * type);
        static Var NewInstance(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryGetterLength(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryGrow(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryGet(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntrySet(RecyclableObject* function, CallInfo callInfo, ...);

        static WebAssemblyTable * Create(uint32_t initial, uint32_t maximum, ScriptContext * scriptContext);

        uint32_t GetCurrentLength() const;
        uint32_t GetInitialLength() const;
        uint32_t GetMaximumLength() const;

        void DirectSetValue(uint index, Var val);
        Var DirectGetValue(uint index) const;

        static uint32_t GetOffsetOfValues() { return offsetof(WebAssemblyTable, m_values); }
        static uint32_t GetOffsetOfCurrentLength() { return offsetof(WebAssemblyTable, m_currentLength); }
    private:
        Field(uint32_t) m_initialLength;
        Field(uint32_t) m_maxLength;

        Field(uint32_t) m_currentLength;
        Field(Field(Var)*) m_values;
#endif
    };

    template <> inline bool VarIsImpl<WebAssemblyTable>(RecyclableObject* obj)
    {
        return JavascriptOperators::GetTypeId(obj) == TypeIds_WebAssemblyTable;
    }
}

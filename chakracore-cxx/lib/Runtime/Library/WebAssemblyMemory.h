//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#pragma once

namespace Js
{
    class WebAssemblyMemory : public DynamicObject
    {
    protected:
        DEFINE_VTABLE_CTOR( WebAssemblyMemory, DynamicObject );
        DEFINE_MARSHAL_OBJECT_TO_SCRIPT_CONTEXT( WebAssemblyMemory );

#ifdef ENABLE_WASM
    public:
        class EntryInfo
        {
        public:
            static FunctionInfo NewInstance;
            static FunctionInfo Grow;
            static FunctionInfo GetterBuffer;
        };

        static Var NewInstance(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryGrow(RecyclableObject* function, CallInfo callInfo, ...);
        static Var EntryGetterBuffer(RecyclableObject* function, CallInfo callInfo, ...);

        static WebAssemblyMemory * CreateMemoryObject(uint32_t initial, uint32_t maximum, bool isShared, ScriptContext * scriptContext);
        static WebAssemblyMemory * CreateForExistingBuffer(uint32_t initial, uint32_t maximum, uint32_t currentByteLength, ScriptContext * scriptContext);
#ifdef ENABLE_WASM_THREADS
        static WebAssemblyMemory * CreateFromSharedContents(uint32_t initial, uint32_t maximum, SharedContents* sharedContents, ScriptContext * scriptContext);
#endif

        ArrayBufferBase * GetBuffer() const;
        uint GetInitialLength() const;
        uint GetMaximumLength() const;
        uint GetCurrentMemoryPages() const;
#ifdef ENABLE_WASM_THREADS
        bool IsSharedMemory() const;
#endif

        int32_t GrowInternal(uint32_t deltaPages);
        static int32_t GrowHelper(Js::WebAssemblyMemory * memory, uint32_t deltaPages);

        static int GetOffsetOfArrayBuffer() { return offsetof(WebAssemblyMemory, m_buffer); }
#if DBG
        static void TraceMemWrite(WebAssemblyMemory* mem, uint32_t index, uint32_t offset, Js::ArrayBufferView::ViewType viewType, uint32_t bytecodeOffset, ScriptContext* context);
#endif
    private:
        WebAssemblyMemory(ArrayBufferBase* buffer, uint32_t initial, uint32_t maximum, DynamicType * type);
        static void CheckLimits(ScriptContext * scriptContext, uint32_t initial, uint32_t maximum);
        static void CheckLimits(ScriptContext * scriptContext, uint32_t initial, uint32_t maximum, uint32_t bufferLength);

        Field(ArrayBufferBase*) m_buffer;

        Field(uint) m_initial;
        Field(uint) m_maximum;
#endif
    };

    template <> inline bool VarIsImpl<WebAssemblyMemory>(RecyclableObject* obj)
    {
        return JavascriptOperators::GetTypeId(obj) == TypeIds_WebAssemblyMemory;
    }
} // namespace Js

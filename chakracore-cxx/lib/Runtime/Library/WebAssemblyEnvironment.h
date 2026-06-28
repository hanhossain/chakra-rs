//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#pragma once

namespace Wasm
{
    class WasmGlobal;
    struct WasmConstLitNode;
}
namespace Js
{
    class WebAssemblyModule;
    class WebAssemblyTable;
    class WebAssemblyMemory;
    class WebAssemblyEnvironment
    {
    public:
        WebAssemblyEnvironment(WebAssemblyModule* module);

        typename WriteBarrierFieldTypeTraits<Var>::Type* GetStartPtr() const { return start; }

        WasmScriptFunction* GetWasmFunction(uint32_t index) const;
        void SetWasmFunction(uint32_t index, WasmScriptFunction* func);
        void SetImportedFunction(uint32_t index, Var importedFunc);
        WebAssemblyTable* GetTable(uint32_t index) const;
        void SetTable(uint32_t index, class WebAssemblyTable* table);
        WebAssemblyMemory* GetMemory(uint32_t index) const;
        void SetMemory(uint32_t index, WebAssemblyMemory* mem);
        Wasm::WasmConstLitNode GetGlobalValue(Wasm::WasmGlobal* global) const;
        void SetGlobalValue(class Wasm::WasmGlobal* global, Wasm::WasmConstLitNode cnst);

        void CalculateOffsets(WebAssemblyTable* table, WebAssemblyMemory* memory);
        uint32_t GetElementSegmentOffset(uint32_t index) const;
        uint32_t GetDataSegmentOffset(uint32_t index) const;

    private:
        Field(WebAssemblyModule*) module;
        typename WriteBarrierFieldTypeTraits<Field(Var>::Type*) start;
        typename WriteBarrierFieldTypeTraits<Field(Var>::Type*) end;
        // Precalculated pointer from ptr using the offsets
        typename WriteBarrierFieldTypeTraits<Field(Var>::Type*) memory;
        typename WriteBarrierFieldTypeTraits<Field(Var>::Type*) imports;
        typename WriteBarrierFieldTypeTraits<Field(Var>::Type*) functions;
        typename WriteBarrierFieldTypeTraits<Field(Var>::Type*) table;
        typename WriteBarrierFieldTypeTraits<Field(Var>::Type*) globals;

        uint32_t* elementSegmentOffsets;
        uint32_t* dataSegmentOffsets;
        DebugOnly(bool offsetInitialized = false);

        ArenaAllocator m_alloc;
    private:
        template<typename T> void CheckPtrIsValid(intptr_t ptr) const;
        template<typename T> T* GetVarElement(typename WriteBarrierFieldTypeTraits<Var>::Type* ptr, uint32_t index, uint32_t maxCount) const;
        template<typename T> void SetVarElement(typename WriteBarrierFieldTypeTraits<Var>::Type* ptr, T* val, uint32_t index, uint32_t maxCount);
        template<typename T> T GetGlobalInternal(uint32_t offset) const;
        template<typename T> void SetGlobalInternal(uint32_t offset, T val);
    };

} // namespace Js

//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

#ifdef ENABLE_WASM
#include "WasmParseTree.h"

namespace Wasm
{
    class WasmSignature;
    class WasmFunctionInfo;
    class WasmBinaryReader;
    class WasmDataSegment;
    class WasmElementSegment;
    class WasmGlobal;
    class WasmCompilationException;
    struct WasmImport;
    struct WasmExport;
    struct CustomSection;
    struct MemorySectionLimits;
    struct TableSectionLimits;
}

namespace Js
{
typedef HeapAllocator::AutoFreeArray<char16_t> AutoFreeExceptionMessage;

class WebAssemblyModule : public DynamicObject
{
protected:
    DEFINE_VTABLE_CTOR(WebAssemblyModule, DynamicObject);
    DEFINE_MARSHAL_OBJECT_TO_SCRIPT_CONTEXT(WebAssemblyModule);

public:

    class EntryInfo
    {
    public:
        static FunctionInfo NewInstance;
        static FunctionInfo Exports;
        static FunctionInfo Imports;
        static FunctionInfo CustomSections;
    };

    static Var NewInstance(RecyclableObject* function, CallInfo callInfo, ...);
    static Var EntryExports(RecyclableObject* function, CallInfo callInfo, ...);
    static Var EntryImports(RecyclableObject* function, CallInfo callInfo, ...);
    static Var EntryCustomSections(RecyclableObject* function, CallInfo callInfo, ...);

    static WebAssemblyModule * CreateModule(
        ScriptContext* scriptContext,
        class WebAssemblySource* src);

    static bool ValidateModule(
        ScriptContext* scriptContext,
        class WebAssemblySource* src);

public:
    WebAssemblyModule(Js::ScriptContext* scriptContext, const byte* binaryBuffer, uint binaryBufferLength, DynamicType * type);

    const byte* GetBinaryBuffer() const { return m_binaryBuffer; }
    uint GetBinaryBufferLength() const { return m_binaryBufferLength; }

    // The index used by those methods is the function index as describe by the WebAssembly design, ie: imports first then wasm functions
    uint32_t GetMaxFunctionIndex() const;
    Wasm::FunctionIndexTypes::Type GetFunctionIndexType(uint32_t funcIndex) const;

    void InitializeMemory(_In_ Wasm::MemorySectionLimits* memoryLimits);
    WebAssemblyMemory * CreateMemory() const;
    bool HasMemory() const { return m_hasMemory; }
    bool HasMemoryImport() const { return m_memImport != nullptr; }
    bool IsSharedMemory() const { return m_memoryIsShared; }
    uint32_t GetMemoryInitSize() const { return m_memoryInitSize; }
    uint32_t GetMemoryMaxSize() const { return m_memoryMaxSize; }

    Wasm::WasmSignature * GetSignatures() const;
    bool IsSignatureIndexValid(uint32_t index) const;
    Wasm::WasmSignature* GetSignature(uint32_t index) const;
    void SetSignatureCount(uint32_t count);
    uint32_t GetSignatureCount() const;

    uint32_t GetEquivalentSignatureId(uint32_t sigId) const;

    void InitializeTable(_In_ Wasm::TableSectionLimits* tableLimits);
    WebAssemblyTable * CreateTable() const;
    bool HasTable() const { return m_hasTable; }
    bool HasTableImport() const { return m_tableImport != nullptr; }
    uint32_t GetTableInitSize() const { return m_tableInitSize; }
    uint32_t GetTableMaxSize() const { return m_tableMaxSize; }

    uint GetWasmFunctionCount() const;
    Wasm::WasmFunctionInfo* AddWasmFunctionInfo(Wasm::WasmSignature* funsig);
    Wasm::WasmFunctionInfo* GetWasmFunctionInfo(uint index) const;
    void SwapWasmFunctionInfo(uint i1, uint i2);
    void AttachCustomInOutTracingReader(Wasm::WasmFunctionInfo* func, uint callIndex);

    void AllocateFunctionExports(uint32_t entries);
    uint GetExportCount() const { return m_exportCount; }
    void SetExport(uint32_t iExport, uint32_t funcIndex, const char16_t* exportName, uint32_t nameLength, Wasm::ExternalKinds kind);
    Wasm::WasmExport* GetExport(uint32_t iExport) const;

    uint32_t GetImportCount() const;
    Wasm::WasmImport* GetImport(uint32_t i) const;
    void AddFunctionImport(uint32_t sigId, const char16_t* modName, uint32_t modNameLen, const char16_t* fnName, uint32_t fnNameLen);
    void AddGlobalImport(const char16_t* modName, uint32_t modNameLen, const char16_t* importName, uint32_t importNameLen);
    void AddMemoryImport(const char16_t* modName, uint32_t modNameLen, const char16_t* importName, uint32_t importNameLen);
    void AddTableImport(const char16_t* modName, uint32_t modNameLen, const char16_t* importName, uint32_t importNameLen);
    uint32_t GetImportedFunctionCount() const { return m_importedFunctionCount; }

    uint GetOffsetFromInit(const Wasm::WasmNode& initExpr, const class WebAssemblyEnvironment* env) const;
    void ValidateInitExportForOffset(const Wasm::WasmNode& initExpr) const;

    void AddGlobal(Wasm::GlobalReferenceTypes::Type refType, Wasm::WasmTypes::WasmType type, bool isMutable, Wasm::WasmNode init);
    uint32_t GetGlobalCount() const;
    Wasm::WasmGlobal* GetGlobal(uint32_t index) const;

    void AllocateDataSegs(uint32_t count);
    void SetDataSeg(Wasm::WasmDataSegment* seg, uint32_t index);
    Wasm::WasmDataSegment* GetDataSeg(uint32_t index) const;
    uint32_t GetDataSegCount() const { return m_datasegCount; }

    void AllocateElementSegs(uint32_t count);
    void SetElementSeg(Wasm::WasmElementSegment* seg, uint32_t index);
    Wasm::WasmElementSegment* GetElementSeg(uint32_t index) const;
    uint32_t GetElementSegCount() const { return m_elementsegCount; }

    void SetStartFunction(uint32_t i);
    uint32_t GetStartFunction() const;

    uint32_t GetModuleEnvironmentSize() const;

    // elements at known offsets
    static uint GetMemoryOffset() { return 0; }
    static uint GetImportFuncOffset() { return GetMemoryOffset() + 1; }

    // elements at instance dependent offsets
    uint GetFuncOffset() const { return GetImportFuncOffset() + GetImportedFunctionCount(); }
    uint GetTableEnvironmentOffset() const { return GetFuncOffset() + GetWasmFunctionCount(); }
    uint GetGlobalOffset() const { return GetTableEnvironmentOffset() + 1; }
    uint GetOffsetForGlobal(Wasm::WasmGlobal* global) const;
    uint AddGlobalByteSizeToOffset(Wasm::WasmTypes::WasmType type, uint32_t offset) const;
    uint GetGlobalsByteSize() const;

    void AddCustomSection(Wasm::CustomSection customSection);
    uint32_t GetCustomSectionCount() const;
    Wasm::CustomSection GetCustomSection(uint32_t index) const;

    Wasm::WasmBinaryReader* GetReader() const { return m_reader; }

    static char16_t* FormatExceptionMessage(Wasm::WasmCompilationException* ex, AutoFreeExceptionMessage* autoClean, WebAssemblyModule* wasmModule = nullptr, FunctionBody* body = nullptr);

    virtual void Finalize(bool isShutdown) override;
    virtual void Dispose(bool isShutdown) override;
    virtual void Mark(Recycler * recycler) override;

private:
    static JavascriptString * GetExternalKindString(ScriptContext * scriptContext, Wasm::ExternalKinds kind);

    typename WriteBarrierFieldTypeTraits<bool>::Type m_hasTable : 1;
    typename WriteBarrierFieldTypeTraits<bool>::Type m_hasMemory : 1;
    typename WriteBarrierFieldTypeTraits<bool>::Type m_memoryIsShared : 1;
    // The binary buffer is recycler allocated, tied the lifetime of the buffer to the module
    typename WriteBarrierFieldTypeTraits<const byte*>::Type m_binaryBuffer;
    typename WriteBarrierFieldTypeTraits<uint>::Type m_binaryBufferLength;
    typename WriteBarrierFieldTypeTraits<uint32_t>::Type m_memoryInitSize;
    typename WriteBarrierFieldTypeTraits<uint32_t>::Type m_memoryMaxSize;
    typename WriteBarrierFieldTypeTraits<uint32_t>::Type m_tableInitSize;
    typename WriteBarrierFieldTypeTraits<uint32_t>::Type m_tableMaxSize;
    typename WriteBarrierFieldTypeTraits<Wasm::WasmSignature*>::Type m_signatures;
    typename WriteBarrierFieldTypeTraits<uint32_t*>::Type m_indirectfuncs;
    typename WriteBarrierFieldTypeTraits<Wasm::WasmElementSegment**>::Type m_elementsegs;
    typedef JsUtil::List<Wasm::WasmFunctionInfo*, Recycler> WasmFunctionInfosList;
    typename WriteBarrierFieldTypeTraits<WasmFunctionInfosList*>::Type m_functionsInfo;
    typename WriteBarrierFieldTypeTraits<Wasm::WasmExport*>::Type m_exports;
    typedef JsUtil::List<Wasm::WasmImport*, ArenaAllocator> WasmImportsList;
    typename WriteBarrierFieldTypeTraits<WasmImportsList*>::Type m_imports;
    typename WriteBarrierFieldTypeTraits<Wasm::WasmImport*>::Type m_memImport;
    typename WriteBarrierFieldTypeTraits<Wasm::WasmImport*>::Type m_tableImport;
    typename WriteBarrierFieldTypeTraits<uint32_t>::Type m_importedFunctionCount;
    typename WriteBarrierFieldTypeTraits<Wasm::WasmDataSegment**>::Type m_datasegs;
    typename WriteBarrierFieldTypeTraits<Wasm::WasmBinaryReader*>::Type m_reader;
    typename WriteBarrierFieldTypeTraits<uint32_t*>::Type m_equivalentSignatureMap;
    typedef JsUtil::List<Wasm::CustomSection, ArenaAllocator> CustomSectionsList;
    typename WriteBarrierFieldTypeTraits<CustomSectionsList*>::Type m_customSections;

    typename WriteBarrierFieldTypeTraits<uint>::Type m_globalCounts[Wasm::WasmTypes::Limit];
    typedef JsUtil::List<Wasm::WasmGlobal*, ArenaAllocator> WasmGlobalsList;
    typename WriteBarrierFieldTypeTraits<WasmGlobalsList *>::Type m_globals;

    typename WriteBarrierFieldTypeTraits<uint>::Type m_signaturesCount;
    typename WriteBarrierFieldTypeTraits<uint>::Type m_exportCount;
    typename WriteBarrierFieldTypeTraits<uint32_t>::Type m_datasegCount;
    typename WriteBarrierFieldTypeTraits<uint32_t>::Type m_elementsegCount;

    typename WriteBarrierFieldTypeTraits<uint32_t>::Type m_startFuncIndex;

    FieldNoBarrier(ArenaAllocator*) m_alloc;
};

template <> inline bool VarIsImpl<WebAssemblyModule>(RecyclableObject* obj)
{
    return JavascriptOperators::GetTypeId(obj) == TypeIds_WebAssemblyModule;
}

} // namespace Js
#endif

//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#pragma once

namespace Wasm
{
    class WasmReaderBase;

    // All the Wasm function's share the same reader (except when using CustomReaders)
    // This struct is used to remember where is this particular function in the buffer
    struct FunctionBodyReaderInfo
    {
        friend class WasmBinaryReader;
        FunctionBodyReaderInfo(uint32_t size = 0, size_t startOffset = 0): size(size), startOffset(startOffset) {}
    private:
        typename WriteBarrierFieldTypeTraits<uint32_t>::Type size = 0;
        typename WriteBarrierFieldTypeTraits<size_t>::Type startOffset = 0;
    };

    class WasmFunctionInfo
    {
    public:
        WasmFunctionInfo(ArenaAllocator* alloc, WasmSignature* signature, uint32_t number);

        void AddLocal(WasmTypes::WasmType type, uint32_t count = 1);
        Local GetLocal(uint32_t index) const;
        uint32_t GetResultCount() const;
        Local GetResult(uint32_t index) const;

        uint32_t GetLocalCount() const;
        Js::ArgSlot GetParamCount() const;

        void SetName(const char16_t* name, uint32_t nameLength) { m_name = name; m_nameLength = nameLength; }
        const char16_t* GetName() const { return m_name; }
        uint32_t GetNameLength() const { return m_nameLength; }

        uint32_t GetNumber() const { return m_number; }
        WasmSignature* GetSignature() const { return m_signature; }

        void SetExitLabel(Js::ByteCodeLabel label) { m_ExitLabel = label; }
        Js::ByteCodeLabel GetExitLabel() const { return m_ExitLabel; }
        Js::FunctionBody* GetBody() const { return m_body; }
        void SetBody(Js::FunctionBody* val) { m_body = val; }

        WasmReaderBase* GetCustomReader() const { return m_customReader; }
        void SetCustomReader(WasmReaderBase* customReader) { m_customReader = customReader; }
#if DBG_DUMP
        typename WriteBarrierFieldTypeTraits<WasmImport*, _no_write_barrier_policy, _no_write_barrier_policy>::Type importedFunctionReference;
#endif

        FunctionBodyReaderInfo GetReaderInfo() const
        {
            AssertMsg(!m_customReader, "ReaderInfo is not needed and invalid when a custom reader is present");
            return m_readerInfo;
        }
        void SetReaderInfo(FunctionBodyReaderInfo info)
        {
            AssertMsg(!m_customReader, "ReaderInfo is not needed and invalid when a custom reader is present");
            m_readerInfo = info;
        }
    private:

        typename WriteBarrierFieldTypeTraits<ArenaAllocator*, _no_write_barrier_policy, _no_write_barrier_policy>::Type m_alloc;
        typedef JsUtil::GrowingArray<Local, ArenaAllocator> WasmTypeArray;
        typename WriteBarrierFieldTypeTraits<WasmTypeArray>::Type m_locals;
        typename WriteBarrierFieldTypeTraits<Js::FunctionBody*>::Type m_body;
        typename WriteBarrierFieldTypeTraits<WasmSignature*>::Type m_signature;
        typename WriteBarrierFieldTypeTraits<Js::ByteCodeLabel>::Type m_ExitLabel;
        typename WriteBarrierFieldTypeTraits<WasmReaderBase*>::Type m_customReader;
        typename WriteBarrierFieldTypeTraits<const char16_t*>::Type m_name;
        typename WriteBarrierFieldTypeTraits<uint32_t>::Type m_nameLength;
        typename WriteBarrierFieldTypeTraits<uint32_t>::Type m_number;
        typename WriteBarrierFieldTypeTraits<FunctionBodyReaderInfo>::Type m_readerInfo;
    };
} // namespace Wasm

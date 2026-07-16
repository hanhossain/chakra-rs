//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#pragma once

namespace Wasm
{
    class WasmSignature
    {
    public:
        WasmSignature();

        void SetSignatureId(uint32_t id);

        void AllocateParams(Js::ArgSlot count, Recycler *recycler);
        void SetParam(WasmTypes::WasmType type, Js::ArgSlot index);
        Local GetParam(Js::ArgSlot index) const;
        Js::ArgSlot GetParamCount() const;
        Js::ArgSlot GetParamSize(Js::ArgSlot index) const;
        Js::ArgSlot GetParamsSize() const;

        void AllocateResults(uint32_t count, Recycler *recycler);
        void SetResult(Local type, uint32_t index);
        uint32_t GetResultCount() const { return m_resultsCount; }
        Local GetResult(uint32_t index) const;

        void FinalizeSignature();
        uint32_t GetSignatureId() const;
        size_t GetShortSig() const;

        template <bool useShortSig = true>
        bool IsEquivalent(const WasmSignature *sig) const;
        static WasmSignature *FromIDL(WasmSignatureIDL *sig);

        static uint32_t GetOffsetOfShortSig() { return offsetof(WasmSignature, m_shortSig); }

        uint32_t WriteSignatureToString(_Out_writes_(maxlen) char16_t *out, uint32_t maxlen);
        void Dump(uint32_t maxlen = 512);

    private:
        typename WriteBarrierFieldTypeTraits<uint32_t>::Type m_id;
        typename WriteBarrierFieldTypeTraits<uint32_t>::Type m_resultsCount = 0;
        typename WriteBarrierFieldTypeTraits<Js::ArgSlot>::Type m_paramSize;
        typename WriteBarrierFieldTypeTraits<Js::ArgSlot>::Type m_paramsCount;
        typename WriteBarrierFieldTypeTraits<size_t>::Type m_shortSig;
        typename WriteBarrierFieldTypeTraits<Local *>::Type m_params;
        typename WriteBarrierFieldTypeTraits<Local *>::Type m_results = nullptr;
    };

} // namespace Wasm

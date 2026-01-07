//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft Corporation and contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#pragma once

namespace Wasm
{

    class WasmElementSegment
    {
    public:
        WasmElementSegment(ArenaAllocator* alloc, const uint32_t index, const WasmNode initExpr, const uint32_t numElem);
        void AddElement(const uint32_t funcIndex);
        uint32_t GetElement(const uint32_t tableIndex) const;
        uint32_t GetNumElements() const { return m_numElem; }
        WasmNode GetOffsetExpr() const { return m_offsetExpr; }
    private:
        ArenaAllocator* m_alloc;
        uint32_t m_index;
        const WasmNode m_offsetExpr;
        uint32_t m_numElem;
        uint32_t m_offset;
        uint32_t m_elemIdx;
        uint32_t* m_elems;

        void Init();
    };
} // Namespace Wasm

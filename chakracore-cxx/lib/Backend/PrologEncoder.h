//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

#define IS_UNIBBLE(x) (!(((size_t)x) & ~((size_t)0xF)))
#define TO_UNIBBLE(x) (x & 0xF)
#define IS_UINT16(x)  (!(((size_t)x) & ~((size_t)0xFFFF)))
#define TO_UINT16(x)  (x & 0xFFFF)
#define IS_UINT32(x)  (!(((size_t)x) & ~((size_t)0xFFFFFFFF)))
#define TO_UINT32(x)  (x & 0xFFFFFFFF)

enum UnwindOp : uint8_t {
    UWOP_IGNORE      =  (uint8_t)-1,
    UWOP_PUSH_NONVOL =  0,
    UWOP_ALLOC_LARGE =  1,
    UWOP_ALLOC_SMALL =  2,
    UWOP_SAVE_XMM128 =  8
};

// ----------------------------------------------------------------------------
//  non-windows x64 unwind uses .eh_frame
// ----------------------------------------------------------------------------
#include "EhFrame.h"

class PrologEncoder
{
public:
    static const int SMALL_EHFRAME_SIZE = 0x40;
    static const int JIT_EHFRAME_SIZE = 0x80;

private:
    EhFrame ehFrame;
    uint8_t buffer[JIT_EHFRAME_SIZE];

    size_t cfiInstrOffset;      // last cfi emit instr offset
    size_t currentInstrOffset;  // current instr offset
                                // currentInstrOffset - cfiInstrOffset == advance
    unsigned cfaWordOffset;

public:
    PrologEncoder()
        :ehFrame(buffer, JIT_EHFRAME_SIZE),
          cfiInstrOffset(0), currentInstrOffset(0), cfaWordOffset(1)
    {}

    void RecordNonVolRegSave() {}
    void RecordXmmRegSave() {}
    void RecordAlloca(size_t size) {}
    void EncodeInstr(IR::Instr *instr, uint8_t size);

    void EncodeSmallProlog(uint8_t prologSize, size_t size);
    uint32_t SizeOfPData();
    uint8_t *Finalize(uint8_t *functionStart, uint32_t codeSize, uint8_t *pdataBuffer);

    void Begin(size_t prologStartOffset);
    void End();
    uint32_t SizeOfUnwindInfo() { return SizeOfPData(); }
    uint8_t *GetUnwindInfo() { return ehFrame.Buffer(); }
    void FinalizeUnwindInfo(uint8_t *functionStart, uint32_t codeSize);
};

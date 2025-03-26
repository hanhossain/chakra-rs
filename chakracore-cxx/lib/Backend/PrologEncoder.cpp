//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "Backend.h"
#include "PrologEncoderMD.h"

// ----------------------------------------------------------------------------
//  non-windows x64 unwind uses .eh_frame
// ----------------------------------------------------------------------------

void PrologEncoder::EncodeSmallProlog(uint8 prologSize, size_t size)
{
    auto fde = ehFrame.GetFDE();

    // prolog: push rbp
    fde->cfi_advance_loc(1);                    // DW_CFA_advance_loc: 1
    fde->cfi_def_cfa_offset(MachPtr * 2);       // DW_CFA_def_cfa_offset: 16
    fde->cfi_offset(GetDwarfRegNum(LowererMDArch::GetRegFramePointer()), 2); // DW_CFA_offset: r6 (rbp) at cfa-16

    ehFrame.End();
}

DWORD PrologEncoder::SizeOfPData()
{
    return ehFrame.Count();
}

BYTE* PrologEncoder::Finalize(BYTE *functionStart, DWORD codeSize, BYTE *pdataBuffer)
{
    auto fde = ehFrame.GetFDE();
    fde->UpdateAddressRange(functionStart, codeSize);
    return ehFrame.Buffer();
}

void PrologEncoder::Begin(size_t prologStartOffset)
{
    Assert(currentInstrOffset == 0);

    currentInstrOffset = prologStartOffset;
}

void PrologEncoder::End()
{
    ehFrame.End();
}

void PrologEncoder::FinalizeUnwindInfo(BYTE *functionStart, DWORD codeSize)
{
    auto fde = ehFrame.GetFDE();
    fde->UpdateAddressRange(functionStart, codeSize);
}

void PrologEncoder::EncodeInstr(IR::Instr *instr, unsigned __int8 size)
{
    auto fde = ehFrame.GetFDE();

    uint8 unwindCodeOp = PrologEncoderMD::GetOp(instr);

    Assert((currentInstrOffset + size) > currentInstrOffset);
    currentInstrOffset += size;

    switch (unwindCodeOp)
    {
    case UWOP_PUSH_NONVOL:
    {
        const uword advance = currentInstrOffset - cfiInstrOffset;
        cfiInstrOffset = currentInstrOffset;
        cfaWordOffset++;

        fde->cfi_advance(advance);                              // DW_CFA_advance_loc: ?
        fde->cfi_def_cfa_offset(cfaWordOffset * MachPtr);       // DW_CFA_def_cfa_offset: ??

        const ubyte reg = PrologEncoderMD::GetNonVolRegToSave(instr) + 1;
        fde->cfi_offset(GetDwarfRegNum(reg), cfaWordOffset);    // DW_CFA_offset: r? at cfa-??
        break;
    }

    case UWOP_SAVE_XMM128:
    {
        // TODO
        break;
    }

    case UWOP_ALLOC_SMALL:
    case UWOP_ALLOC_LARGE:
    {
        size_t allocaSize = PrologEncoderMD::GetAllocaSize(instr);
        Assert(allocaSize % MachPtr == 0);

        size_t slots = allocaSize / MachPtr;
        Assert(cfaWordOffset + slots > cfaWordOffset);

        const uword advance = currentInstrOffset - cfiInstrOffset;
        cfiInstrOffset = currentInstrOffset;
        cfaWordOffset += slots;

        fde->cfi_advance(advance);                          // DW_CFA_advance_loc: ?
        fde->cfi_def_cfa_offset(cfaWordOffset * MachPtr);   // DW_CFA_def_cfa_offset: ??
        break;
    }

    case UWOP_IGNORE:
    {
        return;
    }

    default:
    {
        AssertMsg(false, "PrologEncoderMD returned unsupported UnwindCodeOp.");
    }
    }
}

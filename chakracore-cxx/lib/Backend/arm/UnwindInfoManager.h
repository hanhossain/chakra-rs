//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

enum UnwindCode;

class UnwindInfoManager
{
public:
    UnwindInfoManager() :
        func(NULL),
        fragmentStart(NULL),
        fragmentLength(0),
        epilogEndOffset(0),
        prologOffset(0),
        savedRegMask(0),
        savedDoubleRegMask(0),
        homedParamCount(0),
        stackDepth(0),
        hasCalls(false),
        prologLabelId(0),
        epilogEndLabelId(0),
        jitOutput(nullptr),
        alloc(nullptr),
        xdataTotal(0),
        pdataIndex(0),
        savedScratchReg(false)
    {
    }

    void Init(Func * func);
    void EmitUnwindInfo(JITOutput *jitOutput, CustomHeap::Allocation * alloc);
    uint32_t EmitLongUnwindInfoChunk(uint32_t remainingLength);

    void SetFunc(Func *func)
    {
        Assert(this->func == NULL);
        this->func = func;
    }
    Func * GetFunc() const
    {
        return this->func;
    }

    void SetFragmentStart(size_t pStart)
    {
        this->fragmentStart = pStart;
    }
    size_t GetFragmentStart() const
    {
        return this->fragmentStart;
    }

    void SetEpilogEndOffset(uint32_t offset)
    {
        Assert(this->epilogEndOffset == 0);
        this->epilogEndOffset = offset;
    }
    uint32_t GetEpilogEndOffset() const
    {
        return this->epilogEndOffset;
    }

    void SetPrologOffset(uint32_t offset)
    {
        Assert(this->prologOffset == 0);
        this->prologOffset = offset;
    }
    uint32_t GetPrologOffset() const
    {
        return this->prologOffset;
    }

    void SetFragmentLength(uint32_t length)
    {
        this->fragmentLength = length;
    }
    uint32_t GetFragmentLength() const
    {
        return this->fragmentLength;
    }

    void SetHomedParamCount(uint8_t count)
    {
        Assert(this->homedParamCount == 0);
        this->homedParamCount = count;
    }
    uint32_t GetHomedParamCount() const
    {
        return this->homedParamCount;
    }

    void SetStackDepth(uint32_t depth)
    {
        Assert(this->stackDepth == 0);
        this->stackDepth = depth;
    }
    uint32_t GetStackDepth() const
    {
        return this->stackDepth;
    }

    void SetHasCalls(bool has)
    {
        this->hasCalls = has;
    }
    bool GetHasCalls() const
    {
        return this->hasCalls;
    }

    void SetPrologStartLabel(uint32_t id)
    {
        Assert(this->prologLabelId == 0);
        this->prologLabelId = id;
    }
    uint32_t GetPrologStartLabel() const
    {
        return this->prologLabelId;
    }

    void SetEpilogEndLabel(uint32_t id)
    {
        Assert(this->epilogEndLabelId == 0);
        this->epilogEndLabelId = id;
    }
    uint32_t GetEpilogEndLabel() const
    {
        return this->epilogEndLabelId;
    }

    bool GetHasChkStk() const;
    uint32_t GetPDataCount(uint32_t length);
    void SetSavedReg(uint8_t reg);
    bool TestSavedReg(uint8_t reg) const;
    uint32_t ClearSavedReg(uint32_t mask, uint8_t reg) const;

    void SetDoubleSavedRegList(uint32_t doubleRegMask);
    uint32_t GetDoubleSavedRegList() const;

    static uint8_t GetLastSavedReg(uint32_t mask);
    static uint8_t GetFirstSavedReg(uint32_t mask);

    void SetSavedScratchReg(bool value) { savedScratchReg = value; }
    bool GetSavedScratchReg() { return savedScratchReg; }

private:

    Func * func;
    size_t fragmentStart;
    int pdataIndex;
    int xdataTotal;
    HANDLE processHandle;
    JITOutput *jitOutput;
    CustomHeap::Allocation * alloc;
    uint32_t fragmentLength;
    uint32_t prologOffset;
    uint32_t prologLabelId;
    uint32_t epilogEndLabelId;
    uint32_t epilogEndOffset;
    uint32_t savedRegMask;
    uint32_t savedDoubleRegMask;
    uint32_t stackDepth;
    uint8_t homedParamCount;
    bool hasCalls;
    bool fragmentHasProlog;
    bool fragmentHasEpilog;
    bool savedScratchReg;

    void EmitPdata();
    bool CanEmitPackedPdata() const;
    void EncodePackedUnwindData();
    void EncodeExpandedUnwindData();
    uint8_t * GetBaseAddress();

    bool IsR4SavedRegRange(bool saveR11) const;
    static bool IsR4SavedRegRange(uint32_t saveRegMask);

    uint32_t XdataTemplate(UnwindCode op) const;
    uint32_t XdataLength(UnwindCode op) const;

    uint32_t EmitXdataStackAlloc(uint8_t xData[], uint32_t byte, uint32_t stack);
    uint32_t EmitXdataHomeParams(uint8_t xData[], uint32_t byte);
    uint32_t EmitXdataRestoreRegs(uint8_t xData[], uint32_t byte, uint32_t savedRegMask, bool restoreLR);
    uint32_t EmitXdataRestoreDoubleRegs(uint8_t xData[], uint32_t byte, uint32_t savedDoubleRegMask);
    uint32_t EmitXdataIndirReturn(uint8_t xData[], uint32_t byte);
    uint32_t EmitXdataNop32(uint8_t xData[], uint32_t byte);
    uint32_t EmitXdataNop16(uint8_t xData[], uint32_t byte);
    uint32_t EmitXdataEnd(uint8_t xData[], uint32_t byte);
    uint32_t EmitXdataEndPlus16(uint8_t xData[], uint32_t byte);
    uint32_t EmitXdataLocalsPointer(uint8_t xData[], uint32_t byte, uint8_t regEncode);
    uint32_t RelativeRegEncoding(RegNum reg, RegNum baseReg) const;
    uint32_t WriteXdataBytes(uint8_t xdata[], uint32_t byte, uint32_t encoding, uint32_t length);

    void RecordPdataEntry(uint32_t beginAddress, uint32_t unwindData);
    // Constants defined in the ABI.

    static const uint32_t MaxPackedPdataFuncLength = 0xFFE;
    static const uint32_t MaxPackedPdataStackDepth = 0xFCC;

    static const uint32_t PackedPdataFlagMask = 3;
    static const uint32_t ExpandedPdataFlag = 0;

    // Function length is required to have only these bits set.
    static const uint32_t PackedFuncLengthMask = 0xFFE;
    // Bit offset of length within pdata uint32_t, combined with right-shift of encoded length.
    static const uint32_t PackedFuncLengthShift = 1;

    static const uint32_t PackedNoPrologBits = 2;
    static const uint32_t PackedNormalFuncBits = 1;

    static const uint32_t PackedNonLeafRetBits = 0;
    static const uint32_t PackedLeafRetBits = (1 << 13);
    static const uint32_t PackedNoEpilogBits = (3 << 13);

    // C (frame chaining) and L (save LR) bits.
    static const uint32_t PackedNonLeafFunctionBits = (1 << 20) | (1 << 21);

    static const uint32_t PackedHomedParamsBit = (1 << 15);

    static const uint32_t PackedRegShift = 16;
    static const uint32_t PackedRegMask = 7;
    // Indicate no saved regs with a Reg field of 0x111 and the R bit set.
    static const uint32_t PackedNoSavedRegsBits = (7 << PackedRegShift) | (1 << 19);

    // Stack depth is required to have only these bits set.
    static const uint32_t PackedStackDepthMask = 0xFFC;
    // Bit offset of stack depth within pdata uint32_t, combined with right-shift of encoded value.
    static const uint32_t PackedStackDepthShift = 20;

    static const uint32_t MaxXdataFuncLength = 0x7FFFE;
    static const uint32_t XdataFuncLengthMask = 0x7FFFE;
    static const uint32_t XdataFuncLengthAdjust = 1;

    static const uint32_t XdataSingleEpilogShift = 21;
    static const uint32_t XdataFuncFragmentShift = 22;
    static const uint32_t XdataEpilogCountShift = 23;

    static const uint32_t MaxXdataEpilogCount = 0x1F;
    static const uint32_t MaxXdataDwordCount = 0xF;
    static const uint32_t XdataDwordCountShift = 28;

public:
    // Xdata constants.
    static const uint32_t MaxXdataBytes = 40; //buffer of 4 for any future additions
    //
    // 28 == 4 (header uint32_t) +
    //      (4 (max stack alloc code) +
    //       1 (locals pointer setup) +
    //       5 (NOP for _chkstk case) +
    //       1 (double reg saves) +
    //       2 (reg saves) +
    //       1 (r11 setup) +
    //       2 (r11,lr saves) +
    //       1 (home params) +
    //       1 (NOP) +
    //       1 (end prolog) +
    //       4 (max stack alloc code, in case of locals pointer setup) +
    //       1 (double reg saves) +
    //       2 (reg saves) +
    //       2 (r11 save) +
    //       2 (indir return) +
    //       1 (end epilog)) rounded up to a uint32_t boundary

};

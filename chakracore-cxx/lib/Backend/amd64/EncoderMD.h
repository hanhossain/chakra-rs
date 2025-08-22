//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
class Encoder;


enum RelocType {
    RelocTypeBranch,                // cond, uncond branch
    RelocTypeCallPcrel,             // calls
    RelocTypeLabelUse,              // direct use of a label
    RelocTypeLabel,                 // points to label instr
    RelocTypeAlignedLabel,          // points to loop-top label instr that needs alignment
    RelocTypeInlineeEntryOffset,    // points to offset immediate in buffer
};


///---------------------------------------------------------------------------
///
/// class EncoderReloc
///
///---------------------------------------------------------------------------

class EncodeRelocAndLabels
{
public:
    RelocType           m_type;
    void *              m_ptr;          // points to encoded buffer byte or LabelInstr if RelocTypeLabel
    void *              m_origPtr;      // copy of m_ptr to be used to get offset in the source buffer during BR shortening

private:
    union
    {
        IR::LabelInstr* m_labelInstr;        // ptr to Br Label
        uint8_t            m_nopCount;
        unsigned long          m_InlineeOffset;
    };
    bool                m_isShortBr;

public:
    void                init(RelocType type, void* ptr, IR::LabelInstr* labelInstr = nullptr)
    {
        m_type = type;
        m_ptr = ptr;
        m_InlineeOffset = 0;
        m_isShortBr = false;

        if (type == RelocTypeLabel)
        {
            // preserve original PC for labels
            m_origPtr   = (void*)((IR::LabelInstr*)ptr)->GetPC();
            m_nopCount  = 0;
        }
        else
        {
            m_origPtr = ptr;
            if (type == RelocTypeBranch)
            {
                Assert(labelInstr);
                m_labelInstr = labelInstr;
                m_isShortBr = false;
            }
            else if (type == RelocTypeLabelUse)
            {
                Assert(labelInstr);
                m_labelInstr = labelInstr;
            }
        }
    }

    void                revert()
    {
        // recover old label PC
        if (isLabel())
        {
            // recover old label PC and reset alignment nops
            // we keep aligned labels type so we align them on the second attempt.
            setLabelCurrPC(getLabelOrigPC());
            m_nopCount = 0;
            return;
        }

        if (m_type == RelocTypeBranch)
        {
            m_isShortBr = false;
        }

        m_ptr = m_origPtr;
    }

    bool                isLabel()           const { return isAlignedLabel() || m_type == RelocTypeLabel; }
    bool                isAlignedLabel()    const { return m_type == RelocTypeAlignedLabel; }
    bool                isLongBr()          const { return m_type == RelocTypeBranch && !m_isShortBr; }
    bool                isShortBr()         const { return m_type == RelocTypeBranch && m_isShortBr; }
    uint8_t*               getBrOpCodeByte()   const { return (uint8_t*)m_origPtr - 1;}

    IR::LabelInstr *    getBrTargetLabel()  const
    {
        Assert((m_type == RelocTypeBranch || m_type == RelocTypeLabelUse) && m_labelInstr);
        return m_labelInstr;
    }
    IR::LabelInstr *    getLabel()  const
    {
        Assert(isLabel());
        return (IR::LabelInstr*) m_ptr;
    }
    // get label original PC without shortening/alignment
    uint8_t *  getLabelOrigPC()  const
    {
        Assert(isLabel());
        return ((uint8_t*) m_origPtr);
    }

    // get label PC after shortening/alignment
    uint8_t *  getLabelCurrPC()  const
    {
        Assert(isLabel());
        return getLabel()->GetPC();
    }

    uint8_t    getLabelNopCount() const
    {
        Assert(isAlignedLabel());
        return m_nopCount;
    }

    void    setLabelCurrPC(uint8_t* pc)
    {
        Assert(isLabel());
        getLabel()->SetPC(pc);
    }

    void    setLabelNopCount(uint8_t nopCount)
    {
        Assert(isAlignedLabel());
        Assert (nopCount >= 0 && nopCount < 16);
        m_nopCount = nopCount;
    }

    void                setAsShortBr()
    {
        Assert(m_type == RelocTypeBranch);
        m_isShortBr = true;
    }

    // Validates if the branch is short and its target PC fits in one byte
    bool                validateShortBrTarget() const
    {
        return isShortBr() &&
            getBrTargetLabel()->GetPC() - ((uint8_t*)m_ptr + 1) >= -128 &&
            getBrTargetLabel()->GetPC() - ((uint8_t*)m_ptr + 1) <= 127;
    }

    unsigned long GetInlineOffset()
    {
        return m_InlineeOffset;
    }

    void SetInlineOffset(unsigned long offset)
    {
        m_InlineeOffset = offset;
    }
};



///---------------------------------------------------------------------------
///
/// class EncoderMD
///
///---------------------------------------------------------------------------
enum Forms : uint8_t;

typedef JsUtil::List<EncodeRelocAndLabels, ArenaAllocator> RelocList;
typedef JsUtil::List<InlineeFrameRecord*, ArenaAllocator> InlineeFrameRecords;

struct FixUpMapIndex;

class EncoderMD
{
public:
    EncoderMD(Func * func) : m_func(func) {}
    ptrdiff_t       Encode(IR::Instr * instr, uint8_t *pc, uint8_t* beginCodeAddress = nullptr);
    void            Init(Encoder *encoder);
    void            ApplyRelocs(size_t codeBufferAddress, size_t codeSize, uint* bufferCRC, BOOL isBrShorteningSucceeded, bool isFinalBufferValidation = false);
    uint            GetRelocDataSize(EncodeRelocAndLabels *reloc);
    uint8_t *          GetRelocBufferAddress(EncodeRelocAndLabels * reloc);
    void            EncodeInlineeCallInfo(IR::Instr *instr, uint32 offset);
    static bool     TryConstFold(IR::Instr *instr, IR::RegOpnd *regOpnd);
    static bool     TryFold(IR::Instr *instr, IR::RegOpnd *regOpnd);
    static bool     SetsConditionCode(IR::Instr *instr);
    static bool     UsesConditionCode(IR::Instr *instr);
    static bool     IsOPEQ(IR::Instr *instr);
    static bool     IsSHIFT(IR::Instr *instr);
    static bool     IsMOVEncoding(IR::Instr *instr);
    RelocList*      GetRelocList() const { return m_relocList; }
    int             AppendRelocEntry(RelocType type, void *ptr, IR::LabelInstr *label= nullptr);
    int             FixRelocListEntry(uint32 index, int totalBytesSaved, uint8_t *buffStart, uint8_t* buffEnd);
    void            FixMaps(uint32 brOffset, uint32 bytesSaved, FixUpMapIndex *mapIndices);
    void            UpdateRelocListWithNewBuffer(RelocList * relocList, uint8_t * newBuffer, uint8_t * oldBufferStart, uint8_t * oldBufferEnd);
#ifdef DBG
    void            VerifyRelocList(uint8_t *buffStart, uint8_t *buffEnd);
#endif
    void            AddLabelReloc(uint8_t* relocAddress);

private:
    uint8_t            GetOpcodeByte2(IR::Instr *instr);
    static Forms    GetInstrForm(IR::Instr *instr);
    const uint8_t *    GetFormTemplate(IR::Instr *instr);
    const uint8_t *    GetOpbyte(IR::Instr *instr);
    uint8_t            GetRegEncode(IR::RegOpnd *regOpnd);
    uint8_t            GetRegEncode(RegNum reg);
    static uint32   GetOpdope(IR::Instr *instr);
    uint32          GetLeadIn(IR::Instr * instr);
    uint8_t            EmitModRM(IR::Instr * instr, IR::Opnd *opnd, uint8_t reg1);
    void            EmitConst(size_t val, int size, bool allowImm64 = false);
    uint8_t            EmitImmed(IR::Opnd * opnd, int opSize, int sbit, bool allowImm64 = false);
    static bool     FitsInByte(size_t value);
    bool            IsExtendedRegister(RegNum reg);
    uint8_t            GetMod(IR::IndirOpnd * opr, int* pDispSize);
    uint8_t            GetMod(IR::SymOpnd * opr, int* pDispSize, RegNum& rmReg);
    uint8_t            GetMod(size_t offset, bool regIsRbpOrR13, int * pDispSize);
    uint8_t            GetRexByte(uint8_t rexCode, IR::Opnd * opnd);
    uint8_t            GetRexByte(uint8_t rexCode, RegNum reg);
    int             GetOpndSize(IR::Opnd * opnd);

    void            EmitRexByte(uint8_t * prexByte, uint8_t rexByte, bool skipRexByte, bool reservedRexByte);

    enum
    {
        REXOVERRIDE  = 0x40,
        REXW = 8,
        REXR = 4,
        REXX = 2,
        REXB = 1,
    };

    static const uint8_t Mod00 = 0x00 << 6;
    static const uint8_t Mod01 = 0x01 << 6;
    static const uint8_t Mod10 = 0x02 << 6;
    static const uint8_t Mod11 = 0x03 << 6;

private:
    Func *          m_func;
    Encoder *       m_encoder;
    uint8_t *          m_pc;
    RelocList *     m_relocList;
    int             m_lastLoopLabelPosition;
};

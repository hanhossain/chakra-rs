//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once
using namespace JsUtil;

/*
*   CaseNode - represents the case statements (not the case block) in the switch statement
*/
class CaseNode
{
private:
    uint32_t              offset;         // offset - indicates the bytecode offset of the case instruction
    uint32_t              targetOffset;   // targetOffset - indicates the bytecode offset of the target instruction (case block)
    IR::BranchInstr*    caseInstr;      // caseInstr - stores the case instruction
    IR::Opnd*           lowerBound;     // lowerBound - used for integer cases

    int32_t GetIntConst(IR::Opnd* opnd)
    {
        Assert(IsIntConst(opnd));
        return opnd->IsIntConstOpnd() ? opnd->AsIntConstOpnd()->AsInt32() : opnd->GetStackSym()->GetIntConstValue();
    }

    bool IsIntConst(IR::Opnd* opnd)
    {
        return opnd->IsIntConstOpnd() || opnd->GetStackSym()->IsIntConst();
    }

    bool IsStrConst(IR::Opnd* opnd)
    {
        return opnd->GetStackSym()->m_isStrConst;
    }

public:
    CaseNode(IR::BranchInstr* caseInstr, uint32_t offset, uint32_t targetOffset, IR::Opnd* lowerBound = nullptr)
        : caseInstr(caseInstr),
        offset(offset),
        targetOffset(targetOffset),
        lowerBound(lowerBound)
    {
    }

    int32_t GetUpperBoundIntConst()
    {
        AssertMsg(IsUpperBoundIntConst(), "Source2 operand is not an integer constant");
        return GetIntConst(GetUpperBound());
    }

    JITJavascriptString* GetUpperBoundStringConstLocal()
    {
        AssertMsg(IsUpperBoundStrConst(), "Upper bound operand is not a string constant");
        return JITJavascriptString::FromVar(GetUpperBound()->GetStackSym()->GetConstAddress(true));
    }

    JITJavascriptString* GetUpperBoundStrConst()
    {
        AssertMsg(IsUpperBoundStrConst(), "Upper bound operand is not a string constant");
        return static_cast<JITJavascriptString*>(GetUpperBound()->GetStackSym()->GetConstAddress(false));
    }

    bool IsUpperBoundIntConst()
    {
        return IsIntConst(GetUpperBound());
    }

    bool IsUpperBoundStrConst()
    {
        return IsStrConst(GetUpperBound());
    }

    int32_t GetLowerBoundIntConst()
    {
        AssertMsg(IsLowerBoundIntConst(), "LowerBound is not an integer constant");
        return GetIntConst(lowerBound);
    }

    bool IsLowerBoundIntConst()
    {
        return IsIntConst(lowerBound);
    }

    bool IsLowerBoundStrConst()
    {
        return IsStrConst(lowerBound);
    }

    uint32_t GetOffset()
    {
        return offset;
    }

    uint32_t GetTargetOffset()
    {
        return targetOffset;
    }

    IR::Opnd* GetUpperBound()
    {
        return caseInstr->GetSrc2();
    }

    IR::Opnd* GetLowerBound()
    {
        return lowerBound;
    }

    void SetLowerBound(IR::Opnd* lowerBound)
    {
        this->lowerBound = lowerBound;
    }

    IR::BranchInstr* GetCaseInstr()
    {
        return caseInstr;
    }
};

template <>
struct DefaultComparer<CaseNode *>
{
public:
    static int Compare(CaseNode* caseNode1, CaseNode* caseNode2);
    static bool Equals(CaseNode* x, CaseNode* y);
    static uint GetHashCode(CaseNode * caseNode);
};


//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "Backend.h"
#include "ARM64UnwindEncoder.h"

UnwindInfoManager::UnwindInfoManager()
{
    for (int which = UnwindInvalid; which < UnwindFunctionOffsetCount; which++)
    {
        this->m_functionOffsetLabelId[which] = 0;
        this->m_functionOffset[which] = 0xffffffff;
    }
}

void UnwindInfoManager::Init(Func * func)
{
    // necessary? Maybe not, but keeping around for now
}

uint32_t UnwindInfoManager::GetPDataCount(uint32_t length)
{
    // TODO: Support splitting huge functions if needed?
    return 1;
}

uint32_t UnwindInfoManager::GetFunctionOffset(UnwindFunctionOffsets which)
{
    Assert(which < UnwindFunctionOffsetCount);
    uint32_t result = this->m_functionOffset[which];
    Assert(result != 0xffffffff);
    Assert(result % 4 == 0);
    return result;
}

void UnwindInfoManager::FinalizeUnwindInfo(uint8_t *functionStart, uint32_t codeSize)
{
    // fetch the appropriate offsets and hand off to the generic code
    m_xdata.Generate((uint32_t *)(functionStart + this->GetFunctionOffset(UnwindPrologStart)),
                     (uint32_t *)(functionStart + this->GetFunctionOffset(UnwindPrologEnd)),
                     (uint32_t *)(functionStart + this->GetFunctionOffset(UnwindEpilogStart)),
                     (uint32_t *)(functionStart + this->GetFunctionOffset(UnwindEpilogEnd)),
                     (uint32_t *)(functionStart + codeSize));
}

void UnwindInfoManager::SetFunctionOffsetLabel(UnwindFunctionOffsets which, IR::LabelInstr *label)
{
    Assert(which < UnwindFunctionOffsetCount);
    Assert(this->m_functionOffsetLabelId[which] == 0);
    this->m_functionOffsetLabelId[which] = label->m_id;
}

void UnwindInfoManager::SetLabelOffset(uint32_t id, uint32_t offset)
{
    Assert(offset % 4 == 0);
    for (int which = UnwindInvalid + 1; which < UnwindFunctionOffsetCount; which++)
    {
        if (this->m_functionOffsetLabelId[which] == id)
        {
            Assert(this->m_functionOffset[which] == 0xffffffff);
            this->m_functionOffset[which] = offset;
        }
    }
}

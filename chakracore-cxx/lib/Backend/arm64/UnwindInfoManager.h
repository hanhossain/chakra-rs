//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

#include "ARM64UnwindEncoder.h"

enum UnwindFunctionOffsets
{
    UnwindInvalid,
    UnwindPrologStart,
    UnwindPrologEnd,
    UnwindEpilogStart,
    UnwindEpilogEnd,
    UnwindFunctionOffsetCount
};

class UnwindInfoManager
{
public:
    UnwindInfoManager();

    void Init(Func * func);
    uint32_t GetPDataCount(uint32_t length);
    uint32_t SizeOfUnwindInfo() const { return m_xdata.GetXdataBytes(); }
    uint8_t *GetUnwindInfo() { return reinterpret_cast<uint8_t *>(const_cast<void *>(m_xdata.GetXdata())); }
    void FinalizeUnwindInfo(uint8_t *functionStart, uint32_t codeSize);

    // label management
    void SetFunctionOffsetLabel(UnwindFunctionOffsets which, IR::LabelInstr *label);
    void SetLabelOffset(uint32_t id, uint32_t offset);

private:
    uint32_t GetFunctionOffset(UnwindFunctionOffsets which);

    uint32_t m_functionOffsetLabelId[UnwindFunctionOffsetCount];
    uint32_t m_functionOffset[UnwindFunctionOffsetCount];

    Arm64XdataGenerator m_xdata;
};

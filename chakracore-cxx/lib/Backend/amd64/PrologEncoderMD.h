//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

class PrologEncoderMD
{
public:
    static uint8_t GetRequiredNodeCountForAlloca(size_t size);
    static uint8_t GetOp(IR::Instr *instr);
    static uint8_t GetNonVolRegToSave(IR::Instr *instr);
    static uint8_t GetXmmRegToSave(IR::Instr *instr, uint16_t *scaledOffset);
    static size_t          GetAllocaSize(IR::Instr *instr);
    static uint8_t GetFPReg();
};

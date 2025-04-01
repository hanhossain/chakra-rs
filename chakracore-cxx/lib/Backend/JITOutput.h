//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#pragma once

class JITOutput
{
public:
    JITOutput(JITOutputIDL * outputData);

    void SetHasJITStackClosure();

    void SetVarSlotsOffset(int32 offset);
    void SetVarChangedOffset(int32 offset);
    void SetHasBailoutInstr(bool val);
    void SetArgUsedForBranch(uint8_t param);
    void SetFrameHeight(uint val);
    void RecordThrowMap(Js::ThrowMapEntry * throwMap, uint mapCount);
#ifdef _M_ARM
    void RecordXData(uint8_t * xdata);
#endif
    bool IsTrackCompoundedIntOverflowDisabled() const;
    bool IsMemOpDisabled() const;
    bool IsArrayCheckHoistDisabled() const;
    bool IsStackArgOptDisabled() const;
    bool IsSwitchOptDisabled() const;
    bool IsAggressiveIntTypeSpecDisabled() const;

    uint16 GetArgUsedForBranch() const;
    intptr_t GetCodeAddress() const;
    size_t GetCodeSize() const;
    ushort GetPdataCount() const;
    ushort GetXdataSize() const;

    void SetCodeAddress(intptr_t addr);

    EmitBufferAllocation<VirtualAllocWrapper, PreReservedVirtualAllocWrapper> * RecordInProcNativeCodeSize(Func *func, uint32 bytes, ushort pdataCount, ushort xdataSize);
#if ENABLE_OOP_NATIVE_CODEGEN
    EmitBufferAllocation<SectionAllocWrapper, PreReservedSectionAllocWrapper> * RecordOOPNativeCodeSize(Func *func, uint32 bytes, ushort pdataCount, ushort xdataSize);
#endif
    void RecordNativeCode(const uint8_t* sourceBuffer, uint8_t* localCodeAddress);
    void RecordInlineeFrameOffsetsInfo(unsigned int offsetsArrayOffset, unsigned int offsetsArrayCount);

#if TARGET_64
    void RecordUnwindInfo(uint8_t *unwindInfo, size_t size, uint8_t * xdataAddr, uint8_t* localXdataAddr);
#elif _M_ARM
    size_t RecordUnwindInfo(size_t offset, const uint8_t *unwindInfo, size_t size, uint8_t * xdataAddr);
#endif

    void FinalizeNativeCode();

    JITOutputIDL * GetOutputData();
private:
    template <typename TEmitBufferAllocation, typename TCodeGenAllocators>
    void RecordNativeCode(const uint8_t* sourceBuffer, uint8_t* localCodeAddress, TEmitBufferAllocation allocation, TCodeGenAllocators codeGenAllocators);
    CustomHeap::Allocation * GetAllocation() const;
    union
    {
        EmitBufferAllocation<VirtualAllocWrapper, PreReservedVirtualAllocWrapper> * m_inProcAlloc;
#if ENABLE_OOP_NATIVE_CODEGEN
        EmitBufferAllocation<SectionAllocWrapper, PreReservedSectionAllocWrapper> * m_oopAlloc;
#endif
    };
    Func * m_func;
    JITOutputIDL * m_outputData;
};

//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#include "RuntimeLanguagePch.h"

#if defined(ASMJS_PLAT) || defined(ENABLE_WASM)
#include "InterpreterStackFrame.h"

namespace WAsmJs
{

#ifdef ENABLE_DEBUG_CONFIG_OPTIONS
    namespace Tracing
    {
        // This can be broken if exception are thrown from wasm frames
        int callDepth = 0;
        int GetPrintCol()
        {
            return callDepth;
        }

        void PrintArgSeparator()
        {
            Output::Print(u", ");
        }

        void PrintBeginCall()
        {
            ++callDepth;
            Output::Print(u") {\n");
        }

        void PrintNewLine()
        {
            Output::Print(u"\n");
        }

        void PrintEndCall(int hasReturn)
        {
            callDepth = callDepth > 0 ? callDepth - 1 : 0;
            Output::Print(u"%*s}", GetPrintCol(), u"");
            if (hasReturn)
            {
                Output::Print(u" = ");
            }
        }

        int PrintI32(int val)
        {
            Output::Print(u"%d", val);
            return val;
        }

        long PrintI64(long val)
        {
            Output::Print(u"%lld", val);
            return val;
        }

        float PrintF32(float val)
        {
            Output::Print(u"%.4f", val);
            return val;
        }

        double PrintF64(double val)
        {
            Output::Print(u"%.4f", val);
            return val;
        }
    }
#endif
    void JitFunctionIfReady(Js::ScriptFunction* func, uint interpretedCount /*= 0*/)
    {
#if ENABLE_NATIVE_CODEGEN
        Js::FunctionBody* body = func->GetFunctionBody();
        if (WAsmJs::ShouldJitFunction(body, interpretedCount))
        {
            if (PHASE_TRACE(Js::AsmjsEntryPointInfoPhase, body))
            {
                Output::Print(u"Scheduling %s For Full JIT at callcount:%d\n", body->GetDisplayName(), interpretedCount);
            }
            GenerateFunction(body->GetScriptContext()->GetNativeCodeGenerator(), body, func);
            body->SetIsAsmJsFullJitScheduled(true);
        }
#endif
    }

    bool ShouldJitFunction(Js::FunctionBody* body, uint interpretedCount)
    {
#if ENABLE_NATIVE_CODEGEN
        if (PHASE_OFF(Js::BackEndPhase, body) ||
            PHASE_OFF(Js::FullJitPhase, body) ||
            body->GetScriptContext()->GetConfig()->IsNoNative() ||
            body->GetIsAsmJsFullJitScheduled())
        {
            return false;
        }
#if ENABLE_OOP_NATIVE_CODEGEN
        if (JITManager::GetJITManager()->IsOOPJITEnabled() && !JITManager::GetJITManager()->IsConnected())
        {
            return false;
        }
#endif
        const bool forceNative = CONFIG_ISENABLED(Js::ForceNativeFlag);
        const uint minAsmJsInterpretRunCount = (uint)CONFIG_FLAG(MinAsmJsInterpreterRunCount);
        const uint maxAsmJsInterpretRunCount = (uint)CONFIG_FLAG(MaxAsmJsInterpreterRunCount);
        return forceNative || interpretedCount >= minAsmJsInterpretRunCount || interpretedCount >= maxAsmJsInterpretRunCount;
#else
        return false;
#endif
    }

    uint32_t ConvertOffset(uint32_t offset, uint32_t fromSize, uint32_t toSize)
    {
        if (fromSize == toSize)
        {
            return offset;
        }
        unsigned long tmp = (unsigned long)offset * (unsigned long)fromSize;
        tmp = Math::Align<unsigned long>(tmp, toSize);
        tmp /= (unsigned long)toSize;
        if (tmp > (unsigned long)UINT32_MAX)
        {
            Math::DefaultOverflowPolicy();
        }
        return (uint32_t)tmp;
    }

    uint32_t GetTypeByteSize(Types type)
    {
        // Since this needs to be done manually for each type, this assert will make sure to not forget to update this if a new type is added
        CompileAssert(WAsmJs::LIMIT == 5);
        switch (type)
        {
        case INT32  : return sizeof(int32);
        case INT64  : return sizeof(int64_t);
        case FLOAT32: return sizeof(float);
        case FLOAT64: return sizeof(double);
        case SIMD   : return sizeof(AsmJsSIMDValue);
        default     : break;
        }
        Js::Throw::InternalError();
    }

    WAsmJs::Types FromIRType(IRType irType)
    {
        switch(irType)
        {
        case TyInt8:
        case TyInt16:
        case TyInt32:
        case TyUint8:
        case TyUint16:
        case TyUint32:
            return WAsmJs::INT32;
        case TyInt64:
        case TyUint64:
            return WAsmJs::INT64;
        case TyFloat32:
            return WAsmJs::FLOAT32;
        case TyFloat64:
            return WAsmJs::FLOAT64;
        case TySimd128F4:
        case TySimd128I4:
        case TySimd128I8:
        case TySimd128I16:
        case TySimd128U4:
        case TySimd128U8:
        case TySimd128U16:
        case TySimd128B4:
        case TySimd128B8:
        case TySimd128B16:
        case TySimd128D2:
            return WAsmJs::SIMD;
        }
        return WAsmJs::LIMIT;
    }

    template<> Types FromPrimitiveType<int32>() { return WAsmJs::INT32; }
    template<> Types FromPrimitiveType<long>() { return WAsmJs::INT64; }
    template<> Types FromPrimitiveType<float>() { return WAsmJs::FLOAT32; }
    template<> Types FromPrimitiveType<double>() { return WAsmJs::FLOAT64; }
    template<> Types FromPrimitiveType<AsmJsSIMDValue>() { return WAsmJs::SIMD; }

#if DBG_DUMP
    void RegisterSpace::GetTypeDebugName(Types type, char16_t* buf, uint bufsize, bool shortName)
    {
        // Since this needs to be done manually for each type, this assert will make sure to not forget to update this if a new type is added
        CompileAssert(LIMIT == 5);

        switch (type)
        {
        case INT32: wcscpy_s(buf, bufsize  , shortName ? u"I": u"INT32"); break;
        case INT64: wcscpy_s(buf, bufsize  , shortName ? u"L": u"INT64"); break;
        case FLOAT32: wcscpy_s(buf, bufsize, shortName ? u"F": u"FLOAT32"); break;
        case FLOAT64: wcscpy_s(buf, bufsize, shortName ? u"D": u"FLOAT64"); break;
        case SIMD: wcscpy_s(buf, bufsize   , u"SIMD"); break;
        default: wcscpy_s(buf, bufsize     , u"UNKNOWN"); break;
        }
    }

    void RegisterSpace::PrintTmpRegisterAllocation(RegSlot loc, bool deallocation)
    {
        if (PHASE_TRACE1(Js::AsmjsTmpRegisterAllocationPhase))
        {
            char16_t buf[16];
            GetTypeDebugName(mType, buf, 16, true);
            Output::Print(u"%s%s %d\n", deallocation ? u"-" : u"+", buf, loc);
        }
    }
#endif

    TypedRegisterAllocator::TypedRegisterAllocator(ArenaAllocator* allocator, AllocateRegisterSpaceFunc allocateFunc, uint32_t excludedMask/* = 0*/)
    {
        Assert(allocateFunc);
        AssertMsg(excludedMask >> WAsmJs::LIMIT == 0, "Invalid type in the excluded mask");
        mExcludedMask = excludedMask;

        for (int i = 0; i < WAsmJs::LIMIT; ++i)
        {
            Types type = (Types)i;
            mTypeSpaces[i] = nullptr;
            if (!IsTypeExcluded(type))
            {
                mTypeSpaces[i] = allocateFunc(allocator, type);
#if DBG_DUMP
                mTypeSpaces[i]->mType = type;
#endif
            }
        }
    }

    void TypedRegisterAllocator::CommitToFunctionInfo(Js::AsmJsFunctionInfo* funcInfo, Js::FunctionBody* body) const
    {
        uint32_t offset = Js::AsmJsFunctionMemory::RequiredVarConstants * sizeof(Js::Var);
        WAsmJs::TypedConstSourcesInfo constSourcesInfo = GetConstSourceInfos();

#if DBG_DUMP
        if (PHASE_TRACE(Js::AsmjsInterpreterStackPhase, body))
        {
            Output::Print(u"ASMFunctionInfo Stack Data for %s\n", body->GetDisplayName());
            Output::Print(u"==========================\n");
            Output::Print(u"RequiredVarConstants:%d\n", Js::AsmJsFunctionMemory::RequiredVarConstants);
        }
#endif

        for (int i = 0; i < WAsmJs::LIMIT; ++i)
        {
            Types type = (Types)i;

            TypedSlotInfo& slotInfo = *funcInfo->GetTypedSlotInfo(type);
            // Check if we don't want to commit this type
            if (!IsTypeExcluded(type))
            {
                RegisterSpace* registerSpace = GetRegisterSpace(type);
                slotInfo.constCount = registerSpace->GetConstCount();
                slotInfo.varCount = registerSpace->GetVarCount();
                slotInfo.tmpCount = registerSpace->GetTmpCount();
                slotInfo.constSrcByteOffset = constSourcesInfo.srcByteOffsets[i];

                offset = Math::AlignOverflowCheck(offset, GetTypeByteSize(type));
                slotInfo.byteOffset = offset;

                // Update offset for next type
                uint32_t totalTypeCount = 0;
                totalTypeCount = UInt32Math::Add(totalTypeCount, slotInfo.constCount);
                totalTypeCount = UInt32Math::Add(totalTypeCount, slotInfo.varCount);
                totalTypeCount = UInt32Math::Add(totalTypeCount, slotInfo.tmpCount);

                offset = UInt32Math::Add(offset, UInt32Math::Mul(totalTypeCount, GetTypeByteSize(type)));
#if DBG_DUMP
                if (PHASE_TRACE(Js::AsmjsInterpreterStackPhase, body))
                {
                    char16_t buf[16];
                    RegisterSpace::GetTypeDebugName(type, buf, 16);
                    Output::Print(u"%s Offset:%d  ConstCount:%d  VarCount:%d  TmpCount:%d = %d * %d = 0x%x bytes\n",
                                  buf,
                                  slotInfo.byteOffset,
                                  slotInfo.constCount,
                                  slotInfo.varCount,
                                  slotInfo.tmpCount,
                                  totalTypeCount,
                                  GetTypeByteSize(type),
                                  totalTypeCount * GetTypeByteSize(type));
                }
#endif
            }
        }

        // The offset currently carries the total size of the funcInfo after handling the last type
        funcInfo->SetTotalSizeinBytes(offset);

        // These bytes offset already calculated the alignment, used them to determine how many Js::Var we need to do the allocation
        uint32_t stackByteSize = offset;
        uint32_t bytesUsedForConst = constSourcesInfo.bytesUsed;
        uint32_t jsVarUsedForConstsTable = ConvertOffset<byte, Js::Var>(bytesUsedForConst);
        uint32_t totalVarsNeeded = ConvertOffset<byte, Js::Var>(stackByteSize);

        uint32_t jsVarNeededForVars = totalVarsNeeded - jsVarUsedForConstsTable;
        if (totalVarsNeeded < jsVarUsedForConstsTable)
        {
            // If for some reason we allocated more space in the const table than what we need, just don't allocate anymore vars
            jsVarNeededForVars = 0;
        }

        Assert((jsVarUsedForConstsTable + jsVarNeededForVars) >= totalVarsNeeded);
        body->CheckAndSetVarCount(jsVarNeededForVars);

#if DBG_DUMP
        if (PHASE_TRACE(Js::AsmjsInterpreterStackPhase, body))
        {
            Output::Print(u"Total memory required: (%u consts + %u vars) * sizeof(Js::Var) >= %u * sizeof(Js::Var) = 0x%x bytes\n",
                          jsVarUsedForConstsTable,
                          jsVarNeededForVars,
                          totalVarsNeeded,
                          stackByteSize);
        }
#endif
    }

    void TypedRegisterAllocator::CommitToFunctionBody(Js::FunctionBody* body)
    {
        // this value is the number of Var slots needed to allocate all the const
        uint32_t bytesUsedForConst = GetConstSourceInfos().bytesUsed;
        // Add the registers not included in the const table
        uint32_t nbConst = ConvertOffset<byte, Js::Var>(bytesUsedForConst) + Js::FunctionBody::FirstRegSlot;
        body->CheckAndSetConstantCount(nbConst);
    }

    WAsmJs::TypedConstSourcesInfo TypedRegisterAllocator::GetConstSourceInfos() const
    {
        WAsmJs::TypedConstSourcesInfo infos;
        // The const table doesn't contain the first reg slots (aka return register)
        uint32_t offset = ConvertOffset<Js::Var, byte>(Js::AsmJsFunctionMemory::RequiredVarConstants - Js::FunctionBody::FirstRegSlot);
        for (int i = 0; i < WAsmJs::LIMIT; ++i)
        {
            Types type = (Types)i;
            if (!IsTypeExcluded(type))
            {
                infos.srcByteOffsets[i] = offset;
                uint32_t typeSize = GetTypeByteSize(type);
                uint32_t constCount = GetRegisterSpace(type)->GetConstCount();
                uint32_t typeBytesUsage = ConvertOffset<byte>(constCount, typeSize);
                offset = Math::AlignOverflowCheck(offset, typeSize);
                offset = UInt32Math::Add(offset, typeBytesUsage);
            }
            else
            {
                infos.srcByteOffsets[i] = (uint32_t)Js::Constants::InvalidOffset;
            }
        }
        infos.bytesUsed = offset;
        return infos;
    }

    bool TypedRegisterAllocator::IsValidType(Types type) const
    {
        return (uint)type < WAsmJs::LIMIT;
    }

    bool TypedRegisterAllocator::IsTypeExcluded(Types type) const
    {
        return !IsValidType(type) || (mExcludedMask & (1 << type)) != 0;
    }

#if DBG_DUMP
    void TypedRegisterAllocator::DumpLocalsInfo() const
    {
        for (int i = 0; i < WAsmJs::LIMIT; ++i)
        {
            Types type = (Types)i;
            if (!IsTypeExcluded(type))
            {
                char16_t typeName[16];
                char16_t shortTypeName[16];
                RegisterSpace::GetTypeDebugName(type, typeName, 16);
                RegisterSpace::GetTypeDebugName(type, shortTypeName, 16, true);
                RegisterSpace* registerSpace = GetRegisterSpace(type);
                Output::Print(
                    u"     %-10s : %u locals (%u temps from %s%u)\n",
                    typeName,
                    registerSpace->GetVarCount(),
                    registerSpace->GetTmpCount(),
                    shortTypeName,
                    registerSpace->GetFirstTmpRegister());
            }
        }
    }

    void TypedRegisterAllocator::GetArgumentStartIndex(uint32_t* indexes) const
    {
        for (int i = 0; i < WAsmJs::LIMIT; ++i)
        {
            Types type = (Types)i;
            if (!IsTypeExcluded(type))
            {
                // Arguments starts right after the consts
                indexes[i] = GetRegisterSpace(type)->GetConstCount();
            }
        }
    }
#endif

    RegisterSpace* TypedRegisterAllocator::GetRegisterSpace(Types type) const
    {
        if (!IsValidType(type))
        {
            Assert("Invalid type for RegisterSpace in TypedMemoryStructure");
            Js::Throw::InternalError();
        }
        Assert(!IsTypeExcluded(type));
        return mTypeSpaces[type];
    }
};

#endif

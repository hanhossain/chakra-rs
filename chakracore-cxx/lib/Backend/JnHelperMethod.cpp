//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft Corporation and contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "Backend.h"
#include "ExternalHelperMethod.h"
// Parser includes
#include "RegexCommon.h"

#include "Library/RegexHelper.h"

#ifdef ENABLE_SCRIPT_DEBUGGING
#include "Debug/DiagHelperMethodWrapper.h"
#endif
#include "Math/CrtSSE2Math.h"
#include "Library/JavascriptGeneratorFunction.h"
#include "RuntimeMathPch.h"

namespace IR
{

intptr_t const JnHelperMethodAddresses[] =
{
#define HELPERCALL(Name, Address, Attributes) reinterpret_cast<intptr_t>(Address),
// Because of order-of-initialization problems with the vtable address static field
// and this array, we're going to have to fill these in as we go along.
#include "JnHelperMethodList.h"
    NULL
};

intptr_t const *GetHelperMethods()
{
    return JnHelperMethodAddresses;
}

#ifdef ENABLE_SCRIPT_DEBUGGING
static intptr_t const helperMethodWrappers[] = {
    reinterpret_cast<intptr_t>(&Js::HelperMethodWrapper0),
    reinterpret_cast<intptr_t>(&Js::HelperMethodWrapper1),
    reinterpret_cast<intptr_t>(&Js::HelperMethodWrapper2),
    reinterpret_cast<intptr_t>(&Js::HelperMethodWrapper3),
    reinterpret_cast<intptr_t>(&Js::HelperMethodWrapper4),
    reinterpret_cast<intptr_t>(&Js::HelperMethodWrapper5),
    reinterpret_cast<intptr_t>(&Js::HelperMethodWrapper6),
    reinterpret_cast<intptr_t>(&Js::HelperMethodWrapper7),
    reinterpret_cast<intptr_t>(&Js::HelperMethodWrapper8),
    reinterpret_cast<intptr_t>(&Js::HelperMethodWrapper9),
    reinterpret_cast<intptr_t>(&Js::HelperMethodWrapper10),
    reinterpret_cast<intptr_t>(&Js::HelperMethodWrapper11),
    reinterpret_cast<intptr_t>(&Js::HelperMethodWrapper12),
    reinterpret_cast<intptr_t>(&Js::HelperMethodWrapper13),
    reinterpret_cast<intptr_t>(&Js::HelperMethodWrapper14),
    reinterpret_cast<intptr_t>(&Js::HelperMethodWrapper15),
    reinterpret_cast<intptr_t>(&Js::HelperMethodWrapper16),
};
#endif

///----------------------------------------------------------------------------
///
/// GetMethodAddress
///
///     returns the memory address of the helperMethod,
///     which can the address of debugger wrapper that intercept the original helper.
///
///----------------------------------------------------------------------------
intptr_t
GetMethodAddress(ThreadContextInfo * context, IR::HelperCallOpnd* opnd)
{
    Assert(opnd);
#ifdef ENABLE_SCRIPT_DEBUGGING
#if defined(_M_ARM32_OR_ARM64)
#define LowererMDFinal LowererMD
#else
#define LowererMDFinal LowererMDArch
#endif

    CompileAssert(_countof(helperMethodWrappers) == LowererMDFinal::MaxArgumentsToHelper + 1);

    if (opnd->IsDiagHelperCallOpnd())
    {
        // Note: all arguments are already loaded for the original helper. Here we just return the address.
        IR::DiagHelperCallOpnd* diagOpnd = static_cast<IR::DiagHelperCallOpnd*>(opnd);

        if (0 <= diagOpnd->m_argCount && diagOpnd->m_argCount <= LowererMDFinal::MaxArgumentsToHelper)
        {
            return ShiftAddr(context, helperMethodWrappers[diagOpnd->m_argCount]);
        }
        else
        {
            AssertMsg(FALSE, "Unsupported arg count (need to implement).");
        }
    }
#endif
    return GetMethodOriginalAddress(context, opnd->m_fnHelper);
}

// We need the helper table to be in read-only memory for obvious security reasons.
// Import function ptr require dynamic initialization, and cause the table to be in read-write memory.
// Additionally, all function ptrs are automatically marked as safe CFG addresses by the compiler.
// __declspec(guard(ignore)) can be used on methods to have the compiler not mark these as valid CFG targets.
intptr_t GetNonTableMethodAddress(ThreadContextInfo * context, JnHelperMethod helperMethod)
{
    switch (helperMethod)
    {
    //
    //  DllImport methods
    //
#if defined(_M_IX86)
    // These are internal CRT functions which don't use a standard calling convention
    case HelperDirectMath_Acos:
        return ShiftAddr(context, __libm_sse2_acos);

    case HelperDirectMath_Asin:
        return ShiftAddr(context, __libm_sse2_asin);

    case HelperDirectMath_Atan:
        return ShiftAddr(context, __libm_sse2_atan);

    case HelperDirectMath_Atan2:
        return ShiftAddr(context, __libm_sse2_atan2);

    case HelperDirectMath_Cos:
        return ShiftAddr(context, __libm_sse2_cos);

    case HelperDirectMath_Exp:
        return ShiftAddr(context, __libm_sse2_exp);

    case HelperDirectMath_Log:
        return ShiftAddr(context, __libm_sse2_log);

    case HelperDirectMath_Sin:
        return ShiftAddr(context, __libm_sse2_sin);

    case HelperDirectMath_Tan:
        return ShiftAddr(context, __libm_sse2_tan);

    case HelperAtomicStore64:
        return ShiftAddr(context, (double(*)(double))InterlockedExchange64);

    case HelperMemoryBarrier:
#ifdef _M_HYBRID_X86_ARM64
        AssertOrFailFastMsg(false, "The usage below fails to build for CHPE, and HelperMemoryBarrier is only required "
                                   "for WASM threads, which are currently disabled");
        return 0;
#else
        return ShiftAddr(context, (void(*)())MemoryBarrier);
#endif // !_M_HYBRID_X86_ARM64
#endif

    case HelperDirectMath_FloorDb:
        return ShiftStdcallAddr(context, Js::JavascriptMath::Floor);

    case HelperDirectMath_CeilDb:
        return ShiftStdcallAddr(context, Js::JavascriptMath::Ceil);

    case HelperDirectMath_FloorFlt:
        return ShiftStdcallAddr(context, Js::JavascriptMath::FloorF);

    case HelperDirectMath_CeilFlt:
        return ShiftStdcallAddr(context, Js::JavascriptMath::CeilF);

        //
        // These are statically initialized to an import thunk, but let's keep them out of the table in case a new CRT changes this
        //
    case HelperWMemCmp:
        return ShiftCdeclAddr(context, wmemcmp);

    case HelperMemCpy:
        return ShiftCdeclAddr(context, static_cast<void *(*)(void*, void const*, size_t)>(memcpy));

#if defined(_M_X64) || defined(_M_ARM32_OR_ARM64)
    case HelperDirectMath_Acos:
        return ShiftCdeclAddr(context, static_cast<double(*)(double)>(acos));

    case HelperDirectMath_Asin:
        return ShiftCdeclAddr(context, static_cast<double(*)(double)>(asin));

    case HelperDirectMath_Atan:
        return ShiftCdeclAddr(context, static_cast<double(*)(double)>(atan));

    case HelperDirectMath_Atan2:
        return ShiftCdeclAddr(context, static_cast<double(*)(double, double)>(atan2));

    case HelperDirectMath_Cos:
        return ShiftCdeclAddr(context, static_cast<double(*)(double)>(cos));

    case HelperDirectMath_Exp:
        return ShiftCdeclAddr(context, static_cast<double(*)(double)>(exp));

    case HelperDirectMath_Log:
        return ShiftCdeclAddr(context, static_cast<double(*)(double)>(log));

    case HelperDirectMath_Sin:
        return ShiftCdeclAddr(context, static_cast<double(*)(double)>(sin));

    case HelperDirectMath_Tan:
        return ShiftCdeclAddr(context, static_cast<double(*)(double)>(tan));
#endif

        //
        // Methods that we don't want to get marked as CFG targets as they make unprotected calls
        //

    case HelperOp_TryCatch:
        return ShiftStdcallAddr(context, Js::JavascriptExceptionOperators::OP_TryCatch);

    case HelperOp_TryFinally:
        return ShiftStdcallAddr(context, Js::JavascriptExceptionOperators::OP_TryFinally);


    case HelperOp_TryFinallyNoOpt:
        return ShiftStdcallAddr(context, Js::JavascriptExceptionOperators::OP_TryFinallyNoOpt);

        //
        // Methods that we don't want to get marked as CFG targets as they dump all registers to a controlled address
        //
    case HelperSaveAllRegistersAndBailOut:
        return ShiftStdcallAddr(context, LinearScanMD::SaveAllRegistersAndBailOut);
    case HelperSaveAllRegistersAndBranchBailOut:
        return ShiftStdcallAddr(context, LinearScanMD::SaveAllRegistersAndBranchBailOut);

#ifdef _M_IX86
    case HelperSaveAllRegistersNoSse2AndBailOut:
        return ShiftStdcallAddr(context, LinearScanMD::SaveAllRegistersNoSse2AndBailOut);
    case HelperSaveAllRegistersNoSse2AndBranchBailOut:
        return ShiftStdcallAddr(context, LinearScanMD::SaveAllRegistersNoSse2AndBranchBailOut);
#endif

    }

    Assume(UNREACHED);

    return 0;
}

///----------------------------------------------------------------------------
///
/// GetMethodOriginalAddress
///
///     returns the memory address of the helperMethod,
///     this one is never the intercepted by debugger helper.
///
///----------------------------------------------------------------------------
intptr_t GetMethodOriginalAddress(ThreadContextInfo * context, JnHelperMethod helperMethod)
{
    AssertOrFailFast(helperMethod >= 0 && helperMethod < IR::JnHelperMethodCount);
    intptr_t address = GetHelperMethods()[static_cast<uint16_t>(helperMethod)];
    if (address == 0)
    {
        return GetNonTableMethodAddress(context, helperMethod);
    }

    return ShiftAddr(context, address);
}

#if DBG_DUMP || defined(ENABLE_IR_VIEWER)



char16_t const * const JnHelperMethodNames[] =
{
#define HELPERCALL(Name, Address, Attributes) u"" STRINGIZEW(Name) u"",
#include "JnHelperMethodList.h"
    NULL
};

///----------------------------------------------------------------------------
///
/// GetMethodName
///
///     returns the string representing the name of the helperMethod.
///
///----------------------------------------------------------------------------

char16_t const*
GetMethodName(JnHelperMethod helperMethod)
{
    return JnHelperMethodNames[static_cast<uint16_t>(helperMethod)];
}

#endif  //#if DBG_DUMP


} //namespace IR

namespace HelperMethodAttributes
{

// Position: same as in JnHelperMethod enum.
// Value: one or more of OR'ed HelperMethodAttribute values.
static const uint8_t JnHelperMethodAttributes[] =
{
#define HELPERCALL(Name, Address, Attributes) Attributes,
#include "JnHelperMethodList.h"
};

// Returns true if the helper can throw non-OOM / non-SO exception.
bool CanThrow(IR::JnHelperMethod helper)
{
    return (JnHelperMethodAttributes[helper] & AttrCanThrow) != 0;
}

bool IsInVariant(IR::JnHelperMethod helper)
{
    return (JnHelperMethodAttributes[helper] & AttrInVariant) != 0;
}

bool CanBeReentrant(IR::JnHelperMethod helper)
{
    return (JnHelperMethodAttributes[helper] & AttrCanNotBeReentrant) == 0;
}

bool TempObjectProducing(IR::JnHelperMethod helper)
{
    return (JnHelperMethodAttributes[helper] & AttrTempObjectProducing) != 0;
}

#ifdef DBG_DUMP
struct ValidateHelperHeaders
{
    ValidateHelperHeaders()
    {
#define HELPERCALL(Name, Address, Attributes)
#define HELPERCALLCHK(Name, Address, Attributes) \
        Assert(JitHelperUtils::helper##Name##_implemented);
#include "JnHelperMethodList.h"
    }
};
ValidateHelperHeaders validateHelperHeaders;
#endif

} //namespace HelperMethodAttributes

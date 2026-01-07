//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft Corporation and contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

using namespace Wasm;

template<typename T> 
inline T WasmMath::Shl( T aLeft, T aRight )
{
    return aLeft << (aRight & (sizeof(T)*8-1));
}
template<typename T> 
inline T WasmMath::Shr( T aLeft, T aRight )
{
    return aLeft >> (aRight & (sizeof(T)*8-1));
}

template<typename T> 
inline T WasmMath::ShrU( T aLeft, T aRight )
{
    return aLeft >> (aRight & (sizeof(T)*8-1));
}
template<> 
inline int WasmMath::Ctz(int value)
{
    uint32_t index;
    if (_BitScanForward(&index, value))
    {
        return index;
    }
    return 32;
}

template<> 
inline long WasmMath::Ctz(long value)
{
    uint32_t index;
#if TARGET_64
    if (_BitScanForward64(&index, value))
    {
        return index;
    }
#else
    if (_BitScanForward(&index, (int32)value))
    {
        return index;
    }
    if (_BitScanForward(&index, (int32)(value >> 32)))
    {
        return index + 32;
    }
#endif
    return 64;
}

template<> 
inline long WasmMath::Clz(long value)
{
    uint32_t index;
#if TARGET_64
    if (_BitScanReverse64(&index, value))
    {
        return 63 - index;
    }
#else
    if (_BitScanReverse(&index, (int32)(value >> 32)))
    {
        return 31 - index;
    }
    if (_BitScanReverse(&index, (int32)value))
    {
        return 63 - index;
    }
#endif
    return 64;
}

template<> 
inline int WasmMath::PopCnt(int value)
{
    return ::Math::PopCnt32(value);
}

template<> 
inline long WasmMath::PopCnt(long value)
{
    unsigned long v = (unsigned long)value;
    // https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
    v = v - ((v >> 1) & 0x5555555555555555LL);
    v = (v & 0x3333333333333333LL) + ((v >> 2) & 0x3333333333333333LL);
    v = (v + (v >> 4)) & 0x0f0f0f0f0f0f0f0f;
    v = (unsigned long)(v * 0x0101010101010101LL) >> (sizeof(unsigned long) - 1) * CHAR_BIT;
    return (long)v;
}


template<typename T>
inline int WasmMath::Eqz(T value)
{
    return value == 0;
}

template<>
inline double WasmMath::Copysign(double aLeft, double aRight)
{
    unsigned long aLeftI64 = *(unsigned long*)(&aLeft);
    unsigned long aRightI64 = *(unsigned long*)(&aRight);
    unsigned long res = ((aLeftI64 & 0x7fffffffffffffffull) | (aRightI64 & 0x8000000000000000ull));
    return *(double*)(&res);
}

template<>
inline float WasmMath::Copysign(float aLeft, float aRight)
{
    uint32_t aLeftI32 = *(uint32_t*)(&aLeft);
    uint32_t aRightI32 = *(uint32_t*)(&aRight);
    uint32_t res = ((aLeftI32 & 0x7fffffffu) | (aRightI32 & 0x80000000u));
    return *(float*)(&res);
}

template <typename T> bool WasmMath::LessThan(T aLeft, T aRight)
{
    return aLeft < aRight;
}

template <typename T> bool WasmMath::LessOrEqual(T aLeft, T aRight)
{
    return aLeft <= aRight;
}

template <
    typename SrcType,
    typename DstType,
    typename ReinterpretType,
    ReinterpretType Max,
    ReinterpretType NegZero,
    ReinterpretType NegOne,
    WasmMath::CmpPtr<ReinterpretType> MaxCmp,
    WasmMath::CmpPtr<ReinterpretType> NegOneCmp,
    bool Saturate,
    DstType MinResult,
    DstType MaxResult>
DstType WasmMath::ConvertFloatToInt(SrcType srcVal, _In_ Js::ScriptContext * scriptContext)
{
    CompileAssert(sizeof(SrcType) == sizeof(ReinterpretType));

    if (IsNaN(srcVal))
    {
        if (!Saturate)
        {
            Js::JavascriptError::ThrowWebAssemblyRuntimeError(scriptContext, VBSERR_Overflow);
        }
        return 0;
    }

    // TODO NumberUtilities::ToSpecial

    ReinterpretType val = *reinterpret_cast<ReinterpretType*> (&srcVal);
    if (MaxCmp(val, Max) || (val >= NegZero && NegOneCmp(val, NegOne)))
    {
        return static_cast<DstType>(srcVal);
    }
    if (!Saturate)
    {
        Js::JavascriptError::ThrowWebAssemblyRuntimeError(scriptContext, VBSERR_Overflow);
    }
    return (srcVal < 0) ? MinResult : MaxResult;
}

template <typename STYPE> bool  WasmMath::IsNaN(STYPE src)
{
    return src != src;
}

template<typename T>
inline T WasmMath::Trunc(T value)
{
    if (value == 0.0)
    {
        return value;
    }
    else
    {
        T result;
        if (value < 0.0)
        {
            result = ceil(value);
        }
        else
        {
            result = floor(value);
        }
        // TODO: Propagating NaN sign for now awaiting consensus on semantics
        return result;
    }
}

template<typename T>
inline T WasmMath::Nearest(T value)
{
    if (value == 0.0)
    {
        return value;
    }
    else
    {
        T result;
        T u = ceil(value);
        T d = floor(value);
        T um = fabs(value - u);
        T dm = fabs(value - d);
        if (um < dm || (um == dm && floor(u / 2) == u / 2))
        {
            result = u;
        }
        else
        {
            result = d;
        }
        // TODO: Propagating NaN sign for now awaiting consensus on semantics
        return result;
    }
}

template<>
inline int WasmMath::Rol(int aLeft, int aRight)
{
    return _rotl(aLeft, aRight);
}

template<>
inline long WasmMath::Rol(long aLeft, long aRight)
{
    return _rotl64(aLeft, (int)aRight);
}

template<>
inline int WasmMath::Ror(int aLeft, int aRight)
{
    return _rotr(aLeft, aRight);
}

template<>
inline long WasmMath::Ror(long aLeft, long aRight)
{
    return _rotr64(aLeft, (int)aRight);
}

template<typename To, typename From>
To WasmMath::SignExtend(To value)
{
    return static_cast<To>(static_cast<From>(value));
}

template <bool Saturate>
int32 WasmMath::F32ToI32(float src, _In_ Js::ScriptContext* scriptContext)
{
    return WasmMath::ConvertFloatToInt<
        float, // SrcType
        int32, // DstType
        uint32_t, // ReinterpretType
        Js::NumberConstants::k_Float32TwoTo31,
        Js::NumberConstants::k_Float32NegZero,
        Js::NumberConstants::k_Float32NegTwoTo31,
        &WasmMath::LessThan<uint32_t>,
        &WasmMath::LessOrEqual<uint32_t>,
        Saturate,
        INT32_MIN,
        INT32_MAX>(
            src,
            scriptContext);
}

template <bool Saturate>
uint32_t WasmMath::F32ToU32(float src, _In_ Js::ScriptContext* scriptContext)
{
    return WasmMath::ConvertFloatToInt<
        float, // SrcType
        uint32_t, // DstType
        uint32_t, // ReinterpretType
        Js::NumberConstants::k_Float32TwoTo32,
        Js::NumberConstants::k_Float32NegZero,
        Js::NumberConstants::k_Float32NegOne,
        &WasmMath::LessThan<uint32_t>,
        &WasmMath::LessThan<uint32_t>,
        Saturate,
        0,
        UINT32_MAX>(
            src,
            scriptContext);
}

template <bool Saturate>
int32 WasmMath::F64ToI32(double src, _In_ Js::ScriptContext* scriptContext)
{
    return WasmMath::ConvertFloatToInt<
        double, // SrcType
        int32, // DstType
        unsigned long, // ReinterpretType
        Js::NumberConstants::k_TwoTo31,
        Js::NumberConstants::k_NegZero,
        Js::NumberConstants::k_NegTwoTo31,
        &WasmMath::LessOrEqual<unsigned long>,
        &WasmMath::LessOrEqual<unsigned long>,
        Saturate,
        INT32_MIN,
        INT32_MAX>(
            src,
            scriptContext);
}

template <bool Saturate>
uint32_t WasmMath::F64ToU32(double src, _In_ Js::ScriptContext* scriptContext)
{
    return WasmMath::ConvertFloatToInt<
        double, // SrcType
        uint32_t, // DstType
        unsigned long, // ReinterpretType
        Js::NumberConstants::k_TwoTo32,
        Js::NumberConstants::k_NegZero,
        Js::NumberConstants::k_NegOne,
        &WasmMath::LessOrEqual<unsigned long>,
        &WasmMath::LessThan<unsigned long>,
        Saturate,
        0,
        UINT32_MAX>(
            src,
            scriptContext);
}

template <bool Saturate>
long WasmMath::F32ToI64(float src, _In_ Js::ScriptContext* scriptContext)
{
    return WasmMath::ConvertFloatToInt<
        float, // SrcType
        long, // DstType
        uint32_t, // ReinterpretType
        Js::NumberConstants::k_Float32TwoTo63,
        Js::NumberConstants::k_Float32NegZero,
        Js::NumberConstants::k_Float32NegTwoTo63,
        &WasmMath::LessThan<uint32_t>,
        &WasmMath::LessOrEqual<uint32_t>,
        Saturate,
        INT64_MIN,
        INT64_MAX>(
            src,
            scriptContext);
}

template <bool Saturate>
unsigned long WasmMath::F32ToU64(float src, _In_ Js::ScriptContext* scriptContext)
{
    return WasmMath::ConvertFloatToInt<
        float, // SrcType
        unsigned long, // DstType
        uint32_t, // ReinterpretType
        Js::NumberConstants::k_Float32TwoTo64,
        Js::NumberConstants::k_Float32NegZero,
        Js::NumberConstants::k_Float32NegOne,
        &WasmMath::LessThan<uint32_t>,
        &WasmMath::LessThan<uint32_t>,
        Saturate,
        0,
        UINT64_MAX>(
            src,
            scriptContext);
}

template <bool Saturate>
long WasmMath::F64ToI64(double src, _In_ Js::ScriptContext* scriptContext)
{
    return WasmMath::ConvertFloatToInt<
        double, // SrcType
        long, // DstType
        unsigned long, // ReinterpretType
        Js::NumberConstants::k_TwoTo63,
        Js::NumberConstants::k_NegZero,
        Js::NumberConstants::k_NegTwoTo63,
        &WasmMath::LessThan<unsigned long>,
        &WasmMath::LessOrEqual<unsigned long>,
        Saturate,
        INT64_MIN,
        INT64_MAX>(
            src,
            scriptContext);
}

template <bool Saturate>
unsigned long WasmMath::F64ToU64(double src, _In_ Js::ScriptContext* scriptContext)
{
    return WasmMath::ConvertFloatToInt<
        double, // SrcType
        unsigned long, // DstType
        unsigned long, // ReinterpretType
        Js::NumberConstants::k_TwoTo64,
        Js::NumberConstants::k_NegZero,
        Js::NumberConstants::k_NegOne,
        &WasmMath::LessThan<unsigned long>,
        &WasmMath::LessThan<unsigned long>,
        Saturate,
        0,
        UINT64_MAX>(
            src,
            scriptContext);
}

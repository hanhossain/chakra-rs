//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "RuntimeLanguagePch.h"

namespace Js
{
    Var TaggedInt::Negate(Var aRight,ScriptContext* scriptContext)
    {
        int32 nValue = ToInt32(aRight);
        return JavascriptNumber::ToVar(-nValue,scriptContext);
    }

    Var TaggedInt::Not(Var aRight,ScriptContext* scriptContext)
    {
        int32 nValue = ToInt32(aRight);

        return JavascriptNumber::ToVar(~nValue,scriptContext);
    }

    // Explicitly marking noinline and stdcall since this is called from inline asm
    _NOINLINE Var TaggedInt::OverflowHelper(int overflowValue, ScriptContext* scriptContext)
    {
        Assert( IsOverflow(overflowValue) );
        return JavascriptNumber::NewInlined(static_cast<double>(overflowValue), scriptContext);
    }

    // noinline since it's a rare edge case and we don't want to bloat mainline code
    _NOINLINE Var TaggedInt::DivideByZero(int nLeft, ScriptContext* scriptContext)
    {
        if (nLeft == 0)
        {
            return scriptContext->GetLibrary()->GetNaN();
        }

        if (nLeft < 0)
        {
            return scriptContext->GetLibrary()->GetNegativeInfinite();
        }

        return scriptContext->GetLibrary()->GetPositiveInfinite();
    }

    Var TaggedInt::Divide(Var aLeft,Var aRight,ScriptContext* scriptContext)
    {
        int nLeft   = ToInt32(aLeft);
        int nRight  = ToInt32(aRight);

        if (nRight == 0)
        {
            return DivideByZero(nLeft, scriptContext);
        }

        //
        // If the operands produce an integer, keep the result as an integer to improve performance:
        // - This also bypasses conversion to / from doubles, which is expensive and potentially
        //   lossy.
        //

#if INT32VAR
        // 0x80000000 / -1 (or %) will trigger an integer overflow exception
        if (nLeft != INT_MIN || nRight != -1)
        {
#endif
            if ((nLeft % nRight) == 0)
            {
                //
                // Check that result is not -0. !(Dividend is 0 and Divisor is negative)
                //

                if ((nLeft != 0) || (nRight > 0))
                {
                    return JavascriptNumber::ToVar(nLeft/nRight, scriptContext);
                }
            }
#if INT32VAR
        }
#endif
        //
        // Fallback to creating a floating-point number to preserve the fractional portion.
        //

        double dblResult = (double) nLeft / (double) nRight;

        return JavascriptNumber::ToVarNoCheck(dblResult, scriptContext);
    }

    Var TaggedInt::Modulus(Var aLeft, Var aRight, ScriptContext* scriptContext)
    {
        int nLeft    = ToInt32(aLeft);
        int nRight   = ToInt32(aRight);

        // nLeft is positive and nRight is +2^i
        // Fast path for Power of 2 divisor
        if (nLeft > 0 && ::Math::IsPow2(nRight))
        {
            return ToVarUnchecked(nLeft & (nRight - 1));
        }

        if (nRight == 0)
        {
            return scriptContext->GetLibrary()->GetNaN();
        }

        if (nLeft == 0)
        {
          return ToVarUnchecked(0);
        }
        int result;
#if INT32VAR
        // 0x80000000 / -1 (or %) will trigger an integer overflow exception
        if (nLeft != INT_MIN || nRight != -1)
        {
#endif
            result = nLeft % nRight;

#if INT32VAR
        }
        else
        {
            int64 left64 = nLeft;
            int64 right64 = nRight;
            result = (int)(left64 % right64);
        }
#endif
        if (result != 0)
        {
            return ToVarUnchecked(result);
        }
        else
        {
            //
            // return -0 if left is negative
            //

            if (nLeft >= 0)
            {
                return ToVarUnchecked(0);
            }
            else
            {
                return scriptContext->GetLibrary()->GetNegativeZero();
            }
        }
    }
    Var TaggedInt::DivideInPlace(Var aLeft,Var aRight,ScriptContext* scriptContext, JavascriptNumber *result)
    {
        int nLeft   = ToInt32(aLeft);
        int nRight  = ToInt32(aRight);

        if (nRight == 0)
        {
            return DivideByZero(nLeft, scriptContext);
        }

        //
        // If the operands produce an integer, keep the result as an integer to improve performance:
        // - This also bypasses conversion to / from doubles, which is expensive and potentially
        //   lossy.
        //

        if ((nLeft % nRight) == 0)
        {
            //
            // Check that result is not -0. !(Dividend is 0 and Divisor is negative)
            //

            if ((nLeft != 0) || (nRight > 0))
            {
                return JavascriptNumber::ToVar(nLeft/nRight, scriptContext);
            }
        }


        //
        // Fallback to creating a floating-point number to preserve the fractional portion.
        //

        double dblResult = (double) nLeft / (double) nRight;

        return JavascriptNumber::InPlaceNew(dblResult, scriptContext, result);
    }

    Var TaggedInt::Multiply(Var aLeft, Var aRight, ScriptContext* scriptContext)
    {
        //
        // Perform the signed integer multiplication.
        //

        int nLeft       = ToInt32(aLeft);
        int nRight      = ToInt32(aRight);
        int nResult;
        __int64 int64Result = (__int64)nLeft * (__int64)nRight;
        nResult = (int)int64Result;

        if (((int64Result >> 32) == 0 && (nResult > 0 || (nResult == 0 && nLeft+nRight >= 0)))
            || ((int64Result >> 32) == -1 && nResult < 0))
        {
            return JavascriptNumber::ToVar(nResult,scriptContext);
        }
        else if (int64Result == 0)
        {
            return JavascriptNumber::ToVarNoCheck((double)nLeft * (double)nRight, scriptContext);
        }
        else
        {
            return JavascriptNumber::ToVarNoCheck((double)int64Result, scriptContext);
        }
    }

    Var TaggedInt::MultiplyInPlace(Var aLeft, Var aRight, ScriptContext* scriptContext, JavascriptNumber *result)
    {
        //
        // Perform the signed integer multiplication.
        //

        int nLeft       = ToInt32(aLeft);
        int nRight      = ToInt32(aRight);
        int nResult;
        __int64 int64Result = (__int64)nLeft * (__int64)nRight;
        nResult = (int)int64Result;

        if (((int64Result >> 32) == 0 && nResult > 0)
            || ((int64Result >> 32) == -1 && nResult < 0))
        {
            if (!TaggedInt::IsOverflow(nResult))
            {
                return TaggedInt::ToVarUnchecked(nResult);
            }
            else
            {
                return JavascriptNumber::InPlaceNew((double)nResult, scriptContext, result);
            }
        }
        else if (int64Result == 0)
        {
            return JavascriptNumber::InPlaceNew((double)nLeft * (double)nRight, scriptContext, result);
        }
        else
        {
            return JavascriptNumber::InPlaceNew((double)int64Result, scriptContext, result);
        }
    }


    Var TaggedInt::Subtract(Var aLeft,Var aRight,ScriptContext* scriptContext)
#ifdef DBG
    {
        Var difference = DbgSubtract(aLeft, aRight, scriptContext);
        AssertMsg(JavascriptConversion::ToNumber(difference,scriptContext) == ToDouble(aLeft) - ToDouble(aRight), "TaggedInt fast subtraction is broken");
        return difference;
    }

    Var TaggedInt::DbgSubtract(Var aLeft,Var aRight,ScriptContext* scriptContext)
#endif
    {
#if _M_IX86

        //
        // Perform the signed, integer subtraction directly on Atoms without converting to integers:
        //
        // T        = AtomTag_Int32
        // nResult  = A1 - A2
        //  Step 1: (N1 * S + T) - (N2 * S + T)
        //  Step 2: ((N1 - N2) * S + T) - T
        //  Step 3: A3 - T
        //
        // NOTE: As demonstrated above, the FromVar() / ToVar() calls in (T) will cancel out,
        // enabling an optimized operation.
        //

        __asm
        {
            mov     eax, aLeft
            sub     eax, aRight
            jno     LblDone         // Check for overflow/underflow
                                    // The carry flag indicates whether the sum has
                                    // overflowed (>INT_MAX) or underflowed (< INT_MIN)
            push    scriptContext
            cmc                     // For subtraction, CF=1 indicates an overflow, so reverse the flag
            rcr     eax, 1          // Convert to int32 and set the sign to the carry bit
            push    eax
            call    TaggedInt::OverflowHelper
            dec     eax             // Adjust for the upcoming inc eax
LblDone:
            inc     eax
            // Difference is in eax
        }

#elif defined(_M_X64) || defined(_M_ARM32_OR_ARM64)

        //
        // Perform the signed, integer subtraction directly on Atoms using 64-bit math for overflow
        // checking.
        //

        int64 nResult64 = ToInt64(aLeft) - ToInt64(aRight);
        if (IsOverflow(nResult64))
        {
            //
            // Promote result to double.
            //

            return JavascriptNumber::ToVarNoCheck((double) nResult64, scriptContext);
        }
        else
        {
            //
            // Return A3
            //

            int nResult32 = (int) nResult64;
            return ToVarUnchecked(nResult32);
        }

#else

#error Unsupported processor type

#endif
    }

    // Without checking, bitwise 'and' the two values together. If the result passes the "Is" test
    // then both arguments were valid Int31s and the result is correct.
    Var TaggedInt::Speculative_And(Var aLeft, Var aRight)
    {
        return (Var) (((size_t) aLeft) & ((size_t) aRight));
    }

    Var TaggedInt::And(Var aLeft, Var aRight)
    {
        //
        // Perform the integer "bitwise and":
        //
        // A1       = N1'       | T
        // A2       = N2'       | T
        // A1 & A2  = N1' & N2' | T & T
        // Result   = N1' & N2' | T
        //

        Var aResult = Speculative_And(aLeft, aRight);
        AssertMsg(TaggedInt::Is(aResult), "Ensure result is properly marked");

        return aResult;
    }

    Var TaggedInt::Or(Var aLeft, Var aRight)
    {
        //
        // Perform the integer "bitwise or":
        //
        // A1       = N1'       | T
        // A2       = N2'       | T
        // A1 | A2  = N1' | N2' | T | T
        // Result   = N1' | N2' | T
        //

        Var aResult = (Var) (((size_t) aLeft) | ((size_t) aRight));
        AssertMsg(TaggedInt::Is(aResult), "Ensure result is properly marked");

        return aResult;
    }

#if INT32VAR
    Var TaggedInt::Xor(Var aLeft, Var aRight)
    {
        int32 nResult = ToInt32(aLeft) ^ ToInt32(aRight);
        return TaggedInt::ToVarUnchecked(nResult);
    }
#else
    Var TaggedInt::Xor(Var aLeft, Var aRight)
    {
        //
        // Perform the integer "bitwise xor":
        //
        // A1           = N1'       | T
        // A2           = N2'       | T
        // A1 ^ A2      = N1' ^ N2' | T ^ T
        // Result - T   = N1' ^ N2' | 0
        //

        size_t nResult = ((size_t) aLeft) ^ ((size_t) aRight);
        AssertMsg((nResult & AtomTag) == 0, "Ensure tag-bits cancelled out");

        return (Var) (nResult | AtomTag_IntPtr);
    }
#endif

    Var TaggedInt::ShiftLeft(Var aLeft,Var aRight,ScriptContext* scriptContext)
    {
        //
        // Shifting an integer left will always remain an integer, but it may overflow the Int31
        // range. Therefore, we must call JavascriptNumber::ToVar() to check.
        //

        int nValue      = ToInt32(aLeft);
        uint32 nShift   = ToUInt32(aRight);

        return JavascriptNumber::ToVar(nValue << (nShift & 0x1F),scriptContext);
    }

    Var TaggedInt::ShiftRight(Var aLeft, Var aRight)
    {
        //
        // If aLeft was an Int31 coming in, then the result must always be an Int31 going out because
        // shifting right only makes value smaller. Therefore, we may call ToVarUnchecked()
        // directly.
        //

        int nValue      = ToInt32(aLeft);
        uint32 nShift   = ToUInt32(aRight);

        return ToVarUnchecked(nValue >> (nShift & 0x1F));
    }

    Var TaggedInt::ShiftRightU(Var aLeft, Var aRight, ScriptContext* scriptContext)
    {
        //
        // If aLeft was an Int31 coming in, then the result must always be an Int31 going out because
        // shifting right only makes value smaller. Therefore, we may call ToVarUnchecked()
        // directly.
        //

        uint32 uValue   = ToUInt32(aLeft);
        uint32 nShift   = ToUInt32(aRight);

        return JavascriptNumber::ToVar(uValue >> (nShift & 0x1F), scriptContext);
    }

    int TaggedInt::ToBuffer(Var aValue, __out_ecount_z(bufSize) char16 * buffer, uint bufSize)
    {
        return ToBuffer(ToInt32(aValue), buffer, bufSize);
    }

    // fills the buffer from the end and returns the start index
    int TaggedInt::UnsignedToString(unsigned __int64 value, char16 *buffer, int bufferSize)
    {
        static_assert(sizeof(value) <= 8, "This method may not support the target architecture");
        AssertMsg(bufferSize >= 22, "Error: bufferSize is too small. value may not be represented properly");

        buffer[bufferSize - 1] = char16(0);
        int pos = bufferSize - 2;
        while(value > 9)
        {
            const int val100 = value % 100;
            value /= 100;

            if (val100 < 10)
            {
                buffer[pos--] = _u('0') + static_cast<char>(val100);
                buffer[pos--] = _u('0');
                continue;
            }

            buffer[pos--] = _u('0') + static_cast<char>(val100 % 10);
            buffer[pos--] = _u('0') + static_cast<char>(val100 / 10);
        }

        if (value && value < 10)
        {
            buffer[pos--] = _u('0') + static_cast<char>(value);
        }
        else if (pos == bufferSize - 2) // if it was 0
        {
            buffer[pos--] = _u('0');
        }

        return pos + 1;
    }

    int TaggedInt::SignedToString(__int64 value, char16 *buffer, int bufferSize)
    {
        bool neg = value < 0;
        unsigned __int64 val = (unsigned __int64) (neg ? -1 * value : value);
        int pos = UnsignedToString(val, buffer, bufferSize);
        if (neg) buffer[--pos] = _u('-');
        return pos;
    }

    int TaggedInt::ToBuffer(int value, __out_ecount_z(bufSize) char16 * buffer, uint bufSize)
    {
        Assert(bufSize >= 22);
        return SignedToString(value, buffer, bufSize);
    }

    int TaggedInt::ToBuffer(uint value, __out_ecount_z(bufSize) char16 * buffer, uint bufSize)
    {
        Assert(bufSize >= 22);
        return UnsignedToString(value, buffer, bufSize);
    }

    JavascriptString* TaggedInt::ToString(Var aValue,ScriptContext* scriptContext)
    {
        return ToString(ToInt32(aValue), scriptContext);
    }

    JavascriptString* TaggedInt::ToString(int value, ScriptContext* scriptContext)
    {
        char16 szBuffer[22];
        int pos = ToBuffer(value, szBuffer, _countof(szBuffer));

        return JavascriptString::NewCopyBuffer(szBuffer + pos, (_countof(szBuffer) - 1) - pos, scriptContext);
    }

    JavascriptString* TaggedInt::ToString(uint value, ScriptContext* scriptContext)
    {
        char16 szBuffer[22];
        int pos = ToBuffer(value, szBuffer, _countof(szBuffer));

        return JavascriptString::NewCopyBuffer(szBuffer + pos, (_countof(szBuffer) - 1) - pos, scriptContext);
    }

    Var TaggedInt::NegateUnchecked(Var aValue)
    {
        AssertMsg( Is(aValue), "Ensure var is actually a 'TaggedInt'");
        AssertMsg( aValue != ToVarUnchecked(0), "Do not use NegateUnchecked on zero because NegativeZero is special");
        AssertMsg( aValue != ToVarUnchecked(k_nMinValue), "Do not use NegateUnchecked on min value because it cannot be represented");

#if INT32VAR
        int n = ToInt32(aValue);
        Var result = ToVarUnchecked( 0 - n );
#else
        int n = reinterpret_cast<int>(aValue);

        // Negation can be done by subtracting from "zero". The following method
        // is just two operations: "load constant; sub"
        // The constant 2 in the following expression
        // a) adjusts for the bias of ToVarUnchecked(0) and
        // b) ensures the tag bit is set
        Var result = reinterpret_cast<Var>( 2 - n );
#endif

        // Check against the long way (shift, negate, shift, or)
        AssertMsg( result == ToVarUnchecked( -ToInt32(aValue) ), "Logic error in NegateUnchecked" );
        return result;
    }

    // Explicitly marking noinline and stdcall since this is called from inline asm
    _NOINLINE Var TaggedInt::IncrementOverflowHelper(ScriptContext* scriptContext)
    {
        return JavascriptNumber::New( k_nMaxValue + 1.0, scriptContext );
    }

    Var TaggedInt::Increment(Var aValue, ScriptContext* scriptContext)
    {
#if _M_IX86


        __asm
        {
            mov     eax, aValue
            add     eax, 2
            jno     LblDone
            push    scriptContext
            call    TaggedInt::IncrementOverflowHelper
        LblDone:
            ; result is in eax
        }
#else

#if INT32VAR
        Var result = aValue;
        (*(unsigned int *)&result)++;  // unsigned to avoid signed int overflow
#else
        unsigned int n = reinterpret_cast<unsigned int>(aValue);
        n += 2;
        Var result = reinterpret_cast<Var>(n);
#endif

        // Wrap-around
        if( result == MinVal() )
        {
            // Use New instead of ToVar for this constant
            return IncrementOverflowHelper(scriptContext);
        }

        AssertMsg( result == ToVarUnchecked( ToInt32(aValue) + 1 ), "Logic error in Int31::Increment" );
        return result;
#endif
    }

    // Explicitly marking noinline and stdcall since this is called from inline asm
    _NOINLINE Var TaggedInt::DecrementUnderflowHelper(ScriptContext* scriptContext)
    {
        return JavascriptNumber::New( k_nMinValue - 1.0, scriptContext );
    }

    Var TaggedInt::Decrement(Var aValue, ScriptContext* scriptContext)
    {
#if _M_IX86

        __asm
        {
            mov     eax, aValue
            sub     eax, 2
            jno     LblDone
            push    scriptContext
            call    TaggedInt::DecrementUnderflowHelper
        LblDone:
            ; result is in eax
        }
#else

#if INT32VAR
        Var result = aValue;
        (*(unsigned int *)&result)--;  // unsigned to avoid signed int overflow
#else
        unsigned int n = reinterpret_cast<unsigned int>(aValue);
        n -= 2;
        Var result = reinterpret_cast<Var>(n);
#endif

        // Wrap-around
        if( result == MaxVal() )
        {
            // Use New instead of ToVar for this constant
            return DecrementUnderflowHelper(scriptContext);
        }

        AssertMsg( result == ToVarUnchecked( ToInt32(aValue) - 1 ), "Logic error in Int31::Decrement" );
        return result;
#endif
    }


#if defined(__clang__) && defined(_M_IX86)
    bool TaggedInt::IsOverflow(intptr_t nValue)
    {
        return (nValue < k_nMinValue) || (nValue > k_nMaxValue);
    }
#endif
    bool TaggedInt::IsOverflow(int32 nValue)
    {
        return (nValue < k_nMinValue) || (nValue > k_nMaxValue);
    }

    bool TaggedInt::IsOverflow(uint32 nValue)
    {
        return nValue > k_nMaxValue;
    }

    bool TaggedInt::IsOverflow(int64 nValue)
    {
        return (nValue < k_nMinValue) || (nValue > k_nMaxValue);
    }

    bool TaggedInt::IsOverflow(uint64 nValue)
    {
        return nValue > k_nMaxValue;
    }

#if INT32VAR
    bool TaggedInt::Is(Var aValue)
    {
        bool result = (((uintptr_t) aValue) >> VarTag_Shift) == AtomTag;
        if(result)
        {
            Assert((uintptr_t)aValue >> 32 == (AtomTag << 16));
        }
        return result;
    }

    bool TaggedInt::Is(intptr_t aValue)
    {
        bool result = (aValue >> VarTag_Shift) == AtomTag;
        if (result)
        {
            Assert((aValue >> 32) == (AtomTag << 16));
        }
        return result;
    }

    bool TaggedInt::IsPair(Var aLeft, Var aRight)
    {
        uint32 tags = (uint32)((uint64)aLeft >> 32 | (uint64)aRight >> 48);
        bool result = (tags == AtomTag_Pair);
        AssertMsg(result == (TaggedInt::Is(aLeft) && TaggedInt::Is(aRight)), "TaggedInt::IsPair() logic is incorrect");
        return result;
    }

    int32 TaggedInt::ToInt32(Var aValue)
    {
        AssertMsg(Is(aValue), "Ensure var is actually a 'TaggedInt'");

        return ::Math::PointerCastToIntegralTruncate<int32>(aValue);
    }

    int32 TaggedInt::ToInt32(intptr_t aValue)
    {
        AssertMsg(Is(aValue), "Ensure var is actually an 'TaggedInt'");

        return (int32)aValue;
    }

    uint32 TaggedInt::ToUInt32(Var aValue)
    {
        AssertMsg(Is(aValue), "Ensure var is actually a 'TaggedInt'");

        return ::Math::PointerCastToIntegralTruncate<uint32>(aValue);
    }

    int64 TaggedInt::ToInt64(Var aValue)
    {
        AssertMsg(Is(aValue), "Ensure var is actually a 'TaggedInt'");

        int64 nValue = (int64)(::Math::PointerCastToIntegralTruncate<int32>(aValue));
        AssertMsg(nValue == (int64) ToInt32(aValue),
                "Ensure 32-bit and 64-bit operations return same result");

        return nValue;
    }

    uint16 TaggedInt::ToUInt16(Var aValue)
    {
        AssertMsg(Is(aValue), "Ensure var is actually a 'TaggedInt'");

        return ::Math::PointerCastToIntegralTruncate<uint16>(aValue);
    }

    double TaggedInt::ToDouble(Var aValue)
    {
        return (double)::Math::PointerCastToIntegralTruncate<int32>(aValue);
    }

    Var TaggedInt::ToVarUnchecked(int nValue)
    {
        //
        // To convert to a var we first cast to uint32 to lose the signedness and then
        // extend it to a 64-bit uintptr_t before OR'ing the 64-bit atom tag.
        //

        AssertMsg(!IsOverflow(nValue), "Ensure no information loss from conversion");

        return reinterpret_cast<Var>(((uintptr_t)(uint32)nValue) | AtomTag_IntPtr);
    }

#else
    bool TaggedInt::Is(const Var aValue)
    {
        return (((uintptr_t) aValue) & AtomTag) == AtomTag_IntPtr;
    }

    __inline bool TaggedInt::Is(intptr_t aValue)
    {
        return (aValue & AtomTag) == AtomTag_IntPtr;
    }

    bool TaggedInt::IsPair(Var aLeft, Var aRight)
    {
        //
        // Check if both Atoms are tagged as "SmInts":
        // - Because we're checking tag bits, we don't need to check against 'null', since it won't
        //   be tagged as a TaggedInt.
        // - This degenerates into bitwise arithmetic that avoids the overhead of branching and
        //   short-circuit evaluation.
        //

        return (((uintptr_t) aLeft) & ((uintptr_t) aRight) & AtomTag) == AtomTag_IntPtr;
    }

    int32 TaggedInt::ToInt32(Var aValue)
    {
        //
        // To convert from a var, must first convert to an 'int32' to properly sign-extend
        // negative values.  Then, use shift operation to remove the tag bits.
        //

        AssertMsg(Is(aValue), "Ensure var is actually a 'TaggedInt'");

        return ((int) aValue) >> VarTag_Shift;
    }

    uint32 TaggedInt::ToUInt32(Var aValue)
    {
        //
        // To convert from a var, must use ToInt32() to properly sign-extend negative values, then
        // convert to an (unsigned) uint32.
        //

        return (uint32) ToInt32(aValue);
    }

    int64 TaggedInt::ToInt64(Var aValue)
    {
       //
        // To convert from a var, must first convert to an 'int64' to properly sign-extend
        // negative values.  Then, use shift operation to remove the tag bits.
        //

        AssertMsg(Is(aValue), "Ensure var is actually a 'TaggedInt'");

        int64 nValue = ((int32) aValue) >> VarTag_Shift;
        AssertMsg(nValue == (int64) ToInt32(aValue),
                "Ensure 32-bit and 64-bit operations return same result");

        return nValue;
    }

    uint16 TaggedInt::ToUInt16(Var aValue)
    {
        AssertMsg(Is(aValue), "Ensure var is actually a 'TaggedInt'");

        return (uint16)(((int) aValue) >> VarTag_Shift);
    }

    double TaggedInt::ToDouble(Var aValue)
    {
        return (double) ToInt32(aValue);
    }

    Var TaggedInt::ToVarUnchecked(int nValue)
    {
        //
        // To convert to a var, must first multiply (which will be converted into a shift
        // operation) to create space for the tag while properly preserving negative values.  Then,
        // add the tag.
        //

        AssertMsg(!IsOverflow(nValue), "Ensure no information loss from conversion");

        return reinterpret_cast<Var>((nValue << VarTag_Shift) | AtomTag_IntPtr);
    }
#endif

    // Returns true when the provided args only contains TaggedInts. Note that
    // iteration starts from 1 (to account for 'this' at index 0).
    bool TaggedInt::OnlyContainsTaggedInt(Js::Arguments& args)
    {
        bool onlyContainsInts = true;
        for (uint idxArg = 1; idxArg < args.Info.Count; idxArg++)
        {
            if (!TaggedInt::Is(args[idxArg]))
            {
                onlyContainsInts = false;
                break;
            }
        }

        return onlyContainsInts;
    }

    Var TaggedInt::Add(Var aLeft,Var aRight,ScriptContext* scriptContext)
#ifdef DBG
    {
        Var sum = DbgAdd(aLeft, aRight, scriptContext);
        AssertMsg(JavascriptConversion::ToNumber(sum,scriptContext) == ToDouble(aLeft) + ToDouble(aRight), "TaggedInt fast addition is broken");
        return sum;
    }

    Var TaggedInt::DbgAdd(Var aLeft,Var aRight,ScriptContext* scriptContext)
#endif
    {
#if _M_IX86
        //
        // Perform the signed, integer addition directly on Atoms without converting to integers:
        //
        // T        = AtomTag_Int32
        // nResult  = A1 + A2
        //  Step 1: (N1 * S + T) + (N2 * S + T)
        //  Step 2: ((N1 + N2) * S + T) + T
        //  Step 3: A3 + T
        //
        // NOTE: As demonstrated above, the FromVar() / ToVar() calls in (T) will cancel out,
        // enabling an optimized operation.
        //


        __asm
        {
            mov     eax, aLeft
            dec     eax             // Get rid of one of the tags, since they'll add up
            add     eax, aRight     // Perform the addition
            jno     LblDone         // Check for overflow/underflow
                                    // The carry flag indicates whether the sum has
                                    // overflowed (>INT_MAX) or underflowed (< INT_MIN)

            push    scriptContext
            rcr     eax, 1          // Convert to int32 and set the sign to the carry bit
            push    eax
            call    TaggedInt::OverflowHelper

LblDone:
            // Sum is in eax
        }

#elif defined(_M_X64) || defined(_M_ARM32_OR_ARM64)

        //
        // Perform the signed, integer addition directly on Atoms using 64-bit math for overflow
        // checking.
        //

        int64 nResult64 = ToInt64(aLeft) + ToInt64(aRight);
        if (IsOverflow(nResult64))
        {
            //
            // Promote result to double.
            //

            return JavascriptNumber::ToVarNoCheck((double) nResult64, scriptContext);
        }
        else
        {
            //
            // Return A3
            //

            int nResult32 = (int) nResult64;
            return ToVarUnchecked(nResult32);
        }

#else

#error Unsupported processor type

#endif
    }


    //
    // True if the value is a tagged number representation (for x64 - float & integers) - otherwise false.
    //
    bool TaggedNumber::Is(const Var aValue)
    {
        bool isTaggedNumber;
#if FLOATVAR
        // If we add another tagged representation that is not numerical - this will not work.
        isTaggedNumber = !VarIs<RecyclableObject>(aValue);
#else
        isTaggedNumber = TaggedInt::Is(aValue);
#endif
        return isTaggedNumber;
    }
}

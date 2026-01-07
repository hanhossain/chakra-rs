//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

class UInt32Math
{
public:
    template< class Func >
    static uint32_t Add(uint32_t lhs, uint32_t rhs, __inout Func& overflowFn)
    {
        uint32_t result = lhs + rhs;

        // If the result is smaller than the LHS, then we overflowed
        if( result < lhs )
        {
            overflowFn();
        }

        return result;
    }

    template< class Func >
    static void Inc(uint32_t& lhs, __inout Func& overflowFn)
    {
        ++lhs;

        // If lhs becomes 0, then we overflowed
        if(!lhs)
        {
            overflowFn();
        }
    }

    template< class Func >
    static uint32_t Mul(uint32_t lhs, uint32_t rhs, __inout Func& overflowFn)
    {
        // Do the multiplication using 64-bit unsigned math.
        unsigned long result = static_cast<unsigned long>(lhs) * static_cast<unsigned long>(rhs);

        // Does the result fit in 32-bits?
        if(result >= (1ui64 << 32))
        {
            overflowFn();
        }

        return static_cast<uint32_t>(result);
    }

    template<uint32_t mul, class Func >
    static uint32_t Mul(uint32_t left, __inout Func& overflowFn)
    {
        CompileAssert(mul != 0);

        if (left > (UINT_MAX / mul))
        {
            overflowFn();
        }

        // If mul is a power of 2, the compiler will convert this to a shift left.
        return left * mul;
    }

    // Using 0 for mul will result in compile-time divide by zero error (which is desired behavior)
    template< uint32_t add, uint32_t mul, class Func >
    static uint32_t AddMul(uint32_t left, __inout Func& overflowFn)
    {
        //
        // The result will overflow if (left+add)*mul > UINT_MAX
        // Rearranging, this becomes: left > (UINT_MAX / mul ) - add
        //
        // If mul and add are compile-time constants then LTCG will collapse
        // this to a simple constant comparison.
        //
        CompileAssert(UINT_MAX/mul >= add);

        if( left > ((UINT_MAX / mul) - add) )
        {
            overflowFn();
        }

        // When add and mul are small constants, the compiler is
        // typically able to use the LEA instruction here.
        return (left + add) * mul;
    }

    // Using 0 for mul will result in compile-time divide by zero error (which is desired behavior)
    template< uint32_t mul, uint32_t add, class Func >
    static uint32_t MulAdd(uint32_t left, __inout Func& overflowFn)
    {
        //
        // The result will overflow if (left*mul)+add > UINT_MAX
        // Rearranging, this becomes: left > (UINT_MAX - add) / mul
        //
        // If add and mul are compile-time constants then LTCG will collapse
        // this to a simple constant comparison.
        //
        if( left > ((UINT_MAX - add) / mul) )
        {
            overflowFn();
        }

        // When add and mul are small constants, the compiler is
        // typically able to use the LEA instruction here.
        return (left * mul) + add;
    }

    // Convenience functions which use the DefaultOverflowPolicy (throw OOM upon overflow)
    template< uint32_t add, uint32_t mul >
    static uint32_t AddMul(uint32_t left)
    {
        return AddMul<add,mul>(left, ::Math::DefaultOverflowPolicy);
    }

    _When_(lhs + rhs < lhs, _Analysis_noreturn_)
    _Post_satisfies_(return == lhs + rhs)
    static uint32_t Add(uint32_t lhs, uint32_t rhs)
    {
        uint32_t result = Add( lhs, rhs, ::Math::DefaultOverflowPolicy );
        return result;
    }

    static uint32_t Mul(uint32_t lhs, uint32_t rhs)
    {
        return Mul(lhs, rhs, ::Math::DefaultOverflowPolicy );
    }

    template<uint32_t mul>
    static uint32_t Mul(uint32_t lhs)
    {
        return Mul<mul>(lhs, ::Math::DefaultOverflowPolicy);
    }

    template< uint32_t mul, uint32_t add >
    static uint32_t MulAdd(uint32_t left)
    {
        return MulAdd<mul,add>(left, ::Math::DefaultOverflowPolicy);
    }

    // Convenience functions which return a bool indicating overflow
    static bool Add(uint32_t lhs, uint32_t rhs, uint32_t* result)
    {
        ::Math::RecordOverflowPolicy overflowGuard;
        *result = Add(lhs, rhs, overflowGuard);
        return overflowGuard.HasOverflowed();
    }

    static bool Mul(uint32_t lhs, uint32_t rhs, uint32_t* result)
    {
        ::Math::RecordOverflowPolicy overflowGuard;
        *result = Mul(lhs, rhs, overflowGuard);
        return overflowGuard.HasOverflowed();
    }

    // Convenience function which uses DefaultOverflowPolicy (throws OOM upon overflow)
    static void Inc(uint32_t& lhs)
    {
        Inc(lhs, ::Math::DefaultOverflowPolicy);
    }
};

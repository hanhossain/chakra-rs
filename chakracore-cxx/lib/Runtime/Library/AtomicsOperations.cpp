//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "RuntimeLibraryPch.h"
#include "AtomicsOperations.h"

template<typename T> struct ConvertType {};
template<> struct ConvertType<int8_t> { typedef char _t; };
template<> struct ConvertType<uint8_t> { typedef char _t; };
template<> struct ConvertType<int16> { typedef short _t; };
template<> struct ConvertType<uint16> { typedef short _t; };
template<> struct ConvertType<int32> { typedef int32_t _t; };
template<> struct ConvertType<uint32> { typedef int32_t _t; };
template<> struct ConvertType<long> { typedef long _t; };

template<typename T>
T InterlockedExchangeAdd_t(T *target, T value);

template<>
char InterlockedExchangeAdd_t(char *target, char value) { return InterlockedExchangeAdd8(target, value); }

template<>
short InterlockedExchangeAdd_t(short *target, short value) { return InterlockedExchangeAdd16(target, value); }

template<>
int32_t InterlockedExchangeAdd_t(int32_t *target, int32_t value) { return InterlockedExchangeAdd(target, value); }

template<>
long InterlockedExchangeAdd_t(long *target, long value) { return InterlockedExchangeAdd64(target, value); }

template<typename T>
T InterlockedAnd_t(T *target, T value);

template<>
char InterlockedAnd_t(char *target, char value) { return InterlockedAnd8(target, value); }

template<>
short InterlockedAnd_t(short *target, short value) { return InterlockedAnd16(target, value); }

template<>
int32_t InterlockedAnd_t(int32_t *target, int32_t value) { return InterlockedAnd(target, value); }

template<>
long InterlockedAnd_t(long *target, long value) { return InterlockedAnd64(target, value); }

template<typename T>
T InterlockedOr_t(T *target, T value);

template<>
char InterlockedOr_t(char *target, char value) { return InterlockedOr8(target, value); }

template<>
short InterlockedOr_t(short *target, short value) { return InterlockedOr16(target, value); }

template<>
int32_t InterlockedOr_t(int32_t *target, int32_t value) { return InterlockedOr(target, value); }

template<>
long InterlockedOr_t(long *target, long value) { return InterlockedOr64(target, value); }

template<typename T>
T InterlockedXor_t(T *target, T value);

template<>
char InterlockedXor_t(char *target, char value) { return InterlockedXor8(target, value); }

template<>
short InterlockedXor_t(short *target, short value) { return InterlockedXor16(target, value); }

template<>
int32_t InterlockedXor_t(int32_t *target, int32_t value) { return InterlockedXor(target, value); }

template<>
long InterlockedXor_t(long *target, long value) { return InterlockedXor64(target, value); }

template<typename T>
T InterlockedExchange_t(T *target, T value);

template<>
char InterlockedExchange_t(char *target, char value) { return InterlockedExchange8(target, value); }

template<>
short InterlockedExchange_t(short *target, short value) { return InterlockedExchange16(target, value); }

template<>
int32_t InterlockedExchange_t(int32_t *target, int32_t value) { return InterlockedExchange(target, value); }

template<>
long InterlockedExchange_t(long *target, long value) { return InterlockedExchange64(target, value); }

template<typename T>
T InterlockedCompareExchange_t(T *target, T v1, T v2);

template<>
char InterlockedCompareExchange_t(char *target, char v1, char v2) {
    return InterlockedCompareExchange8(target, v1, v2);
}

template<>
short InterlockedCompareExchange_t(short *target, short v1, short v2) {
    return InterlockedCompareExchange16(target, v1, v2);
}

template<>
int32_t InterlockedCompareExchange_t(int32_t *target, int32_t v1, int32_t v2) {
    return InterlockedCompareExchange(target, v1, v2);
}

template<>
long InterlockedCompareExchange_t(long *target, long v1, long v2) {
    return InterlockedCompareExchange64(target, v1, v2);
}

namespace Js
{
template<typename T> T AtomicsOperations::Load(T* buffer)
{
    // MemoryBarrier only works when the memory size is not greater than the register size
    CompileAssert(sizeof(T) <= sizeof(size_t));
    MemoryBarrier();
    T result = (T)*buffer;
    return result;
}

#if TARGET_32
template<> long AtomicsOperations::Load(long* buffer)
{
    CompileAssert(sizeof(size_t) == 4);
    // Implement 64bits atomic load on 32bits platform with a CompareExchange
    // It is slower, but at least it is garantied to be an atomic operation
    return CompareExchange<long>(buffer, 0, 0);
}
#endif

template<typename T> T AtomicsOperations::Store(T* buffer, T value)
{
    typedef typename ConvertType<T>::_t convertType;
    InterlockedExchange_t<convertType>((convertType*)buffer, (convertType)value);
    return value;
}

template<typename T> T AtomicsOperations::Add(T* buffer, T value)
{
    typedef typename ConvertType<T>::_t convertType;
    T result = (T)InterlockedExchangeAdd_t<convertType>((convertType*)buffer, (convertType)value);
    return result;
}

template<typename T> T AtomicsOperations::And(T* buffer, T value)
{
    typedef typename ConvertType<T>::_t convertType;
    T result = (T)InterlockedAnd_t<convertType>((convertType*)buffer, (convertType)value);
    return result;
}

template<typename T> T AtomicsOperations::CompareExchange(T* buffer, T comparand, T replacementValue)
{
    typedef typename ConvertType<T>::_t convertType;
    T result = (T)InterlockedCompareExchange_t<convertType>((convertType*)buffer, (convertType)replacementValue, (convertType)comparand);
    return result;
}

template<typename T> T AtomicsOperations::Exchange(T* buffer, T value)
{
    typedef typename ConvertType<T>::_t convertType;
    T result = (T)InterlockedExchange_t<convertType>((convertType*)buffer, (convertType)value);
    return result;
}

template<typename T> T AtomicsOperations::Or(T* buffer, T value)
{
    typedef typename ConvertType<T>::_t convertType;
    T result = (T)InterlockedOr_t<convertType>((convertType*)buffer, (convertType)value);
    return result;
}

template<typename T> T AtomicsOperations::Sub(T* buffer, T value)
{
    typedef typename ConvertType<T>::_t convertType;
    T result = (T)InterlockedExchangeAdd_t<convertType>((convertType*)buffer, -(convertType)value);
    return result;
}

template<typename T> T AtomicsOperations::Xor(T* buffer, T value)
{
    typedef typename ConvertType<T>::_t convertType;
    T result = (T)InterlockedXor_t<convertType>((convertType*)buffer, (convertType)value);
    return result;
}

#define ExplicitImplementation(type) \
    template type AtomicsOperations::Load(type*); \
    template type AtomicsOperations::Store(type*, type); \
    template type AtomicsOperations::Add(type*, type); \
    template type AtomicsOperations::And(type*, type); \
    template type AtomicsOperations::CompareExchange(type*, type, type); \
    template type AtomicsOperations::Exchange(type*, type); \
    template type AtomicsOperations::Or(type*, type); \
    template type AtomicsOperations::Sub(type*, type); \
    template type AtomicsOperations::Xor(type*, type); \

ExplicitImplementation(int8_t);
ExplicitImplementation(uint8_t);
ExplicitImplementation(int16);
ExplicitImplementation(uint16);
ExplicitImplementation(int32);
ExplicitImplementation(uint32);
ExplicitImplementation(long);

};

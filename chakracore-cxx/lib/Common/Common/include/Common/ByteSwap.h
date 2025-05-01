//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

#if defined(__APPLE__)
#include <libkern/OSByteOrder.h>

#define RtlUshortByteSwap(_x)    OSSwapInt16((_x))
#define RtlUlongByteSwap(_x)     OSSwapInt32((_x))
#define RtlUlonglongByteSwap(_x) OSSwapInt64((_x))

#elif defined(__ANDROID__)
#define RtlUshortByteSwap(_x)    __builtin_bswap16((_x))
#define RtlUlongByteSwap(_x)     __builtin_bswap32((_x))
#define RtlUlonglongByteSwap(_x) __builtin_bswap64((_x))

#elif defined(__linux__)
#include <byteswap.h>

#define RtlUshortByteSwap(_x)    __bswap_16((_x))
#define RtlUlongByteSwap(_x)     __bswap_32((_x))
#define RtlUlonglongByteSwap(_x) __bswap_64((_x))

#else
// TODO: include endian.h for BSD?
#error "ByteSwap.h: Not implemented for this platform"
#endif

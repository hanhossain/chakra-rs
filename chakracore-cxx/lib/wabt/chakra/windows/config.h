/*
* Copyright 2016 WebAssembly Community Group participants
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#ifndef WABT_CONFIG_H_
#define WABT_CONFIG_H_

/* TODO(binji): nice way to define these with WABT_ prefix? */

/* Whether <alloca.h> is available */
#define HAVE_ALLOCA_H 0

/* Whether <unistd.h> is available */
#define HAVE_UNISTD_H 0

/* Whether snprintf is defined by stdio.h */
#define HAVE_SNPRINTF 1

/* Whether sysconf is defined by unistd.h */
#define HAVE_SYSCONF 0

/* Whether ssize_t is defined by stddef.h */
#define HAVE_SSIZE_T 0

/* Whether strcasecmp is defined by strings.h */
#define HAVE_STRCASECMP 0

/* Whether ENABLE_VIRTUAL_TERMINAL_PROCESSING is defined by windows.h */
#define HAVE_WIN32_VT100 0

#define COMPILER_IS_CLANG 0
#define COMPILER_IS_MSVC 1

#define WITH_EXCEPTIONS 0

#define SIZEOF_SIZE_T 4

#if HAVE_ALLOCA_H
#include <alloca.h>
#elif COMPILER_IS_MSVC
#include <malloc.h>
#define alloca _alloca
#elif defined(__MINGW32__)
#include <malloc.h>
#elif defined(__FreeBSD__)
#include <stdlib.h>
#else
#error no alloca
#endif

#if COMPILER_IS_CLANG

#define WABT_UNUSED __attribute__ ((unused))
#define WABT_WARN_UNUSED __attribute__ ((warn_unused_result))
#define WABT_INLINE inline
#define WABT_UNLIKELY(x) __builtin_expect(!!(x), 0)
#define WABT_LIKELY(x) __builtin_expect(!!(x), 1)

#if __MINGW32__
// mingw defaults to printf format specifier being ms_printf (which doesn't
// understand 'llu', etc.) We always want gnu_printf, and force mingw to always
// use mingw_printf, mingw_vprintf, etc.
#define WABT_PRINTF_FORMAT(format_arg, first_arg) \
  __attribute__((format(gnu_printf, (format_arg), (first_arg))))
#else
#define WABT_PRINTF_FORMAT(format_arg, first_arg) \
  __attribute__((format(printf, (format_arg), (first_arg))))
#endif

#ifdef __cplusplus
#if __cplusplus >= 201103L
#define WABT_STATIC_ASSERT(x) static_assert((x), #x)
#else
#define WABT_STATIC_ASSERT__(x, c) \
  static int static_assert_##c[(x ? 0 : -1)] WABT_UNUSED
#define WABT_STATIC_ASSERT_(x, c) WABT_STATIC_ASSERT__(x, c)
#define WABT_STATIC_ASSERT(x) WABT_STATIC_ASSERT_(x, __COUNTER__)
#endif
#else
#define WABT_STATIC_ASSERT(x) _Static_assert((x), #x)
#endif

#define WABT_UNREACHABLE __builtin_unreachable()

#elif COMPILER_IS_MSVC

#include <string.h>
#include <intrin.h>

#define WABT_UNUSED
#define WABT_WARN_UNUSED
#define WABT_INLINE __inline
#define WABT_STATIC_ASSERT(x) _STATIC_ASSERT(x)
#define WABT_UNLIKELY(x) (x)
#define WABT_LIKELY(x) (x)
#define WABT_PRINTF_FORMAT(format_arg, first_arg)

#define WABT_UNREACHABLE __assume(0)

#else

#error unknown compiler

#endif

#ifdef __cplusplus
namespace wabt
{

#if COMPILER_IS_CLANG

inline int Clz(unsigned x) { return x ? __builtin_clz(x) : sizeof(x) * 8; }
inline int Clz(unsigned long x) { return x ? __builtin_clzl(x) : sizeof(x) * 8; }
inline int Clz(unsigned long long x) { return x ? __builtin_clzll(x) : sizeof(x) * 8; }

inline int Ctz(unsigned x) { return x ? __builtin_ctz(x) : sizeof(x) * 8; }
inline int Ctz(unsigned long x) { return x ? __builtin_ctzl(x) : sizeof(x) * 8; }
inline int Ctz(unsigned long long x) { return x ? __builtin_ctzll(x) : sizeof(x) * 8; }

inline int Popcount(unsigned x) { return __builtin_popcount(x); }
inline int Popcount(unsigned long x) { return __builtin_popcountl(x); }
inline int Popcount(unsigned long long x) { return __builtin_popcountll(x); }

#elif COMPILER_IS_MSVC

#if _M_IX86
inline unsigned long LowDword(unsigned __int64 value)
{
    return (unsigned long)value;
}

inline unsigned long HighDword(unsigned __int64 value)
{
    unsigned long high;
    memcpy(&high, (unsigned char*)&value + sizeof(high), sizeof(high));
    return high;
}
#endif

inline int Clz(unsigned long mask)
{
    if (mask == 0)
        return 32;

    unsigned long index;
    _BitScanReverse(&index, mask);
    return sizeof(unsigned long) * 8 - (index + 1);
}

inline int Clz(unsigned int mask)
{
    return Clz((unsigned long)mask);
}

inline int Clz(unsigned __int64 mask)
{
#if _M_X64
    if (mask == 0)
        return 64;

    unsigned long index;
    _BitScanReverse64(&index, mask);
    return sizeof(unsigned __int64) * 8 - (index + 1);
#elif _M_IX86
    int result = Clz(HighDword(mask));
    if (result == 32)
        result += Clz(LowDword(mask));

    return result;
#else
#error unexpected architecture
#endif
}

inline int Ctz(unsigned long mask)
{
    if (mask == 0)
        return 32;

    unsigned long index;
    _BitScanForward(&index, mask);
    return index;
}

inline int Ctz(unsigned int mask)
{
    return Ctz((unsigned long)mask);
}

inline int Ctz(unsigned __int64 mask)
{
#if _M_X64
    if (mask == 0)
        return 64;

    unsigned long index;
    _BitScanForward64(&index, mask);
    return index;
#elif _M_IX86
    int result = Ctz(LowDword(mask));
    if (result == 32)
        result += Ctz(HighDword(mask));

    return result;
#else
#error unexpected architecture
#endif
}

inline int Popcount(unsigned long value)
{
    return __popcnt(value);
}

inline int Popcount(unsigned int value)
{
    return Popcount((unsigned long)value);
}

inline int Popcount(unsigned __int64 value)
{
#if _M_X64
    return __popcnt64(value);
#elif _M_IX86
    return Popcount(HighDword(value)) + Popcount(LowDword(value));
#else
#error unexpected architecture
#endif
}

#else

#error unknown compiler

#endif

} // namespace wabt


#if COMPILER_IS_MSVC

  /* print format specifier for size_t */
#if SIZEOF_SIZE_T == 4
#define PRIzd "d"
#define PRIzx "x"
#elif SIZEOF_SIZE_T == 8
#define PRIzd "I64d"
#define PRIzx "I64x"
#else
#error "weird sizeof size_t"
#endif

#elif COMPILER_IS_CLANG

  /* print format specifier for size_t */
#define PRIzd "zd"
#define PRIzx "zx"

#else

#error unknown compiler

#endif


#if HAVE_SNPRINTF
#define wabt_snprintf snprintf
#elif COMPILER_IS_MSVC
  /* can't just use _snprintf because it doesn't always null terminate */
#include <cstdarg>
int wabt_snprintf(char* str, size_t size, const char* format, ...);
#else
#error no snprintf
#endif

#if COMPILER_IS_MSVC
  /* can't just use vsnprintf because it doesn't always null terminate */
int wabt_vsnprintf(char* str, size_t size, const char* format, va_list ap);
#else
#define wabt_vsnprintf vsnprintf
#endif

#if !HAVE_SSIZE_T
typedef long ssize_t;
#endif

#if !HAVE_STRCASECMP
#if COMPILER_IS_MSVC
#define strcasecmp _stricmp
#else
#error no strcasecmp
#endif
#endif

#if COMPILER_IS_MSVC && defined(_M_X64)
// MSVC on x64 generates uint64 -> float conversions but doesn't do
// round-to-nearest-ties-to-even, which is required by WebAssembly.
#include <emmintrin.h>
__inline double wabt_convert_uint64_to_double(unsigned __int64 x)
{
    __m128d result = _mm_setzero_pd();
    if (x & 0x8000000000000000ULL)
    {
        result = _mm_cvtsi64_sd(result, (x >> 1) | (x & 1));
        result = _mm_add_sd(result, result);
    }
    else
    {
        result = _mm_cvtsi64_sd(result, x);
    }
    return _mm_cvtsd_f64(result);
}

__inline float wabt_convert_uint64_to_float(unsigned __int64 x)
{
    __m128 result = _mm_setzero_ps();
    if (x & 0x8000000000000000ULL)
    {
        result = _mm_cvtsi64_ss(result, (x >> 1) | (x & 1));
        result = _mm_add_ss(result, result);
    }
    else
    {
        result = _mm_cvtsi64_ss(result, x);
    }
    return _mm_cvtss_f32(result);
}

#else
#define wabt_convert_uint64_to_double(x) static_cast<double>(x)
#define wabt_convert_uint64_to_float(x) static_cast<float>(x)
#endif

#endif  // __cplusplus

#endif /* WABT_CONFIG_H_ */

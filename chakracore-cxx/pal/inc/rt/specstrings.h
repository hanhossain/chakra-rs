//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information. 
//


//
#ifndef SPECSTRINGS_H
#define SPECSTRINGS_H

/***
*specstrings.h - support for markers for documenting the semantics of APIs
*

*
*       [Public]
****/

#if !defined(_SAL_VERSION_SAL2)

 #if defined(__BUILDMACHINE__) || defined(_USE_SAL2_ONLY)
  #define _SAL_VERSION_SAL2(_A) SAL_2_Clean_Violation_using ## _A
 #else
  #define _SAL_VERSION_SAL2(_A)
 #endif

 #ifdef _USE_SAL2_ONLY
  #define _SAL2_STRICT
  #define _SAL_VERSION_CHECK(_A) _SAL_VERSION_SAL2(_A)
 #else
  #define _SAL_VERSION_CHECK(_A)
 #endif

 #ifndef SAL_VERSION_CHECK
  #define SAL_VERSION_CHECK(_A) _SAL_VERSION_CHECK(_A)
  #define SAL_VERSION_SAL2(_A) _SAL_VERSION_SAL2(_A)
 #endif

#endif

#include "sal.h"

#ifndef __SAL_H_FULL_VER
#define __SAL_H_FULL_VER 140050727
#endif

#ifdef  __cplusplus
extern "C" {
#endif

/* version specific fixes to bring sal.h upto date */
#if __SAL_H_FULL_VER <= 140050727

#define __inner_range(lb,ub)

#define __xcount(size)                                          __notnull __inexpressible_writableTo(size)
#define __in_xcount(size)                                       _Pre_ __inexpressible_readableTo(size)
#define __out_xcount(size)                                      __xcount(size) _Post_ __valid __refparam
#define __inout_xcount(size)                                    __out_xcount(size) _Pre_ __valid

#endif  //__SAL_H_FULL_VER <= 140050727

/************************************************************************
 New extensions to sal.h follow here.
*************************************************************************/

/* integer related macros */
#define __range(lb,ub)                      __inner_range(lb,ub)
#define __in_range(lb,ub)                   _Pre_ __inner_range(lb,ub)

#ifdef  __cplusplus
}
#endif

/*
 If no SAL 2 appears to have been defined (_Outptr_ is a representative choice)
 then we must be operating in a downlevel build environment (such as VS10).
 We also test against the compiler version to identify a downlevel environment,
 as VS11 is the minimum required for SAL 2 support.

 If we are operating in a downlevel build environment (such as VS10)
 we need to undefine the following symbols before including driverspecs.h
 or we will end up referencing SAL 2 implementation symbols and cause
 build failures.
*/
#undef __PRIMOP
#define __PRIMOP(type, fun)

// ROTOR doesn't need driverspecs.h
// #include <driverspecs.h>

/*
 If no SAL 2 appears to have been defined (_Outptr_ is a representative choice)
 then we must be operating in a downlevel build environment (such as VS10).
 We also test against the compiler version to identify a downlevel environment,
 as VS11 is the minimum required for SAL 2 support.

 If we are in a downlevel environment, we can go ahead and include no_sal2.h
 to make all of SAL 2 no-ops to ensure no build failures. 
*/
#include "no_sal2.h"

#endif /* #ifndef SPECSTRINGS_H */



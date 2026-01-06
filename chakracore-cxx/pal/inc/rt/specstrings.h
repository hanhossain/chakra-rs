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

#define __inner_bound
#define __inner_range(lb,ub)
#define __inner_assume_bound(i)
#define __inner_allocator

#define __static_context(ctx, annotes)
#define __failure(x)
#define __valueUndefined
#define __failureDefault(x)

#define __xcount(size)                                          __notnull __inexpressible_writableTo(size)
#define __in_xcount(size)                                       _Pre_ __inexpressible_readableTo(size)
#define __out_xcount(size)                                      __xcount(size) _Post_ __valid __refparam
#define __out_xcount_part(size,length)                          __out_xcount(size) _Post_ __inexpressible_readableTo(length)
#define __out_xcount_full(size)                                 __out_xcount_part(size,size)
#define __inout_xcount(size)                                    __out_xcount(size) _Pre_ __valid
#define __inout_xcount_part(size,length)                        __out_xcount_part(size,length) _Pre_ __valid _Pre_ __inexpressible_readableTo(length)
#define __inout_xcount_full(size)                               __inout_xcount_part(size,size)
#define __xcount_opt(size)                                      __xcount(size)                              __exceptthat __maybenull
#define __in_xcount_opt(size)                                   __in_xcount(size)                           __exceptthat __maybenull
#define __out_xcount_opt(size)                                  __out_xcount(size)                          __exceptthat __maybenull
#define __out_xcount_part_opt(size,length)                      __out_xcount_part(size,length)              __exceptthat __maybenull
#define __out_xcount_full_opt(size)                             __out_xcount_full(size)                     __exceptthat __maybenull
#define __inout_xcount_opt(size)                                __inout_xcount(size)                        __exceptthat __maybenull
#define __inout_xcount_part_opt(size,length)                    __inout_xcount_part(size,length)            __exceptthat __maybenull
#define __inout_xcount_full_opt(size)                           __inout_xcount_full(size)                   __exceptthat __maybenull
#define __deref_xcount(size)                                    __ecount(1) _Post_ __elem_readableTo(1) _Post_ __deref __notnull _Post_ __deref __inexpressible_writableTo(size)
#define __deref_in                                              _Pre_ __deref __deref __readonly
#define __deref_in_ecount(size)                                 __deref_in _Pre_ __deref __elem_readableTo(size)
#define __deref_in_bcount(size)                                 __deref_in _Pre_ __deref __byte_readableTo(size)
#define __deref_in_xcount(size)                                 __deref_in _Pre_ __deref __inexpressible_readableTo(size)
#define __deref_out_xcount(size)                                __deref_xcount(size) _Post_ __deref __valid __refparam
#define __deref_out_xcount_part(size,length)                    __deref_out_xcount(size) _Post_ __deref __inexpressible_readableTo(length)
#define __deref_out_xcount_full(size)                           __deref_out_xcount_part(size,size)
#define __deref_out_xcount(size)                                __deref_xcount(size) _Post_ __deref __valid __refparam
#define __inout_xcount_opt(size)                                __inout_xcount(size)                        __exceptthat __maybenull
#define __inout_xcount_part_opt(size,length)                    __inout_xcount_part(size,length)            __exceptthat __maybenull
#define __inout_xcount_full_opt(size)                           __inout_xcount_full(size)                   __exceptthat __maybenull
#define __deref_xcount(size)                                    __ecount(1) _Post_ __elem_readableTo(1) _Post_ __deref __notnull _Post_ __deref __inexpressible_writableTo(size)
#define __deref_in                                              _Pre_ __deref __deref __readonly
#define __deref_in_ecount(size)                                 __deref_in _Pre_ __deref __elem_readableTo(size)
#define __deref_in_bcount(size)                                 __deref_in _Pre_ __deref __byte_readableTo(size)
#define __deref_in_xcount(size)                                 __deref_in _Pre_ __deref __inexpressible_readableTo(size)
#define __deref_out_xcount(size)                                __deref_xcount(size) _Post_ __deref __valid __refparam
#define __deref_out_xcount_part(size,length)                    __deref_out_xcount(size) _Post_ __deref __inexpressible_readableTo(length)
#define __deref_out_xcount_full(size)                           __deref_out_xcount_part(size,size)
#define __deref_out_xcount(size)                                __deref_xcount(size) _Post_ __deref __valid __refparam
#define __deref_inout_xcount(size)                              __deref_inout _Pre_ __deref __inexpressible_writableTo(size) _Post_ __deref __inexpressible_writableTo(size)
#define __deref_inout_xcount_part(size,length)                  __deref_inout_xcount(size) _Pre_ __deref __inexpressible_readableTo(length) _Post_ __deref __inexpressible_readableTo(length)
#define __deref_inout_xcount_full(size)                         __deref_inout_xcount_part(size,size)
#define __deref_xcount_opt(size)                                __deref_xcount(size)                        _Post_ __deref __exceptthat __maybenull
#define __deref_in_opt                                          __deref_in                                  _Pre_ __deref __exceptthat __maybenull
#define __deref_in_opt_out                                      __deref_inout                               _Pre_ __deref __exceptthat __maybenull  _Post_ __deref __notnull
#define __deref_in_ecount_opt(size)                             __deref_in_ecount(size)                     _Pre_ __deref __exceptthat __maybenull
#define __deref_in_bcount_opt(size)                             __deref_in_bcount(size)                     _Pre_ __deref __exceptthat __maybenull
#define __deref_in_xcount_opt(size)                             __deref_in_xcount(size)                     _Pre_ __deref __exceptthat __maybenull
#define __deref_out_xcount_opt(size)                            __deref_out_xcount(size)                    _Post_ __deref __exceptthat __maybenull
#define __deref_out_xcount_part_opt(size,length)                __deref_out_xcount_part(size,length)        _Post_ __deref __exceptthat __maybenull
#define __deref_out_xcount_full_opt(size)                       __deref_out_xcount_full(size)               _Post_ __deref __exceptthat __maybenull
#define __deref_inout_xcount_opt(size)                          __deref_inout_xcount(size)                  _Pre_ __deref __exceptthat __maybenull _Post_ __deref __exceptthat __maybenull
#define __deref_inout_xcount_part_opt(size,length)              __deref_inout_xcount_part(size,length)      _Pre_ __deref __exceptthat __maybenull _Post_ __deref __exceptthat __maybenull
#define __deref_inout_xcount_full_opt(size)                     __deref_inout_xcount_full(size)             _Pre_ __deref __exceptthat __maybenull _Post_ __deref __exceptthat __maybenull
#define __deref_opt_xcount(size)                                __deref_xcount(size)                        __exceptthat __maybenull
#define __deref_opt_in                                          __deref_in                                  __exceptthat __maybenull
#define __deref_opt_in_ecount(size)                             __deref_in_ecount(size)                     __exceptthat __maybenull
#define __deref_opt_in_bcount(size)                             __deref_in_bcount(size)                     __exceptthat __maybenull
#define __deref_opt_in_xcount(size)                             __deref_in_xcount(size)                     __exceptthat __maybenull
#define __deref_opt_out_xcount(size)                            __deref_out_xcount(size)                    __exceptthat __maybenull
#define __deref_opt_out_xcount_part(size,length)                __deref_out_xcount_part(size,length)        __exceptthat __maybenull
#define __deref_opt_out_xcount_full(size)                       __deref_out_xcount_full(size)               __exceptthat __maybenull
#define __deref_opt_inout_xcount(size)                          __deref_inout_xcount(size)                  __exceptthat __maybenull
#define __deref_opt_inout_xcount_part(size,length)              __deref_inout_xcount_part(size,length)      __exceptthat __maybenull
#define __deref_opt_inout_xcount_full(size)                     __deref_inout_xcount_full(size)             __exceptthat __maybenull
#define __deref_opt_xcount_opt(size)                            __deref_xcount_opt(size)                    __exceptthat __maybenull
#define __deref_opt_in_opt                                      __deref_in_opt                              __exceptthat __maybenull
#define __deref_opt_in_ecount_opt(size)                         __deref_in_ecount_opt(size)                 __exceptthat __maybenull
#define __deref_opt_in_bcount_opt(size)                         __deref_in_bcount_opt(size)                 __exceptthat __maybenull
#define __deref_opt_in_xcount_opt(size)                         __deref_in_xcount_opt(size)                 __exceptthat __maybenull
#define __deref_opt_out_xcount_opt(size)                        __deref_out_xcount_opt(size)                __exceptthat __maybenull
#define __deref_opt_out_xcount_part_opt(size,length)            __deref_out_xcount_part_opt(size,length)    __exceptthat __maybenull
#define __deref_opt_out_xcount_full_opt(size)                   __deref_out_xcount_full_opt(size)           __exceptthat __maybenull
#define __deref_opt_inout_xcount_opt(size)                      __deref_inout_xcount_opt(size)              __exceptthat __maybenull
#define __deref_opt_inout_xcount_part_opt(size,length)          __deref_inout_xcount_part_opt(size,length)  __exceptthat __maybenull
#define __deref_opt_inout_xcount_full_opt(size)                 __deref_inout_xcount_full_opt(size)         __exceptthat __maybenull

#define __deref_in_ecount_iterator(size, incr)                  __inout _Pre_ __deref __elem_readableTo(size) __deref_out_range(==, _Old_(*_Curr_) + incr)
#define __deref_out_ecount_iterator(size, incr)                 __inout _Pre_ __deref __elem_writableTo(size) __deref_out_range(==, _Old_(*_Curr_) + incr)
#define __deref_inout_ecount_iterator(size, incr)               __inout _Pre_ __deref __elem_readableTo(size) _Pre_ __deref __elem_writableTo(size) __deref_out_range(==, _Old_(*_Curr_) + incr)

#define __post_bcount(size)                                     _Post_ __byte_writableTo(size)
#define __post_ecount(size)                                     _Post_ __elem_writableTo(size)

#define __deref_realloc_bcount(insize, outsize)                 __inout _Pre_ __deref __byte_readableTo(insize) _Post_ __deref __byte_writableTo(outsize)

/* __in_ecount_or_z(c) specifies semantics like strncmp, where a string
 * parameter is either null terminated, or valid up to c elements.
 */
#define __in_ecount_or_z(c)            _When_(_String_length_(_Curr_) < (c), __in_z) \
                                       _When_(_String_length_(_Curr_) >= (c), __in_ecount(c))


/* Provide default definition to be overridden when needed */
#define __post_nullnullterminated

#undef __nullnullterminated
#define __nullnullterminated __inexpressible_readableTo("string terminated by two nulls") __nullterminated

#undef __post_nullnullterminated
#define __post_nullnullterminated _Post_ __inexpressible_readableTo("string terminated by two nulls") _Post_ __nullterminated

#endif  //__SAL_H_FULL_VER <= 140050727

/************************************************************************
 New extensions to sal.h follow here.
*************************************************************************/

/* Internal defintions */
#define __inner_assume_validated(p)
#define __inner_transfer(formal)
#define __inner_encoded
#define __inner_adt_prop(adt,prop)
#define __inner_adt_add_prop(adt,prop)
#define __inner_adt_remove_prop(adt,prop)
#define __inner_adt_transfer_prop(arg)
#define __inner_adt_type_props(typ)
#define __inner_volatile
#define __inner_nonvolatile
#define __inner_possibly_notnullterminated
#define __inner_analysis_assume_nullterminated(x)

#define __field_ecount(size)                __notnull __elem_writableTo(size)
#define __field_bcount(size)                __notnull __byte_writableTo(size)

/* integer related macros */
#define __range(lb,ub)                      __inner_range(lb,ub)
#define __in_range(lb,ub)                   _Pre_ __inner_range(lb,ub)
#define __deref_in_range(lb,ub)             _Pre_ __deref __inner_range(lb,ub)
#define __deref_out_range(lb,ub)            _Post_ __deref __inner_range(lb,ub)

/**************************************************************************
* SAL 2 extensions for Windows-specific APIs.
***************************************************************************/

//
// Gets the current error code (as returned by GetLastError()), and stores
// in _Curr_ as a postcondition. This is currently approximated by assuming
// that GetLastError() always returns a failed error code. This is not a
// completely accurate approximation, but reasonable.
//
#define _Post_equals_last_error_     _Post_satisfies_(_Curr_ != 0)
                                
#ifdef  __cplusplus
}
#endif

#ifdef _PREFIX_
/**************************************************************************
* Defintion of __pfx_assume and __pfx_assert. Thse should be the only
* defintions of these functions.
***************************************************************************/
#if __cplusplus
extern "C" void __pfx_assert(bool, const char *);
extern "C" void __pfx_assume(bool, const char *);
#else
void __pfx_assert(int, const char *);
void __pfx_assume(int, const char *);
#endif
#endif /* ifdef _PREFIX_ */


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
#undef __ANNOTATION
#define __ANNOTATION(fun) /* fun */
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



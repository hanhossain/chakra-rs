//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Copyright (c) 2021 ChakraCore Project Contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

/*****************************************************************************************************
 * This file contains defines that switch feature on or off, or configuration a feature at build time
 *****************************************************************************************************/

#include "Interface/Warnings.h"
#include "Interface/ChakraCoreVersion.h"

//----------------------------------------------------------------------------------------------------
// Default debug/fretest/release flags values
//  - Set the default values of debug/fretest/release flags if it is not set by the command line
//----------------------------------------------------------------------------------------------------
#ifndef DBG_DUMP
#define DBG_DUMP 0
#endif

#ifdef _DEBUG
#ifdef DEBUG
#undef DEBUG
#endif
#define DEBUG 1
#endif

#if DBG || DBG_DUMP
    // Flag to control availability of other flags to control regex debugging, tracing, profiling, etc.
    #ifndef ENABLE_REGEX_CONFIG_OPTIONS
        #define ENABLE_REGEX_CONFIG_OPTIONS     1
    #endif
#endif

//----------------------------------------------------------------------------------------------------
//  Define Architectures' aliases for Simplicity
//----------------------------------------------------------------------------------------------------
#if defined(_M_ARM) || defined(_M_ARM64)
#define _M_ARM32_OR_ARM64 1
#endif

//----------------------------------------------------------------------------------------------------
// Enabled features
//----------------------------------------------------------------------------------------------------

// NOTE: Disabling these might not work and are not fully supported and maintained
// Even if it builds, it may not work properly. Disable at your own risk

#if !DISABLE_JIT
#define ENABLE_FIXED_FIELDS 1                       // Turn on fixed fields if JIT is enabled
#endif

#if ENABLE_FIXED_FIELDS
#define SUPPORT_FIXED_FIELDS_ON_PATH_TYPES
#endif


// JIT features

#if DISABLE_JIT
#define ENABLE_NATIVE_CODEGEN 0
#define ENABLE_PROFILE_INFO 0
#define ENABLE_BACKGROUND_JOB_PROCESSOR 0
#define ENABLE_BACKGROUND_PARSING 0                 // Disable background parsing in this mode
                                                    // We need to decouple the Jobs infrastructure out of
                                                    // Backend to make background parsing work with JIT disabled
#define DYNAMIC_INTERPRETER_THUNK 0
#define DISABLE_DYNAMIC_PROFILE_DEFER_PARSE
#define ENABLE_COPYONACCESS_ARRAY 0
#else
// By default, enable the JIT
#define ENABLE_NATIVE_CODEGEN 1
#define ENABLE_PROFILE_INFO 1

#define ENABLE_BACKGROUND_JOB_PROCESSOR 1
#define ENABLE_COPYONACCESS_ARRAY 1
#ifndef DYNAMIC_INTERPRETER_THUNK
#define DYNAMIC_INTERPRETER_THUNK 1
#endif

// Only enable background parser in debug build.
#ifdef DBG
#define ENABLE_BACKGROUND_PARSING 1
#endif
#endif

#ifndef PERFMAP_SIGNAL
#define PERFMAP_SIGNAL SIGUSR2
#endif

//----------------------------------------------------------------------------------------------------
// Debug and fretest features
//----------------------------------------------------------------------------------------------------

#if ENABLE_PROFILE_INFO
#define DYNAMIC_PROFILE_STORAGE
#endif
#define SECURITY_TESTING

#define PROFILE_EXEC
#define BGJIT_STATS
#define REJIT_STATS
#define PERF_HINT
#define POLY_INLINE_CACHE_SIZE_STATS

#define JS_PROFILE_DATA_INTERFACE 1
#define ARENA_ALLOCATOR_FREE_LIST_SIZE

// VTUNE profiling requires ETW trace
#if defined(_M_X64)
#define VTUNE_PROFILING
#endif

//----------------------------------------------------------------------------------------------------
// Debug only features
//----------------------------------------------------------------------------------------------------
#ifdef DEBUG
#define BYTECODE_TESTING

// currently depends on io.h
#define RECYCLER_NO_PAGE_REUSE
#endif

#ifdef DBG
#define VALIDATE_ARRAY
#define ENABLE_ENTRYPOINT_CLEANUP_TRACE 1

// xplat-todo: Do we need dump generation for non-Win32 platforms?
#endif

#if DBG_DUMP
#define BGJIT_STATS
#define REJIT_STATS
#define POLY_INLINE_CACHE_SIZE_STATS
#define INLINE_CACHE_STATS
#define FIELD_ACCESS_STATS
#define MISSING_PROPERTY_STATS
#define EXCEPTION_CHECK                     // Check exception handling.
#define MEMSPECT_TRACKING
// Needs to compile in debug mode
// Just needs strings converted

#define RECYCLER_SLOW_CHECK_ENABLED          // This can be disabled to speed up the debug build's GC
#define RECYCLER_STRESS
#define RECYCLER_STATS
#define RECYCLER_FINALIZE_CHECK
#define RECYCLER_FREE_MEM_FILL
#define RECYCLER_DUMP_OBJECT_GRAPH
#define RECYCLER_MEMORY_VERIFY
#define RECYCLER_ZERO_MEM_CHECK
#define RECYCLER_TRACE
#define RECYCLER_VERIFY_MARK

#define PAGEALLOCATOR_PROTECT_FREEPAGE
#define ARENA_MEMORY_VERIFY
#define SEPARATE_ARENA

#ifdef _X64_OR_ARM64
#define MEMORY_ALLOCATION_ALIGNMENT 16
#else
#define MEMORY_ALLOCATION_ALIGNMENT 8
#endif

#define ERROR_TRACE
#define DEBUGGER_TRACE

#define PROPERTY_RECORD_TRACE

#define ARENA_ALLOCATOR_FREE_LIST_SIZE

#endif // DBG_DUMP

//----------------------------------------------------------------------------------------------------
// Special build features
//  - features that can be enabled on private builds for debugging
//----------------------------------------------------------------------------------------------------
// #define OLD_ITRACKER                 // Switch to the old IE8 ITracker GUID
// #define LOG_BYTECODE_AST_RATIO       // log the ratio between AST size and bytecode generated.
// #define DUMP_FRAGMENTATION_STATS        // Display HeapBucket fragmentation stats after sweep

// ----- Fretest or free build special build features (already enabled in debug builds) -----

// #define BGJIT_STATS

// Profile defines that can be enabled in release build
// #define PROFILE_EXEC
// #define MEMSPECT_TRACKING

// Recycler defines that can be enabled in release build
// #define RECYCLER_STRESS
// #define RECYCLER_STATS
// #define RECYCLER_FINALIZE_CHECK
// #define RECYCLER_FREE_MEM_FILL
// #define RECYCLER_DUMP_OBJECT_GRAPH
// #define RECYCLER_MEMORY_VERIFY
// #define RECYCLER_TRACE
// #define RECYCLER_VERIFY_MARK

// Other defines that can be enabled in release build
// #define PAGEALLOCATOR_PROTECT_FREEPAGE
// #define ARENA_MEMORY_VERIFY
// #define SEPARATE_ARENA

#if defined(DUMP_FRAGMENTATION_STATS)
#define ENABLE_MEM_STATS 1
#define POLY_INLINE_CACHE_SIZE_STATS
#endif

//----------------------------------------------------------------------------------------------------
// Disabled features
//----------------------------------------------------------------------------------------------------
//Enable/disable dom properties
#define DOMEnabled 0

//----------------------------------------------------------------------------------------------------
// Platform dependent flags
//----------------------------------------------------------------------------------------------------
#ifndef INT32VAR
#define INT32VAR 1
#endif

#define LOWER_SPLIT_INT64 0

#if defined(_M_X64) && !defined(DISABLE_JIT)
#define ASMJS_PLAT
#endif

#if defined(ASMJS_PLAT)
#define ENABLE_WASM
#define ENABLE_WASM_THREADS
#define ENABLE_WASM_SIMD

#ifdef CAN_BUILD_WABT
#define ENABLE_WABT
#endif

#endif

#ifndef PDATA_ENABLED
#if defined(_M_ARM32_OR_ARM64) || defined(_M_X64)
#define PDATA_ENABLED 1
#define ALLOC_XDATA (true)
#else
#define PDATA_ENABLED 0
#define ALLOC_XDATA (false)
#endif
#endif

//----------------------------------------------------------------------------------------------------
// Dependent flags
//  - flags values that are dependent on other flags
//----------------------------------------------------------------------------------------------------

#define ENABLE_PREJIT

// Enable Output::Trace
#define ENABLE_TRACE

#if !(defined(__clang__) && defined(_M_ARM32_OR_ARM64)) // xplat-todo: ARM
#define STACK_BACK_TRACE
#endif

// ENABLE_DEBUG_STACK_BACK_TRACE is for capturing stack back trace for debug only.
// (STACK_BACK_TRACE is enabled on release build, used by RECYCLER_PAGE_HEAP.)
#if defined(STACK_BACK_TRACE)
#define ENABLE_DEBUG_STACK_BACK_TRACE 1
#endif

#if defined(USED_IN_STATIC_LIB)
#undef RECYCLER_DUMP_OBJECT_GRAPH
#undef RECYCLER_STATS
#endif

//----------------------------------------------------------------------------------------------------
// Default flags values
//  - Set the default values of flags if it is not set by the command line or above
//----------------------------------------------------------------------------------------------------
#ifndef JS_PROFILE_DATA_INTERFACE
#define JS_PROFILE_DATA_INTERFACE 0
#endif

#define JS_REENTRANCY_FAILFAST 1
#if DBG || JS_REENTRANCY_FAILFAST
#define ENABLE_JS_REENTRANCY_CHECK 1
#else
#define ENABLE_JS_REENTRANCY_CHECK 0
#endif

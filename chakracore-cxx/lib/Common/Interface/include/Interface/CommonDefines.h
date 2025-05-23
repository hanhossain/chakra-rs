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

// if test hook is enabled, debug config options are enabled too
#ifdef ENABLE_TEST_HOOKS
#ifndef ENABLE_DEBUG_CONFIG_OPTIONS
#define ENABLE_DEBUG_CONFIG_OPTIONS 1
#endif
#endif

// ENABLE_DEBUG_CONFIG_OPTIONS is enabled in debug build when DBG or DBG_DUMP is defined
// It is enabled in fretest build (jscript9test.dll and jc.exe) in the build script
#if DBG || DBG_DUMP
    #ifndef ENABLE_DEBUG_CONFIG_OPTIONS
        #define ENABLE_DEBUG_CONFIG_OPTIONS     1
    #endif

    // Flag to control availability of other flags to control regex debugging, tracing, profiling, etc. This is separate from
    // ENABLE_DEBUG_CONFIG_OPTIONS because enabling this flag may affect performance significantly, even with default values for
    // the regex flags this flag would make available.
    #ifndef ENABLE_REGEX_CONFIG_OPTIONS
        #define ENABLE_REGEX_CONFIG_OPTIONS     1
    #endif
#endif

// TODO: consider removing before RTM: keep for CHK/FRETEST but remove from FRE.
// This will cause terminate process on AV/Assert rather that letting PDM (F12/debugger scenarios) eat exceptions.
// At least for now, enable this even in FRE builds. See ReportError.h.
#define ENABLE_DEBUG_API_WRAPPER 1

//----------------------------------------------------------------------------------------------------
//  Define Architectures' aliases for Simplicity
//----------------------------------------------------------------------------------------------------
#if defined(_M_ARM) || defined(_M_ARM64)
#define _M_ARM32_OR_ARM64 1
#endif

#if defined(_M_IX86) || defined(_M_ARM)
#define TARGET_32 1
#endif

#if defined(_M_X64) || defined(_M_ARM64)
#define TARGET_64 1
#endif

#ifndef DECLSPEC_CHPE_GUEST
// For CHPE build aka Arm64.x86
// https://osgwiki.com/wiki/ARM64_CHPE
// On ChakraCore alone we do not support this
// so we define to nothing to avoid build breaks
#define DECLSPEC_CHPE_GUEST
#endif

// Memory Protections
#define PAGE_EXECUTE_RO_TARGETS_INVALID   (PAGE_EXECUTE_READ)

//----------------------------------------------------------------------------------------------------
// Enabled features
//----------------------------------------------------------------------------------------------------

// NOTE: Disabling these might not work and are not fully supported and maintained
// Even if it builds, it may not work properly. Disable at your own risk

// Config options
#define CONFIG_PARSE_CONFIG_FILE 1

// ByteCode
#define VARIABLE_INT_ENCODING 1                     // Byte code serialization variable size int field encoding
#define BYTECODE_BRANCH_ISLAND                      // Byte code short branch and branch island
#if defined(HAS_REAL_ICU)
#define ENABLE_UNICODE_API 1                        // Enable use of Unicode-related APIs
#endif

// Language features
#if !defined(CHAKRACORE_LITE) && defined(INTL_ICU)
#define ENABLE_INTL_OBJECT                          // Intl support
#endif

#define ENABLE_JS_BUILTINS                          // Built In functions support

#define ENABLE_ES6_CHAR_CLASSIFIER                  // ES6 Unicode character classifier support

// Type system features
#define PERSISTENT_INLINE_CACHES                    // *** TODO: Won't build if disabled currently

#if !DISABLE_JIT
#define ENABLE_FIXED_FIELDS 1                       // Turn on fixed fields if JIT is enabled
#endif

#if ENABLE_FIXED_FIELDS
#define SUPPORT_FIXED_FIELDS_ON_PATH_TYPES
#endif


// dep: IDebugDocumentContext
#if !BUILD_WITHOUT_SCRIPT_DEBUG
#define ENABLE_SCRIPT_DEBUGGING
#endif

// GC features
#define BUCKETIZE_MEDIUM_ALLOCATIONS 1              // *** TODO: Won't build if disabled currently
#define SMALLBLOCK_MEDIUM_ALLOC 1                   // *** TODO: Won't build if disabled currently
#define LARGEHEAPBLOCK_ENCODING 1                   // Large heap block metadata encoding
#ifndef CHAKRACORE_LITE
#define IDLE_DECOMMIT_ENABLED 1                     // Idle Decommit
#endif

#if defined(ENABLE_DEBUG_CONFIG_OPTIONS)
#define RECYCLER_PAGE_HEAP                          // PageHeap support
#endif

#define USE_FEWER_PAGES_PER_BLOCK 1

#define ENABLE_CONCURRENT_GC 1
#define ENABLE_ALLOCATIONS_DURING_CONCURRENT_SWEEP 0 // Needs ENABLE_CONCURRENT_GC to be enabled for this to be enabled.

#define SYSINFO_IMAGE_BASE_AVAILABLE 0

#ifdef CHAKRACORE_LITE
#define USE_VPM_TABLE 0
#else
#define USE_VPM_TABLE 1
#endif


// templatized code
#define USE_STATIC_VPM 0


#if ENABLE_CONCURRENT_GC
// Write-barrier refers to a software write barrier implementation using a card table.
// Write watch refers to a hardware backed write-watch feature supported by the Windows memory manager.
// Both are used for detecting changes to memory for concurrent and partial GC.
// RECYCLER_WRITE_BARRIER controls the former, RECYCLER_WRITE_WATCH controls the latter.
// GLOBAL_ENABLE_WRITE_BARRIER controls the smart pointer wrapper at compile time, every Field annotation on the
// recycler allocated class will take effect if GLOBAL_ENABLE_WRITE_BARRIER is 1, otherwise only the class declared
// with FieldWithBarrier annotations use the WriteBarrierPtr<>, see WriteBarrierMacros.h and RecyclerPointers.h for detail
#define RECYCLER_WRITE_BARRIER                      // Write Barrier support

#ifdef RECYCLER_WRITE_BARRIER
#if !GLOBAL_ENABLE_WRITE_BARRIER
#define GLOBAL_ENABLE_WRITE_BARRIER 1
#endif
#endif

#define ENABLE_PARTIAL_GC 1
#define ENABLE_BACKGROUND_PAGE_ZEROING 1
#define ENABLE_BACKGROUND_PAGE_FREEING 1
#define ENABLE_RECYCLER_TYPE_TRACKING 1
#else
#define ENABLE_PARTIAL_GC 0
#define ENABLE_BACKGROUND_PAGE_ZEROING 0
#define ENABLE_BACKGROUND_PAGE_FREEING 0
#define ENABLE_RECYCLER_TYPE_TRACKING 0
#endif

#if ENABLE_BACKGROUND_PAGE_ZEROING && !ENABLE_BACKGROUND_PAGE_FREEING
#error "Background page zeroing can't be turned on if freeing pages in the background is disabled"
#endif

#define ENABLE_WEAK_REFERENCE_REGIONS 1

// JIT features

#define ENABLE_SPECTRE_RUNTIME_MITIGATIONS

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
#if defined(TARGET_32) || defined(TARGET_64)
#define DYNAMIC_INTERPRETER_THUNK 1
#else
#define DYNAMIC_INTERPRETER_THUNK 0
#endif
#endif

// Only enable background parser in debug build.
#ifdef DBG
#define ENABLE_BACKGROUND_PARSING 1
#endif

#if ENABLE_DEBUG_CONFIG_OPTIONS
#define ALLOW_JIT_REPRO
#endif

#endif

#if ENABLE_NATIVE_CODEGEN

// ToDo (SaAgarwa): Disable VirtualTypedArray on ARM64 till we make sure it works correctly
#endif

#define VECTORCALL

#define DELAYLOAD_SET_CFG_TARGET 1

#ifndef PERFMAP_SIGNAL
#define PERFMAP_SIGNAL SIGUSR2
#endif

#define DELAYLOAD_SECTIONAPI 1
#define DELAYLOAD_UNLOCKMEMORY 1

// Telemetry features (non-DEBUG related)
#define TELEMETRY_OPCODE_FILTER(propertyId) false

#if ENABLE_DEBUG_CONFIG_OPTIONS
#define ENABLE_DIRECTCALL_TELEMETRY_STATS
#endif

//----------------------------------------------------------------------------------------------------
// Debug and fretest features
//----------------------------------------------------------------------------------------------------
#ifdef ENABLE_DEBUG_CONFIG_OPTIONS

#define BAILOUT_INJECTION
#if ENABLE_PROFILE_INFO
#define DYNAMIC_PROFILE_STORAGE
#define DYNAMIC_PROFILE_MUTATOR
#endif
#define RUNTIME_DATA_COLLECTION
#define SECURITY_TESTING

#define PROFILE_EXEC
#define BGJIT_STATS
#define REJIT_STATS
#define PERF_HINT
#define POLY_INLINE_CACHE_SIZE_STATS

#define JS_PROFILE_DATA_INTERFACE 1
#define EXCEPTION_RECOVERY 1
#define RECYCLER_TEST_SUPPORT
#define ARENA_ALLOCATOR_FREE_LIST_SIZE

// TODO (t-doilij) combine IR_VIEWER and ENABLE_IR_VIEWER
#if 0
#if ENABLE_NATIVE_CODEGEN
#define IR_VIEWER
#define ENABLE_IR_VIEWER
#define ENABLE_IR_VIEWER_DBG_DUMP  // TODO (t-doilij) disable this before check-in
#endif
#endif

#ifdef ENABLE_JS_ETW
#define TEST_ETW_EVENTS
#endif

// VTUNE profiling requires ETW trace
#if defined(_M_IX86) || defined(_M_X64)
#define VTUNE_PROFILING
#endif

#ifndef ENABLE_TEST_HOOKS
#define ENABLE_TEST_HOOKS
#endif
#endif // ENABLE_DEBUG_CONFIG_OPTIONS

////////
//Time Travel flags
//Include TTD code in the build when building for Chakra (except NT/Edge) or for debug/test builds
#if defined(ENABLE_SCRIPT_DEBUGGING)
#define ENABLE_TTD 1
#else
#define ENABLE_TTD 0
#endif

#if ENABLE_TTD
#define TTDAssert(C, M) { if(!(C)) TTDAbort_unrecoverable_error(M); }
#else
#define TTDAssert(C, M)
#endif

#if ENABLE_TTD
//A workaround for profile based creation of Native Arrays -- we may or may not want to allow since it differs in record/replay and (currently) asserts in our snap compare
#define TTD_NATIVE_PROFILE_ARRAY_WORK_AROUND 1

//See also -- Disabled fast path on property enumeration, random number generation, disabled new/eval code cache, and others.
//            Disabled ActivationObjectEx and others.

//Force debug or notjit mode
#define TTD_FORCE_DEBUG_MODE 0
#define TTD_FORCE_NOJIT_MODE 0

//Enable various sanity checking features and asserts
#if ENABLE_DEBUG_CONFIG_OPTIONS
#define ENABLE_TTD_INTERNAL_DIAGNOSTICS 1
#else
#define ENABLE_TTD_INTERNAL_DIAGNOSTICS 0
#endif

#define TTD_LOG_READER TextFormatReader
#define TTD_LOG_WRITER TextFormatWriter

//For now always use the (lower performance) text format for snapshots for easier debugging etc.
#define TTD_SNAP_READER TextFormatReader
#define TTD_SNAP_WRITER TextFormatWriter

//#if ENABLE_TTD_INTERNAL_DIAGNOSTICS
//#define TTD_SNAP_READER TextFormatReader
//#define TTD_SNAP_WRITER TextFormatWriter
//#else
//#define TTD_SNAP_READER BinaryFormatReader
//#define TTD_SNAP_WRITER BinaryFormatWriter
//#endif

#if ENABLE_TTD_INTERNAL_DIAGNOSTICS
#define ENABLE_SNAPSHOT_COMPARE 1
#define ENABLE_OBJECT_SOURCE_TRACKING 0
#define ENABLE_VALUE_TRACE 0
#define ENABLE_BASIC_TRACE 0
#define ENABLE_FULL_BC_TRACE 0
#define ENABLE_CROSSSITE_TRACE 0
#else
#define ENABLE_SNAPSHOT_COMPARE 0
#define ENABLE_OBJECT_SOURCE_TRACKING 0
#define ENABLE_BASIC_TRACE 0
#define ENABLE_FULL_BC_TRACE 0
#define ENABLE_CROSSSITE_TRACE 0
#endif

#define ENABLE_TTD_DIAGNOSTICS_TRACING (ENABLE_OBJECT_SOURCE_TRACKING || ENABLE_BASIC_TRACE || ENABLE_FULL_BC_TRACE)

//End Time Travel flags
////////
#endif

//----------------------------------------------------------------------------------------------------
// Debug only features
//----------------------------------------------------------------------------------------------------
#ifdef DEBUG
#define BYTECODE_TESTING

// xplat-todo: revive FaultInjection on non-Win32 platforms
// currently depends on io.h
#define RECYCLER_NO_PAGE_REUSE
#endif

#ifdef DBG
#define VALIDATE_ARRAY
#define ENABLE_ENTRYPOINT_CLEANUP_TRACE 1

// xplat-todo: Do we need dump generation for non-Win32 platforms?
#endif

#if DBG_DUMP
#undef DBG_EXTRAFIELD   // make sure we don't extra fields in free build.

#define TRACK_DISPATCH
#define BGJIT_STATS
#define REJIT_STATS
#define POLY_INLINE_CACHE_SIZE_STATS
#define INLINE_CACHE_STATS
#define FIELD_ACCESS_STATS
#define MISSING_PROPERTY_STATS
#define EXCEPTION_RECOVERY 1
#define EXCEPTION_CHECK                     // Check exception handling.
#if !(defined(__clang__) && defined(_M_IX86))
// todo: implement this for clang x86
#define PROFILE_MEM
#endif
#define PROFILE_TYPES
#define PROFILE_EVALMAP
#define PROFILE_OBJECT_LITERALS
#define PROFILE_BAILOUT_RECORD_MEMORY
#define MEMSPECT_TRACKING

#define PROFILE_RECYCLER_ALLOC
// Needs to compile in debug mode
// Just needs strings converted
#define PROFILE_DICTIONARY 1

#define PROFILE_STRINGS

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

#ifdef PERF_COUNTERS
#define RECYCLER_PERF_COUNTERS
#define HEAP_PERF_COUNTERS
#endif // PERF_COUNTERS

#define PAGEALLOCATOR_PROTECT_FREEPAGE
#define ARENA_MEMORY_VERIFY
#define SEPARATE_ARENA

#ifdef _X64_OR_ARM64
#define MAX_NATURAL_ALIGNMENT sizeof(ULONGLONG)
#define MEMORY_ALLOCATION_ALIGNMENT 16
#else
#define MAX_NATURAL_ALIGNMENT sizeof(uint32_t)
#define MEMORY_ALLOCATION_ALIGNMENT 8
#endif

#define HEAP_TRACK_ALLOC
#define CHECK_MEMORY_LEAK
#define LEAK_REPORT

#define ERROR_TRACE
#define DEBUGGER_TRACE

#define PROPERTY_RECORD_TRACE

#define ARENA_ALLOCATOR_FREE_LIST_SIZE

#ifdef DBG_EXTRAFIELD
#define HEAP_ENUMERATION_VALIDATION
#endif
#endif // DBG_DUMP

//----------------------------------------------------------------------------------------------------
// Special build features
//  - features that can be enabled on private builds for debugging
//----------------------------------------------------------------------------------------------------
#ifdef ENABLE_JS_ETW
// #define ETW_MEMORY_TRACKING          // ETW events for internal allocations
#endif
// #define OLD_ITRACKER                 // Switch to the old IE8 ITracker GUID
// #define LOG_BYTECODE_AST_RATIO       // log the ratio between AST size and bytecode generated.
// #define DUMP_FRAGMENTATION_STATS        // Display HeapBucket fragmentation stats after sweep

// ----- Fretest or free build special build features (already enabled in debug builds) -----
// #define TRACK_DISPATCH

// #define BGJIT_STATS

// Profile defines that can be enabled in release build
// #define PROFILE_EXEC
// #define PROFILE_MEM
// #define PROFILE_STRINGS
// #define PROFILE_TYPES
// #define PROFILE_OBJECT_LITERALS
// #define PROFILE_RECYCLER_ALLOC
// #define MEMSPECT_TRACKING

// #define HEAP_TRACK_ALLOC

// Recycler defines that can be enabled in release build
// #define RECYCLER_STRESS
// #define RECYCLER_STATS
// #define RECYCLER_FINALIZE_CHECK
// #define RECYCLER_FREE_MEM_FILL
// #define RECYCLER_DUMP_OBJECT_GRAPH
// #define RECYCLER_MEMORY_VERIFY
// #define RECYCLER_TRACE
// #define RECYCLER_VERIFY_MARK

// #ifdef PERF_COUNTERS
// #define RECYCLER_PERF_COUNTERS
// #define HEAP_PERF_COUNTERS
// #endif //PERF_COUNTERS

// Other defines that can be enabled in release build
// #define PAGEALLOCATOR_PROTECT_FREEPAGE
// #define ARENA_MEMORY_VERIFY
// #define SEPARATE_ARENA
// #define LEAK_REPORT
// #define CHECK_MEMORY_LEAK
// #define RECYCLER_MARK_TRACK
// #define INTERNAL_MEM_PROTECT_HEAP_ALLOC

#if defined(ENABLE_JS_ETW) || defined(DUMP_FRAGMENTATION_STATS)
#define ENABLE_MEM_STATS 1
#define POLY_INLINE_CACHE_SIZE_STATS
#endif

#define NO_SANITIZE_ADDRESS
#if defined(__has_feature)
#if __has_feature(address_sanitizer)
#undef NO_SANITIZE_ADDRESS
#define NO_SANITIZE_ADDRESS __attribute__((no_sanitize("address")))
#define NO_SANITIZE_ADDRESS_CHECK
#endif
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
#if defined(TARGET_64)
#define INT32VAR 1
#else
#define INT32VAR 0
#endif
#endif

#ifndef FLOATVAR
#if defined(TARGET_64)
#define FLOATVAR 1
#else
#define FLOATVAR 0
#endif
#endif


#ifdef _M_IX86
#define LOWER_SPLIT_INT64 1
#else
#define LOWER_SPLIT_INT64 0
#endif

#if (defined(_M_IX86) || defined(_M_X64)) && !defined(DISABLE_JIT)
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

#if _M_IX86
#define I386_ASM 1
#endif //_M_IX86

#ifndef PDATA_ENABLED
#if defined(_M_ARM32_OR_ARM64) || defined(_M_X64)
#define PDATA_ENABLED 1
#define ALLOC_XDATA (true)
#else
#define PDATA_ENABLED 0
#define ALLOC_XDATA (false)
#endif
#endif

#define DISABLE_SEH 1

//----------------------------------------------------------------------------------------------------
// Dependent flags
//  - flags values that are dependent on other flags
//----------------------------------------------------------------------------------------------------

#if !ENABLE_CONCURRENT_GC
#undef IDLE_DECOMMIT_ENABLED   // Currently idle decommit can only be enabled if concurrent gc is enabled
#endif

#ifdef BAILOUT_INJECTION
#define ENABLE_PREJIT
#endif

#if defined(ENABLE_DEBUG_CONFIG_OPTIONS)
// Enable Output::Trace
#define ENABLE_TRACE
#endif

#if !(defined(__clang__) && defined(_M_ARM32_OR_ARM64)) // xplat-todo: ARM
#if DBG || defined(CHECK_MEMORY_LEAK) || defined(LEAK_REPORT) || defined(TRACK_DISPATCH) || defined(ENABLE_TRACE) || defined(RECYCLER_PAGE_HEAP)
#define STACK_BACK_TRACE
#endif
#endif

// ENABLE_DEBUG_STACK_BACK_TRACE is for capturing stack back trace for debug only.
// (STACK_BACK_TRACE is enabled on release build, used by RECYCLER_PAGE_HEAP.)
#if ENABLE_DEBUG_CONFIG_OPTIONS && defined(STACK_BACK_TRACE)
#define ENABLE_DEBUG_STACK_BACK_TRACE 1
#endif

#if defined(TRACK_DISPATCH) || defined(CHECK_MEMORY_LEAK) || defined(LEAK_REPORT)
#define TRACK_JS_DISPATCH
#endif

// LEAK_REPORT and CHECK_MEMORY_LEAK requires RECYCLER_DUMP_OBJECT_GRAPH
// HEAP_TRACK_ALLOC and RECYCLER_STATS
#if defined(LEAK_REPORT) || defined(CHECK_MEMORY_LEAK)
#define RECYCLER_DUMP_OBJECT_GRAPH
#define HEAP_TRACK_ALLOC
#define RECYCLER_STATS
#endif

// PROFILE_RECYCLER_ALLOC requires PROFILE_MEM
#if defined(PROFILE_RECYCLER_ALLOC) && !defined(PROFILE_MEM)
#define PROFILE_MEM
#endif

// RECYCLER_DUMP_OBJECT_GRAPH is needed when using PROFILE_RECYCLER_ALLOC
#if defined(PROFILE_RECYCLER_ALLOC) && !defined(RECYCLER_DUMP_OBJECT_GRAPH)
#define RECYCLER_DUMP_OBJECT_GRAPH
#endif


#if defined(HEAP_TRACK_ALLOC) || defined(PROFILE_RECYCLER_ALLOC)

#define TRACK_ALLOC
#define TRACE_OBJECT_LIFETIME           // track a particular object's lifetime
#endif

#if defined(USED_IN_STATIC_LIB)
#undef FAULT_INJECTION
#undef RECYCLER_DUMP_OBJECT_GRAPH
#undef HEAP_TRACK_ALLOC
#undef RECYCLER_STATS
#undef PERF_COUNTERS
#endif

// Not having the config options enabled trumps all the above logic for these switches
#ifndef ENABLE_DEBUG_CONFIG_OPTIONS
#undef ARENA_MEMORY_VERIFY
#undef RECYCLER_MEMORY_VERIFY
#undef PROFILE_MEM
#undef PROFILE_DICTIONARY
#undef PROFILE_RECYCLER_ALLOC
#undef PROFILE_EXEC
#undef PROFILE_EVALMAP
#undef FAULT_INJECTION
#undef RECYCLER_STRESS
#undef RECYCLER_SLOW_VERIFY
#undef RECYCLER_VERIFY_MARK
#undef RECYCLER_STATS
#undef RECYCLER_FINALIZE_CHECK
#undef RECYCLER_DUMP_OBJECT_GRAPH
#undef DBG_DUMP
#undef BGJIT_STATS
#undef EXCEPTION_RECOVERY
#undef PROFILE_STRINGS
#undef PROFILE_TYPES
#undef PROFILE_OBJECT_LITERALS
#undef SECURITY_TESTING
#undef LEAK_REPORT
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

#ifndef PROFILE_DICTIONARY
#define PROFILE_DICTIONARY 0
#endif

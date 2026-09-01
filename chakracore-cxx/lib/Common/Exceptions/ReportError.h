//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#pragma once

#include "Util/Abstractions.h"

enum ErrorReason
{
    JavascriptDispatch_OUTOFMEMORY = 1,
    Fatal_Internal_Error = 2,
    Fatal_Debug_Heap_OUTOFMEMORY = 3,
    Fatal_Amd64StackWalkerOutOfContexts = 4,
    // Unused = 5,
    Fatal_Binary_Inconsistency = 6,
    WriteBarrier_OUTOFMEMORY = 7,
    CustomHeap_MEMORYCORRUPTION = 8,
    LargeHeapBlock_Metadata_Corrupt = 9,
    Fatal_Version_Inconsistency = 10,
    MarkStack_OUTOFMEMORY = 11,
    EnterScript_FromDOM_NoScriptScope = 12,
    Fatal_FailedToBox_OUTOFMEMORY = 13,
    Fatal_Recycler_MemoryCorruption = 14,
    Fatal_Debugger_AttachDetach_Failure = 15,
    Fatal_EntryExitRecordCorruption = 16,
    Fatal_UnexpectedExceptionHandling = 17,
    Fatal_RpcFailure = 18,
    Fatal_JsReentrancy_Error = 19,
    Fatal_TTDAbort = 20,
    Fatal_Failed_API_Result = 21,
    Fatal_OutOfMemory = 22,
    // Unused = 23,
    Fatal_JsBuiltIn_Error = 24,
    Fatal_XDataRegistration = 25,
};

extern "C" void ReportFatalException(
    size_t context,
    int32_t exceptionCode,
    ErrorReason reasonCode,
    size_t scenario);

void CustomHeap_BadPageState_unrecoverable_error(
    size_t context);

void Amd64StackWalkerOutOfContexts_unrecoverable_error(
    size_t context);

void FailedToBox_OOM_unrecoverable_error(
    size_t context);

void DebugHeap_OOM_fatal_error();

void MarkStack_OOM_unrecoverable_error();

void EntryExitRecord_Corrupted_unrecoverable_error();

void LargeHeapBlock_Metadata_Corrupted(
    size_t context, unsigned char calculatedCheckSum);

void FromDOM_NoScriptScope_unrecoverable_error();
void Debugger_AttachDetach_unrecoverable_error(int32_t hr);
void OutOfMemory_unrecoverable_error();
void RecyclerSingleAllocationLimit_unrecoverable_error();
void MemGCSingleAllocationLimit_unrecoverable_error();

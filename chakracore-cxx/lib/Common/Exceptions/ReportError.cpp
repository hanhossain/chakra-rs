//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "CommonExceptionsPch.h"
#include "Exceptions/ReportError.h"

// Disable inline so that _ReturnAddress() will get the address of the calling function.
void ReportFatalException(
    __in size_t context,
    __in HRESULT exceptionCode,
    __in ErrorReason reasonCode,
    __in size_t scenario)
{
    // avoid the error text methods to be optimized out.
    UNREFERENCED_PARAMETER(scenario);

    if (IsDebuggerPresent())
    {
        DebugBreak();
    }

#ifdef DISABLE_SEH
    TerminateProcess(GetCurrentProcess(), (uint32_t)DBG_TERMINATE_PROCESS);
#else
    void * addressToBlame = _ReturnAddress();
    __try
    {
        size_t ExceptionInformation[2];
        ExceptionInformation[0] = (size_t)reasonCode;
        ExceptionInformation[1] = (size_t)context;
        RaiseException(exceptionCode, EXCEPTION_NONCONTINUABLE, 2, (size_t*)ExceptionInformation);
    }
    __except(FatalExceptionFilter(GetExceptionInformation(), addressToBlame))
    {
    }
#endif // DISABLE_SEH
}

// Disable optimization make sure all the frames are still available in Dr. Watson bug reports.
#pragma optimize("", off)
void JavascriptDispatch_OOM_fatal_error(
    __in size_t context)
{
    int scenario = 1;
    ReportFatalException(context, E_OUTOFMEMORY, JavascriptDispatch_OUTOFMEMORY, scenario);
};

void CustomHeap_BadPageState_unrecoverable_error(
    __in size_t context)
{
    int scenario = 1;
    ReportFatalException(context, E_UNEXPECTED, CustomHeap_MEMORYCORRUPTION, scenario);
};

void MarkStack_OOM_unrecoverable_error()
{
    int scenario = 1;
    ReportFatalException(NULL, E_OUTOFMEMORY, MarkStack_OUTOFMEMORY, scenario);
};

void Amd64StackWalkerOutOfContexts_unrecoverable_error(
    __in size_t context)
{
    int scenario = 1;
    ReportFatalException(context, E_UNEXPECTED, Fatal_Amd64StackWalkerOutOfContexts, scenario);
}

void FailedToBox_OOM_unrecoverable_error(
    __in size_t context)
{
    int scenario = 1;
    ReportFatalException(context, E_UNEXPECTED, Fatal_FailedToBox_OUTOFMEMORY, scenario);
}

#if defined(RECYCLER_WRITE_BARRIER) && defined(TARGET_64)
void X64WriteBarrier_OOM_unrecoverable_error()
{
    int scenario = 3;
    ReportFatalException(NULL, E_OUTOFMEMORY, WriteBarrier_OUTOFMEMORY, scenario);
}
#endif

void DebugHeap_OOM_fatal_error()
{
    int scenario = 3;
    ReportFatalException(NULL, E_OUTOFMEMORY, Fatal_Debug_Heap_OUTOFMEMORY, scenario);
}

void Binary_Inconsistency_fatal_error()
{
    int scenario = 4;
    ReportFatalException(NULL, E_UNEXPECTED, Fatal_Binary_Inconsistency, scenario);
}

void Version_Inconsistency_fatal_error()
{
    int scenario = 4;
    ReportFatalException(NULL, E_UNEXPECTED, Fatal_Version_Inconsistency, scenario);
}

#ifdef LARGEHEAPBLOCK_ENCODING
void LargeHeapBlock_Metadata_Corrupted(
    __in size_t context, __in unsigned char calculatedChecksum)
{
    int scenario = calculatedChecksum; /* For debugging purpose if checksum mismatch happen*/
    ReportFatalException(context, E_UNEXPECTED, LargeHeapBlock_Metadata_Corrupt, scenario);
};
#endif

// If you see this error, it is because of a DOM bug. Assign to daniec or jdweiner.
void FromDOM_NoScriptScope_unrecoverable_error()
{
    int scenario = 5;
    ReportFatalException(NULL, E_UNEXPECTED, EnterScript_FromDOM_NoScriptScope, scenario);
}

void Debugger_AttachDetach_unrecoverable_error(HRESULT hr)
{
    int scenario = 5;
    ReportFatalException(NULL, hr, Fatal_Debugger_AttachDetach_Failure, scenario);
}

void EntryExitRecord_Corrupted_unrecoverable_error()
{
    int scenario = 6;
    ReportFatalException(NULL, E_UNEXPECTED, Fatal_EntryExitRecordCorruption, scenario);
}

void UnexpectedExceptionHandling_fatal_error()
{
    int scenario = 7;
    ReportFatalException(NULL, E_UNEXPECTED, Fatal_UnexpectedExceptionHandling, scenario);
}

void RpcFailure_unrecoverable_error(HRESULT hr)
{
    int scenario = 8;
    ReportFatalException(NULL, hr, Fatal_RpcFailure, scenario);
}

void OutOfMemory_unrecoverable_error()
{
    int scenario = 9;
    ReportFatalException(NULL, E_OUTOFMEMORY, Fatal_OutOfMemory, scenario);
}

void RecyclerSingleAllocationLimit_unrecoverable_error()
{
    int scenario = 10;
    ReportFatalException(NULL, E_OUTOFMEMORY, Fatal_OutOfMemory, scenario);
}

void MemGCSingleAllocationLimit_unrecoverable_error()
{
    int scenario = 11;
    ReportFatalException(NULL, E_OUTOFMEMORY, Fatal_OutOfMemory, scenario);
}

// same as OutOfMemory_unrecoverable_error, but with a different `scenario`
// - just to cause separate bucketing of these failures 
void OutOfMemoryTooManyPinnedObjects_unrecoverable_error()
{
    int scenario = 12;
    ReportFatalException(NULL, E_OUTOFMEMORY, Fatal_OutOfMemory, scenario);
}

// same as OutOfMemory_unrecoverable_error, but with a different `scenario`
// - just to cause separate bucketing of these failures 
void OutOfMemoryTooManyClosedContexts_unrecoverable_error()
{
    int scenario = 13;
    ReportFatalException(NULL, E_OUTOFMEMORY, Fatal_OutOfMemory, scenario);
}

// same as OutOfMemory_unrecoverable_error, but with a different `scenario`
// - just to cause separate bucketing of these failures 
void OutOfMemoryAllocationPolicy_unrecoverable_error()
{
    int scenario = 14;
    ReportFatalException(NULL, E_OUTOFMEMORY, Fatal_OutOfMemory, scenario);
}

void XDataRegistration_unrecoverable_error(HRESULT hr, size_t scenario)
{
    ReportFatalException(NULL, hr, Fatal_XDataRegistration, scenario);
}

///////
//
// The following variety of OOM unrecoverable errors are similar to the ones above 
// and exist specifically for additional distinct bucketing of crash dumps for 
// diagnostics purposes.
//
///////

void OutOfMemoryTooManyPinnedObjects_unrecoverable_error_visible()
{
    int scenario = 15;
    ReportFatalException(NULL, E_OUTOFMEMORY, Fatal_OutOfMemory, scenario);
}

void OutOfMemoryTooManyClosedContexts_unrecoverable_error_visible()
{
    int scenario = 16;
    ReportFatalException(NULL, E_OUTOFMEMORY, Fatal_OutOfMemory, scenario);
}

void OutOfMemoryAllocationPolicy_unrecoverable_error_visible()
{
    int scenario = 17;
    ReportFatalException(NULL, E_OUTOFMEMORY, Fatal_OutOfMemory, scenario);
}

void OutOfMemoryTooManyPinnedObjects_unrecoverable_error_notvisible()
{
    int scenario = 18;
    ReportFatalException(NULL, E_OUTOFMEMORY, Fatal_OutOfMemory, scenario);
}

void OutOfMemoryTooManyClosedContexts_unrecoverable_error_notvisible()
{
    int scenario = 19;
    ReportFatalException(NULL, E_OUTOFMEMORY, Fatal_OutOfMemory, scenario);
}

void OutOfMemoryAllocationPolicy_unrecoverable_error_notvisible()
{
    int scenario = 20;
    ReportFatalException(NULL, E_OUTOFMEMORY, Fatal_OutOfMemory, scenario);
}

#pragma optimize("",on)

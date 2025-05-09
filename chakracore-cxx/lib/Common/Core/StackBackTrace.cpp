//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "CommonCorePch.h"

#ifdef STACK_BACK_TRACE
#include "Core/StackBackTrace.h"
#include "Core/DbgHelpSymbolManager.h"

StackBackTrace::StackBackTrace(uint32_t framesToSkip, uint32_t framesToCapture) : requestedFramesToCapture(framesToCapture)
{
    this->Capture(framesToSkip);
}

// Don't capture, just remember requestedFramesToCapture/allocate buffer for them.
StackBackTrace::StackBackTrace(uint32_t framesToCaptureLater) : requestedFramesToCapture(framesToCaptureLater), framesCount(0)
{
}

StackBackTrace *
StackBackTrace::Capture(char* buffer, size_t bufSize, uint32_t framesToSkip)
{
    return new (buffer) StackBackTrace(framesToSkip, (uint32_t)(bufSize - offsetof(StackBackTrace, stackBackTrace)) / sizeof(void*));
}

// This can be called multiple times, together with Create, in which case we will use (overwrite) same buffer.
uint32_t StackBackTrace::Capture(uint32_t framesToSkip)
{
    this->framesCount = CaptureStackBackTrace(framesToSkip + BaseFramesToSkip, this->requestedFramesToCapture, this->stackBackTrace, NULL);
    return this->framesCount;
}

size_t
StackBackTrace::Print()
{
    size_t retValue = 0;

#ifdef DBGHELP_SYMBOL_MANAGER
    DbgHelpSymbolManager::EnsureInitialized();

    for(uint32_t i = 0; i < this->framesCount; i++)
    {
        void * address = this->stackBackTrace[i];
        retValue += Output::Print(_u(" "));
        retValue += DbgHelpSymbolManager::PrintSymbol(address);
        retValue += Output::Print(_u("\n"));
    }
#else
    char** f = backtrace_symbols(this->stackBackTrace, this->framesCount);
    if (f)
    {
        for (uint32_t i = 0; i < this->framesCount; i++)
        {
            retValue += Output::Print(_u(" %S\n"), f[i]);
        }
        free(f);
    }
#endif

    retValue += Output::Print(_u("\n"));
    return retValue;
}

#if ENABLE_DEBUG_CONFIG_OPTIONS
static uint _s_trace_ring_id = 0;
uint _trace_ring_next_id()
{
    return InterlockedIncrement(&_s_trace_ring_id);
}
#endif  // ENABLE_DEBUG_CONFIG_OPTIONS

#endif

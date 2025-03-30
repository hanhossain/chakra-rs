//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "RuntimeBasePch.h"

/*
 * The following diagram depicts the layout of the user-mode stack. To the left is what the stack
 * looks like to the OS. To the right are the logical partitions Chakra divides the usable stack into.
 * The goal of SO checks in the runtime is to never touch a guard page. i.e. we always detect
 * that we might run out of stack space (at specific points in code) and raise a stack overflow
 * exception. We thus guarantee that our data structures are consistent even when a SO exception is raised.
 *
 * All stack probes check if the current stack pointer - requested size > current stack limit.
 *
 *  stack start (low)    +----------------------+
 *                       | MEM_RESERVE          |
 *                       +----------------------+
 *                       | Thread stack         |
 *                       ~ guarantee            ~
 *                       | (PAGE_GUARD)         |
 *                       +----------------------+ <--------------> +----------------------+
 *                       | MEM_RESERVE          |                  |                      |
 *                       ~                      ~                  ~ overflow handling    | (B)
 *                       |                      |                  |   buffer             |
 *                       +----------------------+                  +----------------------+ <---- stackLimit
 *                       | PAGE_GUARD           |                  |                      |
 *                       +----------------------+                  |                      |
 *                       | MEM_COMMIT |         |                  | script region        |
 *                       ~ PAGE_READWRITE       ~                  ~                      ~ (A)
 *                       |                      |                  |                      |
 *               (high)  +----------------------+ <--------------> +----------------------+
 */

void
StackProber::Initialize()
{
    // NumGuardPages is 2 on x86/x86-64
    // 1 MEM_RESERVE page at the bottom of the stack
    // 1 PAGE_GUARD | PAGE_READWRITE page that serves as the guard page
    const size_t guardPageSize = Js::Constants::NumGuardPages * AutoSystemInfo::PageSize;
    const size_t stackOverflowBuffer = Js::Constants::StackOverflowHandlingBufferPages * AutoSystemInfo::PageSize;

    size_t stackBottom = 0;      // This is the low address limit (here we consider stack growing down).
    uint32_t stackGuarantee = 0;

#if defined(_M_ARM)
    uint32_t lowLimit, highLimit;
    ::GetCurrentThreadStackLimits(&lowLimit, &highLimit);
    stackBottom = lowLimit;
#elif defined(_M_ARM64)
    ULONG64 lowLimit, highLimit;
    ::GetCurrentThreadStackLimits(&lowLimit, &highLimit);
    stackBottom =  lowLimit;
#else
    size_t lowLimit = 0;
    size_t highLimit = 0;
    ::GetCurrentThreadStackLimits(&lowLimit, &highLimit);
    stackBottom = lowLimit;
#endif

    Assert(stackBottom);
    stackLimit = stackBottom + guardPageSize + stackGuarantee + stackOverflowBuffer;
}

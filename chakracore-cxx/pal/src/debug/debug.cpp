//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
//

/*++



Module Name:

    debug.c

Abstract:

    Implementation of Win32 debugging API functions.

Revision History:



--*/

#include "pal/thread.hpp"
#include "pal/procobj.hpp"
#include "pal/file.hpp"

#include "pal/palinternal.h"
#include "pal/dbgmsg.h"
#include "pal/process.h"
#include "pal/context.h"
#include "pal/debug.h"
#include "pal/misc.h"
#include <new>
#include "pal/stackstring.hpp"
#include "pal/virtual.h"

#include <signal.h>
#include <unistd.h>
#include <sys/ptrace.h>
#if defined(__APPLE__)
#include <mach/mach.h>
#endif  // defined(__APPLE__)
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

#if defined(__APPLE__)
#include "../exception/machexception.h"
#endif // defined(__APPLE__)

using namespace CorUnix;

SET_DEFAULT_DEBUG_CHANNEL(DEBUG);

/* ------------------- Static function prototypes ----------------------------*/

extern "C" {

/*++
Function:
  FlushInstructionCache

The FlushInstructionCache function flushes the instruction cache for
the specified process.

Remarks

This is a no-op for x86 architectures where the instruction and data
caches are coherent in hardware. For non-X86 architectures, this call
usually maps to a kernel API to flush the D-caches on all processors.

--*/
BOOL FlushInstructionCache(const void * lpBaseAddress, size_t dwSize)
{
    BOOL Ret;

    if (lpBaseAddress != NULL)
    {
        Ret = DBG_FlushInstructionCache(lpBaseAddress, dwSize);
    }
    else
    {
        Ret = TRUE;
    }

    LOGEXIT("FlushInstructionCache returns BOOL %d\n", Ret);
    return Ret;
}


/*++
Function:
  DebugBreak

See MSDN doc.
--*/
void
DebugBreak(
       void)
{
    TRACE("Calling DBG_DebugBreak\n");
    DBG_DebugBreak();

    LOGEXIT("DebugBreak returns\n");
}

/*++
Function:
  GetThreadContext

See MSDN doc.
--*/
BOOL
GetThreadContext(
            HANDLE hThread,
             LPCONTEXT lpContext)
{
    PAL_ERROR palError;
    CPalThread *pThread;
    CPalThread *pTargetThread;
    IPalObject *pobjThread = NULL;
    BOOL ret = FALSE;

    pThread = InternalGetCurrentThread();

    palError = InternalGetThreadDataFromHandle(
        pThread,
        hThread,
        &pTargetThread, // THREAD_GET_CONTEXT
        &pobjThread
    );

    if (NO_ERROR == palError)
    {
        if (!pTargetThread->IsDummy())
        {
            ret = CONTEXT_GetThreadContext(
                pTargetThread->GetPThreadSelf(),
                lpContext
                );
        }
        else
        {
            ASSERT("Dummy thread handle passed to GetThreadContext\n");
            pThread->SetLastError(ERROR_INVALID_HANDLE);
        }
    }
    else
    {
        pThread->SetLastError(palError);
    }

    if (NULL != pobjThread)
    {
        pobjThread->ReleaseReference(pThread);
    }

    LOGEXIT("GetThreadContext returns ret:%d\n", ret);
    return ret;
}

} // extern "C"

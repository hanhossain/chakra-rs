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

extern "C" void DBG_DebugBreak_End();

/* ------------------- Constant definitions ----------------------------------*/

#if !defined(__APPLE__)
const BOOL DBG_ATTACH       = TRUE;
const BOOL DBG_DETACH       = FALSE;
#endif

/* ------------------- Static function prototypes ----------------------------*/

#if !defined(__APPLE__)
static int
DBGWriteProcMem_Int(uint32_t processId, int *addr, int data);
static int
DBGWriteProcMem_IntWithMask(uint32_t processId, int *addr, int data,
                            unsigned int mask);
#endif  // !defined(__APPLE__)

#if !defined(__APPLE__)

static BOOL
DBGAttachProcess(CPalThread *pThread, HANDLE hProcess, uint32_t dwProcessId);

static BOOL
DBGDetachProcess(CPalThread *pThread, HANDLE hProcess, uint32_t dwProcessId);

static int
DBGSetProcessAttached(CPalThread *pThread, HANDLE hProcess, BOOL bAttach);

#endif // !defined(__APPLE__)

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
  IsInDebugBreak(addr)

  Returns true if the address is in DBG_DebugBreak.

--*/
BOOL
IsInDebugBreak(void *addr)
{
    return (addr >= reinterpret_cast<void *>(DBG_DebugBreak)) && (addr <= reinterpret_cast<void *>(DBG_DebugBreak_End));
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

/*++
Function:
  SetThreadContext

See MSDN doc.
--*/
BOOL
SetThreadContext(
            HANDLE hThread,
            const CONTEXT *lpContext)
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
        &pTargetThread, // THREAD_SET_CONTEXT
        &pobjThread
    );

    if (NO_ERROR == palError)
    {
        if (!pTargetThread->IsDummy())
        {
            ret = CONTEXT_SetThreadContext(
                lpContext
            );
        }
        else
        {
            ASSERT("Dummy thread handle passed to SetThreadContext\n");
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

    return ret;
}

/*++
Function:
  WriteProcessMemory

See MSDN doc.
--*/
BOOL
WriteProcessMemory(
            HANDLE hProcess,
            void * lpBaseAddress,
            const void * lpBuffer,
            size_t nSize,
            size_t * lpNumberOfBytesWritten
           )

{
#if !defined(__APPLE__)
    CPalThread *pThread;
#endif
    uint32_t processId;
    Volatile<BOOL> ret = FALSE;
    Volatile<size_t> numberOfBytesWritten = 0;
#if defined(__APPLE__)
    kern_return_t result;
    vm_map_t task;
#else
    size_t FirstIntOffset;
    size_t LastIntOffset;
    unsigned int FirstIntMask;
    unsigned int LastIntMask;
    size_t nbInts;
    int *lpTmpBuffer = 0, *lpInt;
    int* lpBaseAddressAligned;
#endif

#if !defined(__APPLE__)
    pThread = InternalGetCurrentThread();
#endif

    if (!(nSize && (processId = PROCGetProcessIDFromHandle(hProcess))))
    {
        ERROR("Invalid nSize:%u number or invalid process handler "
              "hProcess:%p\n", (unsigned int)nSize, hProcess);
        SetLastError(ERROR_INVALID_PARAMETER);
        goto EXIT;
    }

    // Check if the write request is for the current process.
    // In that case we don't need ptrace.
    if (getpid() == processId)
    {
        TRACE("We are in the same process so we don't need ptrace\n");

        struct Param
        {
            void * lpBaseAddress;
            const void * lpBuffer;
            size_t nSize;
            size_t numberOfBytesWritten;
            BOOL ret;
        } param;
        param.lpBaseAddress = lpBaseAddress;
        param.lpBuffer = lpBuffer;
        param.nSize = nSize;
        param.numberOfBytesWritten = numberOfBytesWritten;
        param.ret = ret;

        {
            size_t i;

            // Seg fault in memcpy can't be caught
            // so we simulate the memcpy here

            for (i = 0; i<param.nSize; i++)
            {
                *(static_cast<char*>(param.lpBaseAddress)+i) = *(const_cast<char*>(static_cast<const char*>(param.lpBuffer))+i);
            }

            param.numberOfBytesWritten = param.nSize;
            param.ret = TRUE;
        }

        numberOfBytesWritten = param.numberOfBytesWritten;
        ret = param.ret;
        goto EXIT;
    }

#if defined(__APPLE__)
    result = task_for_pid(mach_task_self(), processId, &task);
    if (result != KERN_SUCCESS)
    {
        ERROR("No Mach task for pid %d: %d\n", processId, ret.Load());
        SetLastError(ERROR_INVALID_HANDLE);
        goto EXIT;
    }
    result = vm_write(task, reinterpret_cast<vm_address_t>(lpBaseAddress),
                      reinterpret_cast<vm_address_t>(lpBuffer), nSize);
    if (result != KERN_SUCCESS)
    {
        ERROR("vm_write failed for %d bytes from %p in %d: %d\n",
              (int)nSize, lpBaseAddress, task, result);
        if (result <= KERN_RETURN_MAX)
        {
            SetLastError(ERROR_ACCESS_DENIED);
        }
        else
        {
            SetLastError(ERROR_INTERNAL_ERROR);
        }
        goto EXIT;
    }
    numberOfBytesWritten = nSize;
    ret = TRUE;
#else   // defined(__APPLE__)
    /* Attach the process before calling ptrace otherwise it fails */
    if (DBGAttachProcess(pThread, hProcess, processId))
    {
        FirstIntOffset = reinterpret_cast<size_t>(lpBaseAddress) % sizeof(int);
        FirstIntMask = -1;
        FirstIntMask <<= (FirstIntOffset * 8);

        nbInts = (nSize + FirstIntOffset) / sizeof(int) +
                 (((nSize + FirstIntOffset)%sizeof(int)) ? 1:0);
        lpBaseAddressAligned = reinterpret_cast<int*>(static_cast<char*>(lpBaseAddress) - FirstIntOffset);

        if ((lpTmpBuffer = static_cast<int*>(malloc((nbInts * sizeof(int))))) == NULL)
        {
            ERROR("Insufficient memory available !\n");
            SetLastError(ERROR_NOT_ENOUGH_MEMORY);
            goto CLEANUP1;
        }

        memcpy(reinterpret_cast<char*>(lpTmpBuffer) + FirstIntOffset, lpBuffer, nSize);
        lpInt = lpTmpBuffer;

        LastIntOffset = (nSize + FirstIntOffset) % sizeof(int);
        LastIntMask = -1;
        LastIntMask >>= ((sizeof(int) - LastIntOffset) * 8);

        if (nbInts == 1)
        {
            if (DBGWriteProcMem_IntWithMask(processId, lpBaseAddressAligned,
                                            *lpInt,
                                            LastIntMask & FirstIntMask)
                  == 0)
            {
                goto CLEANUP2;
            }
            numberOfBytesWritten = nSize;
            ret = TRUE;
            goto CLEANUP2;
        }

        if (DBGWriteProcMem_IntWithMask(processId,
                                        lpBaseAddressAligned++,
                                        *lpInt++, FirstIntMask)
            == 0)
        {
            goto CLEANUP2;
        }

        while (--nbInts > 1)
        {
          if (DBGWriteProcMem_Int(processId, lpBaseAddressAligned++,
                                  *lpInt++) == 0)
          {
              goto CLEANUP2;
          }
        }

        if (DBGWriteProcMem_IntWithMask(processId, lpBaseAddressAligned,
                                        *lpInt, LastIntMask ) == 0)
        {
            goto CLEANUP2;
        }

        numberOfBytesWritten = nSize;
        ret = TRUE;
    }
    else
    {
        /* Failed to attach processId */
        goto EXIT;
    }

CLEANUP2:
    if (lpTmpBuffer)
    {
        free(lpTmpBuffer);
    }

CLEANUP1:
    if (!DBGDetachProcess(pThread, hProcess, processId))
    {
        /* Failed to detach processId */
        ret = FALSE;
    }
#endif  // defined(__APPLE__)

EXIT:
    if (lpNumberOfBytesWritten)
    {
        *lpNumberOfBytesWritten = numberOfBytesWritten;
    }

    LOGEXIT("WriteProcessMemory returns BOOL %d\n", ret.Load());
    return ret;
}

#if !defined(__APPLE__)
/*++
Function:
  DBGWriteProcMem_Int

Abstract
  write one int to a process memory address

Parameter
  processId : process handle
  addr : memory address where the int should be written
  data : int to be written in addr

Return
  Return 1 if it succeeds, or 0 if it's fails
--*/
static
int
DBGWriteProcMem_Int( uint32_t processId,
                     int *addr,
                     int data)
{
    if (ptrace((PTRACE_POKEDATA), (processId), static_cast<void*>(addr), (data)) == -1)
    {
        if (errno == EFAULT)
        {
            ERROR("ptrace(PT_WRITE_D, pid:%d caddr_t:%p data:%x) failed "
                  "errno:%d (%s)\n", processId, addr, data, errno, strerror(errno));
            SetLastError(ERROR_INVALID_ADDRESS);
        }
        else
        {
            ASSERT("ptrace(PT_WRITE_D, pid:%d caddr_t:%p data:%x) failed "
                  "errno:%d (%s)\n", processId, addr, data, errno, strerror(errno));
            SetLastError(ERROR_INTERNAL_ERROR);
        }
        return 0;
    }

    return 1;
}

/*++
Function:
  DBGWriteProcMem_IntWithMask

Abstract
  write one int to a process memory address space using mask

Parameter
  processId : process ID
  addr : memory address where the int should be written
  data : int to be written in addr
  mask : the mask used to write only a parts of data

Return
  Return 1 if it succeeds, or 0 if it's fails
--*/
static
int
DBGWriteProcMem_IntWithMask( uint32_t processId,
                             int *addr,
                             int data,
                             unsigned int mask )
{
    int readInt;

    if (mask != ~0)
    {
        errno = 0;
        if (((readInt = ptrace((PTRACE_PEEKDATA), (processId), static_cast<void*>(addr), (0))) == -1)
             && errno)
        {
            if (errno == EFAULT)
            {
                ERROR("ptrace(PT_READ_D, pid:%d, caddr_t:%p, 0) failed "
                      "errno:%d (%s)\n", processId, addr, errno, strerror(errno));
                SetLastError(ERROR_INVALID_ADDRESS);
            }
            else
            {
                ASSERT("ptrace(PT_READ_D, pid:%d, caddr_t:%p, 0) failed "
                      "errno:%d (%s)\n", processId, addr, errno, strerror(errno));
                SetLastError(ERROR_INTERNAL_ERROR);
            }

            return 0;
        }
        data = (data & mask) | (readInt & ~mask);
    }
    return DBGWriteProcMem_Int(processId, addr, data);
}
#endif  // !defined(__APPLE__)

#if !defined(__APPLE__)

/*++
Function:
  DBGAttachProcess

Abstract

  Attach the indicated process to the current process.

  if the indicated process is already attached by the current process, then
  increment the number of attachment pending. if ot, attach it to the current
  process (with PT_ATTACH).

Parameter
  hProcess : handle to process to attach to
  processId : process ID to attach
Return
  Return true if it succeeds, or false if it's fails
--*/
static
BOOL
DBGAttachProcess(
    CPalThread *pThread,
    HANDLE hProcess,
    uint32_t processId
    )
{
    int savedErrno;

    int attachmentCount =
        DBGSetProcessAttached(pThread, hProcess, DBG_ATTACH);

    if (attachmentCount == -1)
    {
        /* Failed to set the process as attached */
        goto EXIT;
    }

    if (attachmentCount == 1)
    {
        if (ptrace((PTRACE_ATTACH), (processId), static_cast<void*>(0), (0)) == -1)
        {
            if (errno != ESRCH)
            {
                ASSERT("ptrace(PT_ATTACH, pid:%d) failed errno:%d (%s)\n",
                     processId, errno, strerror(errno));
            }
            goto DETACH1;
        }

        if (waitpid(processId, NULL, WUNTRACED) == -1)
        {
            if (errno != ESRCH)
            {
                ASSERT("waitpid(pid:%d, NULL, WUNTRACED) failed.errno:%d"
                       " (%s)\n", processId, errno, strerror(errno));
            }
            goto DETACH2;
        }
    }

    return TRUE;

DETACH2:
    if (ptrace((PTRACE_DETACH), (processId), static_cast<void*>(0), (0)) == -1)
    {
        ASSERT("ptrace(PT_DETACH, pid:%d) failed. errno:%d (%s)\n", processId,
              errno, strerror(errno));
    }

DETACH1:
    savedErrno = errno;
    DBGSetProcessAttached(pThread, hProcess, DBG_DETACH);
    errno = savedErrno;
EXIT:
    if (errno == ESRCH || errno == ENOENT || errno == EBADF)
    {
        ERROR("Invalid process ID:%d\n", processId);
        SetLastError(ERROR_INVALID_PARAMETER);
    }
    else
    {
        SetLastError(ERROR_INTERNAL_ERROR);
    }
    return FALSE;
}

/*++
Function:
  DBGDetachProcess

Abstract
  Detach the indicated process from the current process.

  if the indicated process is already attached by the current process, then
  decrement the number of attachment pending and detach it from the current
  process (with PT_DETACH) if there's no more attachment left.

Parameter
  hProcess : process handle
  processId : process ID

Return
  Return true if it succeeds, or true if it's fails
--*/
static
BOOL
DBGDetachProcess(
    CPalThread *pThread,
    HANDLE hProcess,
    uint32_t processId
    )
{
    int nbAttachLeft = DBGSetProcessAttached(pThread, hProcess, DBG_DETACH);

    if (nbAttachLeft == -1)
    {
        /* Failed to set the process as detached */
        return FALSE;
    }

    /* check if there's no more attachment left on processId */
    if (nbAttachLeft == 0)
    {
        if (ptrace((PTRACE_DETACH), (processId), reinterpret_cast<void*>(1), (0)) == -1)
        {
            if (errno == ESRCH)
            {
                ERROR("Invalid process ID: %d\n", processId);
                SetLastError(ERROR_INVALID_PARAMETER);
            }
            else
            {
                ASSERT("ptrace(PT_DETACH, pid:%d) failed. errno:%d (%s)\n",
                      processId, errno, strerror(errno));
                SetLastError(ERROR_INTERNAL_ERROR);
            }
            return FALSE;
        }

        if (kill(processId, SIGCONT) == -1)
        {
            ERROR("Failed to continue the detached process:%d errno:%d (%s)\n",
                  processId, errno, strerror(errno));
            return FALSE;
        }
    }
    return TRUE;
}

/*++
Function:
  DBGSetProcessAttached

Abstract
  saves the current process Id in the attached process structure

Parameter
  hProcess : process handle
  bAttach : true (false) to set the process as attached (as detached)
Return
 returns the number of attachment left on attachedProcId, or -1 if it fails
--*/
static int
DBGSetProcessAttached(
    CPalThread *pThread,
    HANDLE hProcess,
    BOOL  bAttach
    )
{
    PAL_ERROR palError = NO_ERROR;
    IPalObject *pobjProcess = NULL;
    IDataLock *pDataLock = NULL;
    CProcProcessLocalData *pLocalData = NULL;
    int ret = -1;
    CAllowedObjectTypes aotProcess(otiProcess);

    palError = g_pObjectManager->ReferenceObjectByHandle(
        pThread,
        hProcess,
        &aotProcess,
        &pobjProcess
    );

    if (NO_ERROR != palError)
    {
        goto DBGSetProcessAttachedExit;
    }

    palError = pobjProcess->GetProcessLocalData(
        pThread,
        WriteLock,
        &pDataLock,
        reinterpret_cast<void **>(&pLocalData)
        );

    if (NO_ERROR != palError)
    {
        goto DBGSetProcessAttachedExit;
    }

    if (bAttach)
    {
        pLocalData->lAttachCount += 1;
    }
    else
    {
        pLocalData->lAttachCount -= 1;

        if (pLocalData->lAttachCount < 0)
        {
            ASSERT("pLocalData->lAttachCount < 0 check for extra DBGDetachProcess calls\n");
            palError = ERROR_INTERNAL_ERROR;
            goto DBGSetProcessAttachedExit;
        }
    }

    ret = pLocalData->lAttachCount;

DBGSetProcessAttachedExit:

    if (NULL != pDataLock)
    {
        pDataLock->ReleaseLock(pThread);
    }

    if (NULL != pobjProcess)
    {
        pobjProcess->ReleaseReference(pThread);
    }

    return ret;
}

#endif // !defined(__APPLE__)

} // extern "C"

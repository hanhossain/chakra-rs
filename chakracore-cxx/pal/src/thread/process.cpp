//-------------------------------------------------------------------------------------------------------
// ChakraCore/Pal
// Contains portions (c) copyright Microsoft, portions copyright (c) the .NET Foundation and Contributors
// and edits (c) copyright the ChakraCore Contributors.
// See THIRD-PARTY-NOTICES.txt in the project root for .NET Foundation license
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

/*++

Module Name:

    process.cpp

Abstract:

    Implementation of process object and functions related to processes.

--*/

#include "pal/dbgmsg.h"
SET_DEFAULT_DEBUG_CHANNEL(PROCESS); // some headers have code with asserts, so do this first

#include "pal/procobj.hpp"
#include "pal/thread.hpp"
#include "pal/file.hpp"
#include "pal/handlemgr.hpp"
#include "procprivate.hpp"
#include "pal/palinternal.h"
#include "pal/process.h"
#include "pal/init.h"
#include "pal/critsect.h"
#include "pal/debug.h"
#include "pal/utils.h"
#include "pal/misc.h"
#include "pal/virtual.h"
#include "pal/stackstring.hpp"
#include "chakra/Logger.h"

#include <errno.h>
#include <poll.h>
#include <format>

#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#if HAVE_PRCTL_H
#include <sys/prctl.h>
#include <sys/syscall.h>
#endif
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <debugmacrosext.h>
#include <semaphore.h>
#include <stdint.h>
#include <dlfcn.h>
#include <limits.h>

#ifdef __linux__
#include <sys/syscall.h> // __NR_membarrier
// Ensure __NR_membarrier is defined for portable builds.
# if !defined(__NR_membarrier)
#  if defined(__amd64__)
#   define __NR_membarrier  324
#  elif defined(__i386__)
#   define __NR_membarrier  375
#  elif defined(__arm__)
#   define __NR_membarrier  389
#  elif defined(__aarch64__)
#   define __NR_membarrier  283
#  elif defined(__loongarch64)
#   define __NR_membarrier  283
#  else
#   error Unknown architecture
#  endif
# endif
#endif

#ifdef __APPLE__
#include <libproc.h>
#include <sys/sysctl.h>
#include <sys/posix_sem.h>
#include <mach/task.h>
#include <mach/vm_map.h>
extern "C"
{
#  include <mach/thread_state.h>
}

#define CHECK_MACH(_msg, machret) do {                                      \
        if (machret != KERN_SUCCESS)                                        \
        {                                                                   \
            char _szError[1024];                                            \
            mach_error(_szError, machret);                                  \
            abort();                                                        \
        }                                                                   \
    } while (false)

#endif // __APPLE__

#ifdef __NetBSD__
#include <sys/cdefs.h>
#include <sys/param.h>
#include <sys/sysctl.h>
#include <kvm.h>
#endif

#ifdef __FreeBSD__
#include <sys/sysctl.h>
#include <sys/user.h>
#endif

using namespace CorUnix;

CObjectType CorUnix::otProcess __attribute__((init_priority(200))) (
                otiProcess,
                NULL,   // No cleanup routine
                0,      // No immutable data
                sizeof(CProcProcessLocalData),
                CObjectType::WaitableObject,
                CObjectType::SingleTransitionObject,
                CObjectType::ThreadReleaseHasNoSideEffects,
                CObjectType::NoOwner
                );

CAllowedObjectTypes aotProcess __attribute__((init_priority(200))) (otiProcess);

//
// The representative IPalObject for this process
//
IPalObject* CorUnix::g_pobjProcess;

//
// Critical section that protects process data (e.g., the
// list of active threads)/
//
CRITICAL_SECTION g_csProcess __attribute__((init_priority(200)));

//
// List and count of active threads
//
CPalThread* CorUnix::pGThreadList;

// Thread ID of thread that has started the ExitProcess process
Volatile<int32_t> terminator __attribute__((init_priority(200))) = 0;

// Application group ID for this process
#ifdef __APPLE__
const char * gApplicationGroupId = nullptr;
int gApplicationGroupIdLength = 0;
#endif // __APPLE__
PathCharString* gSharedFilesPath = nullptr;

// The lowest common supported semaphore length, including null character
// NetBSD-7.99.25: 15 characters
// MacOSX 10.11: 31 -- Core 1.0 RC2 compatibility
#if defined(__NetBSD__)
#define CLR_SEM_MAX_NAMELEN 15
#elif defined(__APPLE__)
#define CLR_SEM_MAX_NAMELEN PSEMNAMLEN
#elif defined(NAME_MAX)
#define CLR_SEM_MAX_NAMELEN (NAME_MAX - 4)
#else
// On Solaris, MAXNAMLEN is 512, which is higher than MAX_PATH defined by pal.h
#define CLR_SEM_MAX_NAMELEN MAX_PATH
#endif

static_assert(CLR_SEM_MAX_NAMELEN <= MAX_PATH, "CLR_SEM_MAX_NAMELEN > MAX_PATH");

//
// Key used for associating CPalThread's with the underlying pthread
// (through pthread_setspecific)
//
pthread_key_t CorUnix::thObjKey;

PAL_ERROR
PrepareStandardHandle(
    CPalThread *pThread,
    HANDLE hFile,
    IPalObject **ppobjFile,
    int *piFd
    )
{
    PAL_ERROR palError = NO_ERROR;
    IPalObject *pobjFile = NULL;
    IDataLock *pDataLock = NULL;
    CFileProcessLocalData *pLocalData = NULL;
    int iError = 0;

    palError = g_pObjectManager->ReferenceObjectByHandle(
        pThread,
        hFile,
        &aotFile,
        &pobjFile
    );

    if (NO_ERROR != palError)
    {
        ERROR("Bad handle passed through CreateProcess\n");
        goto PrepareStandardHandleExit;
    }

    palError = pobjFile->GetProcessLocalData(
        pThread,
        ReadLock,
        &pDataLock,
        reinterpret_cast<void **>(&pLocalData)
        );

    if (NO_ERROR != palError)
    {
        chakra::Logger::error("Unable to access file data\n");
        goto PrepareStandardHandleExit;
    }

    //
    // The passed in file needs to be inheritable
    //

    if (!pLocalData->inheritable)
    {
        ERROR("Non-inheritable handle passed through CreateProcess\n");
        palError = ERROR_INVALID_HANDLE;
        goto PrepareStandardHandleExit;
    }

    iError = fcntl(pLocalData->unix_fd, F_SETFD, 0);
    if (-1 == iError)
    {
        ERROR("Unable to remove close-on-exec for file (errno %i)\n", errno);
        palError = ERROR_INVALID_HANDLE;
        goto PrepareStandardHandleExit;
    }

    *piFd = pLocalData->unix_fd;
    pDataLock->ReleaseLock(pThread);
    pDataLock = NULL;

    //
    // Transfer pobjFile reference to out parameter
    //

    *ppobjFile = pobjFile;
    pobjFile = NULL;

PrepareStandardHandleExit:

    if (NULL != pDataLock)
    {
        pDataLock->ReleaseLock(pThread);
    }

    if (NULL != pobjFile)
    {
        pobjFile->ReleaseReference(pThread);
    }

    return palError;
}

PAL_ERROR
CorUnix::InitializeProcessData(
    void
    )
{
    PAL_ERROR palError = NO_ERROR;
    bool fLockInitialized = FALSE;

    pGThreadList = NULL;

    InternalInitializeCriticalSection(&g_csProcess);
    fLockInitialized = TRUE;

    if (NO_ERROR != palError)
    {
        if (fLockInitialized)
        {
            InternalDeleteCriticalSection(&g_csProcess);
        }
    }

    return palError;
}

/*++
Function:
  CreateInitialProcessAndThreadObjects

Abstract
  Creates the IPalObjects that represent the current process
  and the initial thread

Parameter
  pThread - the initial thread

Return
  PAL_ERROR
--*/

PAL_ERROR
CorUnix::CreateInitialProcessAndThreadObjects(
    CPalThread *pThread
    )
{
    PAL_ERROR palError = NO_ERROR;
    HANDLE hThread;
    IPalObject *pobjProcess = NULL;
    IDataLock *pDataLock;
    CProcProcessLocalData *pLocalData;
    HANDLE hProcess;

    //
    // Create initial thread object
    //

    palError = CreateThreadObject(pThread, pThread, &hThread);
    if (NO_ERROR != palError)
    {
        goto CreateInitialProcessAndThreadObjectsExit;
    }

    //
    // This handle isn't needed
    //

    (void) g_pObjectManager->RevokeHandle(pThread, hThread);

    //
    // Create and initialize process object
    //

    palError = g_pObjectManager->AllocateObject(pThread, &otProcess, &pobjProcess);

    if (NO_ERROR != palError)
    {
        ERROR("Unable to allocate process object");
        goto CreateInitialProcessAndThreadObjectsExit;
    }

    palError = pobjProcess->GetProcessLocalData(
        pThread,
        WriteLock,
        &pDataLock,
        reinterpret_cast<void **>(&pLocalData)
        );

    if (NO_ERROR != palError)
    {
        chakra::Logger::error("Unable to access local data");
        goto CreateInitialProcessAndThreadObjectsExit;
    }

    pLocalData->dwProcessId = getpid();
    pLocalData->ps = PS_RUNNING;
    pDataLock->ReleaseLock(pThread);

    palError = g_pObjectManager->RegisterObject(
        pThread,
        pobjProcess,
        &aotProcess,
        PROCESS_ALL_ACCESS,
        &hProcess,
        &g_pobjProcess
        );

    //
    // pobjProcess is invalidated by the call to RegisterObject, so
    // NULL it out here to prevent it from being released later
    //

    pobjProcess = NULL;

    if (NO_ERROR != palError)
    {
        chakra::Logger::error("Failure registering process object");
        goto CreateInitialProcessAndThreadObjectsExit;
    }

    //
    // There's no need to keep this handle around, so revoke
    // it now
    //

    g_pObjectManager->RevokeHandle(pThread, hProcess);

CreateInitialProcessAndThreadObjectsExit:

    if (NULL != pobjProcess)
    {
        pobjProcess->ReleaseReference(pThread);
    }

    return palError;
}

/*++
Function:
  PROCAddThread

Abstract
  Add a thread to the thread list of the current process

Parameter
  pThread:   Thread object

--*/
void
CorUnix::PROCAddThread(
    CPalThread *pCurrentThread,
    CPalThread *pTargetThread
    )
{
    /* protect the access of the thread list with critical section for
       mutithreading access */
    InternalEnterCriticalSection(pCurrentThread, &g_csProcess);

    pTargetThread->SetNext(pGThreadList);
    pGThreadList = pTargetThread;

    TRACE("Thread 0x%p (id %#x) added to the process thread list\n",
          pTargetThread, pTargetThread->GetThreadId());

    InternalLeaveCriticalSection(pCurrentThread, &g_csProcess);
}


/*++
Function:
  PROCRemoveThread

Abstract
  Remove a thread form the thread list of the current process

Parameter
  CPalThread *pThread : thread object to remove

(no return value)
--*/
void
CorUnix::PROCRemoveThread(
    CPalThread *pCurrentThread,
    CPalThread *pTargetThread
    )
{
    CPalThread *curThread, *prevThread;

    /* protect the access of the thread list with critical section for
       mutithreading access */
    InternalEnterCriticalSection(pCurrentThread, &g_csProcess);

    curThread = pGThreadList;

    /* if thread list is empty */
    if (curThread == NULL)
    {
        chakra::Logger::error("Thread list is empty.\n");
        goto EXIT;
    }

    /* do we remove the first thread? */
    if (curThread == pTargetThread)
    {
        pGThreadList =  curThread->GetNext();
        TRACE("Thread 0x%p (id %#x) removed from the process thread list\n",
            pTargetThread, pTargetThread->GetThreadId());
        goto EXIT;
    }

    prevThread = curThread;
    curThread = curThread->GetNext();
    /* find the thread to remove */
    while (curThread != NULL)
    {
        if (curThread == pTargetThread)
        {
            /* found, fix the chain list */
            prevThread->SetNext(curThread->GetNext());
            TRACE("Thread %p removed from the process thread list\n", pTargetThread);
            goto EXIT;
        }

        prevThread = curThread;
        curThread = curThread->GetNext();
    }

    WARN("Thread %p not removed (it wasn't found in the list)\n", pTargetThread);

EXIT:
    InternalLeaveCriticalSection(pCurrentThread, &g_csProcess);
}

/*++
Function:
  PROCProcessLock

Abstract
  Enter the critical section associated to the current process

Parameter
  void

Return
  void
--*/
void
PROCProcessLock(
    void)
{
    CPalThread * pThread =
        (PALIsThreadDataInitialized() ? InternalGetCurrentThread() : NULL);

    InternalEnterCriticalSection(pThread, &g_csProcess);
}


/*++
Function:
  PROCProcessUnlock

Abstract
  Leave the critical section associated to the current process

Parameter
  void

Return
  void
--*/
void
PROCProcessUnlock(
    void)
{
    CPalThread * pThread =
        (PALIsThreadDataInitialized() ? InternalGetCurrentThread() : NULL);

    InternalLeaveCriticalSection(pThread, &g_csProcess);
}

#if USE_SYSV_SEMAPHORES
/*++
Function:
  PROCCleanupThreadSemIds

Abstract
  Cleanup SysV semaphore ids for all threads

(no parameters, no return value)
--*/
void
PROCCleanupThreadSemIds(void)
{
    //
    // When using SysV semaphores, the semaphore ids used by PAL threads must be removed
    // so they can be used again.
    //

    PROCProcessLock();

    CPalThread *pTargetThread = pGThreadList;
    while (NULL != pTargetThread)
    {
        pTargetThread->suspensionInfo.DestroySemaphoreIds();
        pTargetThread = pTargetThread->GetNext();
    }

    PROCProcessUnlock();

}
#endif // USE_SYSV_SEMAPHORES

/* Internal function definitions **********************************************/
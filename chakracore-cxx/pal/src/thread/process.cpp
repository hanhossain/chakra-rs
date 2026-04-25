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

#include <errno.h>
#include <poll.h>

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

extern char *g_szCoreCLRPath;
extern bool g_running_in_exe;

using namespace CorUnix;

CObjectType CorUnix::otProcess __attribute__((init_priority(200))) (
                otiProcess,
                NULL,   // No cleanup routine
                NULL,   // No initialization routine
                0,      // No immutable data
                sizeof(CProcProcessLocalData),
                0,
                PROCESS_ALL_ACCESS,
                CObjectType::SecuritySupported,
                CObjectType::SecurityInfoNotPersisted,
                CObjectType::UnnamedObject,
                CObjectType::CrossProcessDuplicationAllowed,
                CObjectType::WaitableObject,
                CObjectType::SingleTransitionObject,
                CObjectType::ThreadReleaseHasNoSideEffects,
                CObjectType::NoOwner
                );

//
// Helper membarrier function
//
#ifdef __NR_membarrier
# define membarrier(...)  syscall(__NR_membarrier, __VA_ARGS__)
#else
# define membarrier(...)  -ENOSYS
#endif

enum membarrier_cmd
{
    MEMBARRIER_CMD_QUERY                                 = 0,
    MEMBARRIER_CMD_GLOBAL                                = (1 << 0),
    MEMBARRIER_CMD_GLOBAL_EXPEDITED                      = (1 << 1),
    MEMBARRIER_CMD_REGISTER_GLOBAL_EXPEDITED             = (1 << 2),
    MEMBARRIER_CMD_PRIVATE_EXPEDITED                     = (1 << 3),
    MEMBARRIER_CMD_REGISTER_PRIVATE_EXPEDITED            = (1 << 4),
    MEMBARRIER_CMD_PRIVATE_EXPEDITED_SYNC_CORE           = (1 << 5),
    MEMBARRIER_CMD_REGISTER_PRIVATE_EXPEDITED_SYNC_CORE  = (1 << 6)
};

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
uint32_t g_dwThreadCount;

//
// The command line and app name for the process
//
char16_t* g_lpwstrCmdLine = NULL;
char16_t* g_lpwstrAppDir = NULL;

// Thread ID of thread that has started the ExitProcess process
Volatile<int32_t> terminator __attribute__((init_priority(200))) = 0;

// Process and session ID of this process.
uint32_t gPID = (uint32_t) -1;
uint32_t gSID = (uint32_t) -1;

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
PROCGetProcessStatus(
    CPalThread *pThread,
    HANDLE hProcess,
    PROCESS_STATE *pps,
    uint32_t *pdwExitCode);

static BOOL PROCEndProcess(HANDLE hProcess, uint32_t uExitCode, BOOL bTerminateUnconditionally);

/*++
Function:
  GetCurrentProcessId

See MSDN doc.
--*/
uint32_t
GetCurrentProcessId(
            void)
{
    ENTRY("GetCurrentProcessId()\n" );

    LOGEXIT("GetCurrentProcessId returns DWORD %#x\n", gPID);
    return gPID;
}


/*++
Function:
  GetCurrentSessionId

See MSDN doc.
--*/
uint32_t
GetCurrentSessionId(
            void)
{
    ENTRY("GetCurrentSessionId()\n" );

    LOGEXIT("GetCurrentSessionId returns DWORD %#x\n", gSID);
    return gSID;
}


/*++
Function:
  GetCurrentProcess

See MSDN doc.
--*/
HANDLE
GetCurrentProcess(
          void)
{
    ENTRY("GetCurrentProcess()\n" );

    LOGEXIT("GetCurrentProcess returns HANDLE %p\n", hPseudoCurrentProcess);

    /* return a pseudo handle */
    return hPseudoCurrentProcess;
}

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
        0,
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
        ASSERT("Unable to access file data\n");
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
    pDataLock->ReleaseLock(pThread, FALSE);
    pDataLock = NULL;

    //
    // Transfer pobjFile reference to out parameter
    //

    *ppobjFile = pobjFile;
    pobjFile = NULL;

PrepareStandardHandleExit:

    if (NULL != pDataLock)
    {
        pDataLock->ReleaseLock(pThread, FALSE);
    }

    if (NULL != pobjFile)
    {
        pobjFile->ReleaseReference(pThread);
    }

    return palError;
}

/*++
Function:
  TerminateProcess

Note:
  hProcess is a handle on the current process.

See MSDN doc.
--*/
BOOL
TerminateProcess(
     HANDLE hProcess,
     uint32_t uExitCode)
{
    BOOL ret;

    ENTRY("TerminateProcess(hProcess=%p, uExitCode=%u)\n",hProcess, uExitCode );

    ret = PROCEndProcess(hProcess, uExitCode, TRUE);

    LOGEXIT("TerminateProcess returns BOOL %d\n", ret);
    return ret;
}

/*++
Function:
  PROCEndProcess

  Called from TerminateProcess and ExitProcess. This does the work of
  TerminateProcess, but also takes a flag that determines whether we
  shut down unconditionally. If the flag is set, the PAL will do very
  little extra work before exiting. Most importantly, it won't shut
  down any DLLs that are loaded.

--*/
static BOOL PROCEndProcess(HANDLE hProcess, uint32_t uExitCode, BOOL bTerminateUnconditionally)
{
    uint32_t dwProcessId;
    BOOL ret = FALSE;

    dwProcessId = PROCGetProcessIDFromHandle(hProcess);
    if (dwProcessId == 0)
    {
        SetLastError(ERROR_INVALID_HANDLE);
    }
    else if(dwProcessId != GetCurrentProcessId())
    {
        if (uExitCode != 0)
            WARN("exit code 0x%x ignored for external process.\n", uExitCode);

        if (kill(dwProcessId, SIGKILL) == 0)
        {
            ret = TRUE;
        }
        else
        {
            switch (errno) {
            case ESRCH:
                SetLastError(ERROR_INVALID_HANDLE);
                break;
            case EPERM:
                SetLastError(ERROR_ACCESS_DENIED);
                break;
            default:
                // Unexpected failure.
                ASSERT(FALSE);
                SetLastError(ERROR_INTERNAL_ERROR);
                break;
            }
        }
    }
    else
    {
        // WARN/ERROR before starting the termination process and/or leaving the PAL.
        if (bTerminateUnconditionally)
        {
            WARN("exit code 0x%x ignored for terminate.\n", uExitCode);
        }
        else if ((uExitCode & 0xff) != uExitCode)
        {
            // TODO: Convert uExitCodes into sysexits(3)?
            ERROR("exit() only supports the lower 8-bits of an exit code. "
                "status will only see error 0x%x instead of 0x%x.\n", uExitCode & 0xff, uExitCode);
        }

        TerminateCurrentProcessNoExit(bTerminateUnconditionally);

        LOGEXIT("PROCEndProcess will not return\n");

        if (bTerminateUnconditionally)
        {
            // abort() has the semantics that
            // (1) it doesn't run atexit handlers
            // (2) can invoke CrashReporter or produce a coredump,
            // which is appropriate for TerminateProcess calls

            // If this turns out to be inappropriate for some case, where we
            // call TerminateProcess vs. ExitProcess, then there needs to be
            // a CLR wrapper for TerminateProcess and some exposure for PAL_abort()
            // to selectively use that in all but those cases.

            abort();
        }
        else
        {
            exit(uExitCode);
        }

        ASSERT(FALSE); // we shouldn't get here
    }

    return ret;
}

/*++
Function:
  PROCCleanupProcess

  Do all cleanup work for TerminateProcess, but don't terminate the process.
  If bTerminateUnconditionally is TRUE, we exit as quickly as possible.

(no return value)
--*/
void PROCCleanupProcess(BOOL bTerminateUnconditionally)
{
    /* Declare the beginning of shutdown */
    PALSetShutdownIntent();

    PALCommonCleanup();

    /* This must be called after PALCommonCleanup */
    PALShutdown();
}

#define FATAL_ASSERT(e, msg) \
    do \
    { \
        if (!(e)) \
        { \
            fprintf(stderr, "FATAL ERROR: " msg); \
            abort(); \
        } \
    } \
    while(0)

/*++
Function:
  PROCGetProcessIDFromHandle

Abstract
  Return the process ID from a process handle

Parameter
  hProcess:  process handle

Return
  Return the process ID, or 0 if it's not a valid handle
--*/
uint32_t
PROCGetProcessIDFromHandle(
        HANDLE hProcess)
{
    PAL_ERROR palError;
    IPalObject *pobjProcess = NULL;
    CPalThread *pThread = InternalGetCurrentThread();

    uint32_t dwProcessId = 0;

    if (hPseudoCurrentProcess == hProcess)
    {
        dwProcessId = gPID;
        goto PROCGetProcessIDFromHandleExit;
    }


    palError = g_pObjectManager->ReferenceObjectByHandle(
        pThread,
        hProcess,
        &aotProcess,
        0,
        &pobjProcess
        );

    if (NO_ERROR == palError)
    {
        IDataLock *pDataLock;
        CProcProcessLocalData *pLocalData;

        palError = pobjProcess->GetProcessLocalData(
            pThread,
            ReadLock,
            &pDataLock,
            reinterpret_cast<void **>(&pLocalData)
            );

        if (NO_ERROR == palError)
        {
            dwProcessId = pLocalData->dwProcessId;
            pDataLock->ReleaseLock(pThread, FALSE);
        }

        pobjProcess->ReleaseReference(pThread);
    }

PROCGetProcessIDFromHandleExit:

    return dwProcessId;
}

PAL_ERROR
CorUnix::InitializeProcessData(
    void
    )
{
    PAL_ERROR palError = NO_ERROR;
    bool fLockInitialized = FALSE;

    pGThreadList = NULL;
    g_dwThreadCount = 0;

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
Function
    InitializeProcessCommandLine

Abstract
    Initializes (or re-initializes) the saved command line and exe path.

Parameter
    lpwstrCmdLine
    lpwstrFullPath

Return
    PAL_ERROR

Notes
    This function takes ownership of lpwstrCmdLine, but not of lpwstrFullPath
--*/

PAL_ERROR
CorUnix::InitializeProcessCommandLine(
    char16_t* lpwstrCmdLine,
    char16_t* lpwstrFullPath
)
{
    PAL_ERROR palError = NO_ERROR;
    char16_t* initial_dir = NULL;

    //
    // Save the command line and initial directory
    //

    if (lpwstrFullPath)
    {
        char16_t* lpwstr = PAL_wcsrchr(lpwstrFullPath, '/');
        lpwstr[0] = '\0';
        size_t n = PAL_wcslen(lpwstrFullPath) + 1;

        size_t iLen = n;
        initial_dir = reinterpret_cast<char16_t*>(malloc(iLen*sizeof(char16_t)));
        if (NULL == initial_dir)
        {
            ERROR("malloc() failed! (initial_dir) \n");
            palError = ERROR_NOT_ENOUGH_MEMORY;
            goto exit;
        }

        if (wcscpy_s(initial_dir, iLen, lpwstrFullPath) != SAFECRT_SUCCESS)
        {
            ERROR("wcscpy_s failed!\n");
            free(initial_dir);
            palError = ERROR_INTERNAL_ERROR;
            goto exit;
        }

        lpwstr[0] = '/';

        free(g_lpwstrAppDir);
        g_lpwstrAppDir = initial_dir;
    }

    free(g_lpwstrCmdLine);
    g_lpwstrCmdLine = lpwstrCmdLine;

exit:
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
    CObjectAttributes oa;
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

    palError = g_pObjectManager->AllocateObject(
        pThread,
        &otProcess,
        &oa,
        &pobjProcess
        );

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
        ASSERT("Unable to access local data");
        goto CreateInitialProcessAndThreadObjectsExit;
    }

    pLocalData->dwProcessId = gPID;
    pLocalData->ps = PS_RUNNING;
    pDataLock->ReleaseLock(pThread, TRUE);

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
        ASSERT("Failure registering process object");
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
  PROCCleanupInitialProcess

Abstract
  Cleanup all the structures for the initial process.

Parameter
  void

Return
  void

--*/
void
PROCCleanupInitialProcess(void)
{
    CPalThread *pThread = InternalGetCurrentThread();

    InternalEnterCriticalSection(pThread, &g_csProcess);

    /* Free the application directory */
    free(g_lpwstrAppDir);

    /* Free the stored command line */
    free(g_lpwstrCmdLine);

    InternalLeaveCriticalSection(pThread, &g_csProcess);

    //
    // Object manager shutdown will handle freeing the underlying
    // thread and process data
    //

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
    g_dwThreadCount += 1;

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
        ASSERT("Thread list is empty.\n");
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
            g_dwThreadCount -= 1;
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
  PROCGetNumberOfThreads

Abstract
  Return the number of threads in the thread list.

Parameter
  void

Return
  the number of threads.
--*/
int32_t
CorUnix::PROCGetNumberOfThreads(
    void)
{
    return g_dwThreadCount;
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

/*++
Function:
  TerminateCurrentProcessNoExit

Abstract:
    Terminate current Process, but leave the caller alive

Parameters:
    BOOL bTerminateUnconditionally - If this is set, the PAL will exit as
    quickly as possible. In particular, it will not unload DLLs.

Return value :
    No return

Note:
  This function is used in ExitThread and TerminateProcess

--*/
void
CorUnix::TerminateCurrentProcessNoExit(BOOL bTerminateUnconditionally)
{
    BOOL locked;
    uint32_t old_terminator;

    old_terminator = InterlockedCompareExchange(&terminator, GetCurrentThreadId(), 0);

    if (0 != old_terminator && GetCurrentThreadId() != old_terminator)
    {
        /* another thread has already initiated the termination process. we
           could just block on the PALInitLock critical section, but then
           PROCSuspendOtherThreads would hang... so sleep forever here, we're
           terminating anyway

           Update: [TODO] PROCSuspendOtherThreads has been removed. Can this
           code be changed? */

        /* note that if *this* thread has already started the termination
           process, we want to proceed. the only way this can happen is if a
           call to DllMain (from ExitProcess) brought us here (because DllMain
           called ExitProcess, or TerminateProcess, or ExitThread);
           TerminateProcess won't call DllMain, so there's no danger to get
           caught in an infinite loop */
        WARN("termination already started from another thread; blocking.\n");
        poll(NULL, 0, INFTIM);
    }

    /* Try to lock the initialization count to prevent multiple threads from
       terminating/initializing the PAL simultaneously */

    /* note : it's also important to take this lock before the process lock,
       because Init/Shutdown take the init lock, and the functions they call
       may take the process lock. We must do it in the same order to avoid
       deadlocks */
    locked = PALInitLock();
    if(locked && PALIsInitialized())
    {
         PROCCleanupProcess(bTerminateUnconditionally);
     }
}

/*++
Function:
    PROCGetProcessStatus

Abstract:
    Retrieve process state information (state & exit code).

Parameters:
    uint32_t process_id : PID of process to retrieve state for
    PROCESS_STATE *state : state of process (starting, running, done)
    uint32_t *exit_code : exit code of process (from ExitProcess, etc.)

Return value :
    TRUE on success
--*/
PAL_ERROR
PROCGetProcessStatus(
    CPalThread *pThread,
    HANDLE hProcess,
    PROCESS_STATE *pps,
    uint32_t *pdwExitCode
    )
{
    PAL_ERROR palError = NO_ERROR;
    IPalObject *pobjProcess = NULL;
    IDataLock *pDataLock;
    CProcProcessLocalData *pLocalData;
    pid_t wait_retval;
    int status;

    //
    // First, check if we already know the status of this process. This will be
    // the case if this function has already been called for the same process.
    //

    palError = g_pObjectManager->ReferenceObjectByHandle(
        pThread,
        hProcess,
        &aotProcess,
        0,
        &pobjProcess
        );

    if (NO_ERROR != palError)
    {
        goto PROCGetProcessStatusExit;
    }

    palError = pobjProcess->GetProcessLocalData(
        pThread,
        WriteLock,
        &pDataLock,
        reinterpret_cast<void **>(&pLocalData)
        );

    if (PS_DONE == pLocalData->ps)
    {
        TRACE("We already called waitpid() on process ID %#x; process has "
              "terminated, exit code is %d\n",
              pLocalData->dwProcessId, pLocalData->dwExitCode);

        *pps = pLocalData->ps;
        *pdwExitCode = pLocalData->dwExitCode;

        pDataLock->ReleaseLock(pThread, FALSE);

        goto PROCGetProcessStatusExit;
    }

    /* By using waitpid(), we can even retrieve the exit code of a non-PAL
       process. However, note that waitpid() can only provide the low 8 bits
       of the exit code. This is all that is required for the PAL spec. */
    TRACE("Looking for status of process; trying wait()");

    while(1)
    {
        /* try to get state of process, using non-blocking call */
        wait_retval = waitpid(pLocalData->dwProcessId, &status, WNOHANG);

        if ( wait_retval == (pid_t) pLocalData->dwProcessId )
        {
            /* success; get the exit code */
            if ( WIFEXITED( status ) )
            {
                *pdwExitCode = WEXITSTATUS(status);
                TRACE("Exit code was %d\n", *pdwExitCode);
            }
            else
            {
                WARN("process terminated without exiting; can't get exit "
                     "code. faking it.\n");
                *pdwExitCode = EXIT_FAILURE;
            }
            *pps = PS_DONE;
        }
        else if (0 == wait_retval)
        {
            // The process is still running.
            TRACE("Process %#x is still active.\n", pLocalData->dwProcessId);
            *pps = PS_RUNNING;
            *pdwExitCode = 0;
        }
        else if (-1 == wait_retval)
        {
            // This might happen if waitpid() had already been called, but
            // this shouldn't happen - we call waitpid once, store the
            // result, and use that afterwards.
            // One legitimate cause of failure is EINTR; if this happens we
            // have to try again. A second legitimate cause is ECHILD, which
            // happens if we're trying to retrieve the status of a currently-
            // running process that isn't a child of this process.
            if (EINTR == errno)
            {
                TRACE("waitpid() failed with EINTR; re-waiting");
                continue;
            }
            else if (ECHILD == errno)
            {
                TRACE("waitpid() failed with ECHILD; calling kill instead");
                if (kill(pLocalData->dwProcessId, 0) != 0)
                {
                    if(ESRCH == errno)
                    {
                        WARN("kill() failed with ESRCH, i.e. target "
                             "process exited and it wasn't a child, "
                             "so can't get the exit code, assuming  "
                             "it was 0.\n");
                        *pdwExitCode = 0;
                    }
                    else
                    {
                        ERROR("kill(pid, 0) failed; errno is %d (%s)\n",
                              errno, strerror(errno));
                        *pdwExitCode = EXIT_FAILURE;
                    }
                    *pps = PS_DONE;
                }
                else
                {
                    *pps = PS_RUNNING;
                    *pdwExitCode = 0;
                }
            }
            else
            {
                // Ignoring unexpected waitpid errno and assuming that
                // the process is still running
                ERROR("waitpid(pid=%u) failed with unexpected errno=%d (%s)\n",
                      pLocalData->dwProcessId, errno, strerror(errno));
                *pps = PS_RUNNING;
                *pdwExitCode = 0;
            }
        }
        else
        {
            ASSERT("waitpid returned unexpected value %d\n",wait_retval);
            *pdwExitCode = EXIT_FAILURE;
            *pps = PS_DONE;
        }
        // Break out of the loop in all cases except EINTR.
        break;
    }

    // Save the exit code for future reference (waitpid will only work once).
    if(PS_DONE == *pps)
    {
        pLocalData->ps = PS_DONE;
        pLocalData->dwExitCode = *pdwExitCode;
    }

    TRACE( "State of process 0x%08x : %d (exit code %d)\n",
           pLocalData->dwProcessId, *pps, *pdwExitCode );

    pDataLock->ReleaseLock(pThread, TRUE);

PROCGetProcessStatusExit:

    if (NULL != pobjProcess)
    {
        pobjProcess->ReleaseReference(pThread);
    }

    return palError;
}

#ifdef _DEBUG
void PROCDumpThreadList()
{
    CPalThread *pThread;

    PROCProcessLock();

    TRACE ("Threads:{\n");

    pThread = pGThreadList;
    while (NULL != pThread)
    {
        TRACE ("    {pThr=0x%p tid=%#x lwpid=%#x state=%d finsusp=%d}\n",
               pThread, (int)pThread->GetThreadId(), (int)pThread->GetLwpId(),
               (int)pThread->synchronizationInfo.GetThreadState(),
               (int)pThread->suspensionInfo.GetSuspendedForShutdown());

        pThread = pThread->GetNext();
    }
    TRACE ("Threads:}\n");

    PROCProcessUnlock();
}
#endif

/* Internal function definitions **********************************************/
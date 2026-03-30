//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
//

/*++



Module Name:

    module.c

Abstract:

    Implementation of module related functions in the Win32 API



--*/

#include "pal/thread.hpp"
#include <new>
#include "pal/file.hpp"
#include "pal/palinternal.h"
#include "pal/dbgmsg.h"
#include "pal/module.h"
#include "pal/cs.hpp"
#include "pal/process.h"
#include "pal/file.h"
#include "pal/utils.h"
#include "pal/init.h"
#include "pal/modulename.h"
#include "pal/misc.h"
#include "pal/virtual.h"
#include "pal/map.hpp"
#include "pal/stackstring.hpp"

#include <sys/param.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include <dlfcn.h>
#include <alloca.h>

#ifdef __APPLE__
#include <mach-o/dyld.h>
#include <mach-o/loader.h>
#endif // __APPLE__

#include <sys/types.h>
#include <sys/mman.h>

#if defined(__LINUX__)
#include <gnu/lib-names.h>
#endif

using namespace CorUnix;

SET_DEFAULT_DEBUG_CHANNEL(LOADER);

// In safemath.h, Template SafeInt uses macro _ASSERTE, which need to use variable
// defdbgchan defined by SET_DEFAULT_DEBUG_CHANNEL. Therefore, the include statement
// should be placed after the SET_DEFAULT_DEBUG_CHANNEL(LOADER)
#include <safemath.h>

/* macro definitions **********************************************************/

/* Which path should FindLibrary search? */
#if defined(__APPLE__)
#define LIBSEARCHPATH "DYLD_LIBRARY_PATH"
#else
#define LIBSEARCHPATH "LD_LIBRARY_PATH"
#endif

#define LIBC_NAME_WITHOUT_EXTENSION "libc"

/* static variables ***********************************************************/

/* critical section that regulates access to the module list */
CRITICAL_SECTION module_critsec;

/* always the first, in the in-load-order list */
MODSTRUCT exe_module;

char * g_szCoreCLRPath = nullptr;
size_t g_cbszCoreCLRPath = MAX_LONGPATH * sizeof(char);

int MaxWCharToAcpLength = 3;

/* static function declarations ***********************************************/

/* API function definitions ***************************************************/

/* Internal PAL functions *****************************************************/

/*++
Function :
    LOADInitializeModules

    Initialize the process-wide list of modules

Parameters :
    None

Return value :
    TRUE  if initialization succeedded
    FALSE otherwise

--*/
extern "C"
BOOL LOADInitializeModules()
{
    _ASSERTE(exe_module.prev == nullptr);

    InternalInitializeCriticalSection(&module_critsec);

    // Initialize module for main executable
    TRACE("Initializing module for main executable\n");

    exe_module.self = (HMODULE)&exe_module;
    exe_module.dl_handle = dlopen(nullptr, RTLD_LAZY);
    if (exe_module.dl_handle == nullptr)
    {
        ERROR("Executable module will be broken : dlopen(nullptr) failed dlerror message is \"%s\" \n", dlerror());
        return FALSE;
    }
    exe_module.next = &exe_module;
    exe_module.prev = &exe_module;
    exe_module.hinstance = nullptr;
    exe_module.threadLibCalls = TRUE;
    return TRUE;
}

/*++
Function :
    LOADCallDllMain

    Call DllMain for all modules (that have one) with the given "fwReason"

Parameters :
    uint32_t dwReason : parameter to pass down to DllMain, one of DLL_PROCESS_ATTACH, DLL_PROCESS_DETACH,
        DLL_THREAD_ATTACH, DLL_THREAD_DETACH

    void * lpReserved : parameter to pass down to DllMain
        If dwReason is DLL_PROCESS_ATTACH, lpvReserved is NULL for dynamic loads and non-NULL for static loads.
        If dwReason is DLL_PROCESS_DETACH, lpvReserved is NULL if DllMain has been called by using FreeLibrary
            and non-NULL if DllMain has been called during process termination.

(no return value)

Notes :
    This is used to send DLL_THREAD_*TACH messages to modules
--*/
extern "C"
void LOADCallDllMain(uint32_t dwReason, void * lpReserved)
{
    MODSTRUCT *module = nullptr;
    BOOL InLoadOrder = TRUE; /* true if in load order, false for reverse */
    CPalThread *pThread;

    pThread = InternalGetCurrentThread();
    if (UserCreatedThread != pThread->GetThreadType())
    {
        return;
    }

    /* Validate dwReason */
    switch(dwReason)
    {
    case DLL_PROCESS_ATTACH:
        ASSERT("got called with DLL_PROCESS_ATTACH parameter! Why?\n");
        break;
    case DLL_PROCESS_DETACH:
        ASSERT("got called with DLL_PROCESS_DETACH parameter! Why?\n");
        InLoadOrder = FALSE;
        break;
    case DLL_THREAD_ATTACH:
        TRACE("Calling DllMain(DLL_THREAD_ATTACH) on all known modules.\n");
        break;
    case DLL_THREAD_DETACH:
        TRACE("Calling DllMain(DLL_THREAD_DETACH) on all known modules.\n");
        InLoadOrder = FALSE;
        break;
    default:
        ASSERT("LOADCallDllMain called with unknown parameter %d!\n", dwReason);
        return;
    }

    module = &exe_module;

    do
    {
        if (!InLoadOrder)
            module = module->prev;

        if (InLoadOrder)
            module = module->next;

    } while (module != &exe_module);
}
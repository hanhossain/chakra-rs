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

/* get the full name of a module if available, and the short name otherwise*/
#define MODNAME(x) ((x)->lib_name)

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
MODSTRUCT *pal_module = nullptr;

char * g_szCoreCLRPath = nullptr;
size_t g_cbszCoreCLRPath = MAX_LONGPATH * sizeof(char);

int MaxWCharToAcpLength = 3;

/* static function declarations ***********************************************/

static BOOL LOADValidateModule(MODSTRUCT *module);
static char16_t* LOADGetModuleFileName(MODSTRUCT *module);
static BOOL LOADFreeLibrary(MODSTRUCT *module, BOOL fCallDllMain);
static BOOL LOADCallDllMainSafe(MODSTRUCT *module, uint32_t dwReason, void * lpReserved);

/* API function definitions ***************************************************/

/*++
Function:
  FreeLibrary

See MSDN doc.
--*/
BOOL
FreeLibrary(
      HMODULE hLibModule)
{
    BOOL retval = FALSE;

    ENTRY("FreeLibrary (hLibModule=%p)\n", hLibModule);
    retval = LOADFreeLibrary((MODSTRUCT *)hLibModule, TRUE /* fCallDllMain */);

    LOGEXIT("FreeLibrary returns BOOL %d\n", retval);
    return retval;
}

/*++
Function:
  FreeLibraryAndExitThread

See MSDN doc.

--*/
void
FreeLibraryAndExitThread(
     HMODULE hLibModule,
     uint32_t dwExitCode)
{
    ENTRY("FreeLibraryAndExitThread()\n");
    FreeLibrary(hLibModule);
    ExitThread(dwExitCode);
    LOGEXIT("FreeLibraryAndExitThread\n");
}

/*++
Function:
  GetModuleFileNameW

See MSDN doc.

Notes :
    because of limitations in the dlopen() mechanism, this will only return the
    full path name if a relative or absolute path was given to LoadLibrary, or
    if the module was used in a GetProcAddress call. otherwise, this will return
    the short name as given to LoadLibrary. The exception is if hModule is
    NULL : in this case, the full path of the executable is always returned.
--*/
uint32_t
GetModuleFileNameW(
     HMODULE hModule,
     char16_t* lpFileName,
     uint32_t nSize)
{
    int32_t name_length;
    uint32_t retval = 0;
    char16_t* wide_name = nullptr;

    ENTRY("GetModuleFileNameW (hModule=%p, lpFileName=%p, nSize=%u)\n",
          hModule, lpFileName, nSize);

    LockModuleList();

    wcscpy_s(lpFileName, nSize, W(""));

    if (hModule && !LOADValidateModule((MODSTRUCT *)hModule))
    {
        TRACE("Can't find name for invalid module handle %p\n", hModule);
        SetLastError(ERROR_INVALID_HANDLE);
        goto done;
    }
    wide_name = LOADGetModuleFileName((MODSTRUCT *)hModule);

    if (!wide_name)
    {
        TRACE("Can't find name for valid module handle %p\n", hModule);
        SetLastError(ERROR_INTERNAL_ERROR);
        goto done;
    }

    /* Copy module name into supplied buffer */

    name_length = lstrlenW(wide_name);
    if (name_length >= (int32_t)nSize)
    {
        TRACE("Buffer too small (%u) to copy module's file name (%u).\n", nSize, name_length);
        SetLastError(ERROR_INSUFFICIENT_BUFFER);
        goto done;
    }

    wcscpy_s(lpFileName, nSize, wide_name);

    TRACE("file name of module %p is %S\n", hModule, lpFileName);
    retval = name_length;
done:
    UnlockModuleList();
    LOGEXIT("GetModuleFileNameW returns DWORD %u\n", retval);
    return retval;
}

HMODULE
GetModuleHandleW(
      const char16_t* lpModuleName)
{
    if (lpModuleName)
    {
        // UNIXTODO: Implement this!
        ASSERT("Needs Implementation!!!");
        return nullptr;
    }

    return (HMODULE)&exe_module;
}

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
    exe_module.lib_name = nullptr;
    exe_module.refcount = -1;
    exe_module.next = &exe_module;
    exe_module.prev = &exe_module;
    exe_module.pDllMain = nullptr;
    exe_module.hinstance = nullptr;
    exe_module.threadLibCalls = TRUE;
    return TRUE;
}

/*++
Function :
    LOADSetExeName

    Set the exe name path

Parameters :
    char16_t* man exe path and name

Return value :
    TRUE  if initialization succeedded
    FALSE otherwise

--*/
extern "C"
BOOL LOADSetExeName(char16_t* name)
{
#if RETURNS_NEW_HANDLES_ON_REPEAT_DLOPEN
    char* pszExeName = nullptr;
#endif
    BOOL result = FALSE;

    LockModuleList();

    // Save the exe path in the exe module struct
    free(exe_module.lib_name);
    exe_module.lib_name = name;

    // For platforms where we can't trust the handle to be constant, we need to
    // store the inode/device pairs for the modules we just initialized.
#if RETURNS_NEW_HANDLES_ON_REPEAT_DLOPEN
    {
        struct stat stat_buf;
        pszExeName = UTIL_WCToMB_Alloc(name, -1);
        if (nullptr == pszExeName)
        {
            ERROR("WCToMB failure, unable to get full name of exe\n");
            goto exit;
        }
        if (-1 == stat(pszExeName, &stat_buf))
        {
            SetLastError(ERROR_MOD_NOT_FOUND);
            goto exit;
        }
        TRACE("Executable has inode %d and device %d\n", stat_buf.st_ino, stat_buf.st_dev);

        exe_module.inode = stat_buf.st_ino;
        exe_module.device = stat_buf.st_dev;
    }
#endif
    result = TRUE;

#if RETURNS_NEW_HANDLES_ON_REPEAT_DLOPEN
exit:
    if (pszExeName)
    {
        free(pszExeName);
    }
#endif
    UnlockModuleList();
    return result;
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

    LockModuleList();

    module = &exe_module;

    do
    {
        if (!InLoadOrder)
            module = module->prev;

        if (module->threadLibCalls)
        {
            if (module->pDllMain)
            {
                LOADCallDllMainSafe(module, dwReason, lpReserved);
            }
        }

        if (InLoadOrder)
            module = module->next;

    } while (module != &exe_module);

    UnlockModuleList();
}

/*++
Function:
  LOADFreeLibrary

Parameters:
  MODSTRUCT * module - module to free
  BOOL fCallDllMain - if TRUE, call the DllMain function

Returns:
  TRUE if successful

--*/
static BOOL LOADFreeLibrary(MODSTRUCT *module, BOOL fCallDllMain)
{
    BOOL retval = FALSE;

    LockModuleList();

    if (terminator)
    {
        /* PAL shutdown is in progress - ignore FreeLibrary calls */
        retval = TRUE;
        goto done;
    }

    if (!LOADValidateModule(module))
    {
        TRACE("Can't free invalid module %p\n", module);
        SetLastError(ERROR_INVALID_HANDLE);
        goto done;
    }

    if (module->refcount == -1)
    {
        /* special module - never released */
        retval = TRUE;
        goto done;
    }

    module->refcount--;
    TRACE("Reference count for module %p (named %S) decreases to %d\n",
            module, MODNAME(module), module->refcount);

    if (module->refcount != 0)
    {
        retval = TRUE;
        goto done;
    }

    /* Releasing the last reference : call dlclose(), remove module from the
       process-wide module list */

    TRACE("Reference count for module %p (named %S) now 0; destroying module structure\n",
        module, MODNAME(module));

    /* unlink the module structure from the list */
    module->prev->next = module->next;
    module->next->prev = module->prev;

    /* remove the circular reference so that LOADValidateModule will fail */
    module->self = nullptr;

    /* Call DllMain if the module contains one */
    if (fCallDllMain && module->pDllMain)
    {
        LOADCallDllMainSafe(module, DLL_PROCESS_DETACH, nullptr);
    }

    if (module->hinstance)
    {
        PUNREGISTER_MODULE unregisterModule = (PUNREGISTER_MODULE)dlsym(module->dl_handle, "PAL_UnregisterModule");
        if (unregisterModule != nullptr)
        {
             unregisterModule(module->hinstance);
        }
        module->hinstance = nullptr;
    }

    if (module->dl_handle && 0 != dlclose(module->dl_handle))
    {
        /* report dlclose() failure, but proceed anyway. */
        WARN("dlclose() call failed! error message is \"%s\"\n", dlerror());
    }

    /* release all memory */
    free(module->lib_name);
    free(module);

    retval = TRUE;

done:
    UnlockModuleList();
    return retval;
}

/*++
Function :
    LOADCallDllMainSafe

    Exception-safe call to DllMain.

Parameters :
    MODSTRUCT *module : module whose DllMain must be called

    uint32_t dwReason : parameter to pass down to DllMain, one of DLL_PROCESS_ATTACH, DLL_PROCESS_DETACH,
        DLL_THREAD_ATTACH, DLL_THREAD_DETACH

    void * lpvReserved : parameter to pass down to DllMain,
        If dwReason is DLL_PROCESS_ATTACH, lpvReserved is NULL for dynamic loads and non-NULL for static loads.
        If dwReason is DLL_PROCESS_DETACH, lpvReserved is NULL if DllMain has been called by using FreeLibrary
            and non-NULL if DllMain has been called during process termination.

Returns:
    BOOL : DllMain's return value
*/
static BOOL LOADCallDllMainSafe(MODSTRUCT *module, uint32_t dwReason, void * lpReserved)
{
#if _ENABLE_DEBUG_MESSAGES_
    /* reset ENTRY nesting level back to zero while inside the callback... */
    int old_level = DBG_change_entrylevel(0);
#endif /* _ENABLE_DEBUG_MESSAGES_ */

    struct Param
    {
        MODSTRUCT *module;
        uint32_t dwReason;
        void * lpReserved;
        BOOL ret;
    } param;
    param.module = module;
    param.dwReason = dwReason;
    param.lpReserved = lpReserved;
    param.ret = FALSE;

    {
        TRACE("Calling DllMain (%p) for module %S\n",
              param.module->pDllMain,
              param.module->lib_name ? param.module->lib_name : W16_NULLSTRING);

        {
            param.ret = param.module->pDllMain(param.module->hinstance, param.dwReason, param.lpReserved);
        }
    }

#if _ENABLE_DEBUG_MESSAGES_
    /* ...and set nesting level back to what it was */
    DBG_change_entrylevel(old_level);
#endif /* _ENABLE_DEBUG_MESSAGES_ */

    return param.ret;
}

/*++
Function :
    LOADValidateModule

    Check whether the given MODSTRUCT pointer is valid

Parameters :
    MODSTRUCT *module : module to check

Return value :
    TRUE if module is valid, FALSE otherwise

NOTE :
    The module lock MUST be owned.

--*/
static BOOL LOADValidateModule(MODSTRUCT *module)
{
    MODSTRUCT *modlist_enum = &exe_module;

    /* enumerate through the list of modules to make sure the given handle is
       really a module (HMODULEs are actually MODSTRUCT pointers) */
    do
    {
        if (module == modlist_enum)
        {
            /* found it; check its integrity to be on the safe side */
            if (module->self != module)
            {
                ERROR("Found corrupt module %p!\n",module);
                return FALSE;
            }
            TRACE("Module %p is valid (name : %S)\n", module, MODNAME(module));
            return TRUE;
        }
        modlist_enum = modlist_enum->next;
    }
    while (modlist_enum != &exe_module);

    TRACE("Module %p is NOT valid.\n", module);
    return FALSE;
}

/*++
Function :
    LOADGetModuleFileName [internal]

    Retrieve the module's full path if it is known, the short name given to
    LoadLibrary otherwise.

Parameters :
    MODSTRUCT *module : module to check

Return value :
    pointer to internal buffer with name of module (Unicode)

Notes :
    this function assumes that the module critical section is held, and that
    the module has already been validated.
--*/
static char16_t* LOADGetModuleFileName(MODSTRUCT *module)
{
    char16_t* module_name;
    /* special case : if module is NULL, we want the name of the executable */
    if (!module)
    {
        module_name = exe_module.lib_name;
        TRACE("Returning name of main executable\n");
        return module_name;
    }

    /* return "real" name of module if it is known. we have this if LoadLibrary
       was given an absolute or relative path; we can also determine it at the
       first GetProcAddress call. */
    TRACE("Returning full path name of module\n");
    return module->lib_name;
}

/*++
Function:
  LockModuleList

Abstract
  Enter the critical section associated to the module list

Parameter
  void

Return
  void
--*/
extern "C"
void LockModuleList()
{
    CPalThread * pThread =
        (PALIsThreadDataInitialized() ? InternalGetCurrentThread() : nullptr);

    InternalEnterCriticalSection(pThread, &module_critsec);
}

/*++
Function:
  UnlockModuleList

Abstract
  Leave the critical section associated to the module list

Parameter
  void

Return
  void
--*/
extern "C"
void UnlockModuleList()
{
    CPalThread * pThread =
        (PALIsThreadDataInitialized() ? InternalGetCurrentThread() : nullptr);

    InternalLeaveCriticalSection(pThread, &module_critsec);
}

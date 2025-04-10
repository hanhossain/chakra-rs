//-------------------------------------------------------------------------------------------------------
// ChakraCore/Pal
// Contains portions (c) copyright Microsoft, portions copyright (c) the .NET Foundation and Contributors
// and edits (c) copyright the ChakraCore Contributors.
// See THIRD-PARTY-NOTICES.txt in the project root for .NET Foundation license
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

/*++



Module Name:

    sysinfo.c

Abstract:

    Implements GetSystemInfo.

Revision History:



--*/

#include "pal/palinternal.h"

#include <sched.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#if HAVE_SYSCONF || defined(__ANDROID__)
// <unistd.h> already included
#elif HAVE_SYSCTL
#include <sys/sysctl.h>
#else
#error Either sysctl or sysconf is required for GetSystemInfo.
#endif

#include <sys/param.h>
#if HAVE_SYS_VMPARAM_H
#include <sys/vmparam.h>
#endif  // HAVE_SYS_VMPARAM_H

#if HAVE_MACH_VM_TYPES_H
#include <mach/vm_types.h>
#endif // HAVE_MACH_VM_TYPES_H

#if HAVE_MACH_VM_PARAM_H
#include <mach/vm_param.h>
#endif  // HAVE_MACH_VM_PARAM_H

#if defined(__APPLE__)
#include <mach/vm_statistics.h>
#include <mach/mach_types.h>
#include <mach/mach_init.h>
#include <mach/mach_host.h>
#endif // defined(__APPLE__)

// On some platforms sys/user.h ends up defining _DEBUG; if so
// remove the definition before including the header and put
// back our definition afterwards
#if USER_H_DEFINES_DEBUG
#define OLD_DEBUG _DEBUG
#undef _DEBUG
#endif
#include <sys/user.h>
#if USER_H_DEFINES_DEBUG
#undef _DEBUG
#define _DEBUG OLD_DEBUG
#undef OLD_DEBUG
#endif

#include "pal/dbgmsg.h"


SET_DEFAULT_DEBUG_CHANNEL(MISC);

#if defined(__hppa__) || ( defined (_IA64_) && defined (_HPUX_) )
#include <sys/pstat.h>
#include <sys/vmparam.h>
#endif

#ifndef __APPLE__
#if HAVE_SYSCONF && HAVE__SC_AVPHYS_PAGES
#define SYSCONF_PAGES _SC_AVPHYS_PAGES
#elif HAVE_SYSCONF && HAVE__SC_PHYS_PAGES
#define SYSCONF_PAGES _SC_PHYS_PAGES
#elif !defined(__ANDROID__)
#error Dont know how to get page-size on this architecture!
#endif
#endif // __APPLE__

#ifdef __LINUX__
// There is no reasonable way to get the max. value for the VAS on
// Linux, so just hardcode the ABI values for 64 and 32bits.
#ifdef LINUX64
// The hardware limit for x86-64 CPUs is 256TB, but the practical
// limit at the moment for Linux kernels is 128TB.  See for example:
// https://access.redhat.com/articles/rhel-limits
#define MAX_PROCESS_VA_SPACE_LINUX (128ull * 1024 * 1024 * 1024 * 1024)
#else
// This is unsupported at the moment, but the x32 ABI has a 4GB limit.
#define MAX_PROCESS_VA_SPACE_LINUX (4ull * 1024 * 1024 * 1024)
#endif
#endif // __LINUX__

/*++
Function:
  GetSystemInfo

GetSystemInfo

The GetSystemInfo function returns information about the current system.

Parameters

lpSystemInfo
       [out] Pointer to a SYSTEM_INFO structure that receives the information.

Return Values

This function does not return a value.

Note:
  fields returned by this function are:
    dwNumberOfProcessors
    dwPageSize
Others are set to zero.

--*/

int GetCurrentThreadStackLimits(size_t* lowLimit, size_t* highLimit);

void
GetSystemInfo(
           LPSYSTEM_INFO lpSystemInfo)
{
    int nrcpus = 0;
    long pagesize;

    PERF_ENTRY(GetSystemInfo);
    ENTRY("GetSystemInfo (lpSystemInfo=%p)\n", lpSystemInfo);

    pagesize = getpagesize();

    lpSystemInfo->wProcessorArchitecture_PAL_Undefined = 0;
    lpSystemInfo->wReserved_PAL_Undefined = 0;
    lpSystemInfo->dwPageSize = pagesize;
    lpSystemInfo->dwActiveProcessorMask_PAL_Undefined = 0;

#if HAVE_SYSCONF
    nrcpus = sysconf(_SC_NPROCESSORS_ONLN);
    if (nrcpus < 1)
    {
        ASSERT("sysconf failed for _SC_NPROCESSORS_ONLN (%d)\n", errno);
    }
#elif HAVE_SYSCTL
    int rc;
    size_t sz;
    int mib[2];

    sz = sizeof(nrcpus);
    mib[0] = CTL_HW;
    mib[1] = HW_NCPU;
    rc = sysctl(mib, 2, &nrcpus, &sz, NULL, 0);
    if (rc != 0)
    {
        ASSERT("sysctl failed for HW_NCPU (%d)\n", errno);
    }
#endif // HAVE_SYSCONF

    TRACE("dwNumberOfProcessors=%d\n", nrcpus);
    lpSystemInfo->dwNumberOfProcessors = nrcpus;

// TODO: (BSD) is it a good idea to take maxOf(GetCurrentThreadStackLimits, USRSTACKXX) ?
#ifdef __APPLE__ // defined(USRSTACK64) || defined(USRSTACK)
size_t lowl, highl;
GetCurrentThreadStackLimits(&lowl, &highl);
#ifndef PAL_MAX
#define PAL_MAX(a, b) ((a > b) ? (a) : (b))
#endif
#endif

#ifdef VM_MAXUSER_ADDRESS
    lpSystemInfo->lpMaximumApplicationAddress = (void *) VM_MAXUSER_ADDRESS;
#elif defined(__LINUX__)
    lpSystemInfo->lpMaximumApplicationAddress = (void *) MAX_PROCESS_VA_SPACE_LINUX;
#elif defined(USERLIMIT)
    lpSystemInfo->lpMaximumApplicationAddress = (void *) USERLIMIT;
#elif defined(_M_ARM64)
    lpSystemInfo->lpMaximumApplicationAddress = (void *) (1ull << 47);
#elif defined(USRSTACK64)
    lpSystemInfo->lpMaximumApplicationAddress = (void *) PAL_MAX(highl, USRSTACK64);
#else
#error The maximum application address is not known on this platform.
#endif

    lpSystemInfo->lpMinimumApplicationAddress = (void *) pagesize;

    lpSystemInfo->dwProcessorType_PAL_Undefined = 0;

    lpSystemInfo->dwAllocationGranularity = pagesize;

    lpSystemInfo->wProcessorLevel_PAL_Undefined = 0;
    lpSystemInfo->wProcessorRevision_PAL_Undefined = 0;

    LOGEXIT("GetSystemInfo returns VOID\n");
    PERF_EXIT(GetSystemInfo);
}

/*++
Function:
  GlobalMemoryStatusEx

GlobalMemoryStatusEx

Retrieves information about the system's current usage of both physical and virtual memory.

Return Values

This function returns a BOOL to indicate its success status.

--*/
BOOL
GlobalMemoryStatusEx(
              LPMEMORYSTATUSEX lpBuffer)
{

    PERF_ENTRY(GlobalMemoryStatusEx);
    ENTRY("GlobalMemoryStatusEx (lpBuffer=%p)\n", lpBuffer);

    lpBuffer->dwMemoryLoad = 0;
    lpBuffer->ullTotalPhys = 0;
    lpBuffer->ullAvailPhys = 0;
    lpBuffer->ullTotalPageFile = 0;
    lpBuffer->ullAvailPageFile = 0;
    lpBuffer->ullTotalVirtual = 0;
    lpBuffer->ullAvailVirtual = 0;
    lpBuffer->ullAvailExtendedVirtual = 0;

    BOOL fRetVal = FALSE;

    // Get the physical memory size
#if HAVE_SYSCONF && HAVE__SC_PHYS_PAGES
    int64_t physical_memory;

    // Get the Physical memory size
    physical_memory = sysconf( _SC_PHYS_PAGES ) * sysconf( _SC_PAGE_SIZE );
    lpBuffer->ullTotalPhys = (DWORDLONG)physical_memory;
    fRetVal = TRUE;
#elif HAVE_SYSCTL
    int mib[2];
    int64_t physical_memory;
    size_t length;

    // Get the Physical memory size
    mib[0] = CTL_HW;
    mib[1] = HW_MEMSIZE;
    length = sizeof(int64_t);
    int rc = sysctl(mib, 2, &physical_memory, &length, NULL, 0);
    if (rc != 0)
    {
        ASSERT("sysctl failed for HW_MEMSIZE (%d)\n", errno);
    }
    else
    {
        lpBuffer->ullTotalPhys = (DWORDLONG)physical_memory;
        fRetVal = TRUE;
    }
#else // HAVE_SYSINFO
    // TODO: implement getting memory details via sysinfo. On Linux, it provides swap file details that
    // we can use to fill in the xxxPageFile members.

#endif // HAVE_SYSCONF

    // Get the physical memory in use - from it, we can get the physical memory available.
    // We do this only when we have the total physical memory available.
    if (lpBuffer->ullTotalPhys > 0)
    {
#if defined(__ANDROID__)
        lpBuffer->ullAvailPhys = sysconf(_SC_AVPHYS_PAGES) * sysconf( _SC_PAGE_SIZE );
        int64_t used_memory = lpBuffer->ullTotalPhys - lpBuffer->ullAvailPhys;
        lpBuffer->dwMemoryLoad = (uint32_t)((used_memory * 100) / lpBuffer->ullTotalPhys);
#elif defined(__LINUX__)
        lpBuffer->ullAvailPhys = sysconf(SYSCONF_PAGES) * sysconf(_SC_PAGE_SIZE);
        int64_t used_memory = lpBuffer->ullTotalPhys - lpBuffer->ullAvailPhys;
        lpBuffer->dwMemoryLoad = (uint32_t)((used_memory * 100) / lpBuffer->ullTotalPhys);
#elif defined(__APPLE__)
        vm_size_t page_size;
        mach_port_t mach_port;
        mach_msg_type_number_t count;
        vm_statistics_data_t vm_stats;
        mach_port = mach_host_self();
        count = sizeof(vm_stats) / sizeof(natural_t);
        if (KERN_SUCCESS == host_page_size(mach_port, &page_size))
        {
            if (KERN_SUCCESS == host_statistics(mach_port, HOST_VM_INFO, (host_info_t)&vm_stats, &count))
            {
                lpBuffer->ullAvailPhys = (int64_t)vm_stats.free_count * (int64_t)page_size;
                int64_t used_memory = ((int64_t)vm_stats.active_count + (int64_t)vm_stats.inactive_count + (int64_t)vm_stats.wire_count) *  (int64_t)page_size;
                lpBuffer->dwMemoryLoad = (uint32_t)((used_memory * 100) / lpBuffer->ullTotalPhys);
            }
        }
        mach_port_deallocate(mach_task_self(), mach_port);
#endif // __APPLE__
    }

#ifdef __LINUX__
    lpBuffer->ullTotalVirtual = MAX_PROCESS_VA_SPACE_LINUX;
#else
    // xplat-todo: for all the other unices just use 128TB for now.
    static const UINT64 _128TB = (1ull << 47);
    lpBuffer->ullTotalVirtual = _128TB;
#endif
    lpBuffer->ullAvailVirtual = lpBuffer->ullAvailPhys;

    LOGEXIT("GlobalMemoryStatusEx returns %d\n", fRetVal);
    PERF_EXIT(GlobalMemoryStatusEx);

    return fRetVal;
}

uint32_t
GetCurrentProcessorNumber()
{
#if HAVE_SCHED_GETCPU
    return sched_getcpu();
#else //HAVE_SCHED_GETCPU
    return -1;
#endif //HAVE_SCHED_GETCPU
}

BOOL
PAL_HasGetCurrentProcessorNumber()
{
    return HAVE_SCHED_GETCPU;
}

uint32_t
PAL_GetLogicalCpuCountFromOS()
{
    uint32_t numLogicalCores = 0;

#if HAVE_SYSCONF
    numLogicalCores = sysconf(_SC_NPROCESSORS_ONLN);
#endif

    return numLogicalCores;
}

size_t
PAL_GetLogicalProcessorCacheSizeFromOS()
{
    size_t cacheSize = 0;

#if HAVE_SYSCONF && defined(__LINUX__) && !defined(__ANDROID__)
    cacheSize = max(cacheSize, sysconf(_SC_LEVEL1_DCACHE_SIZE));
    cacheSize = max(cacheSize, sysconf(_SC_LEVEL1_ICACHE_SIZE));
    cacheSize = max(cacheSize, sysconf(_SC_LEVEL2_CACHE_SIZE));
    cacheSize = max(cacheSize, sysconf(_SC_LEVEL3_CACHE_SIZE));
    cacheSize = max(cacheSize, sysconf(_SC_LEVEL4_CACHE_SIZE));
#endif

    return cacheSize;
}

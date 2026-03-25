//-------------------------------------------------------------------------------------------------------
// ChakraCore/Pal
// Contains portions (c) copyright Microsoft, portions copyright (c) the .NET Foundation and Contributors
// and edits (c) copyright the ChakraCore Contributors.
// See THIRD-PARTY-NOTICES.txt in the project root for .NET Foundation license
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

/*++



Module Name:

    time.c

Abstract:

    Implementation of time related WIN API functions.



--*/

#include "pal/palinternal.h"
#include "pal/dbgmsg.h"
#include "pal/misc.h"

#include <time.h>
#include <sys/time.h>
#include <errno.h>
#include <string.h>
#include <sched.h>

#if defined(__APPLE__)
#include <mach/mach_time.h>
static mach_timebase_info_data_t s_TimebaseInfo;
#endif

using namespace CorUnix;

SET_DEFAULT_DEBUG_CHANNEL(MISC);

/*++
Function :
TIMEInitialize

Initialize all Time-related stuff related

(no parameters)

Return value :
TRUE  if Time support initialization succeeded
FALSE otherwise
--*/
BOOL TIMEInitialize(void)
{
#if defined(__APPLE__)
    kern_return_t machRet;
    if ((machRet = mach_timebase_info(&s_TimebaseInfo)) != KERN_SUCCESS)
    {
        ASSERT("mach_timebase_info() failed: %s\n", mach_error_string(machRet));
        return FALSE;
    }
#endif

    return TRUE;
}


/*++
Function:
  GetSystemTime

The GetSystemTime function retrieves the current system date and
time. The system time is expressed in Coordinated Universal Time
(UTC).

Parameters

lpSystemTime
       [out] Pointer to a SYSTEMTIME structure to receive the current system date and time.

Return Values

This function does not return a value.

--*/
void
GetSystemTime(
           LPSYSTEMTIME lpSystemTime)
{
    time_t tt;
    struct tm ut;
    struct tm *utPtr;
    struct timeval timeval;
    int timeofday_retval;

    ENTRY("GetSystemTime (lpSystemTime=%p)\n", lpSystemTime);

    tt = time(NULL);

    /* We can't get millisecond resolution from time(), so we get it from
       gettimeofday() */
    timeofday_retval = gettimeofday(&timeval,NULL);

    utPtr = &ut;
    if (gmtime_r(&tt, utPtr) == NULL)
    {
        ASSERT("gmtime() failed; errno is %d (%s)\n", errno, strerror(errno));
        goto EXIT;
    }

    lpSystemTime->wYear = 1900 + utPtr->tm_year;
    lpSystemTime->wMonth = utPtr->tm_mon + 1;
    lpSystemTime->wDayOfWeek = utPtr->tm_wday;
    lpSystemTime->wDay = utPtr->tm_mday;
    lpSystemTime->wHour = utPtr->tm_hour;
    lpSystemTime->wMinute = utPtr->tm_min;
    lpSystemTime->wSecond = utPtr->tm_sec;

    if(-1 == timeofday_retval)
    {
        ASSERT("gettimeofday() failed; errno is %d (%s)\n",
               errno, strerror(errno));
        lpSystemTime->wMilliseconds = 0;
    }
    else
    {
        int old_seconds;
        int new_seconds;

        lpSystemTime->wMilliseconds = timeval.tv_usec/tccMillieSecondsToMicroSeconds;

        old_seconds = utPtr->tm_sec;
        new_seconds = timeval.tv_sec%60;

        /* just in case we reached the next second in the interval between
           time() and gettimeofday() */
        if( old_seconds!=new_seconds )
        {
            TRACE("crossed seconds boundary; setting milliseconds to 999\n");
            lpSystemTime->wMilliseconds = 999;
        }
    }
EXIT:
    LOGEXIT("GetSystemTime returns void\n");
}

/*++
Function:
  GetTickCount

The GetTickCount function retrieves the number of milliseconds that
have elapsed since the system was started. It is limited to the
resolution of the system timer. To obtain the system timer resolution,
use the GetSystemTimeAdjustment function.

Parameters

This function has no parameters.

Return Values

The return value is the number of milliseconds that have elapsed since
the system was started.

In the ROTOR implementation the return value is the elapsed time since
the start of the epoch.

--*/
uint32_t
GetTickCount(
         void)
{
    uint32_t retval = 0;
    ENTRY("GetTickCount ()\n");

    // Get the 64-bit count from GetTickCount64 and truncate the results.
    retval = (uint32_t) GetTickCount64();

    LOGEXIT("GetTickCount returns DWORD %u\n", retval);
    return retval;
}

BOOL
QueryPerformanceCounter(
     LARGE_INTEGER *lpPerformanceCount
    )
{
    BOOL retval = TRUE;

    ENTRY("QueryPerformanceCounter()\n");
    do
#if defined(__linux__)
    {
        struct timespec ts;
        if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0)
        {
            ASSERT("clock_gettime(CLOCK_MONOTONIC) failed; errno is %d (%s)\n", errno, strerror(errno));
            retval = FALSE;
            break;
        }
        lpPerformanceCount->QuadPart =
            (long)ts.tv_sec * (long)tccSecondsToNanoSeconds + (long)ts.tv_nsec;
    }
#else
    {
        lpPerformanceCount->QuadPart = (long)mach_absolute_time();
    }
#endif // defined(__linux__)
    while (false);

    LOGEXIT("QueryPerformanceCounter\n");
    return retval;
}

BOOL
QueryPerformanceFrequency(
     LARGE_INTEGER *lpFrequency
    )
{
    BOOL retval = TRUE;
    ENTRY("QueryPerformanceFrequency()\n");

    lpFrequency->QuadPart = (long)(tccSecondsToNanoSeconds);

    LOGEXIT("QueryPerformanceFrequency\n");
    return retval;
}

/*++
Function:
  QueryThreadCycleTime

Puts the execution time (in nanoseconds) for the thread pointed to by ThreadHandle, into the unsigned long
pointed to by CycleTime. ThreadHandle must refer to the current thread. Returns TRUE on success, FALSE on
failure.
--*/

BOOL
QueryThreadCycleTime(
     HANDLE ThreadHandle,
     unsigned long * CycleTime
    )
{

    ULONG64 calcTime;
    FILETIME kernelTime, userTime;
    BOOL retval = TRUE;

    if(!GetThreadTimesInternal(ThreadHandle, &kernelTime, &userTime))
    {
        ASSERT("Could not get cycle time for current thread");
        retval = FALSE;
        goto EXIT;
    }

    calcTime = ((ULONG64)kernelTime.dwHighDateTime << 32);
    calcTime += (ULONG64)kernelTime.dwLowDateTime;
    calcTime += ((ULONG64)userTime.dwHighDateTime << 32);
    calcTime += (ULONG64)userTime.dwLowDateTime;
    *CycleTime = calcTime;

EXIT:
    return retval;
}

static unsigned long
GetTickCount64Fallback()
{
    unsigned long retval = 0;

    // The `__APPLE__` below is unlikely. However, if osx future version
    // supports one of the clock types below, we may miss that and
    // break backward compatibility.
#if !defined(__APPLE__)
    {
        clockid_t clockType =
            CLOCK_MONOTONIC_COARSE; // good enough resolution, fastest speed
        struct timespec ts;
        if (clock_gettime(clockType, &ts) != 0)
        {
            ASSERT("clock_gettime(CLOCK_MONOTONIC*) failed; errno is %d (%s)\n", errno, strerror(errno));
            goto EXIT;
        }
        retval = (ts.tv_sec * tccSecondsToMillieSeconds)+(ts.tv_nsec / tccMillieSecondsToNanoSeconds);
    }
#else
    {
        // use denom == 0 to indicate that s_TimebaseInfo is uninitialised.
        if (s_TimebaseInfo.denom == 0)
        {
            ASSERT("s_TimebaseInfo is uninitialized.\n");
            goto EXIT;
        }
        retval = (mach_absolute_time() * s_TimebaseInfo.numer / s_TimebaseInfo.denom) / tccMillieSecondsToNanoSeconds;
    }
#endif
EXIT:
    return retval;
}

#if defined(_X86_) || defined(__AMD64__) || defined(__x86_64__)
inline unsigned long rdtsc()
{
    unsigned long H, L;
    __asm volatile ("rdtsc":"=a"(L), "=d"(H));
#ifdef _X86_
    return L;
#else
    return (H << 32) | L;
#endif
}

static double CPUFreq()
{
    struct timeval tstart, tend;
    unsigned long start, end;

    struct timezone tzone;
    memset(&tzone, 0, sizeof(tzone));

    start = rdtsc();
    gettimeofday(&tstart, &tzone);

    usleep(2000); // 2ms

    end = rdtsc();
    gettimeofday(&tend, &tzone);

    unsigned long usec = ((tend.tv_sec - tstart.tv_sec)*1e6)
                + (tend.tv_usec - tstart.tv_usec);

    if (!usec) return 0;
    return (end - start) / usec;
}

static unsigned long cpu_speed = CPUFreq() * 1e3; // 1000 * 1e6 => ns to ms
typedef unsigned long (*GetTickCount64FallbackCB)(void);
inline unsigned long FastTickCount()
{
  return rdtsc() / cpu_speed;
}
static GetTickCount64FallbackCB getTickCount64FallbackCB = cpu_speed ? FastTickCount : GetTickCount64Fallback;
#endif

/*++
Function:
  GetTickCount64

Returns a 64-bit tick count with a millisecond resolution. It tries its best
to return monotonically increasing counts and avoid being affected by changes
to the system clock (either due to drift or due to explicit changes to system
time).
--*/
unsigned long
GetTickCount64()
{
#if defined(_X86_) || defined(__AMD64__) || defined(__x86_64__)
    return getTickCount64FallbackCB();
#else
    return GetTickCount64Fallback();
#endif
}

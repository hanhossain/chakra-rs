//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
//

/*++



Module Name:

    include/pal/dbgmsg.h

Abstract:
    Header file for Debug Message utilities. Output macros, type definitions,
    extern variables. See overview section below for usage details.

--*/

/*
Overview of Debug Message utilities

Use debug channels to selectively output information to the console.

Available macros :

    - SET_DEFAULT_DEBUG_CHANNEL

    This defines the channel to use with the macros TRACE, ERROR, etc
    Use this macro once at the beginning of your source file.
    (impl. details : this declares a constant static variable defdbgchan and
    sets it to the apropriate channel)

    usage : SET_DEFAULT_DEBUG_CHANNEL(somechannel);

    - TRACE, ENTRY, WARN, ERROR, DBGOUT

    Use this to output debug messages to the default debug channel (set with
    SET_DEFAULT_DEBUG_CHANNEL). Messages will only be output if the channel is
    active for the specified level.

    usage : TRACE("printf format string", params...);

    - TRACE_, ENTRY_, WARN_, ERROR_, DBGOUT_

    Use this to autput debug messages to a channel other than the default.

    usage : TRACE_(someotherchannel)("printf format string",params...);
                 ^                ^^                                ^
    don't forget the double set of parentheses!

Available channels :
    PAL     : PAL-specific functionalities (PAL_Initialize, etc.)
    LOADER  : Loading API (LoadLibrary, etc); loader application
    HANDLE  : Handle manager (CloseHandle, etc.)
    SHMEM   : Shared Memory functions (for IPC)
    THREAD  : Threading mechanism
    EXCEPT  : Structured Exception Handling functions
    CRT     : PAL implementation of the C Runtime Library functions
    UNICODE : Unicode support API
    ARCH    : platform-dependent stuff
    SYNC    : Management of synchronization objects
    FILE    : File I/O API
    VIRTUAL : Virtual memory and File mapping
    MEM     : Memory management (except Virtual* stuff)
    SOCKET  : WINSOCK implementation
    DEBUG   : Debugging API (ReadProcessMemory, etc.)
    LOCALE  : Locale support API
    MISC    : what doesn't fit anywhere else.
    MUTEX   : Mutex management functions
    CRITSEC : Critical section API
    POLL    : ?
    CRYPT   : Cryptographic functions
    SHFOLDER: Shared (well-known) folder functions
    SXS     : Side-by-side PALs (if supported)

    Note : Most channels correspond to subdirectories $(PALROOT)
    Note 2 : DON'T write TRACE("PAL") or TRACE(DCI_PAL), write TRACE(PAL)

Available debug levels :
    ENTRY : use this at the beginning of a function to print parameters.
    TRACE : use this to output informational messages.
    WARN  : use this to report non-critical problems.
    ERROR : use this to report critical problems.

    DBGOUT: same as TRACE, but does not output line headers (thread ID, etc)

Format specifiers :
    These trace functions currently use the native fprintf() to output data.
    All standard printf format specifiers should therefore work, while Microsoft
    extensions will not.
    There is one special case to consider : wide strings and wide characters.
    Microsoft's extensions to printf include the specifiers %S and %C for
    printing strings and characters of wchar_t. In the C99 standard,
    the specifiers %ls and %ls serve the same purpose. However, Windows defines
    wchar_t as a 16bit int, which is NOT guaranteed to match implementations
    on other platforms. glibc on a x86 defines wchar_t as a 32bit int.
    For this reason, %S and %C should be used in TRACE functions to output
    Windows wide strings (of type wchar_t or char16_t). To output wide-strings
    in a platforms native format (litterals L"string" or variables of type
    wchar_native), the specifiers %ls and %lc should be used instead.

Using Debug channels at Run Time
    To tell the PAL which debug channels should be open and which should be
    closed, set the environment variable PAL_DBG_CHANNELS according to the
    following syntax :
    [+|-]<channel>.<level>[: ...]
    + opens a channel, - closes it;
    <channel> must be one of PAL, FILE, (etc), or the wildcard "all"
    <level> must be TRACE, ENTRY, WARN, ERROR or "all"

    Examples (for bash):

    export PAL_DBG_CHANNELS="+PAL.TRACE:-FILE.ERROR"
    export PAL_DBG_CHANNELS="+all.ENTRY"
    export PAL_DBG_CHANNELS="-all.all"

    To explicitly redirect the output of debug messages to a file (instead of
    relying on the shell's > and |), set the environment variable
    PAL_API_TRACING to the name of the file to write to. It can also be set to
    "stdout" or "stderr". If PAL_API_TRACING is not set, output will go to
    stderr.

    ASSERT() messages cannot be controlled with PAL_DBG_CHANNELS; they can be
    globally disabled (in debug builds) by setting the environment variable
    PAL_DISABLE_ASSERTS to 1. In release builds, they will always be disabled

    The environment variable "PAL_API_LEVELS" determines how many levels of
    nesting will be allowed in ENTRY calls; if not set, the default is 1; a
    value of 0 will allow infinite nesting, but will not indent the output

    It is possible to disable/enable all channels during the execution of a
    process; this involves using a debugger to modify a variable within the
    address space of the running process. the variable is named
    'dbg_master_switch'; if set to zero, all debug chanels will be closed; if
    set to nonzero, channels will be open or closed based on PAL_DBG_CHANNELS

    Notes :
    If DEBUG was not defined at build-time, no debug messages
    will be generated.
    If DEBUG was defined, all debug levels will be enabled,
    but all channels will be closed by default

    Another configure option is --enable-appendtraces
    Normally, if the file specified by PAL_API_TRACING exists, its content will
    be overwritten when a PAL process starts using it. If --enable-appendtraces
    is used, debug output will be appended at the end of the file instead.



 */

#ifndef _PAL_DBGMSG_H_
#define _PAL_DBGMSG_H_

#include "pal/palinternal.h"
#include "pal/debug.h"
#include "pal/thread.hpp"

extern "C"
{

/* Channel identifiers */
typedef enum
{
    DCI_PAL,
    DCI_LOADER,
    DCI_HANDLE,
    DCI_SHMEM,
    DCI_THREAD,
    DCI_EXCEPT,
    DCI_CRT,
    DCI_UNICODE,
    DCI_ARCH,
    DCI_SYNC,
    DCI_FILE,
    DCI_VIRTUAL,
    DCI_MEM,
    DCI_SOCKET,
    DCI_DEBUG,
    DCI_LOCALE,
    DCI_MISC,
    DCI_MUTEX,
    DCI_CRITSEC,
    DCI_POLL,
    DCI_CRYPT,
    DCI_SHFOLDER,
#ifdef FEATURE_PAL_SXS
    DCI_SXS,
#endif // FEATURE_PAL_SXS

    DCI_LAST
} DBG_CHANNEL_ID;

/* Level identifiers */
typedef enum
{
    DLI_ENTRY,
    DLI_TRACE,
    DLI_WARN,
    DLI_ERROR,
    DLI_ASSERT,
    DLI_EXIT,

    DLI_LAST
} DBG_LEVEL_ID;


/* extern variables */

// Change W16_NULLSTRING to external variable to avoid multiple warnings showing up in prefast
extern const char16_t* W16_NULLSTRING;

/* compile out these trace levels; see the definition of NOTRACE */
#if !defined(DEBUG)
#define TRACE     NOTRACE
#else
#define TRACE     NOTRACE
#endif
#define TRACE_(x) TRACE
#define WARN      TRACE
#define WARN_(x)  TRACE
#define ENTRY_(x) TRACE
#define LOGEXIT   TRACE
#define LOGEXIT_(x) TRACE
#define DBGOUT     TRACE
#define DBGOUT_(x) TRACE
#define ERROR     TRACE
#define ERROR_(x) TRACE
#define DBG_PRINTF(level, channel, bHeader) TRACE

#define SET_DEFAULT_DEBUG_CHANNEL(x)
#define DBG_ENABLED(level, channel) (false)

/* define NOTRACE as nothing; this will absorb the variable-argument list used
   in tracing macros */
#define NOTRACE(args...)

#if !defined(_DEBUG)

#define ASSERT(args...)
#define _ASSERT(expr)
#define _ASSERTE(expr)
#define _ASSERT_MSG(args...)

#else /* defined(_DEBUG) */

#define ASSERT(...) \
{ \
    PRINT_ERROR("] %s %s:%d",__FUNCTION__,__FILE__,\
                   __LINE__);\
    PRINT_ERROR(__VA_ARGS__);\
}

#define _ASSERTE(expr) do { if (!(expr)) { ASSERT("Expression: " #expr "\n"); } } while(0)
#define _ASSERT _ASSERTE
#define _ASSERT_MSG(expr, args...) \
    do { \
        if (!(expr)) \
        { \
            ASSERT("Expression: " #expr ", Description: " args); \
        } \
    } while(0)

#endif /* defined(_DEBUG) */

}

#endif /* _PAL_DBGMSG_H_ */

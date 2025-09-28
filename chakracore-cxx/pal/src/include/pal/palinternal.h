//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
//

/*++



Module Name:

    palinternal.h

Abstract:

    Rotor Platform Adaptation Layer (PAL) header file used by source
    file part of the PAL implementation. This is a wrapper over
    unix/inc/pal.h. It allows avoiding name collisions when including
    system header files, and it allows redirecting calls to 'standard' functions
    to their PAL counterpart

Details :

A] Rationale (see B] for the quick recipe)
There are 2 types of namespace collisions that must be handled.

1) standard functions declared in pal.h, which do not need to be
   implemented in the PAL because the system's implementation is sufficient.

   (examples : memcpy, strlen, fclose)

   The problem with these is that a prototype for them is provided both in
   pal.h and in a system header (stdio.h, etc). If a PAL file needs to
   include the files containing both prototypes, the compiler may complain
   about the multiple declarations.

   To avoid this, the inclusion of pal.h must be wrapped in a
   #define/#undef pair, which will effectiveily "hide" the pal.h
   declaration by renaming it to something else. this is done by palinternal.h
   in this way :

   #define some_function DUMMY_some_function
   #include <pal.h>
   #undef some_function

   when a PAL source file includes palinternal.h, it will see a prototype for
   DUMMY_some_function instead of some_function; so when it includes the
   system header with the "real" prototype, no collision occurs.

   (note : technically, no functions should ever be treated this way, all
   system functions should be wrapped according to method 2, so that call
   logging through ENTRY macros is done for all functions n the PAL. However
   this reason alone is not currently considered enough to warrant a wrapper)

2) standard functions which must be reimplemented by the PAL, because the
   system's implementation does not offer suitable functionnality.

   (examples : widestring functions, networking)

   Here, the problem is more complex. The PAL must provide functions with the
   same name as system functions. Due to the nature of Unix dynamic linking,
   if this is done, the PAL's implementation will effectively mask the "real"
   function, so that all calls are directed to it. This makes it impossible for
   a function to be implemented as calling its counterpart in the system, plus
   some extra work, because instead of calling the system's implementation, the
   function would only call itself in an infinitely recursing nightmare. Even
   worse, if by bad luck the system libraries attempt to call the function for
   which the PAL provides an implementation, it is the PAL's version that will
   be called.
   It is therefore necessary to give the PAL's implementation of such functions
   a different name. However, PAL consumers (applications built on top of the
   PAL) must be able to call the function by its 'official' name, not the PAL's
   internal name.
   This can be done with some more macro magic, by #defining the official name
   to the internal name *in pal.h*. :

   #define some_function PAL_some_function

   This way, while PAL consumer code can use the official name, it is the
   internal name that wil be seen at compile time.
   However, one extra step is needed. While PAL consumers must use the PAL's
   implementation of these functions, the PAL itself must still have access to
   the "real" functions. This is done by #undefining in palinternal.h the names
   #defined in pal.h :

   #include <pal.h>
   #undef some_function.

   At this point, code in the PAL implementation can access *both* its own
   implementation of the function (with PAL_some_function) *and* the system's
   implementation (with some_function)

    [side note : for the Win32 PAL, this can be accomplished without touching
    pal.h. In Windows, symbols in in dynamic libraries are resolved at
    compile time. if an application that uses some_function is only linked to
    pal.dll, some_function will be resolved to the version in that DLL,
    even if other DLLs in the system provide other implementations. In addition,
    the function in the DLL can actually have a different name (e.g.
    PAL_some_function), to which the 'official' name is aliased when the DLL
    is compiled. All this is not possible with Unix dynamic linking, where
    symbols are resolved at run-time in a first-found-first-used order. A
    module may end up using the symbols from a module it was never linked with,
    simply because that module was located somewhere in the dependency chain. ]

    It should be mentionned that even if a function name is not documented as
    being implemented in the system, it can still cause problems if it exists.
    This is especially a problem for functions in the "reserved" namespace
    (names starting with an underscore : _exit, etc). (We shouldn't really be
    implementing functions with such a name, but we don't really have a choice)
    If such a case is detected, it should be wrapped according to method 2

    Note that for all this to work, it is important for the PAL's implementation
    files to #include palinternal.h *before* any system files, and to never
    include pal.h directly.

B] Procedure for name conflict resolution :

When adding a function to pal.h, which is implemented by the system and
which does not need a different implementation :

- add a #define function_name DUMMY_function_name to palinternal.h, after all
  the other DUMMY_ #defines (above the #include <pal.h> line)
- add the function's prototype to pal.h (if that isn't already done)
- add a #undef function_name to palinternal.h near all the other #undefs
  (after the #include <pal.h> line)

When overriding a system function with the PAL's own implementation :

- add a #define function_name PAL_function_name to pal.h, somewhere
  before the function's prototype, inside a #ifndef _MSCVER/#endif pair
  (to avoid affecting the Win32 build)
- add a #undef function_name to palinternal.h near all the other #undefs
  (after the #include <pal.h> line)
- implement the function in the pal, naming it PAL_function_name
- within the PAL, call PAL_function_name() to call the PAL's implementation,
function_name() to call the system's implementation



--*/

#ifndef _PAL_INTERNAL_H_
#define _PAL_INTERNAL_H_

#include <sys/types.h>

#define PAL_IMPLEMENTATION

/* Include our configuration information so it's always present when
   compiling PAL implementation files. */
#include "config.h"

#ifdef DEBUG
#define _ENABLE_DEBUG_MESSAGES_ 1
#else
#define _ENABLE_DEBUG_MESSAGES_ 0
#endif

#if defined(__APPLE__)
#include "TargetConditionals.h"
#endif // __APPLE__ ?

// TODO (hanhossain): remove all defines and undefs

/* C runtime functions needed to be renamed to avoid duplicate definition
   of those functions when including standard C header files */
#define strlen DUMMY_strlen
#define strnlen DUMMY_strnlen
#define stricmp DUMMY_stricmp
#if HAVE__SNPRINTF
#define _snprintf DUMMY__snprintf
#endif /* HAVE__SNPRINTF */
#if HAVE__SNWPRINTF
#define _snwprintf DUMMY__snwprintf
#endif  /* HAVE__SNWPRINTF */
#define time DUMMY_time
#define tm PAL_tm
#define time_t PAL_time_t
#define abs DUMMY_abs
#define llabs DUMMY_llabs
#define atan DUMMY_atan
#define tan DUMMY_tan
#define cos DUMMY_cos
#define sin DUMMY_sin
#define cosh DUMMY_cosh
#define sinh DUMMY_sinh
#define tanh DUMMY_tanh
#define modf DUMMY_modf
#define fmod DUMMY_fmod
#define fmodf DUMMY_fmodf
#define sqrt DUMMY_sqrt
#define ceil DUMMY_ceil
#define fabs DUMMY_fabs
#define fabsf DUMMY_fabsf
#define floor DUMMY_floor
#define modff DUMMY_modff

/* RAND_MAX needed to be renamed to avoid duplicate definition when including
   stdlib.h header files. PAL_RAND_MAX should have the same value as RAND_MAX
   defined in pal.h  */
#define PAL_RAND_MAX 0x7fff

#ifdef stdin
#undef stdin
#endif
#ifdef stdout
#undef stdout
#endif
#ifdef stderr
#undef stderr
#endif

#ifdef RAND_MAX
#undef RAND_MAX
#endif
#ifdef __record_type_class
#undef __record_type_class
#endif
#ifdef __real_type_class
#undef __real_type_class
#endif

// The standard headers define va_start and va_end as macros,
// To avoid redefinition problems, undefine those macros.

#define uintptr_t PAL_uintptr_t
#define timeval PAL_timeval
#define FILE PAL_FILE
#define fpos_t PAL_fpos_t

#define INCLUDE_PAL_INTERNAL_
#include "pal.h"
#undef INCLUDE_PAL_INTERNAL_

#include "mbusafecrt.h"

#ifdef _VAC_
#undef va_arg
#endif

#undef _BitScanForward64

/* pal.h does "#define alloca _alloca", but we need access to the "real"
   alloca */
#undef alloca

/* Undef all functions and types previously defined so those functions and
   types could be mapped to the C runtime and socket implementation of the
   native OS */
#undef exit
#undef alloca
#undef memmove
#undef strlen
#undef strnlen
#undef stricmp
#undef strdup
#undef malloc
#undef realloc
#undef free
#undef bsearch
#undef time
#undef tm
#undef localtime
#undef FILE
#undef fclose
#undef setbuf
#undef fopen
#undef fread
#undef feof
#undef ferror
#undef ftell
#undef fflush
#undef fwrite
#undef fgets
#undef fgetws
#undef fputc
#undef putchar
#undef fputs
#undef fseek
#undef fgetpos
#undef fsetpos
#undef getcwd
#undef getc
#undef fgetc
#undef ungetc
#undef _flushall
#undef setvbuf
#undef mkstemp
#undef rename
#undef unlink
#undef size_t
#undef time_t
#undef stdin
#undef stdout
#undef stderr
#undef abs
#undef labs
#undef llabs
#undef acos
#undef asin
#undef atan2
#undef exp
#undef log
#undef log10
#undef rand
#undef errno
#undef getenv
#undef wcsspn
#undef open
#undef glob
#undef atan
#undef tan
#undef cos
#undef sin
#undef cosh
#undef sinh
#undef tanh
#undef modf
#undef fmod
#undef fmodf
#undef sqrt
#undef ceil
#undef fabs
#undef fabsf
#undef floor
#undef modff

#undef ptrdiff_t
#undef uintptr_t
#undef timeval
#undef fpos_t


#undef fprintf
#undef fwprintf
#undef vfprintf
#undef vfwprintf
#undef wprintf
#undef _snprintf
#if HAVE__SNWPRINTF
#undef _snwprintf
#endif  /* HAVE__SNWPRINTF */
#undef wcstod
#undef wcstol
#undef wcstoul
#undef _wcstoui64
#undef wcscat
#undef wcscpy
#undef wcslen
#undef wcsncmp
#undef wcschr
#undef wcsrchr
#undef swscanf
#undef wcspbrk
#undef wcsstr
#undef wcscmp
#undef wcsncat
#undef wcsncpy
#undef wcstok
#undef wcscspn
#undef towupper
#undef vswprintf
#undef _vsnprintf
#undef _vsnwprintf
#undef vsnprintf
#undef wvsnprintf

#undef ctime

#undef RAND_MAX
#undef __record_type_class
#undef __real_type_class

// We need a sigsetjmp prototype in pal.h for the SEH macros, but we
// can't use the "real" prototype (because we don't want to define sigjmp_buf).
// So we must rename the "real" sigsetjmp to avoid redefinition errors.
#define sigsetjmp REAL_sigsetjmp
#define siglongjmp REAL_siglongjmp
#include <setjmp.h>
#undef sigsetjmp
#undef siglongjmp

#undef _SIZE_T_DEFINED

#define _DONT_USE_CTYPE_INLINE_
#if HAVE_RUNETYPE_H
#include <runetype.h>
#endif
#include <ctype.h>

#define _WITH_GETLINE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <unistd.h>
#include <fcntl.h>

#ifdef __APPLE__

#undef GetCurrentThread
#include <CoreServices/CoreServices.h>

#include <malloc/malloc.h>

#endif // __APPLE__

/* we don't really need this header here, but by including it we make sure
   we'll catch any definition conflicts */
#include <sys/socket.h>

#if !HAVE_INFTIM
#define INFTIM  -1
#endif // !HAVE_INFTIM

#if (__GNUC__ >= 4)
#define OffsetOf(TYPE, MEMBER) __builtin_offsetof(TYPE, MEMBER)
#else
#define OffsetOf(s, f) (int32_t)(size_t)&(((s*)0)->f)
#endif /* __GNUC__ version check*/

#undef assert
#define assert (Use__ASSERTE_instead_of_assert) assert

#define PROCESS_PIPE_NAME_PREFIX ".dotnet-pal-processpipe"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef enum _TimeConversionConstants
{
    tccSecondsToMillieSeconds       = 1000,         // 10^3
    tccSecondsToMicroSeconds        = 1000000,      // 10^6
    tccSecondsToNanoSeconds         = 1000000000,   // 10^9
    tccMillieSecondsToMicroSeconds  = 1000,         // 10^3
    tccMillieSecondsToNanoSeconds   = 1000000,      // 10^6
    tccMicroSecondsToNanoSeconds    = 1000,         // 10^3
    tccSecondsTo100NanoSeconds      = 10000000,     // 10^7
    tccMicroSecondsTo100NanoSeconds = 10            // 10^1
} TimeConversionConstants;

#ifdef __cplusplus
}

/* This is duplicated in utilcode.h for CLR, with cooler type-traits */
template <typename T>
inline
T* InterlockedExchangePointerT(
    T* volatile *Target,
    T* Value)
{
    return (T*)(InterlockedExchangePointer(
        (void * volatile*)Target,
        (void *)Value));
}

template <typename T>
inline
T* InterlockedCompareExchangePointerT(
    T* volatile *destination,
    T* exchange,
    T* comparand)
{
    return (T*)(InterlockedCompareExchangePointer(
        (void * volatile*)destination,
        (void *)exchange,
        (void *)comparand));
}

template <typename T>
inline T* InterlockedExchangePointerT(
    T* volatile * target,
    int           value) // When NULL is provided as argument.
{
    //STATIC_ASSERT(value == 0);
    return InterlockedExchangePointerT(target, reinterpret_cast<T*>(value));
}

template <typename T>
inline T* InterlockedCompareExchangePointerT(
    T* volatile * destination,
    int           exchange,  // When NULL is provided as argument.
    T*            comparand)
{
    //STATIC_ASSERT(exchange == 0);
    return InterlockedCompareExchangePointerT(destination, reinterpret_cast<T*>(exchange), comparand);
}

template <typename T>
inline T* InterlockedCompareExchangePointerT(
    T* volatile * destination,
    T*            exchange,
    int           comparand) // When NULL is provided as argument.
{
    //STATIC_ASSERT(comparand == 0);
    return InterlockedCompareExchangePointerT(destination, exchange, reinterpret_cast<T*>(comparand));
}

#undef InterlockedExchangePointer
#define InterlockedExchangePointer InterlockedExchangePointerT
#undef InterlockedCompareExchangePointer
#define InterlockedCompareExchangePointer InterlockedCompareExchangePointerT

#include "volatile.h"

#endif // __cplusplus

#define StackOverflowMessage "Process is terminated due to StackOverflowException.\n"

#define MAX_LONGPATH   1024  /* max. length of full pathname */

BOOL PAL_IsDebuggerPresent();

// do not change the location of the definitions below.
// these defs should be always at the end
#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#endif /* _PAL_INTERNAL_H_ */

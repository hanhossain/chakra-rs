# xplat-todo: Cleanup this file!
# we have `feature detection` for direct compilation target and manual entries for cross-compilation below
include(CheckCXXSourceCompiles)
include(CheckCXXSourceRuns)
include(CheckCXXSymbolExists)
include(CheckFunctionExists)
include(CheckIncludeFiles)
include(CheckStructHasMember)
include(CheckTypeSize)
include(CheckLibraryExists)

if(CMAKE_SYSTEM_NAME STREQUAL FreeBSD)
  set(CMAKE_REQUIRED_INCLUDES /usr/local/include)
elseif(CMAKE_SYSTEM_NAME STREQUAL SunOS)
  set(CMAKE_REQUIRED_INCLUDES /opt/local/include)
endif()
if(NOT CMAKE_SYSTEM_NAME STREQUAL Darwin AND NOT CMAKE_SYSTEM_NAME STREQUAL FreeBSD)
  set(CMAKE_REQUIRED_DEFINITIONS "-D_DEFAULT_SOURCE -D_POSIX_C_SOURCE=200809L")
endif()

list(APPEND CMAKE_REQUIRED_DEFINITIONS -D_FILE_OFFSET_BITS=64)

check_include_files(unicode/uchar.h HAVE_LIBICU_UCHAR_H)

set(CMAKE_EXTRA_INCLUDE_FILES machine/reg.h)
check_type_size("struct reg" BSD_REGS_T)
set(CMAKE_EXTRA_INCLUDE_FILES)
set(CMAKE_EXTRA_INCLUDE_FILES asm/ptrace.h)
check_type_size("struct pt_regs" PT_REGS)
set(CMAKE_EXTRA_INCLUDE_FILES)
set(CMAKE_EXTRA_INCLUDE_FILES signal.h)
check_type_size(siginfo_t SIGINFO_T)
set(CMAKE_EXTRA_INCLUDE_FILES)
set(CMAKE_EXTRA_INCLUDE_FILES ucontext.h)
check_type_size(ucontext_t UCONTEXT_T)
set(CMAKE_EXTRA_INCLUDE_FILES)
set(CMAKE_EXTRA_INCLUDE_FILES pthread.h)
check_type_size(pthread_rwlock_t PTHREAD_RWLOCK_T)
set(CMAKE_EXTRA_INCLUDE_FILES)
set(CMAKE_EXTRA_INCLUDE_FILE procfs.h)
check_type_size(prwatch_t PRWATCH_T)
set(CMAKE_EXTRA_INCLUDE_FILE)

check_cxx_source_runs("
#include <stdlib.h>
#include <stdio.h>

extern \"C++\" {
  inline long long abs(long long _X) {
    return llabs(_X);
  }
}

int main(int argc, char **argv) {
  long long X = 123456789 + argc;
  printf(\"%lld\", abs(X));
  exit(0);
}" PLATFORM_ACCEPTS_ABS_OVERLOAD)
set(CMAKE_REQUIRED_LIBRARIES)
set(CMAKE_REQUIRED_LIBRARIES pthread)
check_cxx_source_runs("
#include <stdlib.h>
#include <errno.h>
#include <semaphore.h>

int main() {
  sem_t sema;
  if (sem_init(&sema, 0, 0) == -1){
    exit(1);
  }
  exit(0);
}" HAS_POSIX_SEMAPHORES)
set(CMAKE_REQUIRED_LIBRARIES)
set(CMAKE_REQUIRED_DEFINITIONS)

set(SYNCHMGR_SUSPENSION_SAFE_CONDITION_SIGNALING 1)
set(ERROR_FUNC_FOR_GLOB_HAS_FIXED_PARAMS 1)

if(CMAKE_SYSTEM_NAME STREQUAL Darwin)
  set(HAVE_COREFOUNDATION 1)
  set(DEADLOCK_WHEN_THREAD_IS_SUSPENDED_WHILE_BLOCKED_ON_MUTEX 1)
  set(PAL_PTRACE "ptrace((cmd), (pid), (caddr_t)(addr), (data))")
  set(PAL_PT_ATTACH PT_ATTACH)
  set(PAL_PT_DETACH PT_DETACH)
  set(PAL_PT_READ_D PT_READ_D)
  set(PAL_PT_WRITE_D PT_WRITE_D)
  set(JA_JP_LOCALE_NAME ja_JP.SJIS)
  set(KO_KR_LOCALE_NAME ko_KR.eucKR)
  set(ZH_TW_LOCALE_NAME zh_TG.BIG5)
elseif(CMAKE_SYSTEM_NAME STREQUAL FreeBSD)
  set(DEADLOCK_WHEN_THREAD_IS_SUSPENDED_WHILE_BLOCKED_ON_MUTEX 0)
  set(PAL_PTRACE "ptrace((cmd), (pid), (caddr_t)(addr), (data))")
  set(PAL_PT_ATTACH PT_ATTACH)
  set(PAL_PT_DETACH PT_DETACH)
  set(PAL_PT_READ_D PT_READ_D)
  set(PAL_PT_WRITE_D PT_WRITE_D)
  set(JA_JP_LOCALE_NAME ja_JP_LOCALE_NOT_FOUND)
  set(KO_KR_LOCALE_NAME ko_KR_LOCALE_NOT_FOUND)
  set(ZH_TW_LOCALE_NAME zh_TW_LOCALE_NOT_FOUND)

  if(EXISTS "/lib/libc.so.7")
    set(FREEBSD_LIBC "/lib/libc.so.7")
  else()
    message(FATAL_ERROR "Cannot find libc on this system.")
  endif()
else() # Anything else is Linux
  set(DEADLOCK_WHEN_THREAD_IS_SUSPENDED_WHILE_BLOCKED_ON_MUTEX 0)
  set(PAL_PTRACE "ptrace((cmd), (pid), (void*)(addr), (data))")
  set(PAL_PT_ATTACH PTRACE_ATTACH)
  set(PAL_PT_DETACH PTRACE_DETACH)
  set(PAL_PT_READ_D PTRACE_PEEKDATA)
  set(PAL_PT_WRITE_D PTRACE_POKEDATA)
  set(JA_JP_LOCALE_NAME ja_JP_LOCALE_NOT_FOUND)
  set(KO_KR_LOCALE_NAME ko_KR_LOCALE_NOT_FOUND)
  set(ZH_TW_LOCALE_NAME zh_TW_LOCALE_NOT_FOUND)
endif(CMAKE_SYSTEM_NAME STREQUAL Darwin)

if(NOT NO_ICU)
  if(NOT HAVE_LIBICU_UCHAR_H)
    unset(HAVE_LIBICU_UCHAR_H CACHE)
    message(FATAL_ERROR "Cannot find ICU. Try installing libicu-dev or the appropriate packages for your platform. You may also disable icu/unicode with '--no-icu' argument")
  endif()
endif()
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/pal/src/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/pal/src/config.h)

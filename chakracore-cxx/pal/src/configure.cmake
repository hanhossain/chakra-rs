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
set(CMAKE_REQUIRED_LIBRARIES pthread)
check_cxx_source_runs("
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>

int main(void)
{
  int policy;
  struct sched_param schedParam;
  int max_priority;
  int min_priority;

  if (0 != pthread_getschedparam(pthread_self(), &policy, &schedParam))
  {
    exit(1);
  }

  max_priority = sched_get_priority_max(policy);
  min_priority = sched_get_priority_min(policy);

  exit(-1 == max_priority || -1 == min_priority);
}" HAVE_SCHED_GET_PRIORITY)
set(CMAKE_REQUIRED_LIBRARIES pthread)
check_cxx_source_runs("
#include <stdlib.h>
#include <sched.h>

int main(void)
{
  if (sched_getcpu() >= 0)
  {
    exit(0);
  }
  exit(1);
}" HAVE_SCHED_GETCPU)
set(CMAKE_REQUIRED_LIBRARIES)
check_cxx_source_runs("
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

int main()
{
  int ret;
  struct timeval tv;
  ret = gettimeofday(&tv, NULL);

  exit(ret);
}" HAVE_WORKING_GETTIMEOFDAY)
check_cxx_source_runs("
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

int main()
{
  int ret;
#ifndef __APPLE__
  struct timespec ts;
  ret = clock_gettime(CLOCK_REALTIME, &ts);
#else
  ret = 1; // do not use clock_gettime on osx/ios (backward compatibility)
#endif
  exit(ret);
}" HAVE_WORKING_CLOCK_GETTIME)
check_cxx_source_runs("
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

int main()
{
  int ret;
#ifndef __APPLE__
  struct timespec ts;
  ret = clock_gettime(CLOCK_MONOTONIC, &ts);
#else
  ret = 1; // do not use clock_gettime on osx/ios (backward compatibility)
#endif
  exit(ret);
}" HAVE_CLOCK_MONOTONIC)
check_cxx_source_runs("
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

int main()
{
  int ret;
#ifndef __APPLE__
  struct timespec ts;
  ret = clock_gettime(CLOCK_MONOTONIC_COARSE, &ts);
#else
  ret = 1; // do not use clock_gettime on osx/ios (backward compatibility)
#endif
  exit(ret);
}" HAVE_CLOCK_MONOTONIC_COARSE)
check_cxx_source_runs("
#include <stdlib.h>
#include <mach/mach_time.h>

int main()
{
  int ret;
  mach_timebase_info_data_t timebaseInfo;
  ret = mach_timebase_info(&timebaseInfo);
  mach_absolute_time();
  exit(ret);
}" HAVE_MACH_ABSOLUTE_TIME)
check_cxx_source_runs("
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

int main()
{
  int ret;
#ifndef __APPLE__
  struct timespec ts;
  ret = clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts);
#else
  ret = 1; // do not use clock_gettime on osx/ios (backward compatibility)
#endif
  exit(ret);
}" HAVE_CLOCK_THREAD_CPUTIME)
check_cxx_source_runs("
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>

int main(void) {
  int devzero;
  void *retval;

  devzero = open(\"/dev/zero\", O_RDWR);
  if (-1 == devzero) {
    exit(1);
  }
  retval = mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, devzero, 0);
  if (retval == (void *)-1) {
    exit(1);
  }
  exit(0);
}" HAVE_MMAP_DEV_ZERO)
check_cxx_source_runs("
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>

#ifndef MAP_ANON
#define MAP_ANON MAP_ANONYMOUS
#endif

int main(void) {
  void *hint, *ptr;
  int pagesize;
  int fd;

  pagesize = getpagesize();
  fd = open(\"/etc/passwd\", O_RDONLY);
  if (fd == -1) {
    exit(0);
  }
  ptr = mmap(NULL, pagesize, PROT_NONE, MAP_ANON | MAP_PRIVATE, -1, 0);
  if (ptr == MAP_FAILED) {
    exit(0);
  }
  hint = mmap(NULL, pagesize, PROT_NONE, MAP_ANON | MAP_PRIVATE, -1, 0);
  if (hint == MAP_FAILED) {
    exit(0);
  }
  if (munmap(ptr, pagesize) != 0) {
    exit(0);
  }
  if (munmap(hint, pagesize) != 0) {
    exit(0);
  }
  ptr = mmap(hint, pagesize, PROT_NONE, MAP_FIXED | MAP_PRIVATE, fd, 0);
  if (ptr == MAP_FAILED || ptr != hint) {
    exit(0);
  }
  exit(1);
}" MMAP_IGNORES_HINT)
check_cxx_source_runs("
#include <sys/types.h>
#include <sys/mman.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef MAP_ANON
#define MAP_ANON MAP_ANONYMOUS
#endif

void *handle_signal(int signal) {
  /* If we reach this, we've crashed due to mmap honoring
  PROT_NONE. */
  _exit(1);
}

int main(void) {
  int *ptr;
  struct sigaction action;

  ptr = (int *) mmap(NULL, getpagesize(), PROT_NONE,
                     MAP_ANON | MAP_PRIVATE, -1, 0);
  if (ptr == (int *) MAP_FAILED) {
    exit(0);
  }
  action.sa_handler = &handle_signal;
  action.sa_flags = 0;
  sigemptyset(&action.sa_mask);
  if (sigaction(SIGBUS, &action, NULL) != 0) {
    exit(0);
  }
  if (sigaction(SIGSEGV, &action, NULL) != 0) {
    exit(0);
  }
  /* This will drop us into the signal handler if PROT_NONE
     is honored. */
  *ptr = 123;
  exit(0);
}" MMAP_ANON_IGNORES_PROTECTION)
check_cxx_source_runs("
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>

#ifndef MAP_ANON
#define MAP_ANON MAP_ANONYMOUS
#endif

int main()
{
  int iRet = 1;
  void * pAddr = MAP_FAILED;
  int MemSize = 1024;

  MemSize = getpagesize();
  pAddr = mmap(0x0, MemSize, PROT_NONE, MAP_PRIVATE | MAP_ANON, -1, 0);
  if (pAddr == MAP_FAILED)
    exit(0);

  pAddr = mmap(pAddr, MemSize, PROT_WRITE | PROT_READ, MAP_FIXED | MAP_PRIVATE | MAP_ANON, -1, 0);
  if (pAddr == MAP_FAILED)
    iRet = 0;

  munmap(pAddr, MemSize); // don't care of this
  exit (iRet);
}" MMAP_DOESNOT_ALLOW_REMAP)
check_cxx_source_runs("
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/mman.h>

#define MEM_SIZE 1024

int main(void)
{
  char * fname;
  int fd;
  int ret;
  void * pAddr0, * pAddr1;

  fname = (char *)malloc(MEM_SIZE);
  if (!fname)
    exit(1);
  strcpy(fname, \"/tmp/name/multiplemaptestXXXXXX\");

  fd = mkstemp(fname);
  if (fd < 0)
    exit(1);

  ret = write (fd, (void *)fname, MEM_SIZE);
  if (ret < 0)
    exit(1);

  pAddr0 = mmap(0, MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  pAddr1 = mmap(0, MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  /* In theory we should look for (pAddr1 == MAP_FAILED) && (pAddr1 != MAP_FAILED)
   but in case the first test also failed, i.e. we failed to run the test,
   let's assume that the system might not allow multiple shared mapping of the
   same file region in the same process. The code enabled in this case is
   only a fall-back code path. In case the double mmap actually works, virtually
   nothing will change and the normal code path will be executed */
  if (pAddr1 == MAP_FAILED)
    ret = 1;
  else
    ret = 0;

  if (pAddr0)
    munmap (pAddr0, MEM_SIZE);
  if (pAddr1)
    munmap (pAddr1, MEM_SIZE);
  close(fd);
  unlink(fname);
  free(fname);

  exit(ret != 1);
}" ONE_SHARED_MAPPING_PER_FILEREGION_PER_PROCESS)
set(CMAKE_REQUIRED_LIBRARIES pthread)
check_cxx_source_runs("
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>

void *start_routine(void *param) { return NULL; }

int main() {
  int result;
  pthread_t tid;

  errno = 0;
  result = pthread_create(&tid, NULL, start_routine, NULL);
  if (result != 0) {
    exit(1);
  }
  if (errno != 0) {
    exit(0);
  }
  exit(1);
}" PTHREAD_CREATE_MODIFIES_ERRNO)
set(CMAKE_REQUIRED_LIBRARIES)
set(CMAKE_REQUIRED_LIBRARIES pthread)
check_cxx_source_runs("
#include <errno.h>
#include <semaphore.h>
#include <stdlib.h>

int main() {
  int result;
  sem_t sema;

  errno = 50;
  result = sem_init(&sema, 0, 0);
  if (result != 0)
  {
    exit(1);
  }
  if (errno != 50)
  {
    exit(0);
  }
  exit(1);
}" SEM_INIT_MODIFIES_ERRNO)
set(CMAKE_REQUIRED_LIBRARIES)
check_cxx_source_runs("
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(void) {
  int fd;
#ifdef PATH_MAX
  char path[PATH_MAX];
#elif defined(MAXPATHLEN)
  char path[MAXPATHLEN];
#else
  char path[1024];
#endif

  sprintf(path, \"/proc/%u/$1\", getpid());
  fd = open(path, $2);
  if (fd == -1) {
    exit(1);
  }
  exit(0);
}" HAVE_PROCFS_CTL)
set(CMAKE_REQUIRED_LIBRARIES m)
check_cxx_source_runs("
#include <math.h>
#include <stdlib.h>

int main(void) {
  if (!isnan(acos(10))) {
    exit(1);
  }
  exit(0);
}" HAVE_COMPATIBLE_ACOS)
set(CMAKE_REQUIRED_LIBRARIES)
set(CMAKE_REQUIRED_LIBRARIES m)
check_cxx_source_runs("
#include <math.h>
#include <stdlib.h>

int main(void) {
  if (!isnan(asin(10))) {
    exit(1);
  }
  exit(0);
}" HAVE_COMPATIBLE_ASIN)
set(CMAKE_REQUIRED_LIBRARIES)
set(CMAKE_REQUIRED_LIBRARIES m)
check_cxx_source_runs("
#include <math.h>
#include <stdlib.h>

int main(void) {
  double pi = 3.14159265358979323846;
  double result;

  result = atan2(0.0, -0.0);
  if (fabs(pi - result) > 0.0000001) {
    exit(1);
  }

  result = atan2(-0.0, -0.0);
  if (fabs(-pi - result) > 0.0000001) {
    exit(1);
  }

  result = atan2 (-0.0, 0.0);
  if (result != 0.0 || copysign (1.0, result) > 0) {
    exit(1);
  }

  result = atan2 (0.0, 0.0);
  if (result != 0.0 || copysign (1.0, result) < 0) {
    exit(1);
  }

  exit (0);
}" HAVE_COMPATIBLE_ATAN2)
set(CMAKE_REQUIRED_LIBRARIES)
set(CMAKE_REQUIRED_LIBRARIES m)
check_cxx_source_runs("
#include <math.h>
#include <stdlib.h>

int main(void) {
  double d = exp(1.0), e = M_E;

  /* Used memcmp rather than == to test that the doubles are equal to
   prevent gcc's optimizer from using its 80 bit internal long
   doubles. If you use ==, then on BSD you get a false negative since
   exp(1.0) == M_E to 64 bits, but not 80.
  */

  if (memcmp (&d, &e, sizeof (double)) == 0) {
    exit(0);
  }
  exit(1);
}" HAVE_COMPATIBLE_EXP)
set(CMAKE_REQUIRED_LIBRARIES)
set(CMAKE_REQUIRED_LIBRARIES m)
check_cxx_source_runs("
#include <math.h>
#include <stdlib.h>

int main(void) {
  if (!isnan(log(-10000))) {
    exit(1);
  }
  exit(0);
}" HAVE_COMPATIBLE_LOG)
set(CMAKE_REQUIRED_LIBRARIES)
set(CMAKE_REQUIRED_LIBRARIES m)
check_cxx_source_runs("
#include <math.h>
#include <stdlib.h>

int main(void) {
  if (!isnan(log10(-10000))) {
    exit(1);
  }
  exit(0);
}" HAVE_COMPATIBLE_LOG10)
set(CMAKE_REQUIRED_LIBRARIES)
check_cxx_source_runs("
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
  char* szFileName;
  FILE* pFile = NULL;
  int ret = 1;

  szFileName = tempnam(\".\", \"tmp\");

  /* open the file write-only */
  pFile = fopen(szFileName, \"a\");
  if (pFile == NULL)
  {
    exit(0);
  }
  if (ungetc('A', pFile) != EOF)
  {
    ret = 0;
  }
  unlink(szFileName);
  exit(ret);
}" UNGETC_NOT_RETURN_EOF)
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
  set(HAS_FTRUNCATE_LENGTH_ISSUE 1)
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
  set(HAS_FTRUNCATE_LENGTH_ISSUE 0)

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
  set(HAS_FTRUNCATE_LENGTH_ISSUE 0)
endif(CMAKE_SYSTEM_NAME STREQUAL Darwin)

if(NOT NO_ICU)
  if(NOT HAVE_LIBICU_UCHAR_H)
    unset(HAVE_LIBICU_UCHAR_H CACHE)
    message(FATAL_ERROR "Cannot find ICU. Try installing libicu-dev or the appropriate packages for your platform. You may also disable icu/unicode with '--no-icu' argument")
  endif()
endif()
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/pal/src/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/pal/src/config.h)

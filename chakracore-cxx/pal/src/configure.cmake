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

if(NOT NO_ICU)
  if(NOT HAVE_LIBICU_UCHAR_H)
    unset(HAVE_LIBICU_UCHAR_H CACHE)
    message(FATAL_ERROR "Cannot find ICU. Try installing libicu-dev or the appropriate packages for your platform. You may also disable icu/unicode with '--no-icu' argument")
  endif()
endif()

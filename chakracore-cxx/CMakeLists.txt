cmake_minimum_required(VERSION 3.28)
project (CHAKRACORE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g")
set(CMAKE_COLOR_DIAGNOSTICS ON)

option(DISABLE_JIT "Disable JIT compilation" OFF)
option(INTL_ICU "Enable Intl" ON)

if(CC_USES_SYSTEM_ARCH OR NOT CHAKRACORE_BUILD)
    if(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
        set(CC_TARGETS_AMD64 1)
    elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
        set(CC_TARGETS_ARM64 1)
    endif()
endif()

if(CC_TARGETS_AMD64)
elseif(CC_TARGETS_ARM64)
    add_definitions(-D_ARM64_=1)
    add_definitions(-D__arm64__=1)
    set(CMAKE_SYSTEM_PROCESSOr "arm64")
else()
    message(FATAL_ERROR "Unsupported target processor: ${CMAKE_SYSTEM_PROCESSOR}")
endif()

if(CMAKE_SYSTEM_NAME STREQUAL Linux)
    set(CC_TARGET_OS_LINUX 1)
elseif(CMAKE_SYSTEM_NAME STREQUAL Darwin)
    set(CC_TARGET_OS_OSX 1)
endif()

if(ICU_INCLUDE_PATH)
    add_definitions(-DHAS_REAL_ICU=1)
    add_definitions(-DHAS_ICU)
    set(ICU_LIBRARY_PATH "${ICU_INCLUDE_PATH}/../lib/")
    find_library(ICUUC icuuc PATHS ${ICU_LIBRARY_PATH} NO_DEFAULT_PATH)

    if(INTL_ICU)
        add_definitions(-DINTL_ICU=1)
        find_library(ICUIN icui18n PATHS ${ICU_LIBRARY_PATH} NO_DEFAULT_PATH)
        # In a default install, ICU header files are all in ICU_ROOT/include
        # However, for Node, the include/ folder is never generated, so we have to look
        # in NODE/deps/icu/source/{common|i18n} for headers
        set(ICU_INCLUDE_PATH
            "${ICU_INCLUDE_PATH}"
            "${ICU_INCLUDE_PATH}/../i18n/"
        )
    endif()

    if(ICUUC)
        message("-- found ICU libs: ${ICU_LIBRARY_PATH}")
        find_library(ICUDATA icudata PATHS ${ICU_LIBRARY_PATH} NO_DEFAULT_PATH)
        if (NOT ICUDATA)
            set(ICUDATA "")
        endif()
        set(ICU_LIBRARIES
            ${ICUUC}
            ${ICUIN}
            ${ICUDATA}
        )
    endif()
endif()

if(CC_TARGET_OS_LINUX)
    if(SYSTEM_ICU OR (NOT ICU_INCLUDE_PATH AND NOT NO_ICU))
        set(ICU_LIBRARIES "icuuc")
        if(INTL_ICU)
            add_definitions(-DINTL_ICU=1)
            set(ICU_LIBRARIES
                ${ICU_LIBRARIES}
                icui18n
            )
        endif()
        add_definitions(-DHAS_REAL_ICU=1)
        add_definitions(-DHAS_ICU)
    endif()
elseif(CC_TARGET_OS_OSX)
    # in case ICU path was given but it doesn't exist, build script will fail.
    # so, fallback only if ICU path wasn't given
    if(NOT ICU_INCLUDE_PATH)
        set(NO_ICU 1)
        message("-- Couldn't find ICU. Falling back to NO_ICU build")
    endif()
endif()

set(CLR_CMAKE_PLATFORM_XPLAT 1)
if(CC_TARGETS_AMD64)
    add_definitions(-DTARGET_64)
    add_compile_options(-msse4.2)

    if(NOT CMAKE_BUILD_TYPE STREQUAL Release)
        set(CAN_BUILD_WABT 1)
    endif()
elseif(CC_TARGETS_ARM64)
    add_definitions(-D__aarch64__)
    add_definitions(-DTARGET_64)
    add_definitions(-D_M_ARM32_OR_ARM64)
    if(CC_TARGET_OS_OSX)
        add_compile_options(-arch arm64)
    endif()
else()
    message(FATAL_ERROR "Only AMD64, ARM, and ARM64 are supported")
endif()

if(CAN_BUILD_WABT)
    add_definitions(-DCAN_BUILD_WABT)
endif()

if(CC_TARGET_OS_LINUX)
    set(CLR_CMAKE_PLATFORM_LINUX 1)
    # OSX 10.12 Clang deprecates libstdc++ [See GH #1599]
    # So, -Werror is linux only for now
    # + Android ARM ABI shows ld warnings
    # xplat-todo: Do we need this ?
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
        -Werror"
        )
elseif(CC_TARGET_OS_OSX)
    add_definitions(
        -DPLATFORM_UNIX
    )
else()
    message(FATAL_ERROR "Unsupported OS: ${CMAKE_SYSTEM_NAME}")
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL AppleClang
    OR CMAKE_CXX_COMPILER_ID STREQUAL Clang
    OR CMAKE_CXX_COMPILER_ID STREQUAL GNU)
endif()

# TODO (hanhossain): don't use add_definition for this, just use the CMake variable
add_definitions(-DCHAKRA_STATIC_LIBRARY=1)

if(CLR_CMAKE_PLATFORM_XPLAT)
    add_definitions(-D_CHAKRACOREBUILD)
    add_definitions(-DFEATURE_PAL)
    add_definitions(-DPLATFORM_UNIX=1)

    if(CLR_CMAKE_PLATFORM_LINUX)
        add_definitions(-D__LINUX__=1)
        if(CC_TARGETS_AMD64)
            add_definitions(-DLINUX64)
        endif(CC_TARGETS_AMD64)
    endif(CLR_CMAKE_PLATFORM_LINUX)

    if(CC_TARGETS_AMD64)
        set(IS_64BIT_BUILD 1)
        add_definitions(-D_M_X64 -D_M_AMD64 -D_AMD64_)
    elseif(CC_TARGETS_ARM64)
        set(IS_64BIT_BUILD 1)
        add_definitions(-D_M_ARM64 -D_ARM64_)
    endif()

    add_definitions(
        -DUNICODE
        -D_SAFECRT_USE_CPP_OVERLOADS=1
        -D__STDC_WANT_LIB_EXT1__=1
        )

    # todo: fix general visibility of the interface
    # do not set to `fvisibility=hidden` as it is going to
    # prevent the required interface is being exported
    # clang by default sets fvisibility=default

    # CXX WARNING FLAGS
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
        -Wno-ignored-attributes\
        -Wno-parentheses-equality\
        -Wno-reorder\
        -Wno-microsoft\
        -Wno-unused-value\
        -Wno-int-to-void-pointer-cast\
        -Wno-invalid-offsetof\
        -Wno-undefined-inline\
        -Wno-inconsistent-missing-override\
        -Wno-c++14-extensions\
        -Wno-macro-redefined\
        -Wno-pragmas\
        -Wno-invalid-token-paste\
        -Wno-format\
        -Wno-invalid-noreturn\
        -Wno-null-arithmetic\
        -Wno-tautological-constant-out-of-range-compare\
        -Wno-tautological-undefined-compare\
        -Wno-address-of-temporary\
        -Wno-null-conversion\
        -Wno-return-type\
        -Wno-switch\
        -Wno-int-to-pointer-cast\
        -Wno-tautological-constant-compare\
        -Wno-enum-compare-switch\
        -Wno-unknown-warning-option"
    )
        # notes..
        # -Wno-address-of-temporary  # vtinfo.h, VirtualTableInfo<T>::RegisterVirtualTable
        # -Wno-null-conversion # Check shmemory.cpp and cs.cpp here...
        # -Wno-return-type # switch unreachable code
        # -Wno-switch # switch values not handled
        # -W-enum-compare-switch # throws warning on enum1 == enum2 where both
        # enums represent same value but do not share the type. ie. we use both AsmJsType
        # and AsmJsRetType interchangably and filter things out by `default:` case.
        # -W-tautological-constant-compare throws warning for checks known at compile time.
        # Some of those checks are optimized out during compile / lto time, and others
        # are platform / compiler dependent.
        # -Wno-unknown-warning-option ... well, some of the new switches are not
        # recognized by older compilers and they fail. So, put this one and not fail

    include(Build/CMakeFeatureDetect.cmake)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
        ${CXX_DO_NOT_OPTIMIZE_SIBLING_CALLS} \
        -fno-omit-frame-pointer \
        -fdelayed-template-parsing"
        )

    # CXX / CC COMPILER FLAGS
    add_compile_options(
        -fasm-blocks
        -fms-extensions
        -fwrapv # Treat signed integer overflow as two's complement
    )

    # Only disable RTTI in release builds so that TrackAlloc works for debug and test builds
    # Also disable RTTI when building a shared library
    # TODO: why does the shared library break with rtti disabled?
    if(CMAKE_BUILD_TYPE STREQUAL Release)
        add_compile_options(-fno-rtti)
    endif()
endif(CLR_CMAKE_PLATFORM_XPLAT)

if(CMAKE_BUILD_TYPE STREQUAL Debug)
    add_definitions(
        -DDBG=1
        -DDEBUG=1
        -D_DEBUG=1 # for PAL
        -DDBG_DUMP=1
    )
elseif(CMAKE_BUILD_TYPE STREQUAL RelWithDebInfo)
    add_definitions(
        -DENABLE_DEBUG_CONFIG_OPTIONS=1
    )
endif(CMAKE_BUILD_TYPE STREQUAL Debug)

if(IS_64BIT_BUILD)
    add_definitions(
        -DBIT64=1
        -DSTACK_ALIGN=16
    )
endif(IS_64BIT_BUILD)

if(DISABLE_JIT)
    set(BuildJIT 0)
    add_definitions(-DDISABLE_JIT=1)
else()
    set(BuildJIT 1)
endif()

enable_language(ASM)

if(CMAKE_SYSTEM_NAME STREQUAL Darwin)
    set(DYN_LIB_EXT "dylib")
else()
    set(DYN_LIB_EXT "so")
endif()

if(CC_TARGETS_ARM64)
    if(CC_TARGET_OS_LINUX)
        message(WARNING "ARM64 linux build has not yet been tested, this build is unsupported.")
    endif()
    if(BuildJIT)
        message(WARNING "ARM64 Jit not yet functional on platforms other than windows.")
        message(WARNING "For use rather than development please build with Jit disabled --no-jit with ./build.sh or -DDISABLE_JIT=1 if using CMake directly")
    endif()
endif()

# TODO (hanhossain): remove include_directories in favor of target_include_directories
include_directories(
    pal
    pal/inc
    pal/inc/rt
    ${ICU_INCLUDE_PATH}
    )

if(ICU_INCLUDE_PATH)
    if(NOT HAVE_LIBICU_UCHAR_H)
        set(HAVE_LIBICU_UCHAR_H 1)
    endif()
endif()

# detect features
# TODO (hanhossain): remove include_directories in favor of target_include_directories
include_directories(SYSTEM /usr/local/include)
include(pal/src/configure.cmake)

add_subdirectory (pal)

# build the rest with NO_PAL_MINMAX and PAL_STDCPP_COMPAT
add_definitions(
    -DNO_PAL_MINMAX
    -DPAL_STDCPP_COMPAT
    )

add_subdirectory (lib)

add_subdirectory (bin)

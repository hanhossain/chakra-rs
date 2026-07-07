cmake_minimum_required(VERSION 3.28)
project(CHAKRACORE)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g")

option(DISABLE_JIT "Disable JIT compilation" OFF)
option(IN_DOCKER "Running in docker" OFF)

if (CC_USES_SYSTEM_ARCH OR NOT CHAKRACORE_BUILD)
    if (CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
        set(CC_TARGETS_AMD64 1)
    elseif (CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
        set(CC_TARGETS_ARM64 1)
    endif ()
endif ()

if (CC_TARGETS_AMD64)
elseif (CC_TARGETS_ARM64)
    add_definitions(-D_ARM64_=1)
    add_definitions(-D__arm64__=1)
    set(CMAKE_SYSTEM_PROCESSOr "arm64")
else ()
    message(FATAL_ERROR "Unsupported target processor: ${CMAKE_SYSTEM_PROCESSOR}")
endif ()

if (CMAKE_SYSTEM_NAME STREQUAL Linux)
    set(CC_TARGET_OS_LINUX 1)
elseif (CMAKE_SYSTEM_NAME STREQUAL Darwin)
    set(CC_TARGET_OS_OSX 1)
    if (NOT ICU_INCLUDE_PATH)
        set(ICU_INCLUDE_PATH "/opt/homebrew/opt/icu4c/include")
    endif()
    if (NOT DISABLE_JIT)
        set(DISABLE_JIT ON)
    endif()
endif ()

if (ICU_INCLUDE_PATH)
    add_definitions(-DHAS_ICU)
    set(ICU_LIBRARY_PATH "${ICU_INCLUDE_PATH}/../lib/")
    find_library(ICUUC icuuc PATHS ${ICU_LIBRARY_PATH} NO_DEFAULT_PATH)

    find_library(ICUIN icui18n PATHS ${ICU_LIBRARY_PATH} NO_DEFAULT_PATH)
    # In a default install, ICU header files are all in ICU_ROOT/include
    # However, for Node, the include/ folder is never generated, so we have to look
    # in NODE/deps/icu/source/{common|i18n} for headers
    set(ICU_INCLUDE_PATH
        "${ICU_INCLUDE_PATH}"
        "${ICU_INCLUDE_PATH}/../i18n/"
    )

    if (ICUUC)
        message("-- found ICU libs: ${ICU_LIBRARY_PATH}")
        set(ICU_LIBRARIES
            ${ICUUC}
            ${ICUIN}
        )
    endif ()
endif ()

if (SYSTEM_ICU OR (NOT ICU_INCLUDE_PATH))
    set(ICU_LIBRARIES "icuuc")
    set(ICU_LIBRARIES
        ${ICU_LIBRARIES}
        icui18n
    )
    add_definitions(-DHAS_ICU)
endif ()

if (CC_TARGETS_AMD64)
    add_definitions(-DTARGET_64)
    add_compile_options(-msse4.2)

    if (NOT CMAKE_BUILD_TYPE STREQUAL Release)
        set(CAN_BUILD_WABT 1)
    endif ()
elseif (CC_TARGETS_ARM64)
    add_definitions(-D__aarch64__)
    add_definitions(-DTARGET_64)
    add_definitions(-D_M_ARM32_OR_ARM64)
    if (CC_TARGET_OS_OSX)
        add_compile_options(-arch arm64)
    endif ()
else ()
    message(FATAL_ERROR "Only AMD64, ARM, and ARM64 are supported")
endif ()

if (CAN_BUILD_WABT)
    add_definitions(-DCAN_BUILD_WABT)
endif ()

if (CMAKE_CXX_COMPILER_ID STREQUAL AppleClang
    OR CMAKE_CXX_COMPILER_ID STREQUAL Clang
    OR CMAKE_CXX_COMPILER_ID STREQUAL GNU)
endif ()

add_definitions(-DCHAKRA_STATIC_LIBRARY=1)

add_definitions(-DFEATURE_PAL)

if (CC_TARGET_OS_LINUX)
    add_definitions(-D__LINUX__=1)
    if (CC_TARGETS_AMD64)
        add_definitions(-DLINUX64)
    endif (CC_TARGETS_AMD64)
endif ()

if (CC_TARGETS_AMD64)
    set(IS_64BIT_BUILD 1)
    add_definitions(-D_M_X64 -D_M_AMD64 -D_AMD64_)
elseif (CC_TARGETS_ARM64)
    set(IS_64BIT_BUILD 1)
    add_definitions(-D_M_ARM64 -D_ARM64_)
endif ()

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
add_compile_options(-Wall)
add_compile_options(-Wextra)

# ignore in general
add_compile_options(-Wno-delayed-template-parsing-in-cxx20)
add_compile_options(-Wno-macro-redefined)
add_compile_options(-Wno-null-conversion)
add_compile_options(-Wno-tautological-undefined-compare)
add_compile_options(-Wno-invalid-token-paste)
add_compile_options(-Wno-pragmas)
add_compile_options(-Wno-inconsistent-missing-override)
add_compile_options(-Wno-int-to-void-pointer-cast)
add_compile_options(-Wno-undefined-inline)
add_compile_options(-Wno-switch)
add_compile_options(-Wno-return-type)
add_compile_options(-Wno-invalid-offsetof)
add_compile_options(-Wno-format)
add_compile_options(-Wno-tautological-constant-out-of-range-compare)
add_compile_options(-Wno-address-of-temporary)
add_compile_options(-Wno-null-arithmetic)
add_compile_options(-Wno-int-to-pointer-cast)
add_compile_options(-Wno-enum-compare-switch)
add_compile_options(-Wno-implicit-const-int-float-conversion)
add_compile_options(-Wno-nontrivial-memaccess)

# ignore for -Wall
add_compile_options(-Wno-reorder-ctor)
add_compile_options(-Wno-overloaded-virtual)
add_compile_options(-Wno-tautological-overlap-compare)
add_compile_options(-Wno-sometimes-uninitialized)

# ignore for -Wextra
add_compile_options(-Wno-sign-compare)
add_compile_options(-Wno-missing-field-initializers)
add_compile_options(-Wno-ignored-qualifiers)
add_compile_options(-Wno-cast-function-type-mismatch)
add_compile_options(-Wno-deprecated-copy-with-user-provided-copy)

# this keeps causing issues with the docker build even if paths are correct
add_compile_options(-Wno-nonportable-include-path)

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

add_compile_options(-fno-omit-frame-pointer)
add_compile_options(-fdelayed-template-parsing)

# CXX / CC COMPILER FLAGS
add_compile_options(
    -fasm-blocks
    -fms-extensions
    -fwrapv # Treat signed integer overflow as two's complement
)

# Only disable RTTI in release builds so that TrackAlloc works for debug and test builds
# Also disable RTTI when building a shared library
# TODO: why does the shared library break with rtti disabled?
if (CMAKE_BUILD_TYPE STREQUAL Release)
    add_compile_options(-fno-rtti)
endif ()

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    add_definitions(
        -DDBG=1
        -DDEBUG=1
        -D_DEBUG=1 # for PAL
        -DDBG_DUMP=1
    )
endif (CMAKE_BUILD_TYPE STREQUAL Debug)

if (IS_64BIT_BUILD)
    add_definitions(
        -DBIT64=1
    )
endif (IS_64BIT_BUILD)

if (DISABLE_JIT)
    set(BuildJIT 0)
    add_definitions(-DDISABLE_JIT=1)
else ()
    set(BuildJIT 1)
endif ()

enable_language(ASM)

if (CMAKE_SYSTEM_NAME STREQUAL Darwin)
    set(DYN_LIB_EXT "dylib")
else ()
    set(DYN_LIB_EXT "so")
endif ()

if (CC_TARGETS_ARM64)
    if (CC_TARGET_OS_LINUX)
        message(WARNING "ARM64 linux build has not yet been tested, this build is unsupported.")
    endif ()
    if (BuildJIT)
        message(WARNING "ARM64 Jit not yet functional on platforms other than windows.")
        message(WARNING "For use rather than development please build with Jit disabled --no-jit with ./build.sh or -DDISABLE_JIT=1 if using CMake directly")
    endif ()
endif ()

include_directories(
    ${ICU_INCLUDE_PATH}
    ${CMAKE_SOURCE_DIR}/target/cxxbridge
)

if (IN_DOCKER)
    include_directories(${CMAKE_SOURCE_DIR}/target/docker)
endif ()

# detect features
include_directories(SYSTEM /usr/local/include)

add_subdirectory(pal)

add_subdirectory(lib)

add_subdirectory(bin)

project(CHAKRA_PLATFORM_AGNOSTIC)

set(PL_SOURCE_FILES
  Common/UnicodeText.Common.cpp
  Common/HiResTimer.cpp
  Common/DateTime.cpp
  Linux/NumbersUtility.cpp
  POSIX/ArraysUtility.cpp
  Common/Thread.cpp
  Common/SystemInfo.Common.cpp
  )

if(NO_ICU)
  set(PL_SOURCE_FILES
    ${PL_SOURCE_FILES}
    POSIX/UnicodeText.cpp
  )
else()
  set(PL_SOURCE_FILES
    ${PL_SOURCE_FILES}
    Common/UnicodeText.ICU.cpp
  )
endif()

if(CC_TARGET_OS_LINUX)
  set(PL_SOURCE_FILES ${PL_SOURCE_FILES}
    Linux/SystemInfo.cpp
    Linux/PerfTrace.cpp
    )
elseif(CC_TARGET_OS_OSX)
  set(PL_SOURCE_FILES ${PL_SOURCE_FILES}
    Unix/AssemblyCommon.cpp
    Unix/SystemInfo.cpp
    # Linux/PerfTrace.cpp # TODO : implement for OSX?
    )
endif()

add_library (Chakra.Runtime.PlatformAgnostic OBJECT
  ${PL_SOURCE_FILES}
  )

# TODO (hanhossain): remove most directory paths
target_include_directories (Chakra.Runtime.PlatformAgnostic PUBLIC
#  Chakra.Runtime.PlatformAgnostic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
  ${CHAKRACORE_SOURCE_DIR}/pal
    ${CHAKRACORE_SOURCE_DIR}/pal/inc
    ${CHAKRACORE_SOURCE_DIR}/pal/inc/rt
#    ${CHAKRACORE_SOURCE_DIR}/pal/src
#    ${CHAKRACORE_SOURCE_DIR}/pal/src/include
#    ${CHAKRACORE_SOURCE_DIR}/lib/Common/PlaceHolder
#    ${CHAKRACORE_SOURCE_DIR}/bin
#    ${CHAKRACORE_SOURCE_DIR}/lib/Runtime
#    ${CHAKRACORE_SOURCE_DIR}/lib/JITIDL
    ${CHAKRACORE_SOURCE_DIR}/lib/Common
#    ${CHAKRACORE_SOURCE_DIR}/lib/Backend
#    ${CHAKRACORE_SOURCE_DIR}/lib/Parser
#    ${CHAKRACORE_SOURCE_DIR}/lib/WasmReader
#    ${CHAKRACORE_SOURCE_DIR}/lib/Runtime/ByteCode
    ${CHAKRACORE_SOURCE_DIR}/lib/Runtime/PlatformAgnostic
#    ${CHAKRACORE_SOURCE_DIR}/lib/Runtime/Math
  )

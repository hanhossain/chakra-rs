if(CC_TARGETS_ARM64)
    set(ARCH_SOURCES arm64/arm64_GET_CURRENT_FRAME.S)
endif()

add_library (Chakra.Common.Common OBJECT
    CfgLogger.cpp
    CommonCommonPch.cpp
    DateUtilities.cpp
    Event.cpp
    Int32Math.cpp
    Int64Math.cpp
    Jobs.cpp
    MathUtil.cpp
    NumberUtilities.cpp
    NumberUtilities_strtod.cpp
    RejitReason.cpp
    SmartFpuControl.cpp
    Tick.cpp
    vtinfo.cpp
    ${ARCH_SOURCES}
)

# TODO (hanhossain): remove most directory paths
target_include_directories (Chakra.Common.Common PUBLIC
#    Chakra.Common.Common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    ${CHAKRACORE_SOURCE_DIR}/pal
    ${CHAKRACORE_SOURCE_DIR}/pal/inc
    ${CHAKRACORE_SOURCE_DIR}/pal/inc/rt
#    ${CHAKRACORE_SOURCE_DIR}/pal/src
#    ${CHAKRACORE_SOURCE_DIR}/pal/src/include
    ${CHAKRACORE_SOURCE_DIR}/lib/Common
    ${CHAKRACORE_SOURCE_DIR}/lib/Common/PlaceHolder
#    ${CHAKRACORE_SOURCE_DIR}/bin
#    ${CHAKRACORE_SOURCE_DIR}/lib/Runtime
#    ${CHAKRACORE_SOURCE_DIR}/lib/JITIDL
#    ${CHAKRACORE_SOURCE_DIR}/lib/Common
   ${CHAKRACORE_SOURCE_DIR}/lib/Backend
#    ${CHAKRACORE_SOURCE_DIR}/lib/Parser
#    ${CHAKRACORE_SOURCE_DIR}/lib/WasmReader
#    ${CHAKRACORE_SOURCE_DIR}/lib/Runtime/ByteCode
#    ${CHAKRACORE_SOURCE_DIR}/lib/Runtime/PlatformAgnostic
#    ${CHAKRACORE_SOURCE_DIR}/lib/Runtime/Math
    )

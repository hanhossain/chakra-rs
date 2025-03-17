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

# TODO (hanhossain): cleanup includes
target_include_directories (Chakra.Common.Common PUBLIC
    ${CHAKRACORE_SOURCE_DIR}/pal
    ${CHAKRACORE_SOURCE_DIR}/pal/inc
    ${CHAKRACORE_SOURCE_DIR}/pal/inc/rt
    ${CHAKRACORE_SOURCE_DIR}/lib/Common
    ${CHAKRACORE_SOURCE_DIR}/lib/Common/Interface/include
    ${CHAKRACORE_SOURCE_DIR}/lib/Common/PlaceHolder
    ${CHAKRACORE_SOURCE_DIR}/lib/Common/PlatformAgnostic/include
    ${CHAKRACORE_SOURCE_DIR}/lib/Backend
)

target_precompile_headers(Chakra.Common.Common PUBLIC CommonCommonPch.h)

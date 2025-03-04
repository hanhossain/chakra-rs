if(CC_TARGETS_ARM64)
    set(ARCH_SOURCES arm64/arm64_GET_CURRENT_FRAME.S)
endif()

add_library (Chakra.Common.Common STATIC
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

# TODO: (hanhossain): cleanup
target_include_directories (Chakra.Common.Common PUBLIC
    # ${CHAKRACORE_SOURCE_DIR}/lib/Common
    # ${CHAKRACORE_SOURCE_DIR}/lib/Common/PlaceHolder
    # ${CHAKRACORE_SOURCE_DIR}/lib/Backend
)

target_link_libraries(Chakra.Common.Common PUBLIC
    Chakra.Common.PlatformAgnostic
)

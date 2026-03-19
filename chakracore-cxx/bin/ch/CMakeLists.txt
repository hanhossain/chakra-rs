add_library(chhelper
    chhelper.cpp
    ChakraRtInterface.cpp
    Helpers.cpp
    HostConfigFlags.cpp
    WScriptJsrt.cpp
    RuntimeThreadData.cpp
    ../ChakraCore/TestHooks.cpp
)

# TODO (hanhossain): cleanup includes
target_include_directories(chhelper PUBLIC
    ${CXXBRIDGE_INCLUDE_DIR}
    ${CHAKRACORE_SOURCE_DIR}/lib/Common/PlaceHolder
    ${CHAKRACORE_SOURCE_DIR}/pal/inc
    ${CHAKRACORE_SOURCE_DIR}/pal/inc/rt
    ${CHAKRACORE_SOURCE_DIR}/lib/Runtime
    ${CHAKRACORE_SOURCE_DIR}/lib/Jsrt
    ${CHAKRACORE_SOURCE_DIR}/lib/JITClient
    ${CHAKRACORE_SOURCE_DIR}/lib/JITIDL
    ${CHAKRACORE_SOURCE_DIR}/lib/Common
    ${CHAKRACORE_SOURCE_DIR}/lib/Common/Interface/include
    ${CHAKRACORE_SOURCE_DIR}/lib/Common/PlatformAgnostic/include
    ${CHAKRACORE_SOURCE_DIR}/bin/ChakraCore
    ${CHAKRACORE_SOURCE_DIR}/lib/Backend
    ${CHAKRACORE_SOURCE_DIR}/lib/Parser
    ${CHAKRACORE_SOURCE_DIR}/lib/WasmReader
    ${CHAKRACORE_SOURCE_DIR}/lib/Runtime/ByteCode
)

target_link_libraries(chhelper PUBLIC ChakraCoreStatic)

target_compile_definitions(chhelper PUBLIC
    PAL_STDCPP_COMPAT
)

target_precompile_headers(chhelper PUBLIC stdafx.h)

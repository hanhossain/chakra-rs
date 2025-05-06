add_library (Chakra.Jsrt STATIC
    Jsrt.cpp
    JsrtDebugUtils.cpp
    JsrtDebugManager.cpp
    JsrtDebuggerObject.cpp
    JsrtDiag.cpp
    JsrtContext.cpp
    JsrtExternalArrayBuffer.cpp
    JsrtExternalObject.cpp
    JsrtDebugEventObject.cpp
    JsrtHelper.cpp
    JsrtPch.cpp
    JsrtRuntime.cpp
    JsrtSourceHolder.cpp
    JsrtThreadService.cpp
    )

add_subdirectory(Core)

# TODO (hanhossain): cleanup includes
target_include_directories (Chakra.Jsrt PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CHAKRACORE_SOURCE_DIR}/pal
    ${CHAKRACORE_SOURCE_DIR}/pal/inc
    ${CHAKRACORE_SOURCE_DIR}/pal/inc/rt
    ${CHAKRACORE_SOURCE_DIR}/lib/Common/PlaceHolder
    ${CHAKRACORE_SOURCE_DIR}/lib/Runtime
    ${CHAKRACORE_SOURCE_DIR}/lib/JITClient
    ${CHAKRACORE_SOURCE_DIR}/lib/JITIDL
    ${CHAKRACORE_SOURCE_DIR}/lib/Common
    ${CHAKRACORE_SOURCE_DIR}/lib/Common/Interface/include
    ${CHAKRACORE_SOURCE_DIR}/lib/Common/PlatformAgnostic/include
    ${CHAKRACORE_SOURCE_DIR}/lib/Backend
    ${CHAKRACORE_SOURCE_DIR}/lib/Parser
    ${CHAKRACORE_SOURCE_DIR}/lib/WasmReader
    ${CHAKRACORE_SOURCE_DIR}/lib/Runtime/ByteCode
    ${CHAKRACORE_SOURCE_DIR}/lib/Runtime/Base
    ${CHAKRACORE_SOURCE_DIR}/lib/Runtime/Debug
)

target_link_libraries(Chakra.Jsrt
  PUBLIC
  Chakra.Pal
)

target_compile_definitions(Chakra.Jsrt PUBLIC
    PAL_STDCPP_COMPAT
    NO_PAL_MINMAX)

target_precompile_headers(Chakra.Jsrt PUBLIC JsrtPch.h)

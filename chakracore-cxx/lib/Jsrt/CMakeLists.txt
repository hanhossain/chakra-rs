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

# TODO (hanhossain): remove most directory paths
#target_include_directories (
#    Chakra.Jsrt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
#    ../Backend
#    ../Common
#    ../JITIDL
#    ../Runtime
#    ../Runtime/Base
#    ../Runtime/ByteCode
#    ../Runtime/Debug
#    ../Parser
#    ../SCACore
#    ${CHAKRACORE_SOURCE_DIR}/pal
#    ${CHAKRACORE_SOURCE_DIR}/pal/inc
#    ${CHAKRACORE_SOURCE_DIR}/pal/inc/rt
#    ${CHAKRACORE_SOURCE_DIR}/pal/src
#    ${CHAKRACORE_SOURCE_DIR}/pal/src/include
#    ${CHAKRACORE_SOURCE_DIR}/lib/Common/PlaceHolder
#    ${CHAKRACORE_SOURCE_DIR}/bin
#    ${CHAKRACORE_SOURCE_DIR}/lib/Runtime
#    ${CHAKRACORE_SOURCE_DIR}/lib/JITIDL
#    ${CHAKRACORE_SOURCE_DIR}/lib/Common
#    ${CHAKRACORE_SOURCE_DIR}/lib/Backend
#    ${CHAKRACORE_SOURCE_DIR}/lib/Parser
#    ${CHAKRACORE_SOURCE_DIR}/lib/WasmReader
#    ${CHAKRACORE_SOURCE_DIR}/lib/Runtime/ByteCode
#    ${CHAKRACORE_SOURCE_DIR}/lib/Runtime/PlatformAgnostic
#    ${CHAKRACORE_SOURCE_DIR}/lib/Runtime/Math
#    )

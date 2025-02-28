add_library (Chakra.Runtime.Math OBJECT
    AsmJsMath.cpp
    RuntimeMathPch.cpp
    )

# TODO (hanhossain): remove most directory paths
target_include_directories (
    Chakra.Runtime.Math PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    ${CHAKRACORE_SOURCE_DIR}/pal
    ${CHAKRACORE_SOURCE_DIR}/pal/inc
    ${CHAKRACORE_SOURCE_DIR}/pal/inc/rt
    ${CHAKRACORE_SOURCE_DIR}/pal/src
    ${CHAKRACORE_SOURCE_DIR}/pal/src/include
    ${CHAKRACORE_SOURCE_DIR}/lib/Common/PlaceHolder
    ${CHAKRACORE_SOURCE_DIR}/bin
    ${CHAKRACORE_SOURCE_DIR}/lib/Runtime
    ${CHAKRACORE_SOURCE_DIR}/lib/JITIDL
    ${CHAKRACORE_SOURCE_DIR}/lib/Common
    ${CHAKRACORE_SOURCE_DIR}/lib/Backend
    ${CHAKRACORE_SOURCE_DIR}/lib/Parser
    ${CHAKRACORE_SOURCE_DIR}/lib/WasmReader
    ${CHAKRACORE_SOURCE_DIR}/lib/Runtime/ByteCode
    ${CHAKRACORE_SOURCE_DIR}/lib/Runtime/PlatformAgnostic
    ${CHAKRACORE_SOURCE_DIR}/lib/Runtime/Math)

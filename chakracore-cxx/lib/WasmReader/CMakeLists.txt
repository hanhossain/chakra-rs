add_library (Chakra.WasmReader OBJECT
    WasmParseTree.cpp
    WasmBinaryReader.cpp
    WasmByteCodeGenerator.cpp
    WasmCustomReader.cpp
    WasmDataSegment.cpp
    WasmElementSegment.cpp
    WasmFunctionInfo.cpp
    WasmGlobal.cpp
    WasmReaderPch.cpp
    WasmSection.cpp
    WasmSignature.cpp
    )

# TODO (hanhossain): remove most directory paths
target_include_directories (
    Chakra.WasmReader PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    ../Common
    ../JITIDL
    ../Backend
    ../Runtime
    ../Runtime/ByteCode
    ../Parser
    ${CHAKRACORE_SOURCE_DIR}/pal
    ${CHAKRACORE_SOURCE_DIR}/pal/inc
    ${CHAKRACORE_SOURCE_DIR}/pal/inc/rt
    ${CHAKRACORE_SOURCE_DIR}/pal/src
    ${CHAKRACORE_SOURCE_DIR}/pal/src/include
    )

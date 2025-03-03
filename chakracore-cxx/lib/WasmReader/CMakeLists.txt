add_library (Chakra.WasmReader STATIC
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

# TODO: (hanhossain): cleanup
target_include_directories (Chakra.WasmReader PUBLIC
    # ${CHAKRACORE_SOURCE_DIR}/pal
    # ${CHAKRACORE_SOURCE_DIR}/pal/inc
    # ${CHAKRACORE_SOURCE_DIR}/pal/inc/rt
    # ${CHAKRACORE_SOURCE_DIR}/lib/Common/PlaceHolder
    # ${CHAKRACORE_SOURCE_DIR}/lib/Runtime
    # ${CHAKRACORE_SOURCE_DIR}/lib/JITClient
    # ${CHAKRACORE_SOURCE_DIR}/lib/JITIDL
    # ${CHAKRACORE_SOURCE_DIR}/lib/Common
    # ${CHAKRACORE_SOURCE_DIR}/lib/Backend
    # ${CHAKRACORE_SOURCE_DIR}/lib/Parser
    # ${CHAKRACORE_SOURCE_DIR}/lib/WasmReader
    # ${CHAKRACORE_SOURCE_DIR}/lib/Runtime/ByteCode
)

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
# target_include_directories (
#     Chakra.WasmReader PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
#     ../JITIDL
#     ../Backend
#     ../Runtime
#     ../Runtime/ByteCode
#     ../Parser
#     )

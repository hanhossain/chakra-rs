add_library (Chakra.Runtime.ByteCode STATIC
    AsmJsByteCodeDumper.cpp
    AsmJsByteCodeWriter.cpp
    WasmByteCodeWriter.cpp
    BackendOpCodeAttr.cpp
    ByteBlock.cpp
    ByteCodeDumper.cpp
    ByteCodeEmitter.cpp
    ByteCodeGenerator.cpp
    ByteCodeReader.cpp
    ByteCodeSerializer.cpp
    ByteCodeWriter.cpp
    FuncInfo.cpp
    OpCodeUtil.cpp
    OpCodeUtilAsmJs.cpp
    OpCodes.cpp
    RuntimeByteCodePch.cpp
    Scope.cpp
    ScopeInfo.cpp
    StatementReader.cpp
    Symbol.cpp
    )

# TODO: (hanhossain): cleanup
target_include_directories (Chakra.Runtime.ByteCode PUBLIC
    # ${CMAKE_CURRENT_SOURCE_DIR}
    # ${CHAKRACORE_SOURCE_DIR}/lib/Common/PlaceHolder
    # ${CHAKRACORE_SOURCE_DIR}/lib/Runtime
    # ${CHAKRACORE_SOURCE_DIR}/lib/JITClient
    # ${CHAKRACORE_SOURCE_DIR}/lib/JITIDL
    # ${CHAKRACORE_SOURCE_DIR}/lib/Common
    # ${CHAKRACORE_SOURCE_DIR}/lib/Backend
    # ${CHAKRACORE_SOURCE_DIR}/lib/Parser
    # ${CHAKRACORE_SOURCE_DIR}/lib/WasmReader
)

target_link_libraries(Chakra.Runtime.ByteCode PUBLIC
    Chakra.Common.PlatformAgnostic
)

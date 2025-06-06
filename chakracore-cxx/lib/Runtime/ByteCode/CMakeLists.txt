add_library (Chakra.Runtime.ByteCode OBJECT
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

# TODO (hanhossain): cleanup includes
target_include_directories (Chakra.Runtime.ByteCode PUBLIC
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
)

target_compile_definitions(Chakra.Runtime.ByteCode PUBLIC
    PAL_STDCPP_COMPAT
    NO_PAL_MINMAX)

target_precompile_headers(Chakra.Runtime.ByteCode PUBLIC RuntimeByteCodePch.h)

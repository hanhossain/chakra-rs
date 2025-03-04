add_library (Chakra.Runtime.Math STATIC
    AsmJsMath.cpp
    RuntimeMathPch.cpp
    )

# TODO: (hanhossain): cleanup
target_include_directories (Chakra.Runtime.Math PUBLIC
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

target_link_libraries(Chakra.Runtime.Math PUBLIC
    Chakra.Common.PlatformAgnostic
)

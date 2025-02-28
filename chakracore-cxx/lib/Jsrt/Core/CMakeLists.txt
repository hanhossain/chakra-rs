add_library (Chakra.Jsrt.Core OBJECT
    JsrtContextCore.cpp
    JsrtCore.cpp
    )

# TODO (hanhossain): remove most directory paths
target_include_directories (
    Chakra.Jsrt.Core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    ../../Backend
    ../../Common
    ../../JITIDL
    ../../Runtime
    ../../Runtime/ByteCode
    ../../Parser
    ../../SCACore
    ../
    ${CHAKRACORE_SOURCE_DIR}/pal
    ${CHAKRACORE_SOURCE_DIR}/pal/inc
    ${CHAKRACORE_SOURCE_DIR}/pal/inc/rt
    ${CHAKRACORE_SOURCE_DIR}/pal/src
    ${CHAKRACORE_SOURCE_DIR}/pal/src/include
    )

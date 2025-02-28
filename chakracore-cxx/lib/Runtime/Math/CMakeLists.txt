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
    ${CHAKRACORE_SOURCE_DIR}/pal/src/include)

add_library (Chakra.Common.Codex OBJECT
    Utf8Codex.cpp)

# TODO (hanhossain): remove most directory paths
target_include_directories (
    Chakra.Common.Codex PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    ${CHAKRACORE_SOURCE_DIR}/pal
    ${CHAKRACORE_SOURCE_DIR}/pal/inc
    ${CHAKRACORE_SOURCE_DIR}/pal/inc/rt
    ${CHAKRACORE_SOURCE_DIR}/pal/src
    ${CHAKRACORE_SOURCE_DIR}/pal/src/include)

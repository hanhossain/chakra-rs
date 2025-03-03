add_library (Chakra.Common.Codex STATIC
    Utf8Codex.cpp)

target_include_directories (Chakra.Common.Codex PUBLIC
    ${CHAKRACORE_SOURCE_DIR}/pal
    ${CHAKRACORE_SOURCE_DIR}/pal/inc
)

add_library (Chakra.Common.Codex OBJECT
    Utf8Codex.cpp)

target_include_directories (Chakra.Common.Codex PUBLIC
    ${CHAKRACORE_SOURCE_DIR}/pal/inc
)

target_compile_definitions(Chakra.Common.Codex PUBLIC
    PAL_STDCPP_COMPAT
    NO_PAL_MINMAX)

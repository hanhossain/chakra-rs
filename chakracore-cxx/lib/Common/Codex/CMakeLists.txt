add_library (Chakra.Common.Codex STATIC
    Utf8Codex.cpp)

# TODO: (hanhossain): cleanup
target_include_directories (Chakra.Common.Codex PUBLIC
    # ${CHAKRACORE_SOURCE_DIR}/pal
    # ${CHAKRACORE_SOURCE_DIR}/pal/inc
)

target_link_libraries(Chakra.Common.Codex PUBLIC
    Chakra.Common.PlatformAgnostic
)

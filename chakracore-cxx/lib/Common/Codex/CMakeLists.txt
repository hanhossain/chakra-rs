add_library (Chakra.Common.Codex STATIC
    Utf8Codex.cpp)

# TODO: (hanhossain): cleanup
target_include_directories (Chakra.Common.Codex PUBLIC)

target_link_libraries(Chakra.Common.Codex PUBLIC
    Chakra.Common.PlatformAgnostic
)

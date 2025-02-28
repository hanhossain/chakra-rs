add_library (Chakra.Common.Codex STATIC
    Utf8Codex.cpp)

target_link_libraries(Chakra.Common.Codex
    PUBLIC Chakra.Common.PlatformAgnostic)

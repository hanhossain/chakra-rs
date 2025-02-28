add_library (Chakra.Common.Codex STATIC
    Utf8Codex.cpp)

# TODO (hanhossain): remove most directory paths
# target_include_directories (
#     Chakra.Common.Codex PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Chakra.Common.Codex
    PUBLIC Chakra.Pal)

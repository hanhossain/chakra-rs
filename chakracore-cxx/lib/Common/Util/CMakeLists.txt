add_library (Chakra.Common.Util OBJECT
    Pinned.cpp)

# TODO (hanhossain): remove most directory paths
target_include_directories (
    Chakra.Common.Util PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

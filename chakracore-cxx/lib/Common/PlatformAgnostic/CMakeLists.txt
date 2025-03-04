add_library(Chakra.Common.PlatformAgnostic INTERFACE)
target_include_directories(Chakra.Common.PlatformAgnostic INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(Chakra.Common.PlatformAgnostic INTERFACE Chakra.Pal)

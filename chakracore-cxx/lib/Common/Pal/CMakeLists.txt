add_library(Chakra.Common.Pal INTERFACE)
target_include_directories(Chakra.Common.Pal INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Chakra.Common.Pal INTERFACE Chakra.Pal)

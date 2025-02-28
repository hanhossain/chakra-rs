add_library(Chakra.Common.Primitives INTERFACE)
target_include_directories(Chakra.Common.Primitives INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Chakra.Common.Primitives INTERFACE Chakra.Pal)

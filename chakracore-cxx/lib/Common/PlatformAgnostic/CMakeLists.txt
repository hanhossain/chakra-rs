add_library(Chakra.Common.PlatformAgnostic INTERFACE)
target_include_directories(Chakra.Common.PlatformAgnostic INTERFACE include)
target_link_libraries(Chakra.Common.PlatformAgnostic INTERFACE Chakra.Pal)

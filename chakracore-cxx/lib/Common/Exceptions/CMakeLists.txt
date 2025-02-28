add_library (Chakra.Common.Exceptions STATIC
    ExceptionCheck.cpp
    ExceptionBase.cpp
    ReportError.cpp
    Throw.cpp
)

# TODO (hanhossain): remove most directory paths
# target_include_directories (
#     Chakra.Common.Exceptions PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Chakra.Common.Exceptions
    PUBLIC include)

target_link_libraries(Chakra.Common.Exceptions
    PUBLIC Chakra.Common.Pal)

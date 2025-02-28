add_library (Chakra.Common.Exceptions STATIC
    ExceptionCheck.cpp
    ExceptionBase.cpp
    ReportError.cpp
    Throw.cpp
)

target_include_directories(Chakra.Common.Exceptions
    PUBLIC include)

target_link_libraries(Chakra.Common.Exceptions
    PUBLIC Chakra.Common.PlatformAgnostic)

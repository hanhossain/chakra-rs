add_library (Chakra.Common.Exceptions STATIC
    # CommonExceptionsPch.cpp
    ExceptionCheck.cpp
    ExceptionBase.cpp
    ReportError.cpp
    Throw.cpp
)

# TODO: (hanhossain): cleanup
target_include_directories (Chakra.Common.Exceptions PUBLIC
    # ${CHAKRACORE_SOURCE_DIR}/lib/Common
)

target_link_libraries(Chakra.Common.Exceptions PUBLIC
    Chakra.Common.PlatformAgnostic
)

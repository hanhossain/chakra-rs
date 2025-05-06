add_library (Chakra.Common.Exceptions STATIC
    # CommonExceptionsPch.cpp
    ExceptionCheck.cpp
    ExceptionBase.cpp
    ReportError.cpp
    Throw.cpp
)

# TODO (hanhossain): cleanup includes
target_include_directories (Chakra.Common.Exceptions PUBLIC
    ${CHAKRACORE_SOURCE_DIR}/pal
    ${CHAKRACORE_SOURCE_DIR}/pal/inc
    ${CHAKRACORE_SOURCE_DIR}/pal/inc/rt
    ${CHAKRACORE_SOURCE_DIR}/lib/Common
    ${CHAKRACORE_SOURCE_DIR}/lib/Common/Interface/include
    ${CHAKRACORE_SOURCE_DIR}/lib/Common/PlatformAgnostic/include
)

target_link_libraries(Chakra.Common.Exceptions
  PUBLIC
  Chakra.Pal
)

target_compile_definitions(Chakra.Common.Exceptions PUBLIC
    PAL_STDCPP_COMPAT
    NO_PAL_MINMAX)

target_precompile_headers(Chakra.Common.Exceptions PUBLIC CommonExceptionsPch.h)

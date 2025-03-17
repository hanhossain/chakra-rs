add_library (Chakra.Common.Exceptions OBJECT
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
)

target_precompile_headers(Chakra.Common.Exceptions PUBLIC CommonExceptionsPch.h)

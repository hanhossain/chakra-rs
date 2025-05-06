add_library (Chakra.SCACore STATIC
    SCACore.cpp
    SCACorePch.cpp
    SCADeserialization.cpp
    SCAEngine.cpp
    SCAPropBag.cpp
    SCASerialization.cpp
    StreamHelper.cpp
    StreamReader.cpp
    StreamWriter.cpp
    )


# TODO (hanhossain): cleanup includes
target_include_directories (Chakra.SCACore PUBLIC
    ${CHAKRACORE_SOURCE_DIR}/pal
    ${CHAKRACORE_SOURCE_DIR}/pal/inc
    ${CHAKRACORE_SOURCE_DIR}/pal/inc/rt
    ${CHAKRACORE_SOURCE_DIR}/lib/Common/PlaceHolder
    ${CHAKRACORE_SOURCE_DIR}/lib/Runtime
    ${CHAKRACORE_SOURCE_DIR}/lib/JITClient
    ${CHAKRACORE_SOURCE_DIR}/lib/JITIDL
    ${CHAKRACORE_SOURCE_DIR}/lib/Common
    ${CHAKRACORE_SOURCE_DIR}/lib/Common/Interface/include
    ${CHAKRACORE_SOURCE_DIR}/lib/Common/PlatformAgnostic/include
    ${CHAKRACORE_SOURCE_DIR}/lib/Backend
    ${CHAKRACORE_SOURCE_DIR}/lib/Parser
    ${CHAKRACORE_SOURCE_DIR}/lib/WasmReader
    ${CHAKRACORE_SOURCE_DIR}/lib/Runtime/ByteCode
    )

target_link_libraries(Chakra.SCACore
    PUBLIC
    Chakra.Pal
)

target_compile_definitions(Chakra.SCACore PUBLIC
    PAL_STDCPP_COMPAT
    NO_PAL_MINMAX)

target_precompile_headers(Chakra.SCACore PUBLIC SCACorePch.h)

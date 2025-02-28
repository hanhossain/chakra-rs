add_library (Chakra.SCACore OBJECT
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


# TODO (hanhossain): remove most directory paths
target_include_directories (
    Chakra.SCACore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    ../Parser
    ../Common
    ../Backend
    ../JITIDL
    ../Runtime
    ../Runtime/ByteCode
    ${CHAKRACORE_SOURCE_DIR}/pal
    ${CHAKRACORE_SOURCE_DIR}/pal/inc
    ${CHAKRACORE_SOURCE_DIR}/pal/inc/rt
    ${CHAKRACORE_SOURCE_DIR}/pal/src
    ${CHAKRACORE_SOURCE_DIR}/pal/src/include
    )

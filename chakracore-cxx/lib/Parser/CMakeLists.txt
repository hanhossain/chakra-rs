add_library (Chakra.Parser OBJECT
    Alloc.cpp
    BackgroundParser.cpp
    BGParseManager.cpp
    CaseInsensitive.cpp
    CharClassifier.cpp
    CharSet.cpp
    CharTrie.cpp
    DebugWriter.cpp
    Hash.cpp
    OctoquadIdentifier.cpp
    Parse.cpp
    ParserPch.cpp
    ptree.cpp
    RegexCompileTime.cpp
    RegexParser.cpp
    RegexPattern.cpp
    RegexRuntime.cpp
    RegexStats.cpp
    Scan.cpp
    StandardChars.cpp
    TextbookBoyerMoore.cpp
    cmperr.cpp
    errstr.cpp
    globals.cpp
    jserr.gen
    rterror.cpp
    screrror.cpp
    )

# TODO (hanhossain): cleanup includes
target_include_directories (Chakra.Parser PUBLIC
    ${CHAKRACORE_SOURCE_DIR}/pal
    ${CHAKRACORE_SOURCE_DIR}/pal/inc
    ${CHAKRACORE_SOURCE_DIR}/pal/inc/rt
    ${CHAKRACORE_SOURCE_DIR}/lib/Common/PlaceHolder
    ${CHAKRACORE_SOURCE_DIR}/lib/Runtime
    ${CHAKRACORE_SOURCE_DIR}/lib/JITClient
    ${CHAKRACORE_SOURCE_DIR}/lib/JITIDL
    ${CHAKRACORE_SOURCE_DIR}/lib/Common
    ${CHAKRACORE_SOURCE_DIR}/lib/Common/Interface
    ${CHAKRACORE_SOURCE_DIR}/lib/Backend
    ${CHAKRACORE_SOURCE_DIR}/lib/Parser
    ${CHAKRACORE_SOURCE_DIR}/lib/WasmReader
    ${CHAKRACORE_SOURCE_DIR}/lib/Runtime/ByteCode
    ${CHAKRACORE_SOURCE_DIR}/lib/Runtime/Language
)

target_precompile_headers(Chakra.Parser PUBLIC ParserPch.h)

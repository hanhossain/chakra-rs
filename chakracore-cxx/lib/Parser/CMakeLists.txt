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

# TODO (hanhossain): remove most directory paths
target_include_directories (
    Chakra.Parser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    ../JITIDL
    ../Common
    ../Backend
    ../Runtime
    ../Runtime/ByteCode
    )

add_library (Chakra.Common.DataStructures OBJECT
    BigUInt.cpp
    BufferBuilder.cpp
    CommonDataStructuresPch.cpp
    DictionaryStats.cpp
    FixedBitVector.cpp
    FixedBitVectorEnumerator.cpp
    GrowingArray.cpp
    ImmutableList.cpp
    InternalString.cpp
    InternalStringNoCaseComparer.cpp
    Interval.cpp
    SizePolicy.cpp
    )

# TODO (hanhossain): cleanup includes
target_include_directories (Chakra.Common.DataStructures PUBLIC
    ${CHAKRACORE_SOURCE_DIR}/pal
    ${CHAKRACORE_SOURCE_DIR}/pal/inc
    ${CHAKRACORE_SOURCE_DIR}/pal/inc/rt
    ${CHAKRACORE_SOURCE_DIR}/lib/Common
    ${CHAKRACORE_SOURCE_DIR}/lib/Common/Interface
    ${CHAKRACORE_SOURCE_DIR}/lib/Common/PlatformAgnostic
)

target_precompile_headers(Chakra.Common.DataStructures PUBLIC CommonDataStructuresPch.h)

add_library (Chakra.Common.DataStructures STATIC
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

# TODO: (hanhossain): cleanup
target_include_directories (Chakra.Common.DataStructures PUBLIC
    # ${CHAKRACORE_SOURCE_DIR}/lib/Common
)

target_link_libraries(Chakra.Common.DataStructures PUBLIC
    Chakra.Pal
)

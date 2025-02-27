add_library (Chakra.Runtime.Math OBJECT
    AsmJsMath.cpp
    RuntimeMathPch.cpp
    )

# TODO (hanhossain): remove most directory paths
# target_include_directories (
#     Chakra.Runtime.Math PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

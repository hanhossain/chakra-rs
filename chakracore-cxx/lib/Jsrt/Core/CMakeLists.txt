add_library (Chakra.Jsrt.Core STATIC
    JsrtContextCore.cpp
    JsrtCore.cpp
    )

# TODO (hanhossain): remove most directory paths
# target_include_directories (
#     Chakra.Jsrt.Core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
#     ../../Backend
#     ../../JITIDL
#     ../../Runtime
#     ../../Runtime/ByteCode
#     ../../Parser
#     ../../SCACore
#     ../
#     )

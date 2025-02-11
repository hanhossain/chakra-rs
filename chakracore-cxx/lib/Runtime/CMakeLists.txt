project(CHAKRA_RUNTIME)

# TODO (hanhossain): remove include_directories in favor of target_include_directories
include_directories(
    .
    ../JITIDL
    ../Common
    ../Backend
    ../Parser
    ../WasmReader
    ByteCode
    PlatformAgnostic
    Math
    )

add_subdirectory (Base)
add_subdirectory (PlatformAgnostic)
add_subdirectory (ByteCode)
add_subdirectory (Debug)
add_subdirectory (Language)
add_subdirectory (Library)
add_subdirectory (Math)
add_subdirectory (Types)

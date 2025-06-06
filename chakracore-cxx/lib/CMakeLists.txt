add_compile_options(-fPIC)

if(BuildJIT)
    set(chakra_backend_objects $<TARGET_OBJECTS:Chakra.Backend>)
endif()

if(CC_TARGETS_AMD64)
    set(wasm_objects $<TARGET_OBJECTS:Chakra.WasmReader>)
    set(wasm_includes ${CHAKRACORE_SOURCE_DIR}/lib/WasmReader)
endif()

if(CAN_BUILD_WABT)
    set(wabt_objects $<TARGET_OBJECTS:libwabt>)
    set(wabt_includes ${CHAKRACORE_SOURCE_DIR}/lib/wabt)
endif()

add_library (ChakraCoreStatic STATIC
  ChakraCoreStatic.cpp
  $<TARGET_OBJECTS:Chakra.Pal>
  $<TARGET_OBJECTS:Chakra.Common.Core>
  $<TARGET_OBJECTS:Chakra.Jsrt>
  $<TARGET_OBJECTS:Chakra.Jsrt.Core>
  ${chakra_backend_objects}
  $<TARGET_OBJECTS:Chakra.Common.Common>
  $<TARGET_OBJECTS:Chakra.Common.Codex>
  $<TARGET_OBJECTS:Chakra.Common.DataStructures>
  $<TARGET_OBJECTS:Chakra.Common.Exceptions>
  $<TARGET_OBJECTS:Chakra.Common.Memory>
  $<TARGET_OBJECTS:Chakra.Common.Util>
  $<TARGET_OBJECTS:Chakra.Runtime.Base>
  $<TARGET_OBJECTS:Chakra.Runtime.ByteCode>
  $<TARGET_OBJECTS:Chakra.Runtime.Debug>
  $<TARGET_OBJECTS:Chakra.Runtime.Language>
  $<TARGET_OBJECTS:Chakra.Runtime.Library>
  $<TARGET_OBJECTS:Chakra.Runtime.Math>
  $<TARGET_OBJECTS:Chakra.Runtime.Types>
  $<TARGET_OBJECTS:Chakra.Runtime.PlatformAgnostic>
  $<TARGET_OBJECTS:Chakra.Parser>
  $<TARGET_OBJECTS:Chakra.SCACore>
  ${wasm_objects}
  ${wabt_objects}
)

target_compile_definitions(ChakraCoreStatic PUBLIC
    PAL_STDCPP_COMPAT
    NO_PAL_MINMAX)

if(CC_TARGET_OS_OSX)
  target_link_libraries(ChakraCoreStatic
    "-framework CoreFoundation"
    "-framework Security"
    )
else()
  set(PTHREAD "pthread")

  target_link_libraries(ChakraCoreStatic
    ${PTHREAD}
    "dl"
    )
endif()

if(BuildJIT)
    add_subdirectory (Backend)
endif()

add_subdirectory (Common)
add_subdirectory (Parser)
add_subdirectory (SCACore)
add_subdirectory (Runtime)
add_subdirectory (Jsrt)
if (CC_TARGETS_AMD64)
    add_subdirectory (WasmReader)
    set(BUILD_TESTS OFF)
    set(RUN_BISON OFF)
    set(RUN_RE2C OFF)
endif()

if(CAN_BUILD_WABT)
    add_subdirectory (wabt)
endif()

add_custom_command(TARGET ChakraCoreStatic POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/include"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/Jsrt/ChakraCore.h"
        "${CMAKE_BINARY_DIR}/include"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/Jsrt/ChakraCommon.h"
        "${CMAKE_BINARY_DIR}/include"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/Jsrt/ChakraDebug.h"
        "${CMAKE_BINARY_DIR}/include"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/Common/Interface/include/Interface/ChakraCoreVersion.h"
        "${CMAKE_BINARY_DIR}/include"
)

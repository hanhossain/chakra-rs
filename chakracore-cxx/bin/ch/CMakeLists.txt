find_package (Python COMPONENTS Interpreter)
add_custom_target(dbg_controller_h
  COMMAND ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/jstoc.py ${CMAKE_CURRENT_SOURCE_DIR}/DbgController.js controllerScript
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/jstoc.py ${CMAKE_CURRENT_SOURCE_DIR}/DbgController.js
  )

set(ch_source_files
  ch.cpp
  ChakraRtInterface.cpp
  CodexAssert.cpp
  Debugger.cpp
  Helpers.cpp
  HostConfigFlags.cpp
  WScriptJsrt.cpp
  RuntimeThreadData.cpp
  ../ChakraCore/TestHooks.cpp
  )

add_executable (ch ${ch_source_files})

add_dependencies(ch dbg_controller_h)

set_target_properties(ch
  PROPERTIES
  POSITION_INDEPENDENT_CODE True
  )

# TODO (hanhossain): remove include_directories in favor of target_include_directories
include_directories(..)

if(CC_TARGETS_AMD64)
    set(Wasm_dep ../../lib/WasmReader)
endif()

# TODO (hanhossain): remove most directory paths
target_include_directories (ch
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ../ChakraCore
  ../../lib/Common
  ../../lib/Jsrt
  ../../lib/Runtime
  ../../lib/Parser
  ${Wasm_dep}
  )

if(CC_TARGET_OS_LINUX)
  set(LINKER_START_GROUP -pie -Wl,--start-group)
  set(LINKER_END_GROUP -Wl,--end-group)
  set(LINKER_END_GROUP "${LINKER_END_GROUP} -static-libstdc++")
elseif(CC_TARGET_OS_OSX)
  set(LINKER_START_GROUP -Wl,-force_load,)
endif()

set(LINKER_END_GROUP
  "${LINKER_END_GROUP}"
  pthread
  )

# common link deps
set(lib_target "${lib_target}"
  ${LINKER_START_GROUP}
  ChakraCoreStatic
  ${ICU_LIBRARIES}
  ${LINKER_END_GROUP}
  dl
  )

if(CC_TARGET_OS_OSX)
  set(lib_target "${lib_target}"
    "-framework CoreFoundation"
    "-framework Security"
    )
else()
  set(lib_target "${lib_target}"
    "pthread"
    )
endif()

if(CC_TARGET_OS_LINUX)
  set(lib_target "${lib_target}"
    -Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/ch.version
    )
endif()

target_link_libraries (ch
  ${lib_target}
  )

# Add a post build event to the ch target
# which executes a command to copy ch to
# out/{BUILD_TYPE}/ for convenience
add_custom_command(TARGET ch POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  "${CHAKRACORE_BINARY_DIR}/bin/ch/ch"
  ${CHAKRACORE_BINARY_DIR}/)

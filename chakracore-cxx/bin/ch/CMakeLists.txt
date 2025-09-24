set(ch_source_files
  ch.cpp
  ChakraRtInterface.cpp
  Helpers.cpp
  HostConfigFlags.cpp
  WScriptJsrt.cpp
  RuntimeThreadData.cpp
  ../ChakraCore/TestHooks.cpp
  )

add_executable (ch ${ch_source_files})

set_target_properties(ch
  PROPERTIES
  POSITION_INDEPENDENT_CODE True
  )

if(CC_TARGETS_AMD64)
    set(Wasm_dep ../../lib/WasmReader)
endif()

# TODO (hanhossain): cleanup includes
target_include_directories (ch PUBLIC
  ${CHAKRACORE_SOURCE_DIR}/lib/Common/PlaceHolder
  ${CHAKRACORE_SOURCE_DIR}/pal/inc
  ${CHAKRACORE_SOURCE_DIR}/pal/inc/rt
  ${CHAKRACORE_SOURCE_DIR}/lib/Runtime
  ${CHAKRACORE_SOURCE_DIR}/lib/Jsrt
  ${CHAKRACORE_SOURCE_DIR}/lib/JITClient
  ${CHAKRACORE_SOURCE_DIR}/lib/JITIDL
  ${CHAKRACORE_SOURCE_DIR}/lib/Common
  ${CHAKRACORE_SOURCE_DIR}/lib/Common/Interface/include
  ${CHAKRACORE_SOURCE_DIR}/lib/Common/PlatformAgnostic/include
  ${CHAKRACORE_SOURCE_DIR}/bin/ChakraCore
  ${CHAKRACORE_SOURCE_DIR}/lib/Backend
  ${CHAKRACORE_SOURCE_DIR}/lib/Parser
  ${CHAKRACORE_SOURCE_DIR}/lib/WasmReader
  ${CHAKRACORE_SOURCE_DIR}/lib/Runtime/ByteCode
  )

if(CC_TARGET_OS_LINUX)
  set(LINKER_START_GROUP -pie -Wl,--start-group)
  set(LINKER_END_GROUP -Wl,--end-group)
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

target_link_libraries (ch
  ${lib_target}
  )

target_compile_definitions(ch PUBLIC
    PAL_STDCPP_COMPAT
)

target_precompile_headers(ch PRIVATE stdafx.h)

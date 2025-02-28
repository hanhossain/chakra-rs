add_library (Chakra.Common.Util OBJECT
    Pinned.cpp)

# TODO (hanhossain): remove most directory paths
target_include_directories (
    Chakra.Common.Util PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    ${CHAKRACORE_SOURCE_DIR}/pal
    ${CHAKRACORE_SOURCE_DIR}/pal/inc
    ${CHAKRACORE_SOURCE_DIR}/pal/inc/rt
    ${CHAKRACORE_SOURCE_DIR}/pal/src
    ${CHAKRACORE_SOURCE_DIR}/pal/src/include)

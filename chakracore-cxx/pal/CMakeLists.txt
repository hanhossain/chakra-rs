cmake_minimum_required(VERSION 3.28)

project(COREPAL)

# TODO (hanhossain): remove include_directories in favor of target_include_directories
include_directories(${COREPAL_SOURCE_DIR}/inc)
# TODO (hanhossain): remove include_directories in favor of target_include_directories
include_directories(${COREPAL_SOURCE_DIR}/src)

add_compile_options(-fstack-protector)

add_subdirectory(src)

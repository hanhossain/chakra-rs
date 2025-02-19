cmake_minimum_required(VERSION 3.28)

project(COREPAL)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -stdlib=libc++")

# TODO (hanhossain): remove include_directories in favor of target_include_directories
include_directories(${COREPAL_SOURCE_DIR}/inc)
# TODO (hanhossain): remove include_directories in favor of target_include_directories
include_directories(${COREPAL_SOURCE_DIR}/src)

add_compile_options(-fstack-protector)

add_subdirectory(src)

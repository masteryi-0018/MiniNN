cmake_minimum_required(VERSION 3.0)

# set compiler before call project()
set(CMAKE_C_COMPILER "E:/LLVM/bin/clang.exe")
set(CMAKE_CXX_COMPILER "E:/LLVM/bin/clang++.exe")

# Clang with CUDA is not yet supported on Windows.  See CMake issue #20776.
# set(CMAKE_CUDA_COMPILER "E:/LLVM/bin/clang.exe")

if(WIN32)
    project(MININN)
elseif(UNIX)
    project(MININN LANGUAGES CXX CUDA)
endif()

# 1. environment
set(MININN_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# 2. subdirectory
include_directories("${MININN_SOURCE_DIR}")
include_directories("third_party/flatbuffers/include")

if(WIN32)
    set(gtest_force_shared_crt ON)
endif()

add_subdirectory(third_party/googletest)
add_subdirectory(mininn)
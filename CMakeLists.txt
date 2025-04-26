cmake_minimum_required(VERSION 3.5)

# set compiler before call project()
if(WIN32)
    set(CMAKE_C_COMPILER "clang.exe")
    set(CMAKE_CXX_COMPILER "clang++.exe")
elseif(UNIX)
    set(CMAKE_C_COMPILER "clang")
    set(CMAKE_CXX_COMPILER "clang++")
endif()

# set cpp standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
message(STATUS "Default C++ standard: ${CMAKE_CXX_STANDARD}")

# set release type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
    # set(CMAKE_BUILD_TYPE Release)
    # set(CMAKE_BUILD_TYPE RelWithDebInfo)
    # set(CMAKE_BUILD_TYPE MinSizeRel)
endif()
message(STATUS "Default cmake build type: ${CMAKE_BUILD_TYPE}")

# set PIC for shared library
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Clang with CUDA is not yet supported on Windows.  See CMake issue #20776.
# set(CMAKE_CUDA_COMPILER "E:/LLVM/bin/clang.exe")

if(WIN32)
    project(MININN)
elseif(UNIX)
    project(MININN LANGUAGES CXX CUDA)
endif()

# 1. environment
set(MININN_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# standard opencl
# find_package(OpenCL REQUIRED)
# include_directories(${OpenCL_INCLUDE_DIRS})

# custom opencl
if(WIN32)
    set(OPENCL_INCLUDE_DIR "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v12.2/include")
    set(OPENCL_LIB_DIR "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v12.2/lib/x64")
elseif(UNIX)
    set(OPENCL_INCLUDE_DIR "/usr/local/cuda/include/")
    set(OPENCL_LIB_DIR "/usr/local/cuda/lib64")
    set(MKL_INCLUDE_DIR "/opt/intel/oneapi/mkl/latest/include")
    set(MKL_LIB_DIR "/opt/intel/oneapi/mkl/latest/lib")
endif()

include_directories(${OPENCL_INCLUDE_DIR})
include_directories(${MKL_INCLUDE_DIR})

# 2. subdirectory
include_directories("${MININN_SOURCE_DIR}")
include_directories("third_party/flatbuffers/include")

if(WIN32)
    set(gtest_force_shared_crt ON)
endif()

add_subdirectory(third_party/googletest)
add_subdirectory(third_party/pybind11)
add_subdirectory(mininn)
add_subdirectory(python)
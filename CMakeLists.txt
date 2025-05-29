cmake_minimum_required(VERSION 3.0)

####################  一、build  ####################

# 1. set compiler before call project()
# set(CMAKE_C_COMPILER "clang") # Windows do not need to specify .exe
# set(CMAKE_CXX_COMPILER "clang++")
message(STATUS "Default C++ compiler: ${CMAKE_CXX_COMPILER}")

# 2. set cpp standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
message(STATUS "Default C++ standard: ${CMAKE_CXX_STANDARD}")

# 3. set build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
    # set(CMAKE_BUILD_TYPE Release)
    # set(CMAKE_BUILD_TYPE RelWithDebInfo)
    # set(CMAKE_BUILD_TYPE MinSizeRel)
endif()
message(STATUS "Default cmake build type: ${CMAKE_BUILD_TYPE}")


####################  二、env  ####################

# 1. project
project(MININN)
if((WIN32 AND MSVC) OR UNIX)
    enable_language(CXX CUDA)
else()
    enable_language(CXX)
endif()

if(WIN32)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

# set PIC for shared library
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# 2. backend
# custom libs
if(WIN32)
    set(OPENCL_INCLUDE_DIR "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v12.9/include")
    set(OPENCL_LIB_DIR "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v12.9/lib/x64")
    set(MKL_INCLUDE_DIR "E:/oneAPI/mkl/2024.2/include")
    set(MKL_LIB_DIR "E:/oneAPI/mkl/2024.2/lib")
elseif(UNIX)
    set(OPENCL_INCLUDE_DIR "/usr/local/cuda/include/")
    set(OPENCL_LIB_DIR "/usr/local/cuda/lib64")
    set(MKL_INCLUDE_DIR "/opt/intel/oneapi/mkl/latest/include")
    set(MKL_LIB_DIR "/opt/intel/oneapi/mkl/latest/lib")
endif()

# standard libs
# find_package(OpenCL REQUIRED)
# find_package(MKL REQUIRED)


####################  三、main  ####################

# 1. include directories
set(MININN_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${OPENCL_INCLUDE_DIR})
include_directories(${MKL_INCLUDE_DIR})

include_directories("${MININN_SOURCE_DIR}")
include_directories("third_party/flatbuffers/include")

# 2. subdirectory
set(CLANG ${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
if(WIN32 AND CLANG)
    message(STATUS "Clang on Windows detected, setting up for static libraries.")
    set(BUILD_SHARED_LIBS OFF)
endif()

add_subdirectory(third_party/glog)
add_subdirectory(third_party/googletest)
add_subdirectory(third_party/pybind11)
add_subdirectory(mininn)
add_subdirectory(python)
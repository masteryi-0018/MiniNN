cmake_minimum_required(VERSION 4.0)

####################  一、build  ####################

# 1. set compiler before call project()
message(STATUS "Default C++ compiler: ${CMAKE_CXX_COMPILER}")

# 2. set cpp standard
set(CMAKE_CXX_STANDARD 11)
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

# https://google.github.io/googletest/quickstart-cmake.html
if(WIN32)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

# set PIC for shared library
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# 2. backend


####################  三、main  ####################

# 1. include directories
set(MININN_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

include_directories("${MININN_SOURCE_DIR}")
include_directories("third_party/flatbuffers/include")

# 2. subdirectory
add_subdirectory(third_party/googletest)
if(NOT ANDROID)
    add_subdirectory(third_party/pybind11)
endif()
add_subdirectory(mininn)
if(NOT ANDROID)
    add_subdirectory(python)
endif()

# 3. demo
add_subdirectory(demo)

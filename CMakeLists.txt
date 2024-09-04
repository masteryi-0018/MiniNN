cmake_minimum_required(VERSION 3.0)
project(MININN)

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
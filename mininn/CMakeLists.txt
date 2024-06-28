# 1. environment
set(TEST test-main)
set(GTEST gtest-main)

option(WITH_GTEST "build with gtest" ON)

# 2. head file, source file and so
include_directories(graph)
include_directories(operator)
include_directories(kernel)
include_directories(utils)

file(GLOB GRAPH_SRC "graph/*.cc")
file(GLOB OPERATOR_SRC "operator/*.cc")
file(GLOB KERNEL_SRC "kernel/*.cc")
file(GLOB UTILS_SRC "utils/*.cc")
# set(MININN_SRC ${GRAPH_SRC} ${OPERATOR_SRC} ${KERNEL_SRC} ${UTILS_SRC})

add_library(graph SHARED ${GRAPH_SRC})
add_library(operator SHARED ${OPERATOR_SRC})
add_library(kernel SHARED ${KERNEL_SRC})
add_library(utils SHARED ${UTILS_SRC})

target_link_libraries(operator utils)
target_link_libraries(kernel utils)
target_link_libraries(graph operator kernel)

# 3. gtest
if(WITH_GTEST)
    file(GLOB GTEST_SRC "test/*.cc")
    add_executable(${GTEST} ${GTEST_SRC})
    target_link_libraries(${GTEST} gtest gtest_main graph)

    add_executable(${TEST} test.cc)
    target_link_libraries(${TEST} utils)

else()
    add_executable(${TEST} test.cc)
    target_link_libraries(${TEST} utils)
endif()
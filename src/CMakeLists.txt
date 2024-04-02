cmake_minimum_required(VERSION 3.0)
project(MiniNN)

# 一、环境变量
set(TEST MiniNN-test)
set(GTEST_BUILD third_party/googletest)

set(TENSOR_GTEST tensor-gtest)
set(NODE_GTEST node-gtest)
set(GRAPH_GTEST graph-gtest)

option(WITH_GTEST "build with gtest" ON)

# 二、内部库
include_directories(graph)
add_library(graph SHARED graph/tensor.cc graph/node.cc graph/graph.cc)

include_directories(operator)
add_library(operator SHARED operator/add.cc)
target_link_libraries(operator graph)

include_directories(runtime)

# 三、可执行文件
if(WITH_GTEST)
    add_executable(${TENSOR_GTEST} graph/test/tensor_test.cc)
    add_executable(${NODE_GTEST} graph/test/node_test.cc)
    add_executable(${GRAPH_GTEST} graph/test/graph_test.cc)
    # 1. 外部库，后一个参数指定编译产物路径，默认${CMAKE_CURRENT_BINARY_DIR}/${source_dir}，可能出现找不到的情况
    add_subdirectory(../third_party/googletest ${GTEST_BUILD})
    # 2. 链接库
    target_link_libraries(${TENSOR_GTEST} gtest gtest_main graph)
    target_link_libraries(${NODE_GTEST} gtest gtest_main graph)
    target_link_libraries(${GRAPH_GTEST} gtest gtest_main graph operator)
    # 3. 添加测试
    # add_test(NAME ${GTEST} COMMAND ${GTEST})
else()
    add_executable(${TEST} test.cc)
endif()

set(GTEST_LIST
    graph/tensor.cc
)
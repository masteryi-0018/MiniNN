cmake_minimum_required(VERSION 3.0)
project(MiniNN)

# 一、环境变量
set(GTEST_BUILD third_party/googletest)
set(GFLAGS_BUILD third_party/gflags)
set(GLOG_BUILD third_party/glog)

set(TEST main-test)
set(TENSOR_GTEST tensor-gtest)
set(NODE_GTEST node-gtest)
set(GRAPH_GTEST graph-gtest)

option(WITH_GTEST "build with gtest" ON)

# 二、内部库
include_directories(graph)
include_directories(operator)
include_directories(runtime)

add_library(graph SHARED graph/tensor.cc graph/node.cc graph/graph.cc)
add_library(operator SHARED operator/add.cc)

target_link_libraries(operator graph)

add_subdirectory(../third_party/googletest ${GTEST_BUILD})
add_subdirectory(../third_party/gflags ${GFLAGS_BUILD})

set(WITH_GFLAGS OFF)
add_subdirectory(../third_party/glog ${GLOG_BUILD})

# 三、可执行文件
if(WITH_GTEST)
    add_executable(${TENSOR_GTEST} graph/test/tensor_test.cc)
    add_executable(${NODE_GTEST} graph/test/node_test.cc)
    add_executable(${GRAPH_GTEST} graph/test/graph_test.cc)

    target_link_libraries(${TENSOR_GTEST} gtest gtest_main graph)
    target_link_libraries(${NODE_GTEST} gtest gtest_main graph)
    target_link_libraries(${GRAPH_GTEST} gtest gtest_main graph operator)

    add_executable(${TEST} test.cc)
    target_link_libraries(${TEST} glog::glog)

else()
    add_executable(${TEST} test.cc)
    target_link_libraries(${TEST} glog::glog)
endif()
cmake_minimum_required(VERSION 3.0)
project(MiniNN)

# 1. 环境变量
set(TEST MiniNN-test)
set(GTEST MiniNN-gtest)
set(GTEST_BUILD third_party/googletest)

set(GRAPH_GTEST graph-gtest)

# 2. 添加库
# 后一个参数指定编译产物路径，默认${CMAKE_CURRENT_BINARY_DIR}/${source_dir}，可能出现找不到的情况
add_subdirectory(../third_party/googletest ${GTEST_BUILD})

# 3. 可执行文件
add_executable(${TEST} test.cc)
add_executable(${GTEST} test_gtest.cc)

# 4. 链接库
link_directories(${GTEST_BUILD}/lib) # 目前产物都在build/lib下
target_link_libraries(${GTEST} gtest gtest_main)

# 5. 添加测试
# add_test(NAME ${GTEST} COMMAND ${GTEST})

# 6. gtest
if(${WITHGTEST})
    include_directories(graph)
    add_library(graph SHARED graph/tensor.cc)
    add_executable(${GRAPH_GTEST} graph/test/test_tensor.cc)
    target_link_libraries(${GRAPH_GTEST} gtest gtest_main graph)
endif()
# 一、环境变量
set(TEST test-main)
set(GTEST gtest-main)

option(WITH_GTEST "build with gtest" ON)

# 二、头文件，库文件
include_directories(graph)
include_directories(operator)
include_directories(kernel)
include_directories(runtime)
include_directories(utils)

file(GLOB GRAPH_SRC "graph/*.cc")
file(GLOB OPERATOR_SRC "operator/*.cc")
file(GLOB KERNEL_SRC "kernel/*.cc")
file(GLOB UTILS_SRC "utils/*.cc")
set(MININN_SRC ${GRAPH_SRC} ${OPERATOR_SRC} ${KERNEL_SRC} ${UTILS_SRC})

# TODO: split shared lib to graph, operator, utils...
add_library(mininn SHARED ${MININN_SRC})

# 三、可执行文件
if(WITH_GTEST)
    file(GLOB GTEST_SRC "graph/test/*.cc")
    add_executable(${GTEST} ${GTEST_SRC})
    target_link_libraries(${GTEST} gtest gtest_main mininn)

    add_executable(${TEST} test.cc)
    target_link_libraries(${TEST} mininn)

else()
    add_executable(${TEST} test.cc)
    target_link_libraries(${TEST} mininn)
endif()
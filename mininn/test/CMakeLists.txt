file(GLOB_RECURSE GTEST_MAIN_SRC "*.cc")
add_executable(gtest-main ${GTEST_MAIN_SRC})

message(STATUS "Source files for gtest-main binary: ${GTEST_MAIN_SRC}")

target_link_libraries(gtest-main gtest glog mininn_lib)
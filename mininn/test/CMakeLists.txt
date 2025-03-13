set(GTEST_MAIN gtest-main)

file(GLOB_RECURSE GTEST_MAIN_SRC "*.cc")
add_executable(${GTEST_MAIN} ${GTEST_MAIN_SRC})
target_link_libraries(${GTEST_MAIN} gtest runtime kernel operator parser graph utils)

message(STATUS "Source files for test binary: ${GTEST_MAIN_SRC}")
set(GTEST_MAIN gtest-main)

file(GLOB GTEST_MAIN_SRC "*.cc")
add_executable(${GTEST_MAIN} ${GTEST_MAIN_SRC})
target_link_libraries(${GTEST_MAIN} gtest runtime kernel operator parser graph utils)
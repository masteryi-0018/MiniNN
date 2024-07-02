set(GTEST_UT gtest-ut)
set(GTEST_PREDICTOR gtest-predictor)

file(GLOB GTEST_UT_SRC "*.cc")
file(GLOB_RECURSE EXCLUDE_FILES "predictor_test.cc")
list(REMOVE_ITEM GTEST_UT_SRC ${EXCLUDE_FILES})
add_executable(${GTEST_UT} ${GTEST_UT_SRC})
target_link_libraries(${GTEST_UT} gtest graph operator kernel utils)

file(GLOB GTEST_PREDICTOR_SRC "predictor_test.cc" "gtest_main.cc")
add_executable(${GTEST_PREDICTOR} ${GTEST_PREDICTOR_SRC})
target_link_libraries(${GTEST_PREDICTOR} gtest graph operator kernel utils runtime)
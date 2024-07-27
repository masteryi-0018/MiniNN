# 1. environment
set(TEST_MAIN test-main)

option(WITH_GTEST "build with gtest" ON)

# 2. subdirectory
add_subdirectory(utils)
add_subdirectory(graph)
add_subdirectory(operator)
add_subdirectory(kernel)
add_subdirectory(runtime)
add_subdirectory(parser)

# 3. bin
add_executable(${TEST_MAIN} test.cc)
target_link_libraries(${TEST_MAIN} utils)

if(WITH_GTEST)
    add_subdirectory(test)
endif()
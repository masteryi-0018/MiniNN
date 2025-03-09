# 1. environment
option(WITH_GTEST "build with gtest" ON)

if(UNIX)
    set(LIB_TYPE STATIC)
else(WIN32)
    set(LIB_TYPE STATIC)
endif()

# 2. subdirectory
add_subdirectory(utils)
add_subdirectory(graph)
add_subdirectory(operator)
add_subdirectory(kernel)
add_subdirectory(runtime)
add_subdirectory(parser)
add_subdirectory(backend)

# 3. bin
if(WITH_GTEST)
    add_subdirectory(test)
endif()
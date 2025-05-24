# 1. environment
option(WITH_GTEST "build with gtest" ON)
option(WITH_MULTI_THREADS "build with multi_threads" ON)

if((WIN32 AND MSVC) OR UNIX)
    option(WITH_CUDA "build with cuda" ON)
else()
    option(WITH_CUDA "build with cuda" OFF)
endif()

option(WITH_OPENCL "build with opencl" ON)
option(WITH_AVX "build with avx" ON)
option(WITH_SSE "build with sse" ON)
option(WITH_MKL "build with mkl" ON)

# pybind need one static lib to produce a shared lib, so use a OBJECT to handle it
set(LIB_TYPE OBJECT)
# set(LIB_TYPE SHARED)
# set(LIB_TYPE STATIC)

# 2. subdirectory
add_subdirectory(runtime)
add_subdirectory(parser)
add_subdirectory(graph)
add_subdirectory(operator)
add_subdirectory(kernel)
add_subdirectory(utils)

add_library(mininn_lib STATIC
    $<TARGET_OBJECTS:runtime>
    $<TARGET_OBJECTS:parser>
    $<TARGET_OBJECTS:graph>
    $<TARGET_OBJECTS:operator>
    $<TARGET_OBJECTS:kernel>
    $<TARGET_OBJECTS:utils>
)

# seperate the backend lib
add_subdirectory(backend)
target_link_libraries(mininn_lib backend)

# 3. binary
if(WITH_GTEST)
    add_subdirectory(test)
endif()
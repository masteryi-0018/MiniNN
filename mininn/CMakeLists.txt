# 1. environment
option(WITH_GTEST "build with gtest" ON)
option(WITH_MULTI_THREADS "build with multi_threads" ON)

if((WIN32 AND MSVC) OR (UNIX AND NOT ANDROID))
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

# 4. install
add_custom_command(
    TARGET mininn_lib POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${MININN_SOURCE_DIR}/mininn_sdk/lib
    COMMAND ${CMAKE_COMMAND} -E make_directory ${MININN_SOURCE_DIR}/mininn_sdk/include
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:mininn_lib> ${MININN_SOURCE_DIR}/mininn_sdk/lib/
    COMMAND ${CMAKE_COMMAND} -E copy ${MININN_SOURCE_DIR}/mininn/runtime/predictor.h ${MININN_SOURCE_DIR}/mininn_sdk/include/mininn/runtime/predictor.h
    COMMAND ${CMAKE_COMMAND} -E copy ${MININN_SOURCE_DIR}/mininn/parser/parser.h ${MININN_SOURCE_DIR}/mininn_sdk/include/mininn/parser/parser.h
    COMMAND ${CMAKE_COMMAND} -E copy ${MININN_SOURCE_DIR}/mininn/graph/tensor.h ${MININN_SOURCE_DIR}/mininn_sdk/include/mininn/graph/tensor.h
    COMMAND ${CMAKE_COMMAND} -E copy ${MININN_SOURCE_DIR}/mininn/graph/graph.h ${MININN_SOURCE_DIR}/mininn_sdk/include/mininn/graph/graph.h
    COMMAND ${CMAKE_COMMAND} -E copy ${MININN_SOURCE_DIR}/mininn/graph/kernel.h ${MININN_SOURCE_DIR}/mininn_sdk/include/mininn/graph/kernel.h
    COMMAND ${CMAKE_COMMAND} -E copy ${MININN_SOURCE_DIR}/mininn/graph/node.h ${MININN_SOURCE_DIR}/mininn_sdk/include/mininn/graph/node.h
    COMMAND ${CMAKE_COMMAND} -E copy ${MININN_SOURCE_DIR}/mininn/graph/type.h ${MININN_SOURCE_DIR}/mininn_sdk/include/mininn/graph/type.h
    COMMAND ${CMAKE_COMMAND} -E copy ${MININN_SOURCE_DIR}/mininn/graph/param.h ${MININN_SOURCE_DIR}/mininn_sdk/include/mininn/graph/param.h
    COMMAND ${CMAKE_COMMAND} -E copy ${MININN_SOURCE_DIR}/mininn/graph/use_op.h ${MININN_SOURCE_DIR}/mininn_sdk/include/mininn/graph/use_op.h
    COMMAND ${CMAKE_COMMAND} -E copy ${MININN_SOURCE_DIR}/mininn/graph/use_kernel.h ${MININN_SOURCE_DIR}/mininn_sdk/include/mininn/graph/use_kernel.h
    COMMAND ${CMAKE_COMMAND} -E copy ${MININN_SOURCE_DIR}/mininn/graph/register_helper.h ${MININN_SOURCE_DIR}/mininn_sdk/include/mininn/graph/register_helper.h
    COMMENT "Copying mininn_lib and headers to SDK directory"
)
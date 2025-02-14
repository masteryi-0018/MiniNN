file(GLOB KERNEL_SRC "*.cc")
add_library(kernel ${LIB_TYPE} ${KERNEL_SRC})
# target_link_libraries(kernel graph utils)
# if(UNIX)
#     target_link_libraries(kernel cuda)
# endif()

# target_link_libraries(kernel opencl)

target_link_libraries(kernel avx)
target_link_libraries(kernel sse)

message(STATUS "Source files for kernel library: ${KERNEL_SRC}")
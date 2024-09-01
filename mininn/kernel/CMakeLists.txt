file(GLOB KERNEL_SRC "*.cc")
add_library(kernel ${LIB_TYPE} ${KERNEL_SRC})
# target_link_libraries(kernel graph utils)

message(STATUS "Source files for kernel library: ${KERNEL_SRC}")
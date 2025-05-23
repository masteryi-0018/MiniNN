file(GLOB KERNEL_SRC "*.cc")
add_library(kernel ${LIB_TYPE} ${KERNEL_SRC})

message(STATUS "Source files for kernel library: ${KERNEL_SRC}")
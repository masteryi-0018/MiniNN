file(GLOB KERNEL_SRC "*.cc")
add_library(kernel SHARED ${KERNEL_SRC})
target_link_libraries(kernel graph utils)
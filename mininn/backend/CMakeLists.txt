file(GLOB CUDA_SRC "*.cu")
add_executable(cuda ${CUDA_SRC})

message(STATUS "Source files for cuda library: ${CUDA_SRC}")
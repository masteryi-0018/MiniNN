file(GLOB CUDA_SRC "*.cu")
add_library(cuda ${LIB_TYPE} ${CUDA_SRC})

message(STATUS "Source files for cuda library: ${CUDA_SRC}")
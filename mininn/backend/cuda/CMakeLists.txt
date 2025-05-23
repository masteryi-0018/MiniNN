file(GLOB CUDA_SRC "*.cu")
add_library(cuda ${BACKEND_LIB_TYPE} ${CUDA_SRC})

# look for your cuda_arch: https://developer.nvidia.com/cuda-gpus
set_property(TARGET cuda PROPERTY CUDA_ARCHITECTURES 86)

message(STATUS "Source files for backend-cuda library: ${CUDA_SRC}")
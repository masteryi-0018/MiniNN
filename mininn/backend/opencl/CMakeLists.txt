file(GLOB OPENCL_SRC "*.cc")
add_library(opencl ${BACKEND_LIB_TYPE} ${OPENCL_SRC})

message(STATUS "Source files for backend-opencl library: ${OPENCL_SRC}")
file(GLOB OPENCL_SRC "*.cc")
add_library(opencl ${LIB_TYPE} ${OPENCL_SRC})

# link opencl
# target_link_libraries(opencl ${OpenCL_LIBRARIES})
target_link_libraries(opencl "${OPENCL_LIB_DIR}/OpenCL.lib")

message(STATUS "Source files for opencl library: ${OPENCL_SRC}")
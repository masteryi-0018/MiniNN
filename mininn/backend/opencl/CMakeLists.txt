file(GLOB OPENCL_SRC "*.cc")
add_library(opencl ${LIB_TYPE} ${OPENCL_SRC})

# link opencl
# target_link_libraries(opencl ${OpenCL_LIBRARIES})
if(WIN32)
    target_link_libraries(opencl "${OPENCL_LIB_DIR}/OpenCL.lib")
elseif(UNIX)
    target_link_libraries(opencl "${OPENCL_LIB_DIR}/libOpenCL.so")
endif()

message(STATUS "Source files for opencl library: ${OPENCL_SRC}")
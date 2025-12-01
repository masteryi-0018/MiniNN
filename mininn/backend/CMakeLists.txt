set(BACKEND_LIB_TYPE STATIC)

file(GLOB BACKEND_SRC "*.cc")
add_library(backend ${BACKEND_LIB_TYPE} ${BACKEND_SRC})


if(WITH_OPENCL)
    add_subdirectory(opencl)
    target_link_libraries(backend opencl)
    # target_link_libraries(backend ${OpenCL_LIBRARIES})
    if(WIN32)
        target_link_libraries(backend "${OPENCL_LIB_DIR}/OpenCL.lib")
    elseif(UNIX AND NOT ANDROID)
        target_link_libraries(backend "${OPENCL_LIB_DIR}/libOpenCL.so")
    endif()
endif()

if(WITH_AVX)
    add_subdirectory(avx)
    target_link_libraries(backend avx)
endif()

if(WITH_SSE)
    add_subdirectory(sse)
    target_link_libraries(backend sse)
endif()

if(WITH_NEON)
    add_subdirectory(neon)
    target_link_libraries(backend neon)
endif()

message(STATUS "Source files for backend library: ${BACKEND_SRC}")
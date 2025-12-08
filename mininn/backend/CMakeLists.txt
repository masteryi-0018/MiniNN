set(BACKEND_LIB_TYPE STATIC)

file(GLOB BACKEND_SRC "*.cc")
add_library(backend ${BACKEND_LIB_TYPE} ${BACKEND_SRC})


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
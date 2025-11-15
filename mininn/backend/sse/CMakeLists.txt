file(GLOB SSE_SRC "*.cc")

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:SSE4.2")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.2")
endif()

add_library(sse ${BACKEND_LIB_TYPE} ${SSE_SRC})

message(STATUS "Source files for backend-sse library: ${SSE_SRC}")
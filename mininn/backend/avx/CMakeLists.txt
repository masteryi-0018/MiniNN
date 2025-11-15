file(GLOB AVX_SRC "*.cc")

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:AVX512")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx512vnni")
endif()

add_library(avx ${BACKEND_LIB_TYPE} ${AVX_SRC})

message(STATUS "Source files for backend-avx library: ${AVX_SRC}")
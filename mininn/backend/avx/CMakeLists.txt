file(GLOB AVX_SRC "*.cc")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx512vnni")
add_library(avx ${LIB_TYPE} ${AVX_SRC})

message(STATUS "Source files for avx library: ${AVX_SRC}")
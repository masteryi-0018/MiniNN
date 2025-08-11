file(GLOB NEON_SRC "*.cc")
# don't need -march=armv8-a+simd，ARM64 ABI open NEON by defaults
add_library(neon ${BACKEND_LIB_TYPE} ${NEON_SRC})

message(STATUS "Source files for backend-neon library: ${NEON_SRC}")
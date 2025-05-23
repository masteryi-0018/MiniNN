file(GLOB MKL_SRC "*.cc")
add_library(mkl ${BACKEND_LIB_TYPE} ${MKL_SRC})

message(STATUS "Source files for backend-mkl library: ${MKL_SRC}")
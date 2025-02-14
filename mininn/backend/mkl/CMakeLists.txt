file(GLOB MKL_SRC "*.cc")
add_library(mkl ${LIB_TYPE} ${MKL_SRC})

target_link_libraries(mkl "${MKL_LIB_DIR}/libmkl_rt.so")

message(STATUS "Source files for mkl library: ${MKL_SRC}")
file(GLOB UTILS_SRC "*.cc")
add_library(utils ${LIB_TYPE} ${UTILS_SRC})

message(STATUS "Source files for utils library: ${UTILS_SRC}")
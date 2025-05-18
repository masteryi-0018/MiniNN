file(GLOB RUNTIME_SRC "*.cc")
add_library(runtime ${LIB_TYPE} ${RUNTIME_SRC})

message(STATUS "Source files for runtime library: ${RUNTIME_SRC}")
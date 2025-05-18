file(GLOB OPERATOR_SRC "*.cc")
add_library(operator ${LIB_TYPE} ${OPERATOR_SRC})

message(STATUS "Source files for operator library: ${OPERATOR_SRC}")
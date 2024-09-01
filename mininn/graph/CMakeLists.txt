file(GLOB GRAPH_SRC "*.cc")
add_library(graph ${LIB_TYPE} ${GRAPH_SRC})

message(STATUS "Source files for graph library: ${GRAPH_SRC}")
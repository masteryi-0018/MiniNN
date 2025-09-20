file(GLOB GRAPH_SRC "*.cc")
add_library(graph ${LIB_TYPE} ${GRAPH_SRC})
target_link_libraries(graph PRIVATE mininn_compile_options)

message(STATUS "Source files for graph library: ${GRAPH_SRC}")
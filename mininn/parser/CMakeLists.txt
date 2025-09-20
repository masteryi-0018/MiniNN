file(GLOB PARSER_SRC "*.cc")
add_library(parser ${LIB_TYPE} ${PARSER_SRC})
target_link_libraries(parser PRIVATE mininn_compile_options)

message(STATUS "Source files for parser library: ${PARSER_SRC}")
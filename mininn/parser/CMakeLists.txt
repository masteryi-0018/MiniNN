file(GLOB PARSER_SRC "*.cc")
add_library(parser ${LIB_TYPE} ${PARSER_SRC})

message(STATUS "Source files for parser library: ${PARSER_SRC}")
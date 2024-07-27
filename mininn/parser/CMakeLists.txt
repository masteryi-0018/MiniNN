file(GLOB PARSER_SRC "*.cc")
add_library(parser SHARED ${PARSER_SRC})
target_link_libraries(parser graph)
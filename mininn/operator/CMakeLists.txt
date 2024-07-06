file(GLOB OPERATOR_SRC "*.cc")
add_library(operator SHARED ${OPERATOR_SRC})
# target_link_libraries(operator utils)
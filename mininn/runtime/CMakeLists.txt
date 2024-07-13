file(GLOB RUNTIME_SRC "*.cc")
add_library(runtime SHARED ${RUNTIME_SRC})
target_link_libraries(runtime graph kernel)
file(GLOB BIND_SRC "*.cc")
pybind11_add_module(mininn MODULE ${BIND_SRC})
target_link_libraries(mininn PRIVATE runtime kernel operator parser graph utils)

message(STATUS "Source files for mininn library: ${BIND_SRC}")
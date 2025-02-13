file(GLOB BIND_SRC "*.cc")
pybind11_add_module(mininn_capi MODULE ${BIND_SRC})
target_link_libraries(mininn_capi PRIVATE runtime parser graph utils kernel operator)

message(STATUS "Source files for mininn_capi library: ${BIND_SRC}")
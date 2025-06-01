file(GLOB BIND_SRC "*.cc")
pybind11_add_module(mininn_capi MODULE ${BIND_SRC})

message(STATUS "Source files for mininn_capi library: ${BIND_SRC}")

target_link_libraries(mininn_capi PRIVATE mininn_lib)
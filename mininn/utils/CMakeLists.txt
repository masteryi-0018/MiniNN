file(GLOB UTILS_SRC "*.cc")
add_library(utils ${LIB_TYPE} ${UTILS_SRC})
# target_link_libraries(utils parser)

message(STATUS "Source files for utils library: ${UTILS_SRC}")
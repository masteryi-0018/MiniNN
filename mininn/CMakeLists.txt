# 1. environment
option(WITH_GTEST "build with gtest" ON)

if(UNIX)
    # set(LIB_TYPE SHARED)
    # pybind need one static lib to produce a shared lib, so use a OBJECT to handle it
    set(LIB_TYPE OBJECT)
else(WIN32)
    set(LIB_TYPE STATIC)
endif()

# 2. subdirectory
add_subdirectory(utils)
add_subdirectory(graph)
add_subdirectory(operator)
add_subdirectory(kernel)
add_subdirectory(runtime)
add_subdirectory(parser)
add_subdirectory(backend)

add_library(mininn_lib STATIC
    $<TARGET_OBJECTS:runtime>
    $<TARGET_OBJECTS:parser>
    $<TARGET_OBJECTS:graph>
    $<TARGET_OBJECTS:kernel>
    $<TARGET_OBJECTS:operator>
    $<TARGET_OBJECTS:utils>
)

# 3. bin
if(WITH_GTEST)
    add_subdirectory(test)
endif()
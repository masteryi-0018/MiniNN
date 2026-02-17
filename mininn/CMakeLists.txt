# 1. environment
option(WITH_GTEST "build with gtest" ON)

option(WITH_MULTI_THREADS "build with multi_threads" ON)
option(WITH_AVX "build with avx" ON)
option(WITH_SSE "build with sse" ON)
option(WITH_NEON "build with neon" ON)
option(WITH_METAL "build with metal" ON)

# pybind need one static lib to produce a shared lib, so use a OBJECT to handle it
set(LIB_TYPE OBJECT) # options: OBJECT, STATIC, SHARED

# use interface library to manage compile options
add_library(mininn_compile_options INTERFACE)

# set strict compile options for the interface library
if(MSVC)
    string(REGEX REPLACE "/W[0-4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    target_compile_options(mininn_compile_options INTERFACE
        /W4       # default warning is level 3
        /WX       # treat warnings as errors
    )
else()
    target_compile_options(mininn_compile_options INTERFACE
        -Werror
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

# 2. subdirectory
add_subdirectory(runtime)
add_subdirectory(parser)
add_subdirectory(graph)
add_subdirectory(operator)
add_subdirectory(kernel)
add_subdirectory(utils)

add_library(mininn_lib STATIC
    $<TARGET_OBJECTS:runtime>
    $<TARGET_OBJECTS:parser>
    $<TARGET_OBJECTS:graph>
    $<TARGET_OBJECTS:operator>
    $<TARGET_OBJECTS:kernel>
    $<TARGET_OBJECTS:utils>
)

# seperate the backend lib
add_subdirectory(backend)
target_link_libraries(mininn_lib backend)

# 3. binary
if(WITH_GTEST)
    add_subdirectory(test)
endif()

# 4. install
install(
    DIRECTORY
        ${MININN_SOURCE_DIR}/mininn/runtime
        ${MININN_SOURCE_DIR}/mininn/parser
        ${MININN_SOURCE_DIR}/mininn/graph
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mininn
    FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*/mininn_generated.h" EXCLUDE
        PATTERN "*/register.h" EXCLUDE
)

install(TARGETS mininn_lib
    # shared library
    # LIBRARY DESTINATION lib
    # static library
    ARCHIVE DESTINATION lib
)
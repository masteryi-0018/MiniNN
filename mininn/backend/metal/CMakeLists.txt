file(GLOB METAL_SRC "*.mm")

add_library(metal ${BACKEND_LIB_TYPE} ${METAL_SRC})

# pre build metallib
set(METAL_SHADER ${CMAKE_CURRENT_SOURCE_DIR}/add.metal)
set(AIR_FILE ${CMAKE_CURRENT_BINARY_DIR}/add.air)
set(METALLIB_FILE ${CMAKE_CURRENT_BINARY_DIR}/backend.metallib)

add_custom_command(
    OUTPUT ${AIR_FILE}
    COMMAND xcrun -sdk macosx metal -c ${METAL_SHADER} -o ${AIR_FILE}
    DEPENDS ${METAL_SHADER}
    COMMENT "Compiling add.metal"
)

add_custom_command(
    OUTPUT ${METALLIB_FILE}
    COMMAND xcrun -sdk macosx metallib ${AIR_FILE} -o ${METALLIB_FILE}
    DEPENDS ${AIR_FILE}
    COMMENT "Linking metallib"
)

add_custom_target(
    metal_shaders ALL
    DEPENDS ${METALLIB_FILE}
)

add_dependencies(metal metal_shaders)
# end

target_link_libraries(metal
    "-framework Metal"
    "-framework Foundation"
)

message(STATUS "Source files for backend-metal library: ${METAL_SRC}")
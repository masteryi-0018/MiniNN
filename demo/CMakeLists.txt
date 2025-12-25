file(GLOB DEMO_SRC "*.cc")
add_executable(demo ${DEMO_SRC})

message(STATUS "Source files for demo binary: ${DEMO_SRC}")

# include_directories(../mininn_sdk/include)
target_link_libraries(demo mininn_lib)
if(UNIX AND NOT ANDROID)
    target_link_libraries(demo pthread)
endif()

install(TARGETS demo
    RUNTIME DESTINATION bin
)
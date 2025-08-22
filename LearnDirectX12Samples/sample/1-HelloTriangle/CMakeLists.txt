project(1-HelloTriangle)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(_DEBUG)  
endif()

add_executable(${PROJECT_NAME} main.cpp helloTriangle.cpp)

set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)

target_link_libraries(${PROJECT_NAME}
    PRIVATE DXSampleBase   
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders.hlsl
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
)
project(SupportResolutionOptions LANGUAGES CXX)

add_executable(${PROJECT_NAME} main.cpp SupportResolutionOptions.cpp)

set_target_output_path(${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)

target_link_libraries(${PROJECT_NAME}
    PRIVATE DXSampleBase2   
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders.hlsl
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
)
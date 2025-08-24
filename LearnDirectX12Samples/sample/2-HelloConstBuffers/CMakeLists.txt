project(2-HelloConstBuffers)

set(SampleAppName ${PROJECT_NAME}App)

add_executable(${SampleAppName} main.cpp HelloConstBuffers.cpp )

set_target_properties(${SampleAppName} PROPERTIES WIN32_EXECUTABLE TRUE)

target_link_libraries(${SampleAppName}
    PRIVATE 1-HelloTriangleLib  
)

add_custom_command(TARGET ${SampleAppName} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders.hlsl
    $<TARGET_FILE_DIR:${SampleAppName}>
)
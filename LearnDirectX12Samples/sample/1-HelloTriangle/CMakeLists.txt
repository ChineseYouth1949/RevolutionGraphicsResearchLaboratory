project(1-HelloTriangle)

set(SampleAppName ${PROJECT_NAME}App)

add_executable(${SampleAppName} main.cpp HelloTriangle.cpp)

set_target_properties(${SampleAppName} PROPERTIES WIN32_EXECUTABLE TRUE)

target_link_libraries(${SampleAppName}
    PRIVATE DXSampleBase   
)

add_custom_command(TARGET ${SampleAppName} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders.hlsl
    $<TARGET_FILE_DIR:${SampleAppName}>
)


set(SampleLibName ${PROJECT_NAME}Lib)

add_library(${SampleLibName} STATIC 
    HelloTriangle.h 
    HelloTriangle.cpp)

target_link_libraries(${SampleLibName} PUBLIC SampleBase)
target_include_directories(${SampleLibName} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/HelloTriangle.cpp
)
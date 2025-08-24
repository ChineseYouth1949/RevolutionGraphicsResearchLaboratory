project(1-HelloTriangle)

add_executable(${PROJECT_NAME} main.cpp HelloTriangle.cpp)

set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)

target_link_libraries(${PROJECT_NAME}
    PRIVATE DXSampleBase   
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders.hlsl
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
)


add_library(${PROJECT_NAME}Lib STATIC 
    HelloTriangle.h 
    HelloTriangle.cpp)

target_link_libraries(${PROJECT_NAME}Lib PUBLIC DXSampleBase)
target_include_directories(${PROJECT_NAME}Lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/base              
)
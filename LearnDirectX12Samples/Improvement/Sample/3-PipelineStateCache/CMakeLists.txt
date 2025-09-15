project(PipelineStateCache LANGUAGES CXX)

file(GLOB_RECURSE LIB_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE LIB_HDRS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

add_executable(${PROJECT_NAME} ${LIB_SRCS} ${LIB_HDRS})

set_target_output_path(${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Header
    ${CMAKE_CURRENT_SOURCE_DIR}/Header/Shader
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE DXSampleBase2   
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${AssetsPath}/${PROJECT_NAME}/BlitPixelShader.hlsl
    ${AssetsPath}/${PROJECT_NAME}/BlurPixelShader.hlsl
    ${AssetsPath}/${PROJECT_NAME}/DistortPixelShader.hlsl
    ${AssetsPath}/${PROJECT_NAME}/EdgeDetectPixelShader.hlsl
    ${AssetsPath}/${PROJECT_NAME}/GrayScalePixelShader.hlsl
    ${AssetsPath}/${PROJECT_NAME}/InvertPixelShader.hlsl
    ${AssetsPath}/${PROJECT_NAME}/PixelatePixelShader.hlsl
    ${AssetsPath}/${PROJECT_NAME}/QuadVertexShader.hlsl
    ${AssetsPath}/${PROJECT_NAME}/SimplePixelShader.hlsl
    ${AssetsPath}/${PROJECT_NAME}/UberPixelShader.hlsl
    ${AssetsPath}/${PROJECT_NAME}/WarpPixelShader.hlsl
    ${AssetsPath}/${PROJECT_NAME}/WavePixelShader.hlsl
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
)
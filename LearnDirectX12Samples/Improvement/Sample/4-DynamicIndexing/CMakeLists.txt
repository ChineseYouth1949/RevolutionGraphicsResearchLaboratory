project(DynamicIndexing LANGUAGES CXX)

add_executable(${PROJECT_NAME} main.cpp DynamicIndexing.cpp FrameResource.cpp)

set_target_output_path(${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)

target_link_libraries(${PROJECT_NAME}
    PRIVATE DXSampleBase2   
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${AssetsPath}/${PROJECT_NAME}/occcity.bin
    ${AssetsPath}/${PROJECT_NAME}/shader_mesh_simple_vert.hlsl
    ${AssetsPath}/${PROJECT_NAME}/shader_mesh_dynamic_indexing_pixel.hlsl
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
)
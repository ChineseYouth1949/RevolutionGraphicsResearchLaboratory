project(MiniEngine LANGUAGES CXX)

file(GLOB_RECURSE LIB_SRCS CONFIGURE_DEPENDS 
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

file(GLOB_RECURSE LIB_HDRS CONFIGURE_DEPENDS 
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

add_executable(${PROJECT_NAME} ${LIB_SRCS} ${LIB_HDRS})

set_target_output_path(${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    # ${AssetsPath}/${PROJECT_NAME}/occcity.bin
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
)
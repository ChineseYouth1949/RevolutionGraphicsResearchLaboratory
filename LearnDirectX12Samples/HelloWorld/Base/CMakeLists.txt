cmake_minimum_required(VERSION 3.10)

project(DXSampleBase LANGUAGES CXX)

file(GLOB_RECURSE LIB_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE LIB_HDRS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

add_library(${PROJECT_NAME} STATIC ${LIB_SRCS} ${LIB_HDRS})

set_target_output_path(${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PUBLIC _DEBUG)
endif()

target_include_directories(${PROJECT_NAME} 
    PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    d3d12.lib
    dxgi.lib
    d3dcompiler.lib
    dxguid.lib
)
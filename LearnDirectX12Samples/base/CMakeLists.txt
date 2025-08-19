cmake_minimum_required(VERSION 3.10)

project(SampleBase LANGUAGES CXX)

file(GLOB_RECURSE LIB_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE LIB_HDRS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

message("LIB_SRCS files:")
foreach(file ${LIB_SRCS})
    message("  ${file}")
endforeach()

message("LIB_HDRS files:")
foreach(file ${LIB_HDRS})
    message("  ${file}")
endforeach()

add_library(${PROJECT_NAME} STATIC ${LIB_SRCS} ${LIB_HDRS})

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(${PROJECT_NAME} 
    PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

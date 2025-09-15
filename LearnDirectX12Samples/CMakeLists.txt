cmake_minimum_required(VERSION 3.19)

project(RG_Lab VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

function(set_target_output_path target_name)
    set(OUTPUT_BASE_DIR "${RUNTIME_ARCHIVE_LIBRARY_BASE_DIRECTORY}")
    set_target_properties(${target_name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_BASE_DIR}/${PROJECT_NAME}/Bin"
        ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_BASE_DIR}/${PROJECT_NAME}/Lib"
        LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_BASE_DIR}/${PROJECT_NAME}/Lib"
    )
endfunction()

add_subdirectory(HelloWorld)
add_subdirectory(Improvement)
add_subdirectory(MiniEngine)
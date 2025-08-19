project(1-HelloTriangle VERSION 1.0)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC SampleBase)
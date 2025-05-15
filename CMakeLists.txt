cmake_minimum_required(VERSION 3.25.0)
project(RGS VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src)

add_executable(${PROJECT_NAME} main.cpp test.hpp ${src_list})

target_link_libraries(${PROJECT_NAME} PUBLIC myLib)
target_include_directories(${PROJECT_NAME} PUBLIC include)

target_compile_definitions(${PROJECT_NAME} PRIVATE _GLIBCXX_DEBUG)
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
cmake_minimum_required(VERSION 3.5.0)
project(RGS VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_BUILD_TYPE "Debug")

include_directories(include)
aux_source_directory(src src_list)

add_executable(${PROJECT_NAME} main.cpp ${src_list})

#target_include_directories(${PROJECT_NAME} PUBLIC include)
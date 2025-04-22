cmake_minimum_required(VERSION 3.5.0)
project(CSR VERSION 0.1.0 LANGUAGES C CXX)

aux_source_directory(src src_list)
add_executable(CSR main.cpp ${src_list})

target_include_directories(CSR PUBLIC include)
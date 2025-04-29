file(GLOB_RECURSE SRC "*.cpp")
add_library(myLib ${SRC})

target_include_directories(myLib PUBLIC ${PROJECT_SOURCE_DIR}/include)
file(GLOB_RECURSE SRC "*.cpp")
add_library(myLib SHARED ${SRC})

target_include_directories(myLib PUBLIC ${PROJECT_SOURCE_DIR}/include)
set_target_properties(myLib PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    )
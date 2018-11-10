cmake_minimum_required(VERSION 3.5)
project(ekcc)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

set(PARENT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(util)
add_subdirectory(parser)
add_subdirectory(ast)
add_subdirectory(errorcheck)
add_subdirectory(codegen)

include_directories(
    ${PARENT_DIR}
)

file(GLOB EKCC_SOURCES ekcc.cpp)
add_executable(ekcc ${EKCC_SOURCES})

target_link_libraries(
    ekcc util parser ast errorcheck codegen 
)
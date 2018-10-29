cmake_minimum_required(VERSION 3.5)
project(ekcc)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

file(GLOB SOURCES *.cpp)

add_subdirectory(parser)
add_subdirectory(util)

add_executable(ekcc ${SOURCES})

target_link_libraries(ekcc parser util -lyaml-cpp)

install (TARGETS ekcc DESTINATION ${CMAKE_BINARY_DIR}/bin)

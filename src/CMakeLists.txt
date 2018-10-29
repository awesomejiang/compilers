cmake_minimum_required(VERSION 3.5)
project(ekcc)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

file(GLOB EKCC_SOURCES ekcc.cpp)
file(GLOB FUZZER_SOURCES fuzzerTest.cpp)

add_subdirectory(parser)
add_subdirectory(util)

add_executable(fuzzer ${FUZZER_SOURCES})
target_link_libraries(fuzzer parser util -lyaml-cpp -fsanitize=fuzzer,address)

add_executable(ekcc ${EKCC_SOURCES})
target_link_libraries(ekcc parser util -lyaml-cpp)


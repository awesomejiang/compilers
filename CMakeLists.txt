cmake_minimum_required(VERSION 3.5)
project(ekcc)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

file(GLOB HEADERS "include/*.h" "include/*.hpp")
file(GLOB SOURCES "src/*.cpp")

add_executable(ekcc ${SOURCES})

target_include_directories(ekcc PRIVATE ${CMAKE_SOURCE_DIR}/include)

#target_link_libraries()

install (TARGETS ekcc DESTINATION ${CMAKE_BINARY_DIR}/bin)
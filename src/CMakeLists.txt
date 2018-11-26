cmake_minimum_required(VERSION 3.5)
project(ekcc)

# set compiler args
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

# find packages needed by "node.h"
## yaml-cpp
find_package(Yaml-cpp REQUIRED)
message(STATUS "Found Yaml-cpp ${Yaml-cpp_VERSION}")
## llvm
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
    #message(STATUS "Found LLVM at ${LLVM_INCLUDE_DIRS}")
add_definitions(${LLVM_DEFINITIONS})

# add includes
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/parser #we need flexlexer.h in is dir.
    ${LLVM_INCLUDE_DIRS}
)

# generate .a sub libs
add_subdirectory(codegen)
add_subdirectory(util)
add_subdirectory(parser)
add_subdirectory(ast)
add_subdirectory(errorcheck)

# define executable program
file(GLOB EKCC_SOURCES ekcc.cpp)
add_executable(ekcc ${EKCC_SOURCES})

target_link_libraries(
    ekcc util parser ast errorcheck codegen 
)
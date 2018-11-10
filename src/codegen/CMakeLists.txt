find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")

include_directories(
    ${LLVM_INCLUDE_DIRS}
    ${PARENT_DIR}
)

add_definitions(${LLVM_DEFINITIONS})

set(SOURCES
    codegen.cpp
)

add_library(codegen
    ${SOURCES}
)

llvm_map_components_to_libnames(llvm_libs support core irreader)

target_link_libraries(codegen
    ${llvm_libs}
)
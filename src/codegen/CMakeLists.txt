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
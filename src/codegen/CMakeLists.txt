set(SOURCES
    context.cpp
    codegen.cpp
)

add_library(codegen
    ${SOURCES}
)

add_dependencies(codegen parser)

llvm_map_components_to_libnames(llvm_libs
    support core irreader interpreter mcjit native ipo
)

target_link_libraries(codegen
    ${llvm_libs}
)
find_package(Yaml-cpp REQUIRED)

include_directories(
    ${PARENT_DIR}
)

set(SOURCES
    ast.cpp
)

add_library(ast
    ${SOURCES}
)

target_link_libraries(ast
    yaml-cpp
)

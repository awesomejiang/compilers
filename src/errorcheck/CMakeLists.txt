include_directories(
    ${PARENT_DIR}
)

set(SOURCES
    errorcheck.cpp
)

add_library(errorcheck
    ${SOURCES}
)
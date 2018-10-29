find_package(BISON)
find_package(FLEX)
find_package(Yaml-cpp)

BISON_TARGET(parser
    ${CMAKE_CURRENT_SOURCE_DIR}/parser.y
    ${CMAKE_CURRENT_SOURCE_DIR}/parser.cpp
)

FLEX_TARGET(lexer
    ${CMAKE_CURRENT_SOURCE_DIR}/configure.lex
    ${CMAKE_CURRENT_SOURCE_DIR}/tokens.cpp
)

ADD_FLEX_BISON_DEPENDENCY(lexer parser)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
)

set(PARSER_SOURCES
    yaml.cpp
)

add_library(parser
    ${PARSER_SOURCES}
    ${BISON_parser_OUTPUTS}
    ${FLEX_lexer_OUTPUTS})
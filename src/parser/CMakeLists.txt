find_package(BISON)
find_package(FLEX)

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
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PARENT_DIR}
)

add_library(parser
    ${BISON_parser_OUTPUTS}
    ${FLEX_lexer_OUTPUTS})

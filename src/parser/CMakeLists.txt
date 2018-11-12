find_package(BISON)
message(STATUS "Found Bison ${BISON_VERSION}")

find_package(FLEX)
message(STATUS "Found Flex ${FLEX_VERSION}")

BISON_TARGET(parser
    ${CMAKE_CURRENT_SOURCE_DIR}/parser.y
    ${CMAKE_CURRENT_SOURCE_DIR}/parser.cpp
)

FLEX_TARGET(lexer
    ${CMAKE_CURRENT_SOURCE_DIR}/configure.lex
    ${CMAKE_CURRENT_SOURCE_DIR}/tokens.cpp
)

ADD_FLEX_BISON_DEPENDENCY(lexer parser)

add_library(parser
    ${BISON_parser_OUTPUTS}
    ${FLEX_lexer_OUTPUTS})

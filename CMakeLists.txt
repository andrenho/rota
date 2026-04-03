cmake_minimum_required(VERSION 4.2)
project(fortuna_6 C)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

BISON_TARGET(parser
        compiler/parser.y
        ${CMAKE_CURRENT_BINARY_DIR}/parser.tab.c
        DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.tab.h
)

FLEX_TARGET(lexer
        compiler/lexer.l
        ${CMAKE_CURRENT_BINARY_DIR}/lexer.yy.c
)

ADD_FLEX_BISON_DEPENDENCY(lexer parser)

set(CMAKE_C_STANDARD 11)

add_executable(fortuna_6_tests tests.c ${BISON_parser_OUTPUTS} ${FLEX_lexer_OUTPUTS})
target_include_directories(fortuna_6_tests PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
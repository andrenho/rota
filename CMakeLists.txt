cmake_minimum_required(VERSION 3.31)
project(fortuna_6 C CXX)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/assembler)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/compiler)

BISON_TARGET(assembler_parser
        assembler/assembler.y
        ${CMAKE_CURRENT_BINARY_DIR}/assembler/parser.tab.cc
        DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/assembler/parser.tab.hh
)

BISON_TARGET(compiler_parser
        compiler/compiler.y
        ${CMAKE_CURRENT_BINARY_DIR}/compiler/parser.tab.cc
        DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/compiler/parser.tab.hh
)

FLEX_TARGET(assembler_lexer
        assembler/assembler.l
        ${CMAKE_CURRENT_BINARY_DIR}/assembler/lexer.yy.cc
        DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/assembler/lexer.yy.hh
)

FLEX_TARGET(compiler_lexer
        compiler/compiler.l
        ${CMAKE_CURRENT_BINARY_DIR}/compiler/lexer.yy.cc
        DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/compiler/lexer.yy.hh
)

ADD_FLEX_BISON_DEPENDENCY(assembler_lexer assembler_parser)
ADD_FLEX_BISON_DEPENDENCY(compiler_lexer compiler_parser)

set(CMAKE_C_STANDARD 11)

add_executable(fortuna_6_tests
        tests.cc
        assembler/assembler.hh assembler/assembler.cc
        compiler/compiler.hh compiler/compiler.cc
        common/value.hh common/value.cc
        vm/vm.hh vm/vm.cc
        ${BISON_assembler_parser_OUTPUTS} ${FLEX_assembler_lexer_OUTPUTS}
        ${BISON_compiler_parser_OUTPUTS} ${FLEX_compiler_lexer_OUTPUTS})

target_include_directories(fortuna_6_tests PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
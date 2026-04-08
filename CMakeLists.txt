cmake_minimum_required(VERSION 3.30)

project(rota C CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/vm/compiler)

BISON_TARGET(compiler_parser
        vm/compiler/compiler.y
        ${CMAKE_CURRENT_BINARY_DIR}/vm/compiler/parser.tab.cc
        DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/vm/compiler/parser.tab.hh
)

FLEX_TARGET(compiler_lexer
        vm/compiler/compiler.l
        ${CMAKE_CURRENT_BINARY_DIR}/vm/compiler/lexer.yy.cc
        DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/vm/compiler/lexer.yy.hh
)

ADD_FLEX_BISON_DEPENDENCY(compiler_lexer compiler_parser)

set(VM_SRC
        ${BISON_compiler_parser_OUTPUTS} ${FLEX_compiler_lexer_OUTPUTS}
        vm/rotavm.cc
        vm/rotavm.hh
        vm/exceptions.hh
        vm/value.hh
        vm/value.cc
        vm/optable.cc
        vm/optable.hh
        vm/type.hh
        vm/opcode.hh
        vm/compiler/compiler.cc
        vm/compiler/compiler.hh)

add_executable(rota-vm-test
        tests.cc
        ${VM_SRC}
)

target_include_directories(rota-vm-test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
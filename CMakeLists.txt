cmake_minimum_required(VERSION 3.30)

project(rota C CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/interpreter)

BISON_TARGET(interpreter_parser
        interpreter/interpreter.y
        ${CMAKE_CURRENT_BINARY_DIR}/interpreter/parser.tab.cc
        DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/interpreter/parser.tab.hh
)

FLEX_TARGET(interpreter_lexer
        interpreter/interpreter.l
        ${CMAKE_CURRENT_BINARY_DIR}/interpreter/lexer.yy.cc
        DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/interpreter/lexer.yy.hh
)

ADD_FLEX_BISON_DEPENDENCY(interpreter_lexer interpreter_parser)

set(VM_SRC
        vm/rotavm.cc
        vm/rotavm.hh
        vm/exceptions.hh
        vm/value.hh
        vm/value.cc
        vm/optable.cc
        vm/optable.hh
        compiler/compiler.cc
        compiler/compiler.hh)

set(INTERPRETER_SRC
        ${BISON_interpreter_parser_OUTPUTS} ${FLEX_interpreter_lexer_OUTPUTS}
        interpreter/interpreter.cc
        interpreter/interpreter.hh
        vm/type.hh
        vm/opcode.hh)

add_executable(rota-vm-test
        vm/tests.cc
        ${VM_SRC}
        ${INTERPRETER_SRC}
)

target_include_directories(rota-vm-test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
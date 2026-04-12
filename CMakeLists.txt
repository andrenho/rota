cmake_minimum_required(VERSION 3.30)

project(rota C CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/compiler)

BISON_TARGET(compiler_parser
        compiler/compiler.y
        ${CMAKE_CURRENT_BINARY_DIR}/compiler/parser.tab.cc
        DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/compiler/parser.tab.hh
)

FLEX_TARGET(compiler_lexer
        compiler/compiler.l
        ${CMAKE_CURRENT_BINARY_DIR}/compiler/lexer.yy.cc
        DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/compiler/lexer.yy.hh
)

ADD_FLEX_BISON_DEPENDENCY(compiler_lexer compiler_parser)

set(VM_SRC
        ${BISON_compiler_parser_OUTPUTS} ${FLEX_compiler_lexer_OUTPUTS}
        assembly/assembly.cc
        assembly/assembly.hh
        assembly/assemblybuilder.cc
        assembly/assemblybuilder.hh
        compiler/compiler.cc
        compiler/compiler.hh
        vm/vm.cc
        vm/vm.hh)

add_executable(rota-vm-test
        tests.cc
        ${VM_SRC}
)

target_include_directories(rota-vm-test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

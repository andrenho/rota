#include <iostream>

#include "assembler/assembler.hh"
#include "compiler/compiler.hh"

int main()
{
    std::cout << assembler::assemble(compiler::compile({ "1 + 4 * 3\n" }));
}
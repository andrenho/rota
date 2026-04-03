#include <iostream>

#include "compiler/compiler.hh"

int main()
{
    std::cout << compiler::compile("1 + 4 * 3\n");
}
#include <cstring>
#include <iostream>
#include <string>

#include "vm/vm.hh"
#include "bytecode/bytecodesimple.hh"

static bool debug = false;

static void validate_vm() {
    auto bs = std::make_unique<BytecodeSimple>();
    bs->add(Operation::PushInt, 500);
    std::cout << "Compiled code:\n";
    std::cout << bs->decompile_bytecode();

    VM vm; vm.set_bytecode(std::move(bs));
    std::cout << "Execution:\n";
    vm.run_debug_console();
    std::cout << "------------------------\n";
}

int main(int argc, char* argv[])
{
    debug = (argc == 2 && std::string(argv[1]) == "-d");

    validate_vm();
}

#include <cstring>
#include <iostream>
#include <string>

#include "compiler/compiler.hh"
#include "vm/vm.hh"
#include "bytecode/bytecodesimple.hh"

static bool debug = false;

using VMType = BytecodeSimple;

static void validate_vm()
{
    auto bs = std::make_unique<BytecodeSimple>();
    bs->add(0, Operation::PushInt, 500);
    std::cout << "Compiled code:\n";
    std::cout << bs->decompile_bytecode();

    VM vm; vm.set_bytecode(std::move(bs));
    std::cout << "Execution:\n";
    vm.run_debug_console();
    std::cout << "------------------------\n";
}

template <typename T>
static void test(std::string const& code, T t) {
    VM vm;
    VM::Value expected(t);

    printf("-----------------------------\n");
    printf("%s\n", code.c_str());

    auto bytecode = std::make_unique<VMType>();
    try {
        compile(code, bytecode.get());
        if (debug)
            printf("%s\n", bytecode->decompile_bytecode().c_str());

        vm.set_bytecode(std::move(bytecode));
        if (debug)
            vm.run_debug_console();
        else
            vm.run();

        VM::Value v = vm.debug_saved(), e = expected;
        if (v != e) {
            char buf[255]; snprintf(buf, sizeof(buf), "Expected: %s, found %s", std::to_string(e).c_str(),
                    std::to_string(v).c_str());
            throw std::runtime_error(buf);
        }

        if (vm.stack_sz() != 0)
            throw std::runtime_error("Queue not empty (size " + std::to_string(vm.stack_sz()) + ")");

        printf("  --> \e[0;32mok (%s)\e[0m\n", std::to_string(v).c_str());

    } catch (std::exception& e) {
        printf("  --> \e[0;31m%s\e[0m\n", e.what());
        printf("      \e[0;31m%s\e[0m\n", vm.debug_stack().c_str());
        exit(1);
    }
}

int main(int argc, char* argv[])
{
    debug = (argc == 2 && std::string(argv[1]) == "-d");

    // validate_vm();

    test("@= 500;", 500);
}

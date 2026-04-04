#include <cstdio>
#include <cstring>
#include <stdexcept>

#include "assembler/assembler.hh"
#include "compiler/compiler.hh"
#include "vm/vm.hh"

void test(std::string const& code, Val32 expected, bool show_intermediate=false)
{
    printf("-----------------------------\n");

    printf("%s", code.c_str());

    try {
        std::string assembly = compiler::compile({ code });
        if (show_intermediate)
            printf("%s\n", assembly.c_str());

        auto bytes = assembler::assemble(assembly);
        if (show_intermediate) {
            for (auto byte: bytes)
                printf("%02X ", byte);
            printf("\n");
        }

        VM vm({});
        vm.set_ram(bytes.data(), bytes.size());
        vm.run_until_halt();
        if (show_intermediate)
            printf("A = %d\n", vm.peek());

        if (vm.peek() != expected) {
            char buf[255]; snprintf(buf, sizeof(buf), "Expected: %d, found %d", expected, vm.peek());
            throw std::runtime_error(buf);
        }

        printf("  --> \e[0;32mok (%d)\e[0m\n", vm.peek());

    } catch (std::exception& e) {
        printf("  --> \e[0;31m%s\e[0m\n", e.what());
    }
}

int main(int argc, char* argv[])
{
    bool s = (argc == 2 and strcmp(argv[1], "-d") == 0);  // show intermediate

    test("1 + 4 * 3\n", 13, s);
    test("(1 + 4) * 3\n", 15, s);
    test("1.2\n", 0, s);
}
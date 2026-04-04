#include <cstdio>

#include "assembler/assembler.hh"
#include "compiler/compiler.hh"
#include "vm/vm.hh"

int main()
{
    auto src = "1 + 4 * 3\n";
    printf("%s\n", compiler::compile({ src }).c_str());
    auto bytes = assembler::assemble(compiler::compile({ src }));
    for (auto byte: bytes)
        printf("%02X ", byte);
    printf("\n");

    VM vm({});
    vm.set_ram(bytes.data(), bytes.size());
    vm.run_until_halt();
    printf("A = %d\n", vm.peek());
}
#include <cstring>
#include <iostream>
#include <string>

#include "assembly/bc_generator.hh"
#include "vm/vm.hh"

static bool debug = false;

static void validate_bytecode()
{
    BytecodeGenerator bgen;
    bgen.push(500);
    Bytecode bc = bgen.generate_bytecode();

    uint8_t expected_binary[] = {
            0xa9, 0xb8, 0xc7, 0xd6,     // magic
            0x40, 0x00, 0x00, 0x00,     // code
            0x43, 0x00, 0x00, 0x00,     // empty
            0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00,
            0x02, 0xf4, 0x01,           // code: PUSH 500
    };

    if (bc.binary().size() != sizeof(expected_binary))
        throw std::runtime_error("Unexpected size difference (expected " + std::to_string(sizeof(expected_binary)) + ", found " + std::to_string(bc.binary().size()));

    if (memcmp(bc.binary().data(), expected_binary, sizeof(expected_binary)) != 0)
        throw std::runtime_error("Expected and generated binaries are different.");
}

int main(int argc, char* argv[])
{
    debug = (argc == 2 && std::string(argv[1]) == "-d");

    validate_bytecode();
}

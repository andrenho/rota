#include "bytecode.hh"

using namespace std::string_literals;

std::string Bytecode::decompile_bytecode() const
{
    std::string out;
    size_t pc = 0;

    while (pc < last_pc()) {
        size_t sz;
        out += "\t"s + decompile_instruction_at(pc, &sz) + "\n";
        pc += sz;
    }

    return out;
}

std::string Bytecode::decompile_instruction_at(size_t pc, size_t* psz) const
{
    auto [inst, sz] = next_instruction(pc);
    if (psz)
        *psz = sz;

    switch (inst.operation) {
        case Operation::PushInt:
            return "PUSHI "s + std::to_string(std::get<int32_t>(*inst.operand1));
        default:
            return "???";
    }
}

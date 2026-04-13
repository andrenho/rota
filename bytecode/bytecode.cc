#include "bytecode.hh"

using namespace std::string_literals;

std::string Bytecode::decompile_bytecode() const
{
    std::string out;
    size_t pc = 0;

    for (FunctionId f_id = 0; f_id < n_functions(); ++f_id) {
        out += ".function " + std::to_string(f_id) + "\n";
        pc = 0;
        while (pc < last_pc(f_id)) {
            size_t sz;
            out += "\t"s + decompile_instruction_at(f_id, pc, &sz) + "\n";
            pc += sz;
        }
    }

    return out;
}

std::string Bytecode::decompile_instruction_at(FunctionId f_id, size_t pc, size_t* psz) const
{
    auto [inst, sz] = next_instruction(f_id, pc);
    if (psz)
        *psz = sz;

    switch (inst.operation) {
        case Operation::PushInt:
            return "PUSHI "s + std::to_string(std::get<int32_t>(*inst.operand1));
        case Operation::SaveDebug:
            return "DEBUG";
        case Operation::Pop:
            return "POP";
        default:
            return "???";
    }
}

Bytecode::FunctionId Bytecode::add_function(std::optional<std::string> const& name)
{
    return function_ct++;
}

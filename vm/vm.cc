#include "vm.hh"

#include <cstdio>
#include <string>

#include "vmexceptions.hh"

using namespace std::string_literals;

void VM::run(std::function<void(size_t)> const& after_each_instruction)
{
    while (pc_ < bytecode_->last_pc()) {
        size_t pc = pc_;
        step();
        after_each_instruction(pc);
    }
}

void VM::run_debug_console()
{
    auto format_value = [](Value const& value) -> std::string {
        if (auto* i = std::get_if<int32_t>(&value))
            return std::to_string(*i);
        return "???";
    };

    run([this, &format_value](size_t pc) {
        printf("PC: %zu\n", pc);
        printf("  Instruction: %s\n", bytecode_->decompile_instruction_at(pc).c_str());

        printf("  Stack: %s\n", debug_stack().c_str());
    });
}

void VM::step()
{
    auto [inst, sz] = bytecode_->next_instruction(pc_);
    switch (inst.operation) {
        case Operation::PushInt:
            stack_.emplace_back(std::get<int32_t>(*inst.operand1));
            break;
        default:
            throw VMInvalidOpcodeException();
    }
    pc_ += sz;
}

std::string VM::debug_stack() const
{
    auto format_value = [](Value const& value) -> std::string {
        if (auto* i = std::get_if<int32_t>(&value))
            return std::to_string(*i);
        return "???";
    };

    if (stack_.empty()) {
        return "empty";
    } else {
        std::string out;
        for (auto const& value: stack_)
            out += "["s + format_value(value) + "] ";
        return out;
    }
}

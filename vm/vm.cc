#include "vm.hh"
#include "../common/opcodes.hh"

#include <cstring>
#include <stdexcept>

VM::VM(VMDef&& vmdef)
    : vm_def_(vmdef)
{
    for (auto& f : oper)
        f = [](VM&) { throw std::runtime_error("Opcode with no operation"); };

    oper[OP_NOP]  = [](VM&) {};
    oper[OP_PUSH] = [](VM& vm) { vm.push(vm.nextVal()); };
    oper[OP_ADD]  = [](VM& vm) { vm.push(vm.pop() + vm.pop()); };
    oper[OP_SUB]  = [](VM& vm) { vm.push(vm.pop() - vm.pop()); };
    oper[OP_MUL]  = [](VM& vm) { vm.push(vm.pop() * vm.pop()); };
    oper[OP_DIV]  = [](VM& vm) { vm.push(vm.pop() / vm.pop()); };
    oper[OP_HALT] = [](VM& vm) {};
}

void VM::set_ram(uint8_t const* data, size_t sz)
{
    if (sz > RAM_SZ)
        throw std::runtime_error("Binary too large for ROM");
    memcpy(ram_, data, sz);
}

void VM::run_until_halt()
{
    while (ram_[PC_] != OP_HALT)
        oper[ram_[PC_++]](*this);
}

void VM::push(Val32 v)
{
    if (stack_idx_ == (STACK_SZ - 1))
        throw std::runtime_error("Stack overflow");
    stack_[stack_idx_++] = v;
}

Val32 VM::peek() const
{
    if (stack_idx_ == 0)
        throw std::runtime_error("Stack underflow");
    return stack_[stack_idx_ - 1];
}

Val32 VM::pop()
{
    if (stack_idx_ == 0)
        throw std::runtime_error("Stack underflow");
    return stack_[--stack_idx_];
}

Val32 VM::nextVal()
{
    int32_t val = ram_[PC_] | ((int32_t) ram_[PC_+1] << 8) | ((int32_t) ram_[PC_+2] << 8) | ((int32_t) ram_[PC_+3] << 8);
    PC_ += 4;
    return val;
}

#include "vm.hh"
#include "../common/opcodes.hh"

#include <cstring>
#include <stdexcept>
#include <variant>

using Value = types::Value;
#define H std::holds_alternative

VM::VM(VMDef&& vmdef)
    : vm_def_(vmdef)
{
    for (auto& f : oper)
        f = [](VM&) { throw std::runtime_error("Opcode with no operation"); };

    oper[OP_NOP]  = [](VM&) {};
    oper[OP_PUSH] = [](VM& vm) { vm.push(vm.nextVal()); };
    oper[OP_ADD]  = [](VM& vm) { vm.binary_op(vm, std::plus<>{}); };
    oper[OP_SUB]  = [](VM& vm) { vm.binary_op(vm, std::minus<>{}); };
    oper[OP_MUL]  = [](VM& vm) { vm.binary_op(vm, std::multiplies<>{}); };
    oper[OP_DIV]  = [](VM& vm) { vm.binary_op(vm, std::divides<>{}); };
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

template<typename Op>
void VM::binary_op(VM& vm, Op op) {
    Value a = vm.pop();
    Value b = vm.pop();
    if (H<int>(a) && H<int>(b))
        vm.push(op(std::get<int>(a), std::get<int>(b)));
    else if (H<int>(a) && H<float>(b))
        vm.push(op((float)std::get<int>(a), std::get<float>(b)));
    else if (H<float>(a) && H<int>(b))
        vm.push(op(std::get<float>(a), (float)std::get<int>(b)));
    else if (H<float>(a) && H<float>(b))
        vm.push(op(std::get<float>(a), std::get<float>(b)));
    else
        throw std::runtime_error("Unacceptable combination of types.");
}

void VM::push(types::Value v)
{
    if (stack_idx_ == (STACK_SZ - 1))
        throw std::runtime_error("Stack overflow");
    stack_[stack_idx_++] = v;
}

types::Value VM::peek() const
{
    if (stack_idx_ == 0)
        throw std::runtime_error("Stack underflow");
    return stack_[stack_idx_ - 1];
}

types::Value VM::pop()
{
    if (stack_idx_ == 0)
        throw std::runtime_error("Stack underflow");
    return stack_[--stack_idx_];
}

types::Value VM::nextVal()
{
    auto [value, sz] = types::from_bin(&ram_[PC_], 8);
    PC_ += sz;
    return value;
}
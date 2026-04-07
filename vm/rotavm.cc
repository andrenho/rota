#include <cmath>

#include <functional>

#include "rotavm.hh"
#include "exceptions.hh"

namespace rotavm {

//
// STACK MANIPULATION
//

void RotaVM::push(Value&& value)
{
    if (stack_idx_ == STACK_SZ)
        throw RotaException("Stack overflow");
    stack_[stack_idx_++] = value;
}

Value RotaVM::pop()
{
    if (stack_idx_ == 0)
        throw RotaException("Stack underflow");
    return std::move(stack_[--stack_idx_]);
}

Value const& RotaVM::peek() const
{
    if (stack_idx_ == 0)
        throw RotaException("Stack underflow");
    return stack_[stack_idx_ - 1];
}

std::string RotaVM::debug_stack() const
{
    std::string ret;
    for (size_t i = 0; i < stack_idx_; ++i)
        ret += "[" + stack_[i].debug() + "] ";
    return ret;
}

//
// ARITHMETIC
//

void RotaVM::sum()
{
    Value a = pop();
    Value b = pop();

    push(b + a);
}

void RotaVM::subtract()
{
    Value a = pop();
    Value b = pop();

    push(b - a);
}

void RotaVM::multiply()
{
    Value a = pop();
    Value b = pop();

    push(b * a);
}

void RotaVM::divide()
{
    Value a = pop();
    Value b = pop();

    push(b / a);
}

void RotaVM::idivide()
{
    Value a = pop();
    Value b = pop();

    push(b.int_divide(a));
}

void RotaVM::modulo()
{
    Value a = pop();
    Value b = pop();

    push(b % a);
}

void RotaVM::power()
{
    Value a = pop();
    Value b = pop();

    push(b ^ a);
}

}
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
// OPERATORS
//

#define BINARY_OP(method, operation) void RotaVM::method() { Value a = pop(); Value b = pop(); push(operation); }
#define UNARY_OP(method, operation) void RotaVM::method() { Value a = pop(); push(operation); }

// arithmetic

BINARY_OP(sum,      b + a)
BINARY_OP(subtract, b - a)
BINARY_OP(multiply, b * a)
BINARY_OP(divide,   b / a)
BINARY_OP(idivide,  b.int_divide(a))
BINARY_OP(modulo,   b % a)
BINARY_OP(power,    b ^ a);

// logic

BINARY_OP(equals,                b == a)
BINARY_OP(not_equal,             b != a)
BINARY_OP(greater_than,          b > a)
BINARY_OP(less_than,             b < a)
BINARY_OP(greater_than_or_equal, b >= a)
BINARY_OP(less_than_or_equal,    b <= a)
BINARY_OP(and_,                  b && a)
BINARY_OP(or_,                   b || a)
UNARY_OP(not_,                   !a)

#undef BINARY_OP
#undef UNARY_OP

}
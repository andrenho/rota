#include <cmath>

#include <functional>

#include "rotavm.hh"
#include "exceptions.hh"
#include "opcode.hh"

namespace rotavm {

//
// EXECUTABLE
//

void RotaVM::run_until_halt()
{
    for (;;) {
        if (executable_.at(PC_) == (uint8_t) OpCode::Halt)
            return;
        step();
    }
}

inline void RotaVM::step()
{
    switch ((OpCode) executable_.at(PC_)) {
        case OpCode::Nop:
            break;
        case OpCode::Push:
            break;
        case OpCode::Pop:
            break;
        case OpCode::Sum:
            break;
        case OpCode::Subtract:
            break;
        case OpCode::Multiply:
            break;
        case OpCode::Divide:
            break;
        case OpCode::IntDivide:
            break;
        case OpCode::Modulo:
            break;
        case OpCode::Power:
            break;
        case OpCode::Equals:
            break;
        case OpCode::NotEqual:
            break;
        case OpCode::GreaterThan:
            break;
        case OpCode::LessThan:
            break;
        case OpCode::GreaterThanOrEqual:
            break;
        case OpCode::LessThanOrEqual:
            break;
        case OpCode::And:
            break;
        case OpCode::Or:
            break;
        case OpCode::Not:
            break;
        case OpCode::Halt:
            break;
    }
}

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
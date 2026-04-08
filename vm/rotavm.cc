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
        if (!step())
            return;
    }
}

inline bool RotaVM::step()
{
    auto pop2 = [this]() -> std::pair<Value, Value> {
        Value a = pop();
        Value b = pop();
        return { a, b };
    };

    Executable::Token token = exec_.token(current_function_, PC_);
    if (!std::holds_alternative<OpCode>(token))
        throw RotaInvalidOpcodeError();

    switch (std::get<OpCode>(token)) {
        case OpCode::Nop:
            break;
        case OpCode::Push: {
            Value v = std::get<Value>(exec_.token(current_function_, PC_ + 1));
            push(std::move(v));
            ++PC_;
            break;
        }
        case OpCode::Pop:
            pop();
            break;
        case OpCode::Sum: {
            auto [a, b] = pop2();
            push(b + a);
            break;
        }
        case OpCode::Subtract: {
            auto [a, b] = pop2();
            push(b - a);
            break;
        }
        case OpCode::Multiply: {
            auto [a, b] = pop2();
            push(b * a);
            break;
        }
        case OpCode::Divide: {
            auto [a, b] = pop2();
            push(b / a);
            break;
        }
        case OpCode::IntDivide: {
            auto [a, b] = pop2();
            push(b.int_divide(a));
            break;
        }
        case OpCode::Modulo: {
            auto [a, b] = pop2();
            push(b % a);
            break;
        }
        case OpCode::Power: {
            auto [a, b] = pop2();
            push(b ^ a);
            break;
        }
        case OpCode::Equals: {
            auto [a, b] = pop2();
            push(b == a);
            break;
        }
        case OpCode::NotEqual: {
            auto [a, b] = pop2();
            push(b != a);
            break;
        }
        case OpCode::GreaterThan: {
            auto [a, b] = pop2();
            push(b > a);
            break;
        }
        case OpCode::LessThan: {
            auto [a, b] = pop2();
            push(b < a);
            break;
        }
        case OpCode::GreaterThanOrEqual: {
            auto [a, b] = pop2();
            push(b >= a);
            break;
        }
        case OpCode::LessThanOrEqual: {
            auto [a, b] = pop2();
            push(b <= a);
            break;
        }
        case OpCode::And: {
            auto [a, b] = pop2();
            push(b && a);
            break;
        }
        case OpCode::Or: {
            auto [a, b] = pop2();
            push(b || a);
            break;
        }
        case OpCode::Not:
            push(!pop());
            break;
        case OpCode::Halt:
            return false;
        default:
            throw RotaInvalidOpcodeError();
    }

    ++PC_;
    return true;
}

//
// STACK MANIPULATION
//

void RotaVM::push(Value const& value)
{
    if (stack_idx_ == STACK_SZ)
        throw RotaStackOverflowError();
    stack_[stack_idx_++] = value;
}

Value RotaVM::pop()
{
    if (stack_idx_ == 0)
        throw RotaStackUndeflowError();
    Value v = stack_[--stack_idx_];
    if (stack_idx_ == 0)
        last_value_ = v;
    return v;
}

std::string RotaVM::debug_stack() const
{
    std::string ret;
    for (size_t i = 0; i < stack_idx_; ++i)
        ret += "[" + stack_[i].debug() + "] ";
    return ret;
}

}
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

#pragma clang diagnostic push
#pragma ide diagnostic ignored "ArgumentSelectionDefects"
inline bool RotaVM::step()
{
    auto pop2 = [this]() -> std::pair<Value, Value> {
        Value a = pop();
        Value b = pop();
        return { a, b };
    };

    Executable::Token token = exec_.token(current_function_, PC_);

    switch (token.opcode) {
        case OpCode::Nop:
            break;
        case OpCode::Push: {
            push(*token.p1);
            ++PC_;
            break;
        }
        case OpCode::Pop:
            pop();
            break;
        case OpCode::Sum: {
            auto [a, b] = pop2();
            push(op_table.execute(BinaryOp::Plus, b, a));
            break;
        }
        case OpCode::Subtract: {
            auto [a, b] = pop2();
            push(op_table.execute(BinaryOp::Subtract, b, a));
            break;
        }
        case OpCode::Multiply: {
            auto [a, b] = pop2();
            push(op_table.execute(BinaryOp::Multiply, b, a));
            break;
        }
        case OpCode::Divide: {
            auto [a, b] = pop2();
            push(op_table.execute(BinaryOp::Divide, b, a));
            break;
        }
        case OpCode::IntDivide: {
            auto [a, b] = pop2();
            push(op_table.execute(BinaryOp::IntDivide, b, a));
            break;
        }
        case OpCode::Modulo: {
            auto [a, b] = pop2();
            push(op_table.execute(BinaryOp::Modulo, b, a));
            break;
        }
        case OpCode::Power: {
            auto [a, b] = pop2();
            push(op_table.execute(BinaryOp::Power, b, a));
            break;
        }
        case OpCode::Equals: {
            auto [a, b] = pop2();
            push(op_table.execute(BinaryOp::Equals, b, a));
            break;
        }
        case OpCode::NotEqual: {
            auto [a, b] = pop2();
            push((bool) !op_table.execute(BinaryOp::Equals, b, a).i());
            break;
        }
        case OpCode::GreaterThan: {
            auto [a, b] = pop2();
            push(op_table.execute(BinaryOp::GreaterThan, b, a));
            break;
        }
        case OpCode::LessThan: {
            auto [a, b] = pop2();
            push(op_table.execute(BinaryOp::LessThan, b, a));
            break;
        }
        case OpCode::GreaterThanOrEqual: {
            auto [a, b] = pop2();
            push(op_table.execute(BinaryOp::GreaterThan, b, a).i() || op_table.execute(BinaryOp::Equals, b, a).i());
            break;
        }
        case OpCode::LessThanOrEqual: {
            auto [a, b] = pop2();
            push(op_table.execute(BinaryOp::LessThan, b, a).i() || op_table.execute(BinaryOp::Equals, b, a).i());
            break;
        }
        case OpCode::And: {
            auto [a, b] = pop2();
            push(op_table.execute(BinaryOp::And, b, a));
            break;
        }
        case OpCode::Or: {
            auto [a, b] = pop2();
            push(op_table.execute(BinaryOp::Or, b, a));
            break;
        }
        case OpCode::Not:
            push(op_table.execute(UnaryOp::Not, pop()));
            break;
        case OpCode::Halt:
            return false;
        default:
            throw RotaInvalidOpcodeError();
    }

    ++PC_;
    return true;
}
#pragma clang diagnostic pop

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
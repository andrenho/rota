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
    auto pop2 = [this]() -> std::pair<Value, Value> {
        Value a = pop();
        Value b = pop();
        return { a, b };
    };

    switch ((OpCode) executable_.at(PC_)) {
        case OpCode::Nop:
            break;
        case OpCode::Push: {
            auto [v, sz] = value_at(PC_ + 1);
            push(std::move(v));
            PC_ += sz;
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
        default:
            throw RotaInvalidOpcodeError();
    }

    ++PC_;
}

//
// STACK MANIPULATION
//

void RotaVM::push(Value&& value)
{
    if (stack_idx_ == STACK_SZ)
        throw RotaStackOverflowError();
    stack_[stack_idx_++] = value;
}

Value RotaVM::pop()
{
    if (stack_idx_ == 0)
        throw RotaStackUndeflowError();
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

std::string RotaVM::debug_executable() const
{
    size_t p = 0;
    std::string out;

    while (p < executable_.size()) {
        switch ((OpCode) executable_.at(p)) {
            case OpCode::Nop: out += "\tNOP\n"; break;
            case OpCode::Push: {
                auto [v, sz] = value_at(p + 1);
                out += "\tPUSH " + v.debug() + "\n";
                p += sz;
                break;
            }
            case OpCode::Pop: out += "\tPOP\n"; break;
            case OpCode::Sum: out += "\tSUM\n"; break;
            case OpCode::Subtract: out += "\tSUB\n"; break;
            case OpCode::Multiply: out += "\tMUL\n"; break;
            case OpCode::Divide: out += "\tDIV\n"; break;
            case OpCode::IntDivide: out += "\tIDIV\n"; break;
            case OpCode::Modulo: out += "\tMOD\n"; break;
            case OpCode::Power: out += "\tPOW\n"; break;
            case OpCode::Equals: out += "\tEQ\n"; break;
            case OpCode::NotEqual: out += "\tNEQ\n"; break;
            case OpCode::GreaterThan: out += "\tGT\n"; break;
            case OpCode::LessThan: out += "\tLT\n"; break;
            case OpCode::GreaterThanOrEqual: out += "\tGTE\n"; break;
            case OpCode::LessThanOrEqual: out += "\tLTE\n"; break;
            case OpCode::And: out += "\tAND\n"; break;
            case OpCode::Or: out += "\tOR\n"; break;
            case OpCode::Not: out += "\tNOT\n"; break;
            case OpCode::Halt: out += "\tHALT\n"; break;
        }

        ++p;
    }

    return out;
}

std::string RotaVM::debug_executable_memory() const
{
    std::string out;

    for (auto b: executable_) {
        char buf[4];
        sprintf(buf, "%02X ", b);
        out += buf;
    }

    return out;
}

std::pair<Value, size_t> RotaVM::value_at(size_t pc) const
{
    return Value::from_bytes(&executable_.at(pc), executable_.size() - pc);
}

void RotaVM::set_executable_memory(std::vector<uint8_t> const& data, bool add_halt)
{
    executable_ = data;
    if (add_halt)
        executable_.push_back((uint8_t) OpCode::Halt);
}

}
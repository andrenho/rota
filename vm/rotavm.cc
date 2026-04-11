#include <cmath>

#include <functional>

#include "rotavm.hh"
#include "exceptions.hh"
#include "opcode.hh"

namespace rotavm {

//
// EXECUTABLE
//

RotaVM::RotaVM()
{
    call_stack_.push({ 0, 0 });
}


void RotaVM::run_until_halt()
{
    for (;;) {
        if (!step())
            return;
    }
}

void RotaVM::set_executable(Executable const& exec)
{
    exec_ = exec;
    global_vars_.resize(exec_.number_of_globals());
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

    Executable::Token token = exec_.token(current_function(), addr());

    switch (token.opcode) {
        case OpCode::Nop:
            break;
        case OpCode::Push: {
            push(*token.p1);
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
        case OpCode::Call: {
            Value v = pop();
            if (v.type() != T_FUNCTION)
                throw std::runtime_error("Can't call non-function value");
            enter_function(v.functionId());
            return true;
        }
        case OpCode::Return:
            return_from_function();
            return true;
        case OpCode::StoreLocal:
            locals_vars_.push_back(pop());
            break;
        case OpCode::LoadLocal:
            push(locals_vars_.at(token.p1->i()));
            break;
        case OpCode::StoreGlobal:
            global_vars_.at(token.p1->i()) = pop();
            break;
        case OpCode::LoadGlobal:
            push(global_vars_.at(token.p1->i()));
            break;
        case OpCode::Halt:
            return false;
        default:
            throw RotaInvalidOpcodeError();
    }

    ++addr();
    return true;
}
#pragma clang diagnostic pop

void RotaVM::enter_function(FunctionId f_id)
{
    ++addr();
    call_stack_.push({ .f_id = f_id, .addr = 0 });
}

void RotaVM::return_from_function()
{
    // TODO - remove variables from scope
    call_stack_.pop();
}

//
// STACK MANIPULATION
//

void RotaVM::push(Value const& value)
{
    if (stack_.size() == STACK_SZ)
        throw RotaStackOverflowError();
    stack_.push_back(value);
}

Value RotaVM::pop()
{
    if (stack_.empty())
        throw RotaStackUndeflowError();
    Value v = stack_.back();
    stack_.pop_back();
    if (stack_.empty())
        last_value_ = v;
    return v;
}

Value RotaVM::peek()
{
    if (stack_.empty())
        throw RotaStackUndeflowError();
    return stack_.back();
}

std::string RotaVM::debug_stack() const
{
    std::string ret;
    for (auto const& value: stack_)
        ret += "[" + value.debug() + "] ";
    return ret;
}

}
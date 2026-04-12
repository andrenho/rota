#include <cmath>

#include <functional>
#include <ranges>

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
    local_vars_fp_.push(0);
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
            function_call(token.p1->i());
            return true;
        }
        case OpCode::Return:
            return_from_function();
            return true;
        case OpCode::StoreLocal: {
            size_t idx = local_vars_fp_.top() + token.p1->i();
            if (idx >= locals_vars_.size())
                locals_vars_.resize(idx + 1);
            locals_vars_[idx]= pop();
            break;
        }
        case OpCode::LoadLocal:
            push(locals_vars_.at(local_vars_fp_.top() + token.p1->i()));
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

void RotaVM::function_call(size_t param_count)
{
    // create new variable context
    local_vars_fp_.push(locals_vars_.size());

    // get function parameters from the stack, and add it in the reverse order to the function variables
    std::vector<Value> pars;
    for (size_t i = 0; i < param_count; ++i)
        pars.push_back(pop());
    for (auto& par : std::ranges::reverse_view(pars))
        locals_vars_.push_back(par);

    // find function id
    Value v = pop();
    if (v.type() != T_FUNCTION)
        throw std::runtime_error("Can't call non-function value");

    // save current state and enter new context
    ++addr();
    call_stack_.push({ .f_id = (FunctionId) v.i(), .addr = 0 });
}

void RotaVM::return_from_function()
{
    local_vars_fp_.pop();
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
#include "rotavm.hh"
#include "exceptions.hh"

#define H std::holds_alternative

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
        ret += "[" + std::to_string(stack_[i]) + "] ";
    return ret;
}

//
// ARITHMETIC
//

void RotaVM::sum()
{
    binary_op(std::plus {});
}

void RotaVM::subtract()
{
    binary_op(std::minus {});
}

void RotaVM::multiply()
{
    binary_op(std::multiplies {});
}

void RotaVM::divide()
{
    binary_op(std::divides {});
}

template<typename Op>
void RotaVM::binary_op(Op op) {
    Value a = pop();
    Value b = pop();

    if (H<int>(a) && H<int>(b))
        push(op(std::get<int>(b), std::get<int>(a)));
    else if (H<int>(a) && H<float>(b))
        push(op((float)std::get<int>(a), std::get<float>(b)));
    else if (H<float>(a) && H<int>(b))
        push(op(std::get<float>(a), (float)std::get<int>(b)));
    else if (H<float>(a) && H<float>(b))
        push(op(std::get<float>(a), std::get<float>(b)));
    else
        throw RotaException("Unacceptable combination of types.");
}

}

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
std::string std::to_string(rotavm::Value const& v)
{
    return std::visit(overloaded {
        [](int i) { return std::to_string(i); },
        [](float f) { return std::to_string(f); },
    }, v);
}

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
        ret += "[" + std::to_string(stack_[i]) + "] ";
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

/*
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
    Value a = pop();
    Value b = pop();

    float ax = to_float(a);
    float bx = to_float(b);

    push(bx / ax);
}

void RotaVM::idivide()
{
    Value a = pop();
    Value b = pop();

    int ax = to_int(a);
    int bx = to_int(b);

    push(bx / ax);
}

void RotaVM::modulo()
{
    binary_op(std::modulus {}, fmodf);
}

void RotaVM::power()
{
    binary_op(powl, powf);
}

template<typename Op>
constexpr void RotaVM::binary_op(Op op) {
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
        throw RotaException("Type error");
}

template<typename IntOp, typename FloatOp>
constexpr void RotaVM::binary_op(IntOp int_op, FloatOp float_op)
{
    Value a = pop();
    Value b = pop();

    if (H<int>(a) && H<int>(b))
        push((int) int_op(std::get<int>(b), std::get<int>(a)));
    else if (H<int>(a) && H<float>(b))
        push(float_op((float)std::get<int>(a), std::get<float>(b)));
    else if (H<float>(a) && H<int>(b))
        push(float_op(std::get<float>(a), (float)std::get<int>(b)));
    else if (H<float>(a) && H<float>(b))
        push(float_op(std::get<float>(a), std::get<float>(b)));
    else
        throw RotaException("Type error");
}
 */

}

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
std::string std::to_string(rotavm::Value const& v)
{
    return "";
    /*
    return std::visit(overloaded {
        [](int i) { return std::to_string(i); },
        [](float f) { return std::to_string(f); },
    }, v);
     */
}

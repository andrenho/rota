#include "value.hh"

#include <functional>

#include "exceptions.hh"
#include "optable.hh"
#include "type.hh"

constexpr const char* TYPE_ERROR = "Type error";

namespace rotavm {

static OpTable op_tabl;
OpTable const& Value::op_table = op_tabl;

//
// BINARY FUNCTIONS
//

bool Value::operator==(Value const& other) const
{
    return std::get<bool>(op_table.execute(BinaryOp::Equals, *this, other));
}

bool Value::operator!=(Value const& other) const
{
    return !std::get<bool>(op_table.execute(BinaryOp::Equals, *this, other));
}

bool Value::operator<(Value const& other) const
{
    return std::get<bool>(op_table.execute(BinaryOp::LessThan, *this, other));
}

bool Value::operator>(Value const& other) const
{
    return std::get<bool>(op_table.execute(BinaryOp::GreaterThan, *this, other));
}

bool Value::operator<=(Value const& other) const
{
    return *this < other || *this == other;
}

bool Value::operator>=(Value const& other) const
{
    return *this > other || *this == other;
}

Value Value::operator+(Value const& other) const
{
    return std::get<Value>(op_table.execute(BinaryOp::Plus, *this, other));
}

Value Value::operator-(Value const& other) const
{
    return std::get<Value>(op_table.execute(BinaryOp::Subtract, *this, other));
}

Value Value::operator/(Value const& other) const
{
    return std::get<Value>(op_table.execute(BinaryOp::Divide, *this, other));
}

Value Value::operator*(Value const& other) const
{
    return std::get<Value>(op_table.execute(BinaryOp::Multiply, *this, other));
}

Value Value::int_divide(Value const& other) const
{
    return std::get<Value>(op_table.execute(BinaryOp::IntDivide, *this, other));
}

Value Value::operator%(Value const& other) const
{
    return std::get<Value>(op_table.execute(BinaryOp::Modulo, *this, other));
}

Value Value::operator^(Value const& other) const
{
    return std::get<Value>(op_table.execute(BinaryOp::Power, *this, other));
}

bool Value::operator&&(Value const& other) const
{
    return std::get<bool>(op_table.execute(BinaryOp::And, *this, other));
}

bool Value::operator||(Value const& other) const
{
    return std::get<bool>(op_table.execute(BinaryOp::Or, *this, other));
}

bool Value::operator!() const
{
    return std::get<bool>(op_table.execute(UnaryOp::Not, *this));
}

std::string Value::debug() const
{
    switch (type_) {
        case T_INT: return std::to_string(i_);
        case T_FLOAT: return std::to_string(f_);
        default: throw std::runtime_error("not implemented");
    }
}
}
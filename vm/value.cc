#include "value.hh"

#include <cstring>
#include <functional>
#include <limits>

#include "exceptions.hh"
#include "optable.hh"
#include "type.hh"

constexpr const char* TYPE_ERROR = "Type error";

namespace rotavm {

static OpTable op_tabl;
OpTable const& Value::op_table = op_tabl;

//
// BYTE PARSING
//

enum TypeByte {
    TB_INT8=0x0, TB_INT16=0x1, TB_INT32=0x2, TB_FLOAT=0x3,
};

std::vector<uint8_t> Value::to_bytes() const
{
    std::vector<uint8_t> bytes;
    switch (type_) {
        case T_INT:
            if (i_ >= std::numeric_limits<int8_t>::min() && i_ <= std::numeric_limits<int8_t>::max())
                return {TB_INT8, (uint8_t) (i_ & 0xff) };
            if (i_ >= std::numeric_limits<int16_t>::min() && i_ <= std::numeric_limits<int16_t>::max())
                return {TB_INT16, (uint8_t) (i_ & 0xff), (uint8_t) ((i_ >> 8) & 0xff) };
            else
                return {TB_INT32, (uint8_t) (i_ & 0xff), (uint8_t) ((i_ >> 8) & 0xff), (uint8_t) ((i_ >> 16) & 0xff), (uint8_t) ((i_ >> 24) & 0xff) };
        case T_FLOAT: {
            uint32_t bits;
            std::memcpy(&bits, &f_, 4);
            return { TB_FLOAT, (uint8_t) (bits & 0xff), (uint8_t) ((bits >> 8) & 0xff), (uint8_t) ((bits >> 16) & 0xff), (uint8_t) ((bits >> 24) & 0xff) };
        }
        default:
            throw std::runtime_error("not implemented");
    }
}

std::pair<Value, size_t> Value::from_bytes(uint8_t const* data, size_t max_bytes)
{
    if (max_bytes < 1)
        throw std::runtime_error("no data");

    auto type = (TypeByte) data[0];

    switch (type) {
        case TB_INT8: {
            if (max_bytes < 2)
                throw std::runtime_error("truncated INT8");
            return { Value((int8_t) data[1]), 2 };
        }
        case TB_INT16: {
            if (max_bytes < 3)
                throw std::runtime_error("truncated INT16");
            return { Value((int16_t) (data[1] | (data[2] << 8))), 3 };
        }
        case TB_INT32: {
            if (max_bytes < 5)
                throw std::runtime_error("truncated INT32");
            return { Value((int32_t) (data[1] | (data[2] << 8) | (data[3] << 16) | (data[4] << 24))), 5 };
        }
        case TB_FLOAT: {
            if (max_bytes < 5)
                throw std::runtime_error("truncated FLOAT");
            uint32_t bits = data[1] | (data[2] << 8) | (data[3] << 16) | (data[4] << 24);
            float f;
            std::memcpy(&f, &bits, 4);
            return { Value(f), 5 };
        }
        default:
            throw std::runtime_error("unknown type byte");
    }
}

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
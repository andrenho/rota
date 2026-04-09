#ifndef ROTA_OPTABLE_HH
#define ROTA_OPTABLE_HH

#include <cstddef>
#include <functional>
#include <variant>

#include "type.hh"

namespace rotavm {

class Value;

enum class UnaryOp : size_t {
    Not, COUNT
};

enum class BinaryOp : size_t {
    Plus, Subtract, Multiply, Divide, IntDivide,
    Modulo, Power,
    Equals, LessThan, GreaterThan, And, Or,
    COUNT
};

class OpTable {
public:
    OpTable();

    [[nodiscard]] Value execute(BinaryOp op, Value const& a, Value const& b) const;
    [[nodiscard]] Value execute(UnaryOp op, Value const& a) const;

private:
    using BinaryOpFunc = std::function<Value(Value const&, Value const&)>;
    BinaryOpFunc binary_op_[(size_t) BinaryOp::COUNT][T_COUNT][T_COUNT];

    using UnaryOpFunc = std::function<Value(Value const&)>;
    UnaryOpFunc unary_op_[(size_t) UnaryOp::COUNT][T_COUNT];
};

}

#endif //ROTA_OPTABLE_HH

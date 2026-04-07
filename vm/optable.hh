#ifndef ROTA_OPTABLE_HH
#define ROTA_OPTABLE_HH

#include <cstddef>
#include <functional>
#include <variant>

#include "type.hh"

namespace rotavm {

class Value;

enum class BinaryOp : size_t {
    Plus, Subtract, Multiply, Divide, IntDivide,
    Modulo, Power,
    Equals, LessThan, GreaterThan, And, Or,
    COUNT };
using BinaryOpRet = std::variant<Value, bool>;

class OpTable {
public:
    OpTable();

    [[nodiscard]] BinaryOpRet execute(BinaryOp op, Value const& a, Value const& b) const;

private:
    using BinaryOpFunc = std::function<BinaryOpRet(Value const&, Value const&)>;
    BinaryOpFunc binary_op_[(size_t) BinaryOp::COUNT][T_COUNT][T_COUNT];
};

}

#endif //ROTA_OPTABLE_HH

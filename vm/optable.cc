#include "optable.hh"

#include <cmath>
#include <limits>

#include "value.hh"
#include "exceptions.hh"

namespace rotavm {

OpTable::OpTable()
{
    for (size_t i = 0; i < (size_t) BinaryOp::COUNT; ++i) {
        for (size_t j = 0; j < (size_t) T_COUNT; ++j) {
            for (size_t k = 0; k < (size_t) T_COUNT; ++k) {
                binary_op_[i][j][k] = [](Value const&, Value const&) -> BinaryOpRet { throw RotaTypeError(); };
            }
        }
    }

#define BIN_OP(OP, TYPE_A, TYPE_B) binary_op_[(size_t) BinaryOp::OP][TYPE_A][TYPE_B] = [](Value const& a, Value const& b) -> BinaryOpRet
    BIN_OP(Equals, T_INT, T_INT)     { return a.i() == b.i(); };
    BIN_OP(Equals, T_INT, T_FLOAT)   { return std::fabs((float) a.i() - b.f()) < std::numeric_limits<float>::epsilon(); };
    BIN_OP(Equals, T_FLOAT, T_INT)   { return std::fabs(a.f() - (float) b.i()) < std::numeric_limits<float>::epsilon(); };
    BIN_OP(Equals, T_FLOAT, T_FLOAT) { return std::fabs(a.f() - b.f()) < std::numeric_limits<float>::epsilon(); };

    BIN_OP(Plus, T_INT, T_INT)     { return Value(a.i() + b.i()); };
    BIN_OP(Plus, T_INT, T_FLOAT)   { return Value((float) a.i() + b.f()); };
    BIN_OP(Plus, T_FLOAT, T_INT)   { return Value(a.f() + (float) b.i()); };
    BIN_OP(Plus, T_FLOAT, T_FLOAT) { return Value(a.f() + b.f()); };
#undef BIN_OP
}

BinaryOpRet OpTable::execute(BinaryOp op, Value const& a, Value const& b) const
{
    return binary_op_[(size_t) op][a.type()][b.type()](a, b);
}

}
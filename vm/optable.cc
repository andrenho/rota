#include "optable.hh"

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
}

BinaryOpRet OpTable::execute(BinaryOp op, Value const& a, Value const& b) const
{
    return binary_op_[(size_t) op][a.type()][b.type()](a, b);
}

}
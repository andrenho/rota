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
                if (j == T_NIL || k == T_NIL)
                    binary_op_[i][j][k] = [](Value const&, Value const&) -> OpRet { throw RotaInvalidNilOperation(); };
                else
                    binary_op_[i][j][k] = [](Value const&, Value const&) -> OpRet { throw RotaTypeError(); };
            }
        }
    }

    for (size_t i = 0; i < (size_t) UnaryOp::COUNT; ++i) {
        for (size_t j = 0; j < (size_t) T_COUNT; ++j) {
            if (j == T_NIL)
                unary_op_[i][j] = [](Value const&) -> OpRet { throw RotaInvalidNilOperation(); };
            else
                unary_op_[i][j] = [](Value const&) -> OpRet { throw RotaTypeError(); };
        }
    }

#define BIN_OP(OP, TYPE_A, TYPE_B) binary_op_[(size_t) BinaryOp::OP][TYPE_A][TYPE_B] = [](Value const& a, Value const& b) -> OpRet
    BIN_OP(Plus, T_INT, T_INT)     { return Value(a.i() + b.i()); };
    BIN_OP(Plus, T_INT, T_FLOAT)   { return Value((float) a.i() + b.f()); };
    BIN_OP(Plus, T_FLOAT, T_INT)   { return Value(a.f() + (float) b.i()); };
    BIN_OP(Plus, T_FLOAT, T_FLOAT) { return Value(a.f() + b.f()); };

    BIN_OP(Subtract, T_INT, T_INT)     { return Value(a.i() - b.i()); };
    BIN_OP(Subtract, T_INT, T_FLOAT)   { return Value((float) a.i() - b.f()); };
    BIN_OP(Subtract, T_FLOAT, T_INT)   { return Value(a.f() - (float) b.i()); };
    BIN_OP(Subtract, T_FLOAT, T_FLOAT) { return Value(a.f() - b.f()); };

    BIN_OP(Multiply, T_INT, T_INT)     { return Value(a.i() * b.i()); };
    BIN_OP(Multiply, T_INT, T_FLOAT)   { return Value((float) a.i() * b.f()); };
    BIN_OP(Multiply, T_FLOAT, T_INT)   { return Value(a.f() * (float) b.i()); };
    BIN_OP(Multiply, T_FLOAT, T_FLOAT) { return Value(a.f() * b.f()); };

    BIN_OP(Divide, T_INT, T_INT)     { return Value((float) a.i() / (float) b.i()); };
    BIN_OP(Divide, T_INT, T_FLOAT)   { return Value((float) a.i() / b.f()); };
    BIN_OP(Divide, T_FLOAT, T_INT)   { return Value(a.f() / (float) b.i()); };
    BIN_OP(Divide, T_FLOAT, T_FLOAT) { return Value(a.f() / b.f()); };

    BIN_OP(IntDivide, T_INT, T_INT)     { return Value(a.i() / b.i()); };
    BIN_OP(IntDivide, T_INT, T_FLOAT)   { return Value(a.i() / (int) b.f()); };
    BIN_OP(IntDivide, T_FLOAT, T_INT)   { return Value((int) a.f() / b.i()); };
    BIN_OP(IntDivide, T_FLOAT, T_FLOAT) { return Value((int) a.f() / (int) b.f()); };

    BIN_OP(Modulo, T_INT, T_INT)     { return Value(a.i() % b.i()); };
    BIN_OP(Modulo, T_INT, T_FLOAT)   { return Value(fmodf((float) a.i(), b.f())); };
    BIN_OP(Modulo, T_FLOAT, T_INT)   { return Value(fmodf(a.f(), (float) b.i())); };
    BIN_OP(Modulo, T_FLOAT, T_FLOAT) { return Value(fmodf(a.f(), b.f())); };

    BIN_OP(Modulo, T_INT, T_INT)     { return Value(a.i() % b.i()); };
    BIN_OP(Modulo, T_INT, T_FLOAT)   { return Value(fmodf((float) a.i(), b.f())); };
    BIN_OP(Modulo, T_FLOAT, T_INT)   { return Value(fmodf(a.f(), (float) b.i())); };
    BIN_OP(Modulo, T_FLOAT, T_FLOAT) { return Value(fmodf(a.f(), b.f())); };

    BIN_OP(Power, T_INT, T_INT)     { return Value((int) powl(a.i(), b.i())); };
    BIN_OP(Power, T_INT, T_FLOAT)   { return Value(powf((float) a.i(), b.f())); };
    BIN_OP(Power, T_FLOAT, T_INT)   { return Value(powf(a.f(), (float) b.i())); };
    BIN_OP(Power, T_FLOAT, T_FLOAT) { return Value(powf(a.f(), b.f())); };

    BIN_OP(Equals, T_NIL, T_NIL)     { return true; };
    BIN_OP(Equals, T_NIL, T_INT)     { return false; };
    BIN_OP(Equals, T_NIL, T_FLOAT)   { return false; };
    BIN_OP(Equals, T_INT, T_NIL)     { return false; };
    BIN_OP(Equals, T_INT, T_INT)     { return a.i() == b.i(); };
    BIN_OP(Equals, T_INT, T_FLOAT)   { return std::fabs((float) a.i() - b.f()) < std::numeric_limits<float>::epsilon(); };
    BIN_OP(Equals, T_FLOAT, T_NIL)   { return false; };
    BIN_OP(Equals, T_FLOAT, T_INT)   { return std::fabs(a.f() - (float) b.i()) < std::numeric_limits<float>::epsilon(); };
    BIN_OP(Equals, T_FLOAT, T_FLOAT) { return std::fabs(a.f() - b.f()) < std::numeric_limits<float>::epsilon(); };

    BIN_OP(LessThan, T_INT, T_INT)     { return a.i() < b.i(); };
    BIN_OP(LessThan, T_INT, T_FLOAT)   { return (float) a.i() < b.f(); };
    BIN_OP(LessThan, T_FLOAT, T_INT)   { return a.f() < (float) b.i(); };
    BIN_OP(LessThan, T_FLOAT, T_FLOAT) { return a.f() < b.f(); };

    BIN_OP(GreaterThan, T_INT, T_INT)     { return a.i() > b.i(); };
    BIN_OP(GreaterThan, T_INT, T_FLOAT)   { return (float) a.i() > b.f(); };
    BIN_OP(GreaterThan, T_FLOAT, T_INT)   { return a.f() > (float) b.i(); };
    BIN_OP(GreaterThan, T_FLOAT, T_FLOAT) { return a.f() > b.f(); };

    BIN_OP(And, T_NIL, T_INT)     { return false; };
    BIN_OP(And, T_INT, T_INT)     { return (bool) (a.i() && b.i()); };
    BIN_OP(And, T_INT, T_NIL)     { return false; };

    BIN_OP(Or,  T_NIL, T_INT)     { return (bool) b.i(); };
    BIN_OP(Or,  T_INT, T_INT)     { return (bool) (a.i() || b.i()); };
    BIN_OP(Or,  T_INT, T_NIL)     { return (bool) a.i(); };
#undef BIN_OP

#define UNARY_OP(OP, TYPE) unary_op_[(size_t) UnaryOp::OP][TYPE] = [](Value const& a) -> OpRet
    UNARY_OP(Not, T_INT)    { return !a.i(); };
    UNARY_OP(Not, T_FLOAT)  { return !((int) a.f()); };
#undef UNARY_OP
}

OpRet OpTable::execute(BinaryOp op, Value const& a, Value const& b) const
{
    return binary_op_[(size_t) op][a.type()][b.type()](a, b);
}

OpRet OpTable::execute(UnaryOp op, Value const& a) const
{
    return unary_op_[(size_t) op][a.type()](a);
}

}
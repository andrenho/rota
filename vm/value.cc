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

Value Value::operator+(Value const& other) const
{
    return std::get<Value>(op_table.execute(BinaryOp::Plus, *this, other));
}

/*
int to_int(Value const& v)
{
    if (H<int>(v))
        return std::get<int>(v);
    else if (H<float>(v))
        return (int) std::get<float>(v);
    throw RotaException("Type error");
}

float to_float(Value const& v)
{
    if (H<int>(v))
        return (float) std::get<int>(v);
    else if (H<float>(v))
        return std::get<float>(v);
    throw RotaException("Type error");
}
*/

}
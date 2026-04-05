#include "value.hh"
#include "exceptions.hh"

namespace rotavm {

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

}
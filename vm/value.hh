#ifndef ROTA_VALUE_HH
#define ROTA_VALUE_HH

#include <variant>

#define H std::holds_alternative

namespace rotavm {

using Value = std::variant<int, float>;

int   to_int(Value const& v);
float to_float(Value const& v);

}

#endif //ROTA_VALUE_HH

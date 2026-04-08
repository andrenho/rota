#ifndef ROTA_FUNCTION_HH
#define ROTA_FUNCTION_HH

#include <cstddef>

using FunctionId = size_t;

struct Function {
    explicit Function(FunctionId id) : id(id) {}

    FunctionId id;
};

#endif //ROTA_FUNCTION_HH

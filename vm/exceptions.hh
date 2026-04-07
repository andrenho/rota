#ifndef ROTA_EXCEPTIONS_HH
#define ROTA_EXCEPTIONS_HH

#include <stdexcept>

namespace rotavm {

class RotaException : public std::runtime_error {
public:
    explicit RotaException(const char* description) : std::runtime_error(description) {}
    explicit RotaException(std::string const& description) : std::runtime_error(description) {}
};

class RotaTypeError : public RotaException {
public:
    RotaTypeError() : RotaException("Type error") {}
};

}

#endif //ROTA_EXCEPTIONS_HH

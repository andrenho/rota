#ifndef ROTA_VMEXCEPTIONS_HH
#define ROTA_VMEXCEPTIONS_HH

#include <stdexcept>

struct VMInvalidOpcodeException : public std::runtime_error {
    VMInvalidOpcodeException() : std::runtime_error("Invalid opcode.") {}
};

#endif //ROTA_VMEXCEPTIONS_HH

#ifndef ROTA_OPCODE_HH
#define ROTA_OPCODE_HH

#include <cstdint>

namespace rotavm {

enum class OpCode : uint8_t {
    Sum = 0x10, Subtract = 0x11, Multiply = 0x12, Divide = 0x13, Modulo = 0x14, Power = 0x15,
    Equals = 0x20, NotEqual = 0x21, GreaterThan = 0x22, LessThan = 0x23, GreaterThanOrEqual = 0x24, LessThanOrEqual = 0x25,
    And = 0x26, Or = 0x27, Not = 0x28,
};

}

#endif //ROTA_OPCODE_HH

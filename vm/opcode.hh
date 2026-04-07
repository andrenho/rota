#ifndef ROTA_OPCODE_HH
#define ROTA_OPCODE_HH

#include <cstdint>

namespace rotavm {

enum class OpCode : uint8_t {
    Nop    = 0x00,
    Push   = 0x01, Pop      = 0x02,
    Sum    = 0x10, Subtract = 0x11, Multiply    = 0x12, Divide   = 0x13, IntDivide          = 0x14, Modulo          = 0x15, Power = 0x16,
    Equals = 0x20, NotEqual = 0x21, GreaterThan = 0x22, LessThan = 0x23, GreaterThanOrEqual = 0x24, LessThanOrEqual = 0x25,
    And    = 0x26, Or       = 0x27, Not         = 0x28,
    Halt   = 0xff,
};

}

#endif //ROTA_OPCODE_HH

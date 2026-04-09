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
    Return = 0x30, Call     = 0x31,
    Halt   = 0xff,
};

inline const char* opcode_name(OpCode opcode) {
    switch (opcode) {
        case OpCode::Nop:       return "NOP";
        case OpCode::Push:      return "PUSH";
        case OpCode::Pop:       return "POP";
        case OpCode::Sum:       return "SUM";
        case OpCode::Subtract:  return "SUB";
        case OpCode::Multiply:  return "MUL";
        case OpCode::Divide:    return "DIV";
        case OpCode::IntDivide: return "IDIV";
        case OpCode::Modulo:    return "MOD";
        case OpCode::Power:     return "POW";
        case OpCode::Equals:    return "EQ";
        case OpCode::NotEqual:  return "NEQ";
        case OpCode::GreaterThan: return "GT";
        case OpCode::LessThan:  return "LT";
        case OpCode::GreaterThanOrEqual: return "GTE";
        case OpCode::LessThanOrEqual:    return "LTE";
        case OpCode::And:       return "AND";
        case OpCode::Or:        return "OR";
        case OpCode::Not:       return "NOT";
        case OpCode::Return:    return "RET";
        case OpCode::Call:      return "CALL";
        case OpCode::Halt:      return "HALT";
    }
    return "???";
}

}

#endif //ROTA_OPCODE_HH

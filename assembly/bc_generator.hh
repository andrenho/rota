#ifndef ROTA_BC_GENERATOR_HH
#define ROTA_BC_GENERATOR_HH

#include "bytecode.hh"

class BytecodeGenerator {
public:
    void push(int32_t number);

    [[nodiscard]] Bytecode generate_bytecode() const;

private:
    // update bytecode
    BytecodeGenerator& add(Opcode opcode)   { code_.push_back((uint8_t) opcode); return *this; }
    BytecodeGenerator& add8(int8_t number)  { code_.push_back((uint8_t) number); return *this; }
    BytecodeGenerator& add16(int16_t number);
    BytecodeGenerator& add32(int32_t number);

    // sections
    std::vector<uint8_t> code_;
};

#endif //ROTA_BC_GENERATOR_HH

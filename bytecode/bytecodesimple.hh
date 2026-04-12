#ifndef ROTA_BYTECODESIMPLE_HH
#define ROTA_BYTECODESIMPLE_HH

#include <cstdint>
#include <vector>
#include <optional>

#include "bytecode.hh"

class BytecodeSimple : public Bytecode {    // to be used mostly for testing
public:
    void add(Operation operation, std::optional<Value> operand1) override { instructions_.emplace_back(operation, operand1); }

    [[nodiscard]] std::pair<Instruction, size_t> next_instruction(size_t pc) const override { return { instructions_.at(pc), 1 }; }
    [[nodiscard]] size_t last_pc() const override { return instructions_.size(); }

private:
    std::vector<Instruction> instructions_;
};

#endif //ROTA_BYTECODESIMPLE_HH

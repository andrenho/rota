#ifndef ROTA_BYTECODESIMPLE_HH
#define ROTA_BYTECODESIMPLE_HH

#include <cstdint>
#include <vector>
#include <optional>

#include "bytecode.hh"

class BytecodeSimple : public Bytecode {    // to be used mostly for testing
public:
    void add(FunctionId f_id, Operation operation, std::optional<Value> operand1) override { instructions_.at(f_id).emplace_back(operation, operand1); }

    [[nodiscard]] std::pair<Instruction, size_t> next_instruction(FunctionId f_id, size_t pc) const override { return { instructions_.at(f_id).at(pc), 1 }; }
    [[nodiscard]] size_t last_pc(FunctionId f_id) const override { return instructions_.at(f_id).size(); }

private:
    std::vector<std::vector<Instruction>> instructions_ = { {} };
};

#endif //ROTA_BYTECODESIMPLE_HH

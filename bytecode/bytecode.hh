#ifndef ROTA_BYTECODE_HH
#define ROTA_BYTECODE_HH

#include <optional>
#include <string>
#include <variant>

enum class Operation {
    PushInt, Pop,
    SaveDebug,
};

class Bytecode {
public:
    // definitions
    using Value = std::variant<int32_t>;

    struct Instruction {
        Operation            operation;
        std::optional<Value> operand1;

        Instruction(Operation operation, std::optional<Value> operand1) : operation(operation), operand1(operand1) {}
    };

    // operation
    [[nodiscard]] virtual std::pair<Instruction, size_t> next_instruction(size_t pc) const = 0;
    [[nodiscard]] virtual size_t last_pc() const = 0;

    // generation
    virtual void add(Operation operation, std::optional<Value> operand1) = 0;
    void add(Operation operation) { add(operation, {}); }

    // inspection
    [[nodiscard]] std::string decompile_bytecode() const;
    [[nodiscard]] std::string decompile_instruction_at(size_t pc, size_t* sz=nullptr) const;
};

#endif //ROTA_BYTECODE_HH

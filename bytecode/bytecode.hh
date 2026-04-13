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
    using FunctionId = size_t;

    struct Instruction {
        Operation            operation;
        std::optional<Value> operand1;

        Instruction(Operation operation, std::optional<Value> operand1) : operation(operation), operand1(operand1) {}
    };

    // operation
    [[nodiscard]] virtual std::pair<Instruction, size_t> next_instruction(FunctionId f_id, size_t pc) const = 0;
    [[nodiscard]] virtual size_t                         last_pc(FunctionId f_id) const = 0;
    [[nodiscard]] size_t                                 n_functions() const { return function_ct; }

    // generation
    virtual void       add(FunctionId f_id, Operation operation, std::optional<Value> operand1) = 0;
    void               add(FunctionId f_id, Operation operation) { add(f_id, operation, {}); }
    virtual FunctionId add_function(std::optional<std::string> const& name={});

    // inspection
    [[nodiscard]] std::string decompile_bytecode() const;
    [[nodiscard]] std::string decompile_instruction_at(FunctionId f_id, size_t pc, size_t* sz=nullptr) const;

private:
    size_t function_ct = 1;
};

#endif //ROTA_BYTECODE_HH

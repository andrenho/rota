#ifndef ROTA_EXECUTABLE_HH
#define ROTA_EXECUTABLE_HH

#include "vm/opcode.hh"
#include "vm/value.hh"

#include <optional>
#include <unordered_map>
#include <stack>
#include <vector>

namespace rotavm {

class Executable {
public:
    struct Token {
        explicit Token(OpCode opcode) : opcode(opcode) {}
        Token(OpCode opcode, rotavm::Value const& p1) : opcode(opcode), p1(p1) {}
        Token(OpCode opcode, rotavm::Value const& p1, std::optional<std::string> const& var_name)
            : opcode(opcode), p1(p1), variable_name(var_name) {}

        OpCode opcode;
        std::optional<rotavm::Value> p1;

        // debugging info
        std::optional<std::string> variable_name;
    };

    struct Function {
        std::vector<Token>                      tokens {};
        size_t                                  total_variables = 0;
        std::unordered_map<std::string, size_t> var_idx {};
    };

    void add(OpCode opcode);
    void add(OpCode opcode, rotavm::Value const& p1, std::optional<std::string> const& var_name={});

    void add_function();
    void end_function();

    void assignment(std::string const& identifier);
    void load_identifier(std::string const& identifier);

    [[nodiscard]] Token token(FunctionId f_id, size_t pos) const { return functions_.at(f_id).tokens.at(pos); }

    [[nodiscard]] std::string debug() const;

private:
    size_t current_function_ = 0;
    std::vector<Function> functions_ = { { /* main */ } };
};

}

#endif //ROTA_EXECUTABLE_HH

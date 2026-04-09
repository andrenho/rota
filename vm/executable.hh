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

        OpCode opcode;
        std::optional<rotavm::Value> p1;
    };

    struct Function {
        std::string        name;
        std::vector<Token> tokens;
    };

    void add(OpCode opcode);
    void add(OpCode opcode, rotavm::Value const& p1);

    void add_function();
    void end_function();

    [[nodiscard]] Token token(FunctionId f_id, size_t pos) const { return functions_.at(f_id).tokens.at(pos); }

    [[nodiscard]] std::string debug() const;

private:
    size_t current_function_ = 0;
    std::vector<Function> functions_ = { { .name = "#main", .tokens = {} } };
};

}

#endif //ROTA_EXECUTABLE_HH

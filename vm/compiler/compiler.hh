#ifndef ROTA_COMPILER_HH
#define ROTA_COMPILER_HH

#include "vm/opcode.hh"
#include "vm/value.hh"

#include <unordered_map>
#include <stack>
#include <vector>

namespace rotavm {

class Executable {
public:
    using Token = std::variant<rotavm::OpCode, rotavm::Value>;

    struct Function {
        std::string        name;
        std::vector<Token> tokens;
    };

    Executable& operator<<(Token const& token);

    void add_function();
    void end_function();

    [[nodiscard]] Token token(FunctionId f_id, size_t pos) const { return functions_.at(f_id).tokens.at(pos); }

    [[nodiscard]] std::string debug() const;

private:
    size_t current_function_ = 0;
    std::vector<Function> functions_ = { { .name = "#main", .tokens = {} } };
};


Executable compile(std::string const& code);

}

#endif //ROTA_COMPILER_HH

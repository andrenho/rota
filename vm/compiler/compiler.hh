#ifndef ROTA_COMPILER_HH
#define ROTA_COMPILER_HH

#include "vm/opcode.hh"
#include "vm/value.hh"

#include <unordered_map>
#include <stack>
#include <vector>

namespace rotavm {

std::vector<uint8_t> compile(std::string const& code);

class CompilationOutput {
public:
    CompilationOutput& operator<<(rotavm::OpCode opcode);
    CompilationOutput& operator<<(rotavm::Value const& value);

    template <typename T>
    CompilationOutput& operator<<(T const& t) { *this << Value(t); return *this; }

    [[nodiscard]] std::vector<uint8_t> const& data() const;

    void add_function();
    void end_function();

private:
    static constexpr FunctionId MAIN_FUNCTION = 0;

    FunctionId function_id_counter_ = 1;
    FunctionId current_fn_id_ = MAIN_FUNCTION;
    std::unordered_map<FunctionId, std::vector<uint8_t>> data_ = {
            { MAIN_FUNCTION, {} },
    };
};

}

#endif //ROTA_COMPILER_HH

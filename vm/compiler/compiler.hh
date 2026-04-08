#ifndef ROTA_COMPILER_HH
#define ROTA_COMPILER_HH

#include "vm/opcode.hh"
#include "vm/value.hh"

#include <stack>
#include <vector>

namespace rotavm {

std::vector<uint8_t> compile(std::string const& code);

class CompilationOutput {
public:
    CompilationOutput();

    CompilationOutput& operator<<(rotavm::OpCode opcode);
    CompilationOutput& operator<<(rotavm::Value const& value);

    template <typename T>
    CompilationOutput& operator<<(T const& t) { *this << Value(t); return *this; }

    [[nodiscard]] std::vector<uint8_t> const& data() const { return data_; }

private:
    std::vector<uint8_t> data_;
};

}

#endif //ROTA_COMPILER_HH

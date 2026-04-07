#ifndef ROTA_COMPILER_HH
#define ROTA_COMPILER_HH

#include "vm/opcode.hh"
#include "vm/value.hh"

#include <vector>

namespace rotavm {

class CompilationOutput {
public:
    CompilationOutput& operator<<(rotavm::OpCode opcode);
    CompilationOutput& operator<<(rotavm::Value const& value);

    [[nodiscard]] std::vector<uint8_t> const& data() const { return data_; }

private:
    std::vector<uint8_t> data_;
};

}

#endif //ROTA_COMPILER_HH

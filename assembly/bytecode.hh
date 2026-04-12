#ifndef ROTA_BYTECODE_HH
#define ROTA_BYTECODE_HH

#include <cstdint>
#include <string>
#include <vector>

enum class Opcode : uint8_t {
    PushInt8 = 0x1, PushInt16 = 0x02, PushInt32 = 0x03,
};

enum class BytecodeSection : uint8_t {
    Magic = 0x00,
    Code  = 0x01,
};

constexpr uint8_t MAGIC[] = { 0xa9, 0xb8, 0xc7, 0xd6 };

class Bytecode {
public:
    Bytecode() = default;
    explicit Bytecode(std::vector<uint8_t> const& binary) : binary_(binary) {}

    [[nodiscard]] std::string decompile() const;
    [[nodiscard]] std::vector<uint8_t> binary() const { return binary_; }

protected:
    std::vector<uint8_t> binary_;
};

#endif //ROTA_BYTECODE_HH

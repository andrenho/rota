#include "bc_generator.hh"

#include <cstring>
#include <functional>
#include <limits>

void BytecodeGenerator::push(int32_t number)
{
    if (number >= std::numeric_limits<int8_t>::min() && number <= std::numeric_limits<int8_t>::max())
        add(Opcode::PushInt8).add8((int8_t) number);
    else if (number >= std::numeric_limits<int16_t>::min() && number <= std::numeric_limits<int16_t>::max())
        add(Opcode::PushInt16).add16((int16_t) number);
    else
        add(Opcode::PushInt32).add32(number);
}

//
// UPDATE BYTECODE
//

BytecodeGenerator& BytecodeGenerator::add16(int16_t number)
{
    code_.push_back(number & 0xff);
    code_.push_back(number >> 8);
    return *this;
}

BytecodeGenerator& BytecodeGenerator::add32(int32_t number)
{
    code_.push_back(number & 0xff);
    code_.push_back(number >> 8);
    code_.push_back(number >> 16);
    code_.push_back(number >> 24);
    return *this;
}

Bytecode BytecodeGenerator::generate_bytecode() const
{
    constexpr size_t N_SECTIONS = 16;
    constexpr size_t SECTION_SZ = 4;
    constexpr size_t SECTIONS_SZ = N_SECTIONS * SECTION_SZ;

    std::vector<uint8_t> data = { MAGIC[0], MAGIC[1], MAGIC[2], MAGIC[3] };
    data.resize(SECTIONS_SZ);

    auto add_section = [&data](std::function<std::pair<BytecodeSection, std::vector<uint8_t>>()> const& f) {
        auto [section_id, bytes] = f();

        // create new section index
        uint8_t section[SECTION_SZ] = {
                (uint8_t) (data.size() & 0xff), (uint8_t) (data.size() >> 8), (uint8_t) (data.size() >> 16), (uint8_t) (data.size() >> 24),
        };
        memcpy(&data[(uint8_t) section_id * SECTION_SZ], section, SECTION_SZ);

        // add new section
        data.append_range(bytes);
    };

    add_section([this]() -> std::pair<BytecodeSection, std::vector<uint8_t>> {
        return { BytecodeSection::Code, code_ };
    });

    add_section([this]() -> std::pair<BytecodeSection, std::vector<uint8_t>> {
        return { (BytecodeSection) ((uint8_t) BytecodeSection::Code + 1), {} };
    });

    return Bytecode(data);
}

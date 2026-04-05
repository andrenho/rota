#include "types.hh"

#include <cstring>
#include <stdexcept>

namespace types {

std::vector<uint8_t> to_bin(int n)
{
    if (n >= -128 && n <= 127)
        return { Int1Byte, (uint8_t) n };
    else if (n >= -32768 && n <= 32767)
        return { Int2Bytes, (uint8_t) n, (uint8_t) (((uint16_t) n) >> 8) };
    else if (n >= -8388608 && n <= 8388607)
        return { Int3Bytes, (uint8_t) n, (uint8_t) (((uint32_t) n) >> 8), (uint8_t) (((uint32_t) n) >> 16) };
    return { Int4Bytes, (uint8_t) n, (uint8_t) (((uint32_t) n) >> 8), (uint8_t) (((uint32_t) n) >> 16), (uint8_t) (((uint32_t) n) >> 24) };
}

std::vector<uint8_t> to_bin(float n)
{
    std::vector<uint8_t> vec = { Float, 0, 0, 0, 0 };  // TODO - endianess??
    memcpy(&vec[1], &n, 4);
    return vec;
}

std::pair<Value, uint8_t> from_bin(uint8_t* data, uint8_t max_sz) {
    switch ((TypeId) data[0]) {
        case Int1Byte:
            return { (int) data[1], 2 };
        case Int2Bytes:
            return { (int) (data[1] | (data[2] << 8)), 3 };
        case Int3Bytes:
            return { (int) (data[1] | (data[2] << 8) | (data[3] << 16)), 4 };
        case Int4Bytes:
            return { (int) (data[1] | (data[2] << 8) | (data[3] << 16) | (data[4] << 24)), 5 };
        case Float: {
            float f;
            memcpy(&f, data + 1, 4);   // TODO - endianess?
            return { f, 5 };
        }
        default:
            throw std::runtime_error("Invalid type");
    }
}

}

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
std::string std::to_string(types::Value const& value)
{
    return std::visit(overloaded{
        [](int i) { return std::to_string(i); },
        [](float f) { return std::to_string(f); }
    }, value);
}

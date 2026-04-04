#include "types.hh"

// TODO - redo all this, consider compactness and endianess

#include <cstring>
#include <stdexcept>

namespace types {

std::vector<uint8_t> to_bin(int n)
{
    std::vector<uint8_t> vec = { Int, 0, 0, 0, 0 };
    int32_t val = n;
    memcpy(&vec[1], &val, 4);
    return vec;
}

std::vector<uint8_t> to_bin(float n)
{
    std::vector<uint8_t> vec = { Float, 0, 0, 0, 0 };
    memcpy(&vec[1], &n, 4);
    return vec;
}

std::pair<Value, uint8_t> from_bin(uint8_t* data, uint8_t max_sz) {
    switch ((TypeId) data[0]) {
        case Int: {
            int n;
            memcpy(&n, data + 1, 4);
            return { n, 5 };
        }
        case Float: {
            float f;
            memcpy(&f, data + 1, 4);
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

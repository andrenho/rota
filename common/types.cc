#include "types.hh"

namespace types {

std::array<uint8_t, 4> to_bin(int n) {
    return { 0, 0, 0, 0 };
}

std::array<uint8_t, 4> to_bin(float n) {
    return { 0, 0, 0, 0 };
}

int int_from_bin(std::array<uint8_t, 4> bin)
{
    return 0;
}

float float_from_bin(std::array<uint8_t, 4> bin)
{
    return 0;
}

}

#ifndef FORTUNA_6_TYPES_HH
#define FORTUNA_6_TYPES_HH

#include <array>
#include <cstdint>

namespace types {

std::array<uint8_t, 4> to_bin(int n);
std::array<uint8_t, 4> to_bin(float n);

int   int_from_bin(std::array<uint8_t, 4> bin);
float float_from_bin(std::array<uint8_t, 4> bin);

}

#endif //FORTUNA_6_TYPES_HH

#ifndef FORTUNA_6_TYPES_HH
#define FORTUNA_6_TYPES_HH

#include <cstdint>
#include <string>
#include <variant>
#include <vector>
#include <utility>

namespace types {

using Value = std::variant<int, float>;
enum TypeId : uint8_t { Int1Byte = 0, Int2Bytes = 1, Int3Bytes = 2, Int4Bytes = 3, Float = 4 };

std::vector<uint8_t> to_bin(int n);
std::vector<uint8_t> to_bin(float n);

std::pair<Value, uint8_t> from_bin(uint8_t* data, uint8_t max_sz);

}

namespace std {
std::string to_string(types::Value const& value);
}

#endif //FORTUNA_6_TYPES_HH

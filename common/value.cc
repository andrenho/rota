#include "value.hh"

#include <stdexcept>

std::vector<uint8_t> Value::bytes() const
{
    if (const int* v = std::get_if<int>(&val_)) {
        return {
                (uint8_t) *v,
                (uint8_t) ((*v) >> 8),
                (uint8_t) ((*v) >> 16),
                (uint8_t) ((*v) >> 24),
        };
    } else {
        throw std::runtime_error("Unsupported type");
    }
}

void Value::add(std::vector<uint8_t>& output) const
{
    auto b = bytes();
    output.insert(output.end(), b.begin(), b.end());
}

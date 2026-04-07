#ifndef ROTA_VALUE_HH
#define ROTA_VALUE_HH

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "type.hh"

namespace rotavm {

class Value {
public:
    Value() :type_(T_INT), i_(0) {}
    explicit Value(int i) :type_(T_INT), i_(i) {}
    explicit Value(float f) :type_(T_FLOAT), f_(f) {}

    [[nodiscard]] bool operator==(Value const& other) const;
    [[nodiscard]] bool operator!=(Value const& other) const;
    [[nodiscard]] bool operator<(Value const& other) const;
    [[nodiscard]] bool operator>(Value const& other) const;
    [[nodiscard]] bool operator<=(Value const& other) const;
    [[nodiscard]] bool operator>=(Value const& other) const;

    [[nodiscard]] Value operator+(Value const& other) const;
    [[nodiscard]] Value operator-(Value const& other) const;
    [[nodiscard]] Value operator*(Value const& other) const;
    [[nodiscard]] Value operator/(Value const& other) const;
    [[nodiscard]] Value operator%(Value const& other) const;
    [[nodiscard]] Value operator^(Value const& other) const;
    [[nodiscard]] Value int_divide(Value const& other) const;

    [[nodiscard]] bool operator&&(Value const& other) const;
    [[nodiscard]] bool operator||(Value const& other) const;

    [[nodiscard]] bool operator!() const;

    [[nodiscard]] Type  type() const { return type_; }
    [[nodiscard]] int   i() const { return i_; }
    [[nodiscard]] float f() const { return f_; }

    [[nodiscard]] std::string debug() const;

    std::vector<uint8_t> to_bytes() const;
    static std::pair<Value, size_t> from_bytes(uint8_t const* data, size_t max_bytes);

private:
    Type type_;
    union {
        int i_;
        float f_;
    };

    static class OpTable const& op_table;
};

/*
class ValueInt;
class ValueFloat;

class ValueInt {
public:
    ValueInt() : value_(0) {}
    explicit ValueInt(int v) : value_(v) {}

    auto operator<=>(ValueInt const& i) const = default;
    std::strong_ordering operator<=>(ValueFloat const& f);

    ValueInt operator+(ValueInt const& i) const;
    ValueFloat operator+(ValueFloat const& f) const;

private:
    int value_;
};

class ValueFloat {
public:
    ValueFloat() : value_(0.f) {}
    explicit ValueFloat(float f) : value_(f) {}

    std::strong_ordering operator<=>(ValueInt const& i);
    auto operator<=>(ValueFloat const& f) const = default;

    ValueFloat operator+(ValueFloat const& f) const;
    ValueFloat operator+(ValueInt const& i) const;

private:
    float value_;
};

using Value = std::variant<ValueInt, ValueFloat>;

int   to_int(Value const& v);
float to_float(Value const& v);
 */

}

#endif //ROTA_VALUE_HH

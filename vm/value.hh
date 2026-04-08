#ifndef ROTA_VALUE_HH
#define ROTA_VALUE_HH

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "type.hh"

namespace rotavm {

using FunctionId = size_t;

struct Function {
    explicit Function(FunctionId id) : id(id) {}

    FunctionId id;
};

class Value {
public:
    Value() : type_(T_NIL), i_(0) {}
    explicit Value(int32_t i) : type_(T_INT), i_(i) {}
    explicit Value(float f) : type_(T_FLOAT), f_(f) {}
    explicit Value(Function const& fn) : type_(T_FUNCTION), fn_(fn) {}

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

    [[nodiscard]] std::vector<uint8_t> to_bytes() const;
    [[nodiscard]] static std::pair<Value, size_t> from_bytes(uint8_t const* data, size_t max_bytes);

private:
    Type type_;
    union {
        int32_t  i_;
        float    f_;
        Function fn_;
    };

    static class OpTable const& op_table;
};

}

#endif //ROTA_VALUE_HH

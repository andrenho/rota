#ifndef FORTUNA_6_VALUE_HH
#define FORTUNA_6_VALUE_HH

#include <cstdint>
#include <variant>
#include <vector>

class Value {
public:
    static Value Number(int n) { return Value(n);}

    [[nodiscard]] std::vector<uint8_t> bytes() const;

    void add(std::vector<uint8_t>& output) const;

private:
    using ValueVariant = std::variant<int>;
    explicit Value(ValueVariant v) : val_(v) {}
    ValueVariant val_;
};

#endif //FORTUNA_6_VALUE_HH

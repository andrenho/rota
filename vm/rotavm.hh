#ifndef ROTA_ROTAVM_HH
#define ROTA_ROTAVM_HH

#include <cstdint>
#include <cstddef>

#include <array>
#include <string>
#include <variant>

#define STACK_SZ 512

namespace rotavm {

using Value = std::variant<int>;

class RotaVM {
public:
    // stack manipulation
    void         push(Value&& value);   // +1
    Value        pop();                 // -1
    [[nodiscard]] Value const& peek() const;
    [[nodiscard]] size_t       stack_sz() const { return stack_idx_; }

    // arithmetic
    void sum();         // -2, +1
    void subtract();    // -2, +1
    void multiply();    // -2, +1
    void divide();      // -2, +1

    [[nodiscard]] std::string debug_stack() const;

private:
    std::array<Value, STACK_SZ> stack_ {};
    size_t                      stack_idx_ = 0;

    template<typename Op> void binary_op(Op op);
};

}

namespace std {
string to_string(rotavm::Value const& v);
}

#endif //ROTA_ROTAVM_HH

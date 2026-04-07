#ifndef ROTA_ROTAVM_HH
#define ROTA_ROTAVM_HH

#include <cstdint>
#include <cstddef>

#include <array>
#include <string>
#include <variant>

#include "value.hh"

#define STACK_SZ 512

namespace rotavm {

class RotaVM {
public:
    // execution
    void set_executable_memory(std::vector<uint8_t> const& data) { executable_ = data; }
    void run_until_halt();

    // debug
    [[nodiscard]] Value const& peek() const;
    [[nodiscard]] size_t       stack_sz() const { return stack_idx_; }
    [[nodiscard]] std::string  debug_stack() const;

private:
    std::array<Value, STACK_SZ> stack_ {};
    size_t                      stack_idx_ = 0;
    std::vector<uint8_t>        executable_;
    uint32_t                    PC_ = 0;

    void step();

    // stack manipulation
    void         push(Value&& value);   // +1
    void         push(bool v) { push(Value(v ? -1 : 0)); }
    Value        pop();                 // -1

    // arithmetic
    void sum();         // -2, +1
    void subtract();    // -2, +1
    void multiply();    // -2, +1
    void divide();      // -2, +1
    void idivide();     // -2, +1
    void modulo();      // -2, +1
    void power();       // -2, +1

    // logical
    void equals();                 // -2, +1
    void not_equal();              // -2, +1
    void greater_than();           // -2, +1
    void less_than();              // -2, +1
    void greater_than_or_equal();  // -2, +1
    void less_than_or_equal();     // -2, +1
    void and_();                   // -2, +1
    void or_();                    // -2, +1
    void not_();                   // -1, +1
};

}

#endif //ROTA_ROTAVM_HH

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
    void set_executable_memory(std::vector<uint8_t> const& data, bool add_halt);
    void run_until_halt();

    // debug
    [[nodiscard]] Value const& last_value() const { return last_value_; }
    [[nodiscard]] size_t       stack_sz() const { return stack_idx_; }
    [[nodiscard]] std::string  debug_stack() const;
    [[nodiscard]] std::string  debug_executable() const;
    [[nodiscard]] std::string  debug_executable_memory() const;

private:
    std::array<Value, STACK_SZ> stack_ {};
    size_t                      stack_idx_ = 0;
    std::vector<uint8_t>        executable_;
    Value                       last_value_;
    uint32_t                    PC_ = 0;

    void step();

    [[nodiscard]] std::pair<Value, size_t> value_at(size_t pc) const;

    // stack manipulation
    void         push(Value const& value);   // +1
    void         push(bool v) { push(Value(v ? -1 : 0)); }
    Value        pop();                 // -1
};

}

#endif //ROTA_ROTAVM_HH
